
/// @file ModuleGen.cc
/// @brief ElbMgr の実装ファイル (module のインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ModuleGen.h"
#include "ElbParamCon.h"
#include "ElbStub.h"
#include "ErrorGen.h"

#include "ym/vl/AstModule.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstDecl.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbDecl.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/RangeVal.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ModuleGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ModuleGen::ModuleGen(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy{elab, elb_mgr}
{
}

// @brief デストラクタ
ModuleGen::~ModuleGen()
{
}

// top module 用の骨組みを作る．
void
ModuleGen::phase1_topmodule(
  const VlScope* toplevel,
  const AstModule* ast_module
)
{
  const auto& file_region = ast_module->file_region();
  auto name = ast_module->name();

  {
    std::ostringstream buf;
    buf << "instantiating top module \"" << name << "\".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region,
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  // モジュール本体の生成
  auto module = mgr().new_Module(toplevel,
				 ast_module,
				 nullptr,
				 nullptr);

  // attribute instance の生成
  const auto& attr_list = attribute_list(ast_module);
  mgr().reg_attr(module, attr_list);

  {
    std::ostringstream buf;
    buf << "module \"" << module->full_name() << "\" has been created.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region,
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  // 中身のうちスコープに関係する要素の生成
  phase1_module_item(module, ast_module, std::vector<ElbParamCon>());
}

// @brief module の中身のうちスコープに関係する要素のインスタンス化をする．
void
ModuleGen::phase1_module_item(
  ElbModule* module,
  const AstModule* ast_module,
  const std::vector<ElbParamCon>& param_con_list
)
{
  // ループチェック用のフラグを立てる．
  ast_module->set_in_use();

  // パラメータポートを実体化する．
  bool has_paramportdecl = (ast_module->paramport_num() > 0);
  if ( has_paramportdecl ) {
    phase1_decl(module, ast_module->paramport_list(), false);
  }

  // parameter と genvar を実体化する．
  phase1_decl(module, ast_module->declhead_list(), has_paramportdecl);

  // パラメータの割り当てを作る．
  bool named_con = (param_con_list.size() > 0 &&
		    param_con_list[0].mAstCon->name() != nullptr);
  // パラメータポートリストの名前を現れた順番に paramport_list に入れる．
  std::vector<const char*> paramport_list;
  if ( named_con ) {
    // 名前による割り当て
    for ( const auto& param_con: param_con_list ) {
      auto ast_con = param_con.mAstCon;
      paramport_list.push_back(ast_con->name());
    }
  }
  else {
    // 順序による割り当て
    if ( has_paramportdecl ) {
      for ( auto ast_param: ast_module->paramport_list() ) {
	for ( auto ast_item: ast_param->item_list() ) {
	  paramport_list.push_back(ast_item->name());
	}
      }
    }
    else {
      for ( auto ast_decl: ast_module->declhead_list() ) {
	if ( ast_decl->type() == AstDeclHead::Param ) {
	  for ( auto ast_item: ast_decl->item_list() ) {
	    paramport_list.push_back(ast_item->name());
	  }
	}
      }
    }
    if ( paramport_list.size() < param_con_list.size() ) {
      // 実際のパラメータの数より割り当てリストの要素数が多い．
      ErrorGen::too_many_param(__FILE__, __LINE__, param_con_list);
    }
  }

  // param_con を paramport_list の名前と結びつける．
  // named_con の場合には冗長なことをやっている．
  SizeType index{0};
  for ( const auto& param_con: param_con_list ) {
    auto ast_con = param_con.mAstCon;
    auto name = paramport_list[index]; ++ index;
    auto handle = mgr().find_obj(module, name);
    if ( handle == nullptr || handle->type() != VpiObjType::Parameter ) {
      ErrorGen::no_param(__FILE__, __LINE__, ast_con, name);
    }

    auto param = handle->parameter();
    if ( param == nullptr ) {
      throw std::logic_error{"param == nullptr"};
    }

    auto expr = param_con.mExpr;
    auto value = param_con.mValue;
    param->set_init_expr(expr, value);

    auto pa = mgr().new_NamedParamAssign(module, ast_con,
					 param, expr, value);
  }

  // それ以外の要素を実体化する．
  phase1_items(module, ast_module->item_list());

  // phase2 で行う処理を登録しておく．
  add_phase2stub(module, ast_module);

  // ループチェック用のフラグを下ろす．
  ast_module->reset_in_use();
}

// @brief module の中身のインスタンス化を行う．
void
ModuleGen::phase2_module_item(
  ElbModule* module,
  const AstModule* ast_module
)
{
  // 宣言要素を実体化する．
  instantiate_decl(module, ast_module->declhead_list());

  // IODecl を実体化する．
  instantiate_iodecl(module, ast_module->iohead_list());

  // ポートを実体化する
  SizeType index = 0;
  for ( auto ast_port: ast_module->port_list() ) {
    instantiate_port(module, index, ast_port);
    ++ index;
  }
}

// port の生成を行う．
void
ModuleGen::instantiate_port(
  ElbModule* module,
  SizeType index,
  const AstPort* ast_port
)
{
  // 内側の接続と向きを作る．
  auto n = ast_port->portref_size();
  if ( n == 0 ) {
    // 空のポートの場合
    module->init_port(index, nullptr, nullptr, VpiDir::NoDirection);
  }
  else if ( n == 1 ) {
    // 単一の要素の場合
    auto ast_expr = ast_port->portref(0);
    auto dir = ast_port->portref_dir(0);
    auto low_conn = instantiate_portref(module, ast_expr);
    module->init_port(index, ast_port, low_conn, dir);
  }
  else if ( n > 1 ) {
    // 複数要素の結合の場合
    std::vector<ElbExpr*> expr_list;
    expr_list.reserve(n);
    auto dir = VpiDir::NoDirection;
    for ( SizeType i = 0; i < ast_port->portref_size(); ++ i ) {
      auto ast_expr = ast_port->portref(i);
      auto expr = instantiate_portref(module, ast_expr);
      if ( !expr ) {
	return;
      }

      auto dir1 = ast_port->portref_dir(expr_list.size());
      if ( dir == VpiDir::NoDirection ) {
	dir = dir1;
      }
      else if ( dir != dir1 ) {
	dir = VpiDir::MixedIO;
      }
      expr_list.push_back(expr);
    }

    // lhs_elem_array は expr_list の逆順にする．
    std::vector<ElbExpr*> lhs_elem_array(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto expr = expr_list[i];
      lhs_elem_array[n - i - 1] = expr;
    }

    auto low_conn = mgr().new_Lhs(ast_port->expr(), expr_list, lhs_elem_array);
    module->init_port(index, ast_port, low_conn, dir);
  }
}

// AstPortRef から expression を生成する．
ElbExpr*
ModuleGen::instantiate_portref(
  ElbModule* module,
  const AstExpr* ast_expr
)
{
  auto name = ast_expr->name();
  auto handle = mgr().find_obj(module, name);
  if ( !handle ) {
    ErrorGen::not_found(__FILE__, __LINE__,
			ast_expr->file_region(), name);
  }

  if ( handle->declarray() ) {
    ErrorGen::port_array(__FILE__, __LINE__,
			 ast_expr->file_region(), handle->declarray());
  }

  auto decl = handle->decl();
  if ( decl == nullptr ) {
    ErrorGen::illegal_port(__FILE__, __LINE__,
			   ast_expr->file_region(), name);
  }

  auto primary = mgr().new_Primary(ast_expr, decl);

  // 添字の部分を実体化する．
  const AstExpr* ast_index = nullptr;
  if ( ast_expr->index_num() > 0 ) {
    ast_index = ast_expr->index_list().front();
  }
  if ( ast_index ) {
    int index_val = evaluate_int(module, ast_index);
    SizeType offset;
    bool stat2 = decl->calc_bit_offset(index_val, offset);
    if ( !stat2 ) {
      // 添字が範囲外
      warning_index_out_of_range(ast_index->file_region());
    }
    return mgr().new_BitSelect(ast_expr, primary, ast_index, index_val);
  }
  auto ast_part = ast_expr->part();
  if ( ast_part != nullptr ) {
    auto range = evaluate_range(module, ast_part);
    SizeType offset;
    bool stat1 = decl->calc_bit_offset(range.left, offset);
    if ( !stat1 ) {
      // 左の添字が範囲外
      warning_left_index_out_of_range(ast_part->left()->file_region());
    }
    bool stat2 = decl->calc_bit_offset(range.right, offset);
    if ( !stat2 ) {
      // 右の添字が範囲外
      warning_right_index_out_of_range(ast_part->right()->file_region());
    }
    return mgr().new_PartSelect(ast_expr, primary,
				ast_part->left(), ast_part->right(),
				range.left, range.right);
  }
  return primary;
}

// @brief 添字が範囲外
void
ModuleGen::warning_index_out_of_range(
  const FileRegion& file_region
)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Index is out of range.");
}

// @brief 左の範囲が範囲外
void
ModuleGen::warning_left_index_out_of_range(
  const FileRegion& file_region
)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Left index is out of range.");
}

// @brief 右の範囲が範囲外
void
ModuleGen::warning_right_index_out_of_range(
  const FileRegion& file_region
)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Right index is out of range.");
}

END_NAMESPACE_YM_VERILOG
