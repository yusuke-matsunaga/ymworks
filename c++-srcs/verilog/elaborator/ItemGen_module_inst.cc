
/// @file ItemGen_module_inst.cc
/// @brief ElbMgr の実装ファイル(モジュールインスタンスの実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "ElbEnv.h"
#include "ElbParamCon.h"
#include "ErrorGen.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstModule.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"
#include "ym/vl/VlPort.h"
#include "elaborator/ElbModule.h"
#include "elaborator/ElbModuleArray.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/RangeVal.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// インスタンス関係の instantiate 関数 (変な日本語)
//////////////////////////////////////////////////////////////////////

// @brief module instance のスコープ生成を行う．
void
ItemGen::phase1_muheader(
  const VlScope* parent,
  const AstItem* ast_head
)
{
  auto defname = ast_head->name();

  // モジュールを探す
  auto ast_module = find_moduledef(defname);
  if ( ast_module ) {
    // モジュール定義が見つかった．
    phase1_module(parent, ast_head, ast_module);
    return;
  }

  // 次に udp を探す．
  auto udpdefn = mgr().find_udp(defname);
  if ( udpdefn ) {
    phase1_udp(parent, ast_head, udpdefn);
    return;
  }

  // 正式な仕様にはないが，セルライブラリを探す．
  auto cell = find_cell(defname);
  if ( cell.is_valid() ) {
    phase1_cell(parent, ast_head, cell);
    return;
  }

  // どれもなければエラー
  ErrorGen::instance_not_found(__FILE__, __LINE__, ast_head);
}

// @brief module instance の生成を行う．
void
ItemGen::phase1_module(
  const VlScope* parent,
  const AstItem* ast_head,
  const AstModule* ast_module
)
{
  if ( ast_module->is_in_use() ) {
    // 依存関係が循環している．
    ErrorGen::cyclic_dependency(__FILE__, __LINE__, ast_module);
  }

  for ( auto ast_inst: ast_head->inst_list() ) {
    auto name = ast_inst->name();
    if ( name == nullptr ) {
      // 名無しのモジュールインスタンスはない
      ErrorGen::noname_module(__FILE__, __LINE__, ast_inst);
    }

    auto ast_range = ast_inst->range();
    if ( ast_range != nullptr ) {
      // 配列型は今すぐにはインスタンス化できない．
      add_phase1stub(make_stub(this, &ItemGen::phase1_module_array,
			       parent, ast_module, ast_head, ast_inst));
    }
    else {
      // 単一の要素
      auto module1 = mgr().new_Module(parent,
				      ast_module,
				      ast_head,
				      ast_inst);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_module, ast_head);
      mgr().reg_attr(module1, attr_list);

      {
	std::ostringstream buf;
	buf << "\"" << module1->full_name() << "\" has been created.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			ast_inst->file_region(),
			MsgType::Info,
			"ELAB",
			buf.str());
      }

      // パラメータ割り当て式の生成
      auto param_con_list = gen_param_con_list(parent, ast_head);
      phase1_module_item(module1, ast_module, param_con_list);

      add_phase3stub(make_stub(this, &ItemGen::link_module,
			       module1, ast_module, ast_inst));
    }
  }
}

// @brief module array のインスタンス化を行う．
void
ItemGen::phase1_module_array(
  const VlScope* parent,
  const AstModule* ast_module,
  const AstItem* ast_head,
  const AstInst* ast_inst
)
{
  auto defname = ast_head->name();

  auto name = ast_inst->name();
  auto ast_range = ast_inst->range();

  auto range = evaluate_range(parent, ast_range);
  auto module_array = mgr().new_ModuleArray(parent, ast_module,
					    ast_head, ast_inst,
					    ast_range, range);

  {
    std::ostringstream buf;
    buf << "instantiating module array \"" << name << "\" of \""
	<< defname << "\" [" << range.left << " : " << range.right << "].";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    ast_head->file_region(),
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  add_phase3stub(make_stub(this, &ItemGen::link_module_array,
			   module_array, ast_module, ast_inst));

  // パラメータ割り当て式の生成
  auto param_con_list = gen_param_con_list(parent, ast_head);
  SizeType n = module_array->elem_num();
  auto attr_list = attribute_list(ast_module, ast_head);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto module = module_array->elem(i);

    {
      std::ostringstream buf;
      buf << "\"" << module->full_name() << "\" has been created.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      module_array->file_region(),
		      MsgType::Info,
		      "ELAB",
		      buf.str());
    }

    // モジュール要素を作る．
    phase1_module_item(module, ast_module, param_con_list);
    // attribute instance の登録
    mgr().reg_attr(module, attr_list);
  }
}

