
/// @file ModuleGen.cc
/// @brief ElbMgr の実装ファイル (module のインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ModuleGen.h"
#include "ElbParamCon.h"
#include "ElbStub.h"

#include "ym/vl/AstPort.h"
#include "ym/vl/AstInst.h"
#include "ym/vl/AstPart.h"
#include "ym/vl/VlIODecl.h"
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
  const AstModule& ast_module
)
{
  const auto& file_region = ast_module.file_region();
  auto name = ast_module.name();

  {
    std::ostringstream buf;
    buf << "instantiating top module \"" << name << "\".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region,
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  auto prev_obj = find_obj(toplevel, name);
  if ( prev_obj != nullptr ) {
    // 同名のオブジェクト(たぶんモジュールかUDP)が存在した．
    error_module_redefined(__FILE__, __LINE__,
			   ast_module,
			   prev_obj->file_region());
  }

  // モジュール本体の生成
  auto module = mgr().new_Module(toplevel,
				 ast_module,
				 AstItem(),
				 AstInst());

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
  const AstModule& ast_module,
  const std::vector<ElbParamCon>& param_con_list
)
{
  // ループチェック用のフラグを立てる．
  set_instance_mark(ast_module);

  // パラメータポートを実体化する．
  auto has_paramportdecl = !ast_module.paramport_list().empty();
  if ( has_paramportdecl ) {
    phase1_decl(module, ast_module.paramport_list(), false);
  }

  // parameter と genvar を実体化する．
  phase1_decl(module, ast_module.declhead_list(), has_paramportdecl);

  // パラメータの割り当てを作る．
  bool named_con = (!param_con_list.empty() &&
		    param_con_list[0].mAstCon.name() != nullptr);
  // パラメータポートリストの名前を現れた順番に paramport_list に入れる．
  std::vector<const char*> paramport_list;
  if ( named_con ) {
    // 名前による割り当て
    for ( const auto& param_con: param_con_list ) {
      auto ast_con = param_con.mAstCon;
      paramport_list.push_back(ast_con.name());
    }
  }
  else {
    // 順序による割り当て
    if ( has_paramportdecl ) {
      for ( auto ast_param: ast_module.paramport_list() ) {
	for ( auto ast_item: ast_param.item_list() ) {
	  paramport_list.push_back(ast_item.name());
	}
      }
    }
    else {
      for ( auto ast_decl: ast_module.declhead_list() ) {
	if ( ast_decl.type() == AstDeclHead::Param ) {
	  for ( auto ast_item: ast_decl.item_list() ) {
	    paramport_list.push_back(ast_item.name());
	  }
	}
      }
    }
    if ( paramport_list.size() < param_con_list.size() ) {
      // 実際のパラメータの数より割り当てリストの要素数が多い．
      error_too_many_params(__FILE__, __LINE__, param_con_list);
    }
  }

  // param_con を paramport_list の名前と結びつける．
  // named_con の場合には冗長なことをやっている．
  SizeType index = 0;
  for ( const auto& param_con: param_con_list ) {
    auto ast_con = param_con.mAstCon;
    auto name = paramport_list[index]; ++ index;
    auto handle = find_obj(module, name);
    if ( handle == nullptr || handle->type() != VpiObjType::Parameter ) {
      error_param_not_found(__FILE__, __LINE__, ast_con);
    }

    auto param = handle->parameter();
    if ( param == nullptr ) {
      // handle->type() == VpiObjType::Parameter なので起こらないはず．
      throw std::logic_error{"param == nullptr"};
    }

    auto expr = param_con.mExpr;
    auto value = param_con.mValue;
    param->set_init_expr(expr, value);
    // 仮想的な パラメータ割り当て文があるものとみなす．
    auto pa = mgr().new_NamedParamAssign(module, ast_con,
					 param, expr, value);
  }

  // それ以外の要素を実体化する．
  phase1_items(module, ast_module.item_list());

  // phase2 で行う処理を登録しておく．
  add_phase2stub(module, ast_module);

  // ループチェック用のフラグを下ろす．
  clear_instance_mark(ast_module);
}

// @brief module の中身のインスタンス化を行う．
void
ModuleGen::phase2_module_item(
  ElbModule* module,
  const AstModule& ast_module
)
{
  // 宣言要素を実体化する．
  instantiate_decl(module, ast_module.declhead_list());

  // IODecl を実体化する．
  instantiate_iodecl(module, ast_module.iohead_list());

  // ポートを実体化する
  if ( ast_module.port_list().empty() ) {
    // Verilog2001 形式
    instantiate_ports(module, ast_module.iohead_list());
  }
  else {
    // verilog1995 形式
    instantiate_ports(module, ast_module.port_list());
  }
}

// port の生成を行う．
void
ModuleGen::instantiate_ports(
  ElbModule* module,
  const AstPortList& ast_port_list
)
{
  // ポート名の重複チェックを行う．
  std::unordered_map<std::string, AstPort> port_dict;
  for ( auto ast_port: ast_port_list ) {
    auto name = ast_port.ext_name();
    if ( name != nullptr && port_dict.count(name) > 0 ) {
      auto prev_port = port_dict.at(name);
      error_dup_name(__FILE__, __LINE__,
		     ast_port.file_region(),
		     name,
		     prev_port.file_region());
    }
  }

  for ( auto ast_port: ast_port_list ) {
    // 内側の接続と向きを作る．
    auto n = ast_port.portref_list().size();
    if ( n == 0 ) {
      // 空のポートの場合
      module->add_port(AstPort(), nullptr, VpiDir::NoDirection);
    }
    else if ( n == 1 ) {
      // 単一の要素の場合
      auto ast_expr = ast_port.portref_list().front();
      VpiDir dir;
      auto low_conn = instantiate_portref(module, ast_expr, dir);
      module->add_port(ast_port, low_conn, dir);
    }
    else if ( n > 1 ) {
      // 複数要素の結合の場合
      std::vector<ElbExpr*> expr_list;
      expr_list.reserve(n);
      VpiDir dir = VpiDir::NoDirection;
      for ( auto ast_expr: ast_port.portref_list() ) {
	VpiDir dir1;
	auto expr = instantiate_portref(module, ast_expr, dir1);
	if ( !expr ) {
	  // エラーが起こった．
	  return;
	}
	expr_list.push_back(expr);
	if ( dir == VpiDir::NoDirection ) {
	  dir = dir1;
	}
	else {
	  dir = VpiDir::MixedIO;
	}
      }

      // lhs_elem_array は expr_list の逆順にする．
      std::vector<ElbExpr*> lhs_elem_array(n);
      for ( SizeType i = 0; i < n; ++ i ) {
	auto expr = expr_list[i];
	lhs_elem_array[n - i - 1] = expr;
      }

      auto low_conn = mgr().new_Lhs(ast_port.expr(), expr_list, lhs_elem_array);
      module->add_port(ast_port, low_conn, dir);
    }
  }
}

// AstPortRef から expression を生成する．
ElbExpr*
ModuleGen::instantiate_portref(
  ElbModule* module,
  const AstExpr& ast_expr,
  VpiDir& dir
)
{
  auto name = ast_expr.name();
  auto handle = find_obj(module, name);
  if ( handle == nullptr ) {
    // name という名の要素がなかった．
    error_not_found(__FILE__, __LINE__,
		    ast_expr.file_region(), name);
  }

  if ( handle->declarray() ) {
    // 配列要素は IO ポートに使えない．
    error_array_in_port_connection(__FILE__, __LINE__,
				   ast_expr.file_region());
  }

  auto decl = handle->decl();
  if ( decl == nullptr ) {
    // 宣言要素ではなかった．
    error_illegal_port(__FILE__, __LINE__,
		       ast_expr.file_region());

  }
  auto io_decl = module->find_io(decl);
  if ( io_decl == nullptr ) {
    // 入出力宣言ではなかった．
    error_illegal_port(__FILE__, __LINE__,
		       ast_expr.file_region());
  }
  dir = io_decl->direction();

  auto primary = mgr().new_Primary(ast_expr, decl);

  // 添字の部分を実体化する．
  AstExpr ast_index;
  if ( !ast_expr.index_list().empty() ) {
    ast_index = ast_expr.index_list().front();
  }
  if ( ast_index.is_valid() ) {
    int index_val = evaluate_int(module, ast_index);
    SizeType offset;
    bool stat2 = decl->calc_bit_offset(index_val, offset);
    if ( !stat2 ) {
      // 添字が範囲外
      warning_index_out_of_range(ast_index.file_region());
    }
    return mgr().new_BitSelect(ast_expr, primary, ast_index, index_val);
  }
  auto ast_part = ast_expr.part();
  if ( ast_part.is_valid() ) {
    auto range = evaluate_range(module, ast_part);
    SizeType offset;
    bool stat1 = decl->calc_bit_offset(range.left, offset);
    if ( !stat1 ) {
      // 左の添字が範囲外
      warning_left_index_out_of_range(ast_part.left().file_region());
    }
    bool stat2 = decl->calc_bit_offset(range.right, offset);
    if ( !stat2 ) {
      // 右の添字が範囲外
      warning_right_index_out_of_range(ast_part.right().file_region());
    }
    return mgr().new_PartSelect(ast_expr, primary,
				ast_part.left(), ast_part.right(),
				range.left, range.right);
  }
  return primary;
}

// port の生成を行う．
void
ModuleGen::instantiate_ports(
  ElbModule* module,
  const AstIOHeadList& ast_iohead_list
)
{
  // 内側の接続と向きを作る．
  // Verilog2001 形式の場合は単純で対応する入出力要素と1対1の対応が付く．
  // 名前の重複チェックも instantiate_iodecl() で行っているはず．
  for ( auto ast_iohead: ast_iohead_list ) {
    auto dir = ast_iohead.direction();
    for ( auto ast_ioitem: ast_iohead.item_list() ) {
      auto name = ast_ioitem.name();
      auto handle = find_obj(module, name);
      if ( handle == nullptr ) {
	error_not_found(__FILE__, __LINE__,
			ast_ioitem.file_region(),
			name);
      }

      if ( handle->declarray() ) {
	error_array_in_port_connection(__FILE__, __LINE__,
				       ast_ioitem.file_region());
      }

      auto decl = handle->decl();
      if ( decl == nullptr ) {
	// ast_ioitem は instantiate_iodecl() で正しく生成されているはず．
	throw std::logic_error{"decl == nullptr"};
      }
      auto io_decl = module->find_io(decl);
      if ( io_decl == nullptr ) {
	// ast_ioitem は instantiate_iodecl() で正しく生成されているはず．
	throw std::logic_error{"decl == nullptr"};
      }

      auto low_conn = mgr().new_Primary(ast_ioitem, decl);
      module->add_port(ast_ioitem, low_conn, dir);
    }
  }
}

END_NAMESPACE_YM_VERILOG
