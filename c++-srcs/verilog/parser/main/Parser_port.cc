/// @file Parser_port.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtPort.h"
#include "parser/PtExpr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtiPortArray の生成
//////////////////////////////////////////////////////////////////////

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

// @brief 入出力宣言からポートを作る．
std::vector<PtPort*>
Parser::new_PortArray(
  const AstIOHeadList& iohead_list
)
{
  SizeType num = 0;
  for ( auto head: iohead_list ) {
    num += head->item_list().size();
  }

  // ポートを生成し vec に格納する．
  std::vector<PtPort*> vec;
  vec.reserve(num);
  for ( auto head: iohead_list ) {
    for ( auto elem: head->item_list() ) {
      auto name = elem->name();
      auto portref = mFactory.new_Primary(elem->file_region(), name);
      auto port = mFactory.new_Port(elem->file_region(), name, portref);
      auto dir = head->direction();
      port->set_portref_dir(0, dir);
      vec.push_back(port);
    }
  }
  return vec;
}

END_NAMESPACE_YM_VERILOG
