
/// @file ItemGen.cc
/// @brief ElbMgr の実装ファイル(要素の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "DeclGen.h"
#include "DefParamStub.h"
#include "ElbEnv.h"
#include "ElbStub.h"
#include "ElbError.h"
#include "ErrorGen.h"
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

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// item 関係の生成を行うクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ItemGen::ItemGen(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy{elab, elb_mgr}
{
}

// @brief デストラクタ
ItemGen::~ItemGen()
{
}

// @brief スコープに関係する要素を実体化する．
void
ItemGen::phase1_items(
  const VlScope* parent,
  const AstItemList& ast_item_list
)
{
  for ( auto ast_item: ast_item_list ) {
    try {
      phase1_item(parent, ast_item);
    }
    catch ( const ElbError& error ) {
      put_error(error);
    }
  }
}

// @brief スコープに関係する要素を実体化する．
void
ItemGen::phase1_item(
  const VlScope* parent,
  const AstItem& ast_item
)
{
  switch ( ast_item.type() ) {
  case AstItem::DefParam:
    // 実際には登録するだけ
    add_defparamstub(parent->parent_module(), ast_item);
    break;

  case AstItem::ContAssign:
    // phase3 で処理する．
    add_phase3stub(cont_assign_stub(parent, ast_item));
    break;

  case AstItem::Initial:
  case AstItem::Always:
    phase1_stmt(parent, ast_item.body());
    // 本体の生成は phase3 で処理する．
    add_phase3stub(process_stub(parent, ast_item));
    break;

  case AstItem::Task:
  case AstItem::Func:
    phase1_tf(parent, ast_item);
    break;

  case AstItem::GateInst:
    // 今すぐには処理できないのでキューに積む．
    add_phase2stub(gateheader_stub(parent, ast_item));
    break;

  case AstItem::MuInst:
    phase1_muheader(parent, ast_item);
    break;

  case AstItem::Generate:
    // 実際にはキューに積まれるだけ
    add_phase1stub(generate_stub(parent, ast_item));
    break;

  case AstItem::GenBlock:
    phase1_genblock(parent, ast_item);
    break;

  case AstItem::GenIf:
    phase1_genif(parent, ast_item);
    break;

  case AstItem::GenCase:
    phase1_gencase(parent, ast_item);
    break;

  case AstItem::GenFor:
    phase1_genfor(parent, ast_item);
    break;

  case AstItem::SpecItem:
    // 未対応
    break;

  case AstItem::SpecPath:
    // 未対応
    break;

  default:
    throw std::logic_error{"Should not be reached"};
    break;
  }
}

