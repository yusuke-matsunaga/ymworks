
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
#include "ym/vl/AstPort.h"
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
  const PtDeclHead* paramport_top,
  const PtPort* port_top,
  const PtIOHead* iohead_top,
  const PtDeclHead* declhead_top,
  const PtItem* item_top
)
{
  bool is_cell = lex().cell_define();
  int time_u = lex().time_unit();
  int time_p = lex().time_precision();
  VpiNetType nettype = lex().default_nettype();
  VpiUnconnDrive unconn = lex().unconnected_drive();
  VpiDefDelayMode delay = lex().delay_mode();
  int decay = lex().default_decay_time();

  return mFactory.new_Module(file_region,
			     module_name,
			     is_macro, is_cell,
			     time_u, time_p,
			     nettype, unconn,
			     delay, decay,
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
  const PtDeclHead* paramport_top,
  const PtIOHead* portdecl_top,
  const PtDeclHead* declhead_top,
  const PtItem* item_top
)
{
  bool is_cell = lex().cell_define();
  int time_u = lex().time_unit();
  int time_p = lex().time_precision();
  VpiNetType nettype = lex().default_nettype();
  VpiUnconnDrive unconn = lex().unconnected_drive();
  VpiDefDelayMode delay = lex().delay_mode();
  int decay = lex().default_decay_time();

  return mFactory.new_Module(file_region,
			     module_name,
			     is_macro, is_cell,
			     time_u, time_p, nettype,
			     unconn, delay, decay,
			     paramport_top,
			     nullptr,
			     portdecl_top,
			     declhead_top,
			     item_top);
}

END_NAMESPACE_YM_VERILOG