// @brief UDP instance の生成を行う．
void
ItemGen::phase1_udp(
  const VlScope* parent,
  const AstItem* ast_head,
  const VlUdpDefn* udpdefn
)
{
  // この場合, mParamList は空でなければならない．
  // 問題は delay が mParamList に見える場合があるということ．
  auto pa_list = ast_head->paramassign_list();
  auto param_size = pa_list.size();
  auto ast_delay = ast_head->delay();
  if ( param_size > 0 && pa_list.front()->name() != nullptr ) {
    ErrorGen::udp_with_named_paramassign(__FILE__, __LINE__, ast_head);
  }
  if ( (ast_delay && param_size > 0) || param_size > 1 ) {
    ErrorGen::udp_with_ordered_paramassign(__FILE__, __LINE__, ast_head);
  }

  // 今すぐには処理できないのでキューに積む．
  add_phase2stub(make_stub(this, &ItemGen::instantiate_udpheader,
			   parent, ast_head, udpdefn));
}

// @brief cell instance の生成を行う．
void
ItemGen::phase1_cell(
  const VlScope* parent,
  const AstItem* ast_head,
  ClibCell cell
)
{
  // この場合, parameter 割り当てリストは空でなければならない．
  auto pa_list = ast_head->paramassign_list();
  if ( pa_list.size() > 0 ) {
    ErrorGen::cell_with_paramassign(__FILE__, __LINE__, ast_head);
  }

  // 今すぐには処理できないのでキューに積む．
  add_phase2stub(make_stub(this, &ItemGen::instantiate_cell,
			   parent, ast_head, cell));
}

