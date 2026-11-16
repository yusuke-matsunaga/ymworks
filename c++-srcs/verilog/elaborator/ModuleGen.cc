
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
#include "elaborator/ElbDeclRef.h"
#include "elaborator/ElbSelecter.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ModuleGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ModuleGen::ModuleGen(
  Elaborator& elab
) : ElbProxy{elab}
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

  log_mgr().info_topmodule(__FILE__, __LINE__,
			   file_region,
			   name);

  auto prev_obj = find_obj(toplevel, name);
  if ( prev_obj != nullptr ) {
    // トップモジュール名はモジュール定義名なので重複しないはず．
    // (というか重複していたらそれ以前にエラーになっている)
    throw std::logic_error{"prev_obj != nullptr"};
  }

  // モジュール本体の生成
  auto module = elb_mgr().new_Module(toplevel,
				     ast_module,
				     AstItem(),
				     AstInst());

  // attribute instance の生成
  const auto& attr_list = attribute_list(ast_module);
  elb_mgr().reg_attr(module, attr_list);

  log_mgr().info_module(__FILE__, __LINE__, module);

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
      auto last = param_con_list[paramport_list.size()];
      log_mgr().error_too_many_params(__FILE__, __LINE__,
				      last.mAstCon.file_region());
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
      log_mgr().error_param_not_found(__FILE__, __LINE__, ast_con);
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
    auto loc = ast_con.file_region();
    auto pa = elb_mgr().new_NamedParamAssign(module, loc,
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
  AstModule ast_module
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
      log_mgr().error_dup_name(__FILE__, __LINE__,
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
      module->add_port(FileRegion{}, nullptr, nullptr, VpiDir::NoDirection);
    }
    else if ( n == 1 ) {
      // 単一の要素の場合
      auto ast_expr = ast_port.portref_list().front();
      auto portref = instantiate_portref(module, ast_expr);
      auto low_conn = elb_mgr().new_Primary(portref.primary);
      auto dir = portref.dir;
      module->add_port(ast_port.file_region(),
		       ast_port.ext_name(),
		       low_conn, dir);
    }
    else if ( n > 1 ) {
      // 複数要素の結合の場合
      std::vector<const VlExpr*> expr_list;
      expr_list.reserve(n);
      VpiDir dir = VpiDir::NoDirection;
      for ( auto ast_expr: ast_port.portref_list() ) {
	auto portref = instantiate_portref(module, ast_expr);
	auto primary = elb_mgr().new_Primary(portref.primary);
	expr_list.push_back(primary);
	if ( dir == VpiDir::NoDirection ) {
	  dir = portref.dir;
	}
	else if ( dir != portref.dir ) {
	  dir = VpiDir::MixedIO;
	}
      }
      auto low_conn = elb_mgr().new_Lhs(expr_list);
      module->add_port(ast_port.file_region(),
		       ast_port.ext_name(),
		       low_conn, dir);
    }
  }
}

// @brief ポート参照式の生成を行う．
ModuleGen::PortRefOut
ModuleGen::instantiate_portref(
  ElbModule* module,
  const AstExpr& ast_expr
)
{
  auto name = ast_expr.name();
  auto handle = find_obj(module, name);
  if ( handle == nullptr ) {
    // name という名の要素がなかった．
    log_mgr().error_not_found(__FILE__, __LINE__,
			      ast_expr.file_region(), name);
  }

  if ( handle->declarray() != nullptr ) {
    // 配列要素は IO ポートに使えない．
    log_mgr().error_array_in_port_connection(__FILE__, __LINE__,
					     ast_expr.file_region());
  }

  auto decl = handle->decl();
  if ( decl == nullptr ) {
    // 宣言要素ではなかった．
    log_mgr().error_illegal_port(__FILE__, __LINE__,
				 ast_expr.file_region());

  }

  // 対応する入出力宣言を求める．
  // 方向の取得のため
  auto io_decl = module->find_io(decl);
  if ( io_decl == nullptr ) {
    // 入出力宣言ではなかった．
    log_mgr().error_illegal_port(__FILE__, __LINE__,
				 ast_expr.file_region());
  }
  auto decl_ref = ElbDeclRef(decl);
  auto selecter = instantiate_selecter(module, ast_expr, decl);
  auto primary = ElbPrimary(ast_expr, decl_ref, selecter);
  auto dir = io_decl->direction();
  return PortRefOut{primary, dir};
}

// @brief ポート参照式のビット/部分選択子を作る．
ElbSelecter
ModuleGen::instantiate_selecter(
  const VlModule* module,
  const AstExpr& ast_expr,
  const VlDecl* decl
)
{
  // ビット選択子
  AstExpr ast_bit;
  if ( !ast_expr.index_list().empty() ) {
    ast_bit = ast_expr.index_list().front();
  }
  if ( ast_bit.is_valid() ) {
    // ビット選択
    int index_val = evaluate_int(module, ast_bit);
    SizeType offset;
    bool stat = decl->calc_bit_offset(index_val, offset);
    if ( !stat ) {
      // 添字が範囲外
      log_mgr().warning_index_out_of_range(__FILE__, __LINE__,
					   ast_bit.file_region());
    }
    return ElbSelecter::new_CONST(1, offset);
  }

  auto ast_part = ast_expr.part();
  if ( ast_part.is_valid() ) {
    // 部分選択
    auto range = evaluate_range(module, ast_part);
    SizeType left_offset;
    bool stat1 = decl->calc_bit_offset(range.left, left_offset);
    if ( !stat1 ) {
      // 左の添字が範囲外
      log_mgr().warning_left_index_out_of_range(__FILE__, __LINE__,
						ast_part.left().file_region());
    }
    SizeType right_offset;
    bool stat2 = decl->calc_bit_offset(range.right, right_offset);
    if ( !stat2 ) {
      // 右の添字が範囲外
      log_mgr().warning_right_index_out_of_range(__FILE__, __LINE__,
						 ast_part.right().file_region());
    }
    SizeType offset = std::min(left_offset, right_offset);
    SizeType width = std::max(left_offset, right_offset) - offset + 1;
    return ElbSelecter::new_CONST(width, offset);
  }

  // 選択なし
  return ElbSelecter::new_NONE(decl->bit_size());
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
	log_mgr().error_not_found(__FILE__, __LINE__,
				  ast_ioitem.file_region(),
				  name);
      }

      if ( handle->declarray() ) {
	log_mgr().error_array_in_port_connection(__FILE__, __LINE__,
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

      auto loc = ast_ioitem.file_region();
      auto low_conn = elb_mgr().new_Primary(loc, decl);
      module->add_port(loc, ast_ioitem.name(),
		       low_conn, dir);
    }
  }
}

END_NAMESPACE_YM_VERILOG