// defparam 文によるパラメータ割り当てを行う．
bool
ItemGen::defparam_override(
  const DefParamStub& stub,
  const VlScope* ulimit
)
{
  // この時点では階層展開が済んでいないため存在しない名前の場合があるので
  // 即エラーとはせずに false を返す．

  auto module = stub.mModule;
  auto ast_header = stub.mAstHeader;
  auto ast_defparam = stub.mAstDefparam;

  auto handle = mgr().find_obj_up(module, ast_defparam, ulimit);
  if ( handle == nullptr ) {
    // 見つからなかった．
    // エラーとは限らない
    return false;
  }

  auto param = handle->parameter();
  if ( param == nullptr ) {
    // 対象がパラメータではなかった．
    try {
      error_not_a_parameter(__FILE__, __LINE__, ast_defparam);
    }
    // ただし無視する．
    catch ( const ElbError& error ) {
      put_error(error);
    }

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  if ( param->is_local_param() ) {
    // 対象が localparam だった(書き換えできない)
    try {
      error_localparam_override(__FILE__, __LINE__, ast_defparam);
    }
    // ただし無視する．
    catch ( const ElbError& error ) {
      put_error(error);
    }

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  auto ast_rhs_expr = ast_defparam.expr();
  auto value = evaluate_expr(module, ast_rhs_expr);

  {
    std::ostringstream buf;
    buf << "instantiating defparam: " << param->full_name()
	<< " = " << ast_rhs_expr.decompile() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    ast_defparam.file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  param->set_init_expr(ast_rhs_expr, value);

  auto dp = mgr().new_DefParam(module,
			       ast_header,
			       ast_defparam,
			       param, ast_rhs_expr, value);

  return true;
}

// @brief continous assignment に関連した式の名前解決を行う．
void
ItemGen::instantiate_cont_assign_head(
  const VlScope* parent,
  const AstItem& ast_header
)
{
  // delay の実体化でエラーが置きても nullptr になっているだけで処理を続ける．
  // エラーメッセージは出力されている．
  auto module = parent->parent_module();
  auto ast_delay = ast_header.delay();
  auto delay = instantiate_delay(parent, ast_delay);
  auto ca_head = mgr().new_CaHead(module, ast_header, delay);

  ElbEnv env;
  ElbNetLhsEnv env1(env);
  for ( auto ast_contassign: ast_header.contassign_list() ) {
    try {
      // 左辺式の生成
      auto ast_lhs = ast_contassign.lhs();
      auto lhs = instantiate_lhs(parent, env1, ast_lhs);

      // 右辺式の生成
      auto ast_rhs = ast_contassign.rhs();
      auto rhs = instantiate_rhs(parent, env, ast_rhs, lhs);

      auto ca = mgr().new_ContAssign(ca_head, ast_contassign, lhs, rhs);

      {
	std::ostringstream buf;
	buf << "instantiating continuous assign: "
	    << lhs->decompile() << " = " << rhs->decompile() << ".";
	MsgMgr::put_msg(__FILE__, __LINE__,
			ast_contassign.file_region(),
			MsgType::Info,
			"ELAB",
			buf.str());
      }
    }
    catch ( const ElbError& error ) {
      put_error(error);
    }
  }
}

// @brief process 文の生成を行う．
void
ItemGen::instantiate_process(
  const VlScope* parent,
  const AstItem& ast_item
)
{
  try {
    auto process = mgr().new_Process(parent, ast_item);

    ElbEnv env;
    auto body = instantiate_stmt(parent, process, env,
				 ast_item.body());
    process->set_stmt(body);
  }
  catch ( const ElbError& error ) {
    put_error(error);
  }
}

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
    parent = mgr().new_GenBlock(parent, ast_genblock);
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

  auto handle = mgr().find_obj(parent, ast_genfor.loop_var());
  if ( handle == nullptr ) {
    // 見つからなかった．
    error_genvar_not_found(__FILE__, __LINE__, ast_genfor);
  }

  auto genvar = handle->genvar();
  if ( genvar == nullptr ) {
    // genvar ではなかった．
    error_not_a_genvar(__FILE__, __LINE__, ast_genfor);
  }
  if ( genvar->is_inuse() ) {
    // すでに他の generate-for loop が使用中
    error_genvar_in_use(__FILE__, __LINE__, ast_genfor);
  }

  // genvar を使用中にする．
  GenvarHolder holder(genvar);

  // 子供のスコープの検索用オブジェクト
  auto gfroot = mgr().new_GfRoot(parent, ast_genfor);

  auto ast_init_expr = ast_genfor.init_expr();
  auto init_val = evaluate_int(parent, ast_init_expr);
  if ( init_val < 0 ) {
    error_genvar_negative(__FILE__, __LINE__, ast_genfor);
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
      auto genblock = mgr().new_GfBlock(parent, ast_genfor, gvi);
      gfroot->add(gvi, genblock);

      auto ast_item = genvar->ast_item();
      auto genvar1 = mgr().new_Genvar(genblock, ast_item, gvi);

      phase1_generate(genblock, ast_genfor);
    }

    // genvar の増加分の処理．
    auto ast_next_expr = ast_genfor.next_expr();
    auto next_val = evaluate_int(parent, ast_next_expr);
    if ( next_val < 0 ) {
      error_genvar_negative(__FILE__, __LINE__, ast_genfor);
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