// @brief module array instance の入出力端子の接続を行う．
void
ItemGen::link_module_array(
  ElbModuleArray* module_array,
  const AstModule* ast_module,
  const AstInst* ast_inst
)
{
  auto parent = module_array->parent_scope();

  SizeType module_size = module_array->elem_num();
  auto module0 = module_array->elem_by_offset(0);
  SizeType port_num = module0->port_num();

  auto port_list = ast_inst->port_list();
  SizeType n = port_list.size();

  // ポートの割り当てを行う．
  // 例外: ポートを一つも取らないモジュールの場合
  // module_name instance_name ()
  // という形になって一つのポートが省略された形と区別がつかない．
  // これは Verilog-HDL の仕様がアホ
  // () を取らない形を用意しておけば良かったのに．
  if ( port_num == 0 && n == 1 ) {
    auto con = port_list.front();
    if ( /* con->attr_top() == nullptr &&*/
	 con->name() == nullptr &&
	 con->expr() == nullptr ) {
      // この要素は無視する．
      return;
    }
  }
  if ( n > port_num ) {
    ErrorGen::too_many_items_in_port_list(__FILE__, __LINE__, ast_inst);
  }
  // どうやら実際のポート数よりも少ないのはいいらしい

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (port_list.front()->name() != nullptr);
  std::unordered_map<std::string, int> port_index;
  if ( conn_by_name ) {
    // ポート名とインデックスの辞書を作る．
    SizeType index{0};
    for ( auto ast_port: ast_module->port_list() ) {
      auto name = ast_port->ext_name();
      if ( name != nullptr ) {
	port_index[std::string(name)] = index;
      }
      ++ index;
    }
  }

  // ポートに接続する式を生成する．
  ElbEnv env;
  SizeType pos{0};
  for ( auto ast_con: ast_inst->port_list() ) {
    auto ast_expr = ast_con->expr();
    if ( !ast_expr ) {
      continue;
    }

    // この式が対応するインデックス
    SizeType index;
    if ( conn_by_name ) {
      // 名前による割り当ての場合はポート名で探す．
      auto port_name = ast_con->name();
      if ( port_name == nullptr ) {
	throw std::logic_error{"part_name == nullptr"};
      }
      if ( port_index.count(port_name) == 0 ) {
	ErrorGen::illegal_port_name(__FILE__, __LINE__, ast_con);
      }
      index = port_index.at(port_name);
      if ( index >= port_num ) {
	throw std::logic_error{"index >= port_num"};
      }
    }
    else {
      // 順序に割り当ての場合は単純に pos
      index = pos;
      // 前にも書いたように YACC の文法で規定されているのでこれは常に偽のはず
      if ( ast_con->name() != nullptr ) {
	throw std::logic_error{"ast_con->name() != nullptr"};
      }
      ++ pos;
    }

    // 割り当てるポートを取り出す．
    auto port = module0->port(index);
    if ( port == nullptr ) {
      // このポートはダミー
      continue;
    }

    SizeType port_size = port->bit_size();
    if ( port->direction() == VpiDir::Input ) {
      // 入力ポートには任意の式を接続できる．
      auto tmp = instantiate_expr(parent, env, ast_expr);
      auto type = tmp->value_type();
      // ただし real 型は駄目
      if ( type.is_real_type() ) {
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }

      SizeType expr_size = type.size();
      if ( expr_size == 0 ) {
	// もともとサイズがなければ port_size に合わせる．
	tmp->set_reqsize(VlValueType(false, true, port_size));
	expr_size = port_size;
      }

      // 配列型インスタンスの場合 expr_size に制限がある．
      if ( port_size == expr_size ) {
	// サイズが等しい場合はそのまま接続する．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  auto module = module_array->elem(i);
	  module->set_port_high_conn(index, tmp, conn_by_name);
	}
      }
      else if ( port_size * module_size == expr_size ) {
	if ( module_size <= 1 ) {
	  throw std::logic_error{"module_size <= 1"};
	}
	// tmp を 分割する．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  ElbExpr* tmp1 = nullptr;
	  if ( port_size == 1 ) {
	    tmp1 = mgr().new_BitSelect(ast_expr, tmp, i);
	  }
	  else {
	    int lsb = i;
	    int msb = lsb + port_size - 1;
	    tmp1 = mgr().new_PartSelect(ast_expr, tmp, msb, lsb);
	  }
	  auto module = module_array->elem(i);
	  module->set_port_high_conn(index, tmp1, conn_by_name);
	}
      }
      else {
	ErrorGen::port_size_mismatch(__FILE__, __LINE__, ast_expr,
				     module_array->full_name(), index);
      }
    }
    else {
      // それ以外の場合には左辺式のみが接続できる．
      auto tmp = instantiate_lhs(parent, env, ast_expr);
      auto type = tmp->value_type();
      if ( type.is_real_type() ) {
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }

      auto expr_size = type.size();
      if ( expr_size == port_size ) {
	// 式のサイズとポートサイズが等しければ全部のモジュールに
	// 同一の式を接続する．
	// 普通に考えていいアイデアとは思えない．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  auto module = module_array->elem(i);
	  module->set_port_high_conn(index, tmp, conn_by_name);
	}
      }
      else if ( expr_size == port_size * module_size ) {
	// 式を分割する．
	// 0番目のモジュールがLSB側になる．
	for ( SizeType i = 0; i < module_size; ++ i ) {
	  auto module = module_array->elem(i);
	  int index1 = i * port_size;
	  int index2 = index2 + port_size - 1;
	  auto expr1 = mgr().new_PartSelect(ast_expr, tmp, index1, index2);
	  module->set_port_high_conn(index, expr1, conn_by_name);
	}
      }
      else {
	// サイズが合わない．
	ErrorGen::port_size_mismatch(__FILE__, __LINE__, ast_expr,
				     module_array->full_name(), index);
      }
    }

    // attribute instance の設定
    for ( SizeType i = 0; i < module_size; ++ i ) {
      auto module = module_array->elem(i);
      auto port = module->port(index);

      // attribute instance の生成
      auto attr_list = attribute_list(ast_con);
      mgr().reg_attr(port, attr_list);
    }
  }
}

