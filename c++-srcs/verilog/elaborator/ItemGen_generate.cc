
/// @file ItemGen.cc
/// @brief ItemGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "DeclGen.h"
#include "DefParamStub.h"
#include "ElbEnv.h"
#include "ElbStub.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstContAssign.h"
#include "ym/vl/AstGenCaseItem.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/VlModule.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbGfRoot.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbError.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ItemGen
//////////////////////////////////////////////////////////////////////

// @brief generate block を実際にインスタンス化を行う．
void
ItemGen::phase1_generate(
  const VlScope* parent,
  const AstItem& ast_generate
)
{
  phase1_genitem(parent,
		 ast_generate.declhead_list(),
		 ast_generate.item_list());
}

// @brief AstGenBlock に対応するインスタンスの生成を行う
void
ItemGen::phase1_genblock(
  const VlScope* parent,
  const AstItem& ast_genblock
)
{
  auto* name = ast_genblock.name();
  if ( name != nullptr ) {
    parent = elb_mgr().new_GenBlock(parent, ast_genblock);
  }
  phase1_generate(parent, ast_genblock);
}

// @brief generate if に対応するインスタンスの生成を行う
void
ItemGen::phase1_genif(
  const VlScope* parent,
  const AstItem& ast_genif
)
{
  auto ast_cond = ast_genif.cond_expr();
  bool cond = evaluate_bool(parent, ast_cond);
  if ( cond ) {
    phase1_genitem(parent,
		   ast_genif.then_declhead_list(),
		   ast_genif.then_item_list());
  }
  else {
    phase1_genitem(parent,
		   ast_genif.else_declhead_list(),
		   ast_genif.else_item_list());
  }
}

// @brief generate case に対応するインスタンスの生成を行う
void
ItemGen::phase1_gencase(
  const VlScope* parent,
  const AstItem& ast_gencase
)
{
  auto ast_expr = ast_gencase.cond_expr();
  auto val = evaluate_bitvector(parent, ast_expr);

  for ( auto ast_caseitem: ast_gencase.caseitem_list() ) {
    // default(ラベルリストが空) なら常にマッチする．
    bool match = ast_caseitem.label_list().empty();
    for ( auto ast_expr: ast_caseitem.label_list() ) {
      auto label_val = evaluate_bitvector(parent, ast_expr);
      if ( label_val == val ) {
	match = true;
	break;
      }
    }
    if ( match ) {
      // マッチした．
      phase1_genitem(parent,
		     ast_caseitem.declhead_list(),
		     ast_caseitem.item_list());
      return;
    }
  }
  // マッチが見つからなかったらなにもしない
}

// @brief generate for に対応するインスタンスの生成を行う
void
ItemGen::phase1_genfor(
  const VlScope* parent,
  const AstItem& ast_genfor
)
{
  // Genvar を使用中にするオブジェクト
  // デストラクタの起動されるタイミングで
  // 使用中のフラグを降ろす．
  // このメカニズムにより例外が送出されても正しく働く
  class GenvarHolder
  {
  public:

    GenvarHolder(ElbGenvar* genvar) :
      mGenvar{genvar}
    {
      mGenvar->set_inuse();
    }

    ~GenvarHolder()
    {
      mGenvar->reset_inuse();
    }

  private:

    ElbGenvar* mGenvar;

  };

  auto name0 = ast_genfor.name();
  if ( name0 == nullptr ) {
    throw std::logic_error{"name0 == nullptr"};
  }

  auto handle = elb_mgr().find_obj(parent, ast_genfor.loop_var());
  if ( handle == nullptr ) {
    // 見つからなかった．
    log_mgr().error_genvar_not_found(__FILE__, __LINE__, ast_genfor);
  }

  auto genvar = handle->genvar();
  if ( genvar == nullptr ) {
    // genvar ではなかった．
    log_mgr().error_not_a_genvar(__FILE__, __LINE__, ast_genfor);
  }
  if ( genvar->is_inuse() ) {
    // すでに他の generate-for loop が使用中
    log_mgr().error_genvar_in_use(__FILE__, __LINE__, ast_genfor);
  }

  // genvar を使用中にする．
  GenvarHolder holder(genvar);

  // 子供のスコープの検索用オブジェクト
  auto gfroot = elb_mgr().new_GfRoot(parent, ast_genfor);

  auto ast_init_expr = ast_genfor.init_expr();
  auto init_val = evaluate_int(parent, ast_init_expr);
  if ( init_val < 0 ) {
    log_mgr().error_genvar_negative(__FILE__, __LINE__, ast_genfor);
  }
  genvar->set_value(init_val);

  for ( ; ; ) {
    // 終了条件のチェック
    auto ast_cond_expr = ast_genfor.cond_expr();
    bool cond_val = evaluate_bool(parent, ast_cond_expr);
    if ( !cond_val ) {
      break;
    }

    // スコープ名生成のために genvar の値を取得
    {
      int gvi = genvar->value();
      auto genblock = elb_mgr().new_GfBlock(parent, ast_genfor, gvi);
      gfroot->add(gvi, genblock);

      auto ast_item = genvar->ast_item();
      auto genvar1 = elb_mgr().new_Genvar(genblock, ast_item, gvi);

      phase1_generate(genblock, ast_genfor);
    }

    // genvar の増加分の処理．
    auto ast_next_expr = ast_genfor.next_expr();
    auto next_val = evaluate_int(parent, ast_next_expr);
    if ( next_val < 0 ) {
      log_mgr().error_genvar_negative(__FILE__, __LINE__, ast_genfor);
    }
    genvar->set_value(next_val);
  }
}

// @brief generate block の要素でスコープに関連するものの生成を行う．
void
ItemGen::phase1_genitem(
  const VlScope* parent,
  const AstDeclHeadList& ast_decl_list,
  const AstItemList& ast_item_list
)
{
  phase1_items(parent, ast_item_list);
  auto stub = make_stub<ElbProxy,
			const VlScope*,
			const AstDeclHeadList&>(static_cast<ElbProxy*>(this),
						&ElbProxy::instantiate_decl,
						parent, ast_decl_list);
  add_phase2stub(stub);
}

END_NAMESPACE_YM_VERILOG
