
/// @file ItemGen.cc
/// @brief ItemGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "DefParamStub.h"
#include "ElbEnv.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstContAssign.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbProcess.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbError.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ItemGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ItemGen::ItemGen(
  Elaborator& elab
) : ElbProxy{elab}
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
      // 無視して処理を続ける．
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

  auto handle = elb_mgr().find_obj_up(module, ast_defparam, ulimit);
  if ( handle == nullptr ) {
    // 見つからなかった．
    // エラーとは限らない
    return false;
  }

  auto param = handle->parameter();
  if ( param == nullptr ) {
    // 対象がパラメータではなかった．
    try {
      log_mgr().error_not_a_parameter(__FILE__, __LINE__,
				      ast_defparam.file_region(),
				      ast_defparam.decompile_name());
    }
    catch ( const ElbError& error ) {
      // ただし無視する．
    }

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  if ( param->is_local_param() ) {
    // 対象が localparam だった(書き換えできない)
    try {
      log_mgr().error_localparam_override(__FILE__, __LINE__, ast_defparam);
    }
    catch ( const ElbError& error ) {
      // ただし無視する．
    }

    // もうこれ以降は処理したくないので true を返す．
    return true;
  }

  auto ast_rhs_expr = ast_defparam.expr();
  auto value = evaluate_expr(module, ast_rhs_expr);

  log_mgr().info_defparam(__FILE__, __LINE__,
			  ast_defparam.file_region(),
			  param, ast_rhs_expr);
  param->set_init_expr(ast_rhs_expr, value);

  auto dp = elb_mgr().new_DefParam(module,
				   ast_header,
				   ast_defparam,
				   param, ast_rhs_expr, value);

  return true;
}

// @brief continous assignment に関連した式の名前解決を行う．
void
ItemGen::instantiate_cont_assign_head(
  const VlScope* parent,
  AstItem ast_header
)
{
  // delay の実体化でエラーが置きても nullptr になっているだけで処理を続ける．
  // エラーメッセージは出力されている．
  auto module = parent->parent_module();
  auto ast_delay = ast_header.delay();
  auto delay = instantiate_delay(parent, ast_delay);
  auto ca_head = elb_mgr().new_CaHead(module, ast_header, delay);

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

      auto loc = ast_contassign.file_region();
      auto ca = elb_mgr().new_ContAssign(ca_head, loc, lhs, rhs);

      log_mgr().info_contassign(__FILE__, __LINE__, ca);
    }
    catch ( const ElbError& error ) {
      // 無視して処理を続ける．
    }
  }
}

// @brief process 文の生成を行う．
void
ItemGen::instantiate_process(
  const VlScope* parent,
  AstItem ast_item
)
{
  try {
    auto process = elb_mgr().new_Process(parent, ast_item);

    ElbEnv env;
    auto body = instantiate_stmt(parent, process, env,
				 ast_item.body());
    process->set_stmt(body);
  }
  catch ( const ElbError& error ) {
    // 無視して処理を続ける．
  }
}

END_NAMESPACE_YM_VERILOG