// @brief module instance の入出力端子の接続を行う．
void
ItemGen::link_module(
  ElbModule* module,
  const AstModule* ast_module,
  const AstInst* ast_inst
)
{
  auto parent = module->parent_scope();
  SizeType port_num = module->port_num();
  auto port_list = ast_inst->port_list();
  SizeType n = port_list.size();
  // ポートの割り当てを行う．
  // 例外: ポートを一つも取らないモジュールの場合
  // module_name instance_name ()
  // という形になって一つのポートが省略された形と区別がつかない．
  // これは Verilog-HDL の仕様がアホ
  // () を取らない形を用意しておけば良かったのに．
  if ( port_num == 0 && n == 1 ) {
    auto ast_con = port_list.front();
    if ( /* ast_con->attr_top() == nullptr &&*/
	 ast_con->name() == nullptr &&
	 ast_con->expr() == nullptr ) {
      // この要素は無視する．
      return;
    }
  }
  if ( n > port_num ) {
    ErrorGen::too_many_items_in_port_list(__FILE__, __LINE__, ast_inst);
  }
  // どうやら実際のポート数よりも少ないのはいいらしい

  // YACC の文法から一つでも named_con なら全部そう
  bool conn_by_name = (port_list.front()->name() != nullptr);
  std::unordered_map<std::string, SizeType> port_index;
  if ( conn_by_name ) {
    // ポート名とインデックスの辞書を作る．
    SizeType index{0};
    for ( auto ast_port: ast_module->port_list() ) {
      auto name = ast_port->ext_name();
      if ( name != nullptr ) {
	port_index[std::string(name)] = index;
      }
      ++ index;
    }
  }

  // ポートに接続する式を生成する．
  ElbEnv env;
  SizeType pos{0};
  for ( auto ast_con: ast_inst->port_list() ) {
    auto ast_expr = ast_con->expr();
    if ( !ast_expr ) {
      continue;
    }

    // この式に対応するポート番号を求める．
    int index{-1};
    if ( conn_by_name ) {
      // 名前による割り当ての場合はポート名で探す．
      auto port_name = ast_con->name();
      if ( port_name == nullptr ) {
	throw std::logic_error{"port_name == nullptr"};
      }
      if ( port_index.count(port_name) == 0 ) {
	ErrorGen::illegal_port_name(__FILE__, __LINE__, ast_con);
      }
      index = port_index.at(port_name);
      if ( index < 0 || index >= port_num ) {
	throw std::logic_error{"index < 0 || index >= port_num"};
      }
    }
    else {
      // 順序による割り当ての場合は単純に pos
      index = pos;
      // 前にも書いたように YACC の文法から下の仮定は常に成り立たないはず．
      if ( ast_con->name() != nullptr ) {
	throw std::logic_error{"ast_con->name() != nullptr"};
      }
      ++ pos;
    }

    auto port = module->port(index);
    if ( !port ) {
      // このポートはダミー
      continue;
    }

    SizeType port_size = port->bit_size();
    if ( port->direction() == VpiDir::Input ) {
      // 入力ポートには任意の式を接続できる．
      auto tmp = instantiate_expr(parent, env, ast_expr);
      auto type = tmp->value_type();
      if ( type.is_real_type() ) {
	// ただし real 型は駄目
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }

      SizeType expr_size = type.size();

      // 単独のインスタンスの場合 expr のサイズは補正される．
      // ... でいいんだよね．
      if ( port_size != expr_size ) {
	if ( expr_size != 0 ) {
	  {
	    std::ostringstream buf;
	    buf << "port_size: " << port_size << ", expr_size: " << expr_size;
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    ast_expr->file_region(),
			    MsgType::Debug,
			    "ELAB",
			    buf.str());
	  }
	  ErrorGen::port_size_mismatch(__FILE__, __LINE__, ast_expr,
				       module->full_name(), index);
	}
	tmp->set_reqsize(VlValueType(false, true, port_size));
      }
      module->set_port_high_conn(index, tmp, conn_by_name);
    }
    else {
      // それ以外のポートに接続できるのは左辺式だけ．
      auto tmp = instantiate_lhs(parent, env, ast_expr);
      auto type = tmp->value_type();
      if ( type.is_real_type() ) {
	ErrorGen::real_type_in_port_list(__FILE__, __LINE__, tmp);
      }
      // 左辺はサイズの補正をしても意味がないのでそのまま接続する．
      module->set_port_high_conn(index, tmp, conn_by_name);
    }

    // attribute instance の生成
    auto attr_list = attribute_list(ast_con);
    mgr().reg_attr(port, attr_list);
  }
}

// @brief パラメータ割り当て情報を作る．
std::vector<ElbParamCon>
ItemGen::gen_param_con_list(
  const VlScope* parent,
  const AstItem* ast_head
)
{
  std::vector<ElbParamCon> param_con_list;
  auto pa_list = ast_head->paramassign_list();
  for ( auto ast_con: pa_list ) {
    auto ast_expr = ast_con->expr();
    auto value = evaluate_expr(parent, ast_expr);
    param_con_list.push_back({ast_con, ast_expr, value});
  }
  return param_con_list;
}

END_NAMESPACE_YM_VERILOG
