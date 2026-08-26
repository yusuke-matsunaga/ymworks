
/// @file Parser_module.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "scanner/Lex.h"
#include "parser/AstMgr.h"
#include "parser/PtFactory.h"
#include "parser/PtModule.h"
#include "parser/PtPort.h"
#include "parser/PtItem.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Module 関係
//////////////////////////////////////////////////////////////////////

// Verilog1995 タイプのモジュールを生成する．
PtModule*
Parser::new_Module1995(
  const FileRegion& file_region,
  bool is_macro,
  const char* module_name,
  PtDeclHead* paramport_top,
  PtPort* port_top,
  PtIOHead* iohead_top,
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  bool is_cell = lex().cell_define();
  bool is_protected = false; // これどうやって決めるの？
  int time_u = lex().time_unit();
  int time_p = lex().time_precision();
  VpiNetType nettype = lex().default_nettype();
  VpiUnconnDrive unconn = lex().unconnected_drive();
  VpiDefDelayMode delay = lex().delay_mode();
  int decay = lex().default_decay_time();

#if 0 // VERIFAULT
  bool portfaults = lex().portfaults();
  bool suppress_faults = lex().suppress_faults();
#else
  bool portfaults = false;
  bool suppress_faults = false;
#endif

  std::string config;  // ?
  std::string library; // ?
  std::string cell;    // ?

  // ポート宣言とIO宣言のチェックを行う．
  std::unordered_map<std::string, VpiDir> iodecl_dirs;
  check_IO(port_top, iohead_top, iodecl_dirs);

  // 今度はポートリストに現れている信号線が入出力ポート宣言されているか
  // 調べる．
  // 同時に名無しのポートがあるかどうかしらべる．
  bool named_port = true;
  for ( auto port = port_top; port != nullptr; port = port->_link() ) {
    if ( port->ext_name() == nullptr ) {
      // 1つでも名前を持たないポートがあったら名前での結合はできない．
      named_port = false;
    }
    SizeType index = 0;
    for ( auto expr: port->portref_list() ) {
      auto name = expr->name();
      if ( iodecl_dirs.count(name) == 0 ) {
	// name は IOH リストに存在しない．
	std::ostringstream buf;
	buf << "\"" << name << "\" is in the port list but not declared.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			port->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
      }
      else {
	auto dir = iodecl_dirs.at(name);
	port->set_portref_dir(index, dir);
      }
      ++ index;
    }
  }

  return mFactory.new_Module(file_region,
			     module_name,
			     is_macro,
			     is_cell,
			     is_protected,
			     time_u, time_p,
			     nettype, unconn,
			     delay, decay,
			     named_port,
			     portfaults, suppress_faults,
			     config, library, cell,
			     paramport_top,
			     port_top,
			     iohead_top,
			     declhead_top,
			     item_top);
}

// Verilog2001 タイプのモジュールを生成する．
PtModule*
Parser::new_Module2001(
  const FileRegion& file_region,
  bool is_macro,
  const char* module_name,
  PtDeclHead* paramport_top,
  PtIOHead* portdecl_top,
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  bool is_cell = lex().cell_define();
  bool is_protected = false; // これどうやって決めるの？
  int time_u = lex().time_unit();
  int time_p = lex().time_precision();
  VpiNetType nettype = lex().default_nettype();
  VpiUnconnDrive unconn = lex().unconnected_drive();
  VpiDefDelayMode delay = lex().delay_mode();
  int decay = lex().default_decay_time();

#if 0 // VERIFAULT
  bool portfaults = lex().portfaults();
  bool suppress_faults = lex().suppress_faults();
#else
  bool portfaults = false;
  bool suppress_faults = false;
#endif

  std::string config;  // ?
  std::string library; // ?
  std::string cell;    // ?

  if ( !check_PortArray(AstIOHeadList(portdecl_top)) ) {
    return nullptr;
  }

  // iohead_array からポートリストを作る．
  auto port_list = new_PortArray(portdecl_top);

  return mFactory.new_Module(file_region,
			     module_name,
			     is_macro, is_cell, is_protected,
			     time_u, time_p, nettype,
			     unconn, delay, decay,
			     true,
			     portfaults, suppress_faults,
			     config, library, cell,
			     paramport_top,
			     port_list.top,
			     portdecl_top,
			     declhead_top,
			     item_top);
}

// @brief ポート宣言とIO宣言の齟齬をチェックする．
void
Parser::check_IO(
  PtPort* port_top,
  PtIOHead* iohead_top,
  std::unordered_map<std::string, VpiDir>& iodecl_dirs
)
{
  // port_array をスキャンして中で用いられている名前を portref_dic
  // に登録する．
  std::unordered_set<std::string> portref_dic;
  for ( auto port: AstPortList(port_top) ) {
    for ( auto expr: port->portref_list() ) {
      auto name = expr->name();
      portref_dic.insert(name);
    }
  }

  // 入出力ポート宣言に現れる名前を iodecl_names に入れる．
  // ポート宣言が型を持つ場合にはモジュール内部の宣言要素を生成する．
  // 持たない場合にはデフォルトタイプのネットを生成する．
  for ( auto io_head: AstIOHeadList(iohead_top) ) {
    // 名前をキーにして方向を記録しておく
    VpiDir dir = io_head->direction();
    for ( auto elem: io_head->item_list() ) {
      auto elem_name = elem->name();

      // まず未定義/多重定義のエラーをチェックする．
      if ( portref_dic.count(elem_name) == 0 ) {
	// port expression に現れない信号線名
	// 未定義エラー
	std::ostringstream buf;
	buf << "\"" << elem_name << "\" is not defined in the port list.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
      }
      if ( iodecl_dirs.count(elem_name) > 0 ) {
	// 二重登録エラー
	std::ostringstream buf;
	buf << "\"" << elem_name << "\" is redefined.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
      }
      else {
	iodecl_dirs[elem_name] = dir;
      }
    }
  }
}

// @brief 入出力宣言中の重複チェックを行う．
bool
Parser::check_PortArray(
  const AstIOHeadList& iohead_list
)
{
  std::unordered_set<std::string> portref_dic;
  for ( auto head: iohead_list ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      if ( portref_dic.count(name) > 0 ) {
	std::ostringstream buf;
	buf << "\"" << name << "\" is redefined.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"ELAB",
			buf.str());
	return false;
      }
      portref_dic.insert(name);
    }
  }
  return true;
}

// @brief 入出力宣言からポートリストを作る．
PtPortList
Parser::new_PortArray(
  const AstIOHeadList& iohead_list
)
{
  // ポートを生成し vec に格納する．
  PtPortList port_list;
  port_list.init();
  for ( auto head: iohead_list ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      auto portref = mFactory.new_Primary(elem->file_region(), name);
      auto port = mFactory.new_Port(elem->file_region(), name, portref);
      auto dir = head->direction();
      port->set_portref_dir(0, dir);
      port_list.add(port);
    }
  }
  return port_list;
}


END_NAMESPACE_YM_VERILOG
