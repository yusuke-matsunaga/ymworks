
/// @file Parser_udp.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/AstMgr.h"
#include "parser/PtFactory.h"
#include "parser/PtUdp.h"
#include "parser/PtPort.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// UDP 関係
//////////////////////////////////////////////////////////////////////

// UDP を生成する．(Verilog-1995)
PtUdp*
Parser::new_Udp1995(
  const FileRegion& file_region,
  const char* udp_name,
  const char* init_name,
  const FileRegion& init_loc,
  const PtExpr* init_value,
  const PtPort* port_top,
  const PtIOHead* io_top,
  const PtDeclHead* decl_top,
  const PtUdpEntry* entry_top
)
{
  const PtIOItem* out_item = nullptr;
  bool is_seq = false;

  bool sane = true;

  // Verilog1995 の形式の時には YACC の文法では規定できないので，
  // 1. port_list に現れる名前が portdecl_list 中に存在すること．
  // 2. その最初の名前は output であること．
  // 3. それ以外はすべて input であること
  // の確認を行う．
  // まず portdecl_list の各要素を名前をキーにした連想配列に格納する．
  // ついでに output の数を数える．
  std::unordered_map<std::string, const PtIOItem*> iomap;
  for ( auto io: PtList<const PtIOHead>::new_obj(io_top) ) {
    if ( io->direction() == VpiDir::Output ) {
      if ( out_item ) {
	// 複数の出力宣言があった．
	MsgMgr::put_msg(__FILE__, __LINE__,
			io->file_region(),
			MsgType::Error,
			"PARS",
			"More than two output declarations");
	sane = false;
	break;
      }

      // YACC の文法が正しくかけていれば io の要素数は 1
      out_item = io->item_top();
      if ( io->aux_type() == VpiAuxType::Reg ) {
	is_seq = true;
      }
    }
    for ( auto elem: PtList<const PtIOItem>::new_obj(io->item_top()) ) {
      if ( iomap.count(elem->name()) > 0 ) {
	// 二重登録
	std::ostringstream buf;
	buf << elem->name() << ": Defined more than once.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			elem->file_region(),
			MsgType::Error,
			"PARS",
			buf.str());
	sane = false;
	break;
      }
      iomap[elem->name()] = elem;
    }
  }

  // port_list に現れる名前が iolist 中にあるか調べる．
  bool first = true;
  for ( auto port: PtList<const PtPort>::new_obj(port_top) ) {
    auto port_name = port->ext_name();
    if ( iomap.count(port_name) == 0 ) {
      std::ostringstream buf;
      buf << "\"" << port_name << "\" undefined.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      file_region,
		      MsgType::Error,
		      "PARS",
		      buf.str());
      sane = false;
      break;
    }
    if ( first ) {
      first = false;
      auto ioelem = iomap.at(port_name);
      if ( out_item != ioelem ) {
	// 最初の名前は output でなければならない．
	std::ostringstream buf;
	buf << port_name << " must be an output.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			ioelem->file_region(),
			MsgType::Error,
			"PARS",
			buf.str());
	sane = false;
	break;
      }
    }
    iomap.erase(port_name);
  }

  if ( iomap.size() > 0 ) {
    // iolist 中のみに現れる要素がある．
    for ( auto q: iomap ) {
      auto ioelem = q.second;
      std::ostringstream buf;
      buf << "\"" << ioelem->name() << "\" does not appear in portlist.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      ioelem->file_region(),
		      MsgType::Error,
		      "PARS",
		      buf.str());
    }
    sane = false;
  }

  // 次に decl_list の要素数が1以下であり，
  // さらにその要素が REG で名前が出力名と一致することを確認する．
  // ちなみに YACC の文法から REG 以外の宣言要素はありえない．
  if ( decl_top != nullptr ) {
    if ( decl_top->link() != nullptr ) {
      // 二つ以上の reg 宣言があった．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      decl_top->file_region(),
		      MsgType::Error,
		      "PARS",
		      "More than two 'reg' declarations.");
      sane = false;
    }
    else {
      auto reghead = decl_top;
      is_seq = true;
      ASSERT_COND( reghead->type() == AstDeclHead::Reg );
      auto regitem = reghead->item_top();
      ASSERT_COND( regitem );
      ASSERT_COND( regitem->link() == nullptr );
      if ( strcmp(regitem->name(), out_item->name()) != 0 ) {
	// output と名前が違う
	std::ostringstream buf;
	buf << "Reg name \"" << regitem->name()
	    << "\" differes from output name \""
	    << out_item->name() << "\".";
	MsgMgr::put_msg(__FILE__, __LINE__,
			regitem->file_region(),
			MsgType::Error,
			"PARS",
			buf.str());
	sane = false;
      }
    }
  }

  if ( !sane ) {
    return nullptr;
  }

  return new_Udp(file_region,
		 udp_name,
		 init_name,
		 init_loc,
		 init_value,
		 is_seq,
		 out_item,
		 port_top,
		 io_top,
		 entry_top);
}

// UDP を生成する．(Verilog-2001)
PtUdp*
Parser::new_Udp2001(
  const FileRegion& file_region,
  const char* udp_name,
  const char* init_name,
  const FileRegion& init_loc,
  const PtExpr* init_value,
  const PtIOHead* io_top,
  const PtUdpEntry* entry_top
)
{
  bool is_seq = false;

  // YACC の文法が正しく書かれていれば最初のヘッダが出力で
  // 要素数が1となっているはず．
  auto out_head = io_top;
  ASSERT_COND( out_head->direction() == VpiDir::Output );
  auto out_item = out_head->item_top();
  ASSERT_COND( out_item->link() == nullptr );

  if ( out_head->aux_type() == VpiAuxType::Reg ) {
    is_seq = true;
  }
  // 残りの要素は入力になっているはず．

  return new_Udp(file_region,
		 udp_name,
		 init_name,
		 init_loc,
		 init_value,
		 is_seq,
		 out_item,
		 nullptr, /*port_top,*/
		 io_top,
		 entry_top);
}

// @brief new_Udp の下請け関数
PtUdp*
Parser::new_Udp(
  const FileRegion& file_region,
  const char* udp_name,
  const char* init_name,
  const FileRegion& init_loc,
  const PtExpr* init_value,
  bool is_seq,
  const PtIOItem* out_item,
  const PtPort* port_top,
  const PtIOHead* iohead_top,
  const PtUdpEntry* entry_top
)
{
  const AstUdp* udp = nullptr;
  if ( is_seq ) {
    // 初期値の設定がある．
    if ( init_name ) {

      // init_name が正しいかチェックする．
      if ( strcmp(init_name, out_item->name()) != 0 ) {
	// output 文の名前と異なる．
	std::ostringstream buf;
	buf << "Lhs of initial \"" << init_name
	    << "\" differes from output name \""
	    << out_item->name() << "\".";
	MsgMgr::put_msg(__FILE__, __LINE__,
			init_loc,
			MsgType::Error,
			"PARS",
			buf.str());
	return nullptr;
      }

      if ( out_item->init_value() != nullptr ) {
	// output 文にも初期値割り当てがある．
	// これは warning にする．
	MsgMgr::put_msg(__FILE__, __LINE__,
			init_value->file_region(),
			MsgType::Warning,
			"PARS",
			"Both output declaration and initial block"
			" have the initial values,"
			" output declarations's initial value is ignored.");
      }
    }

    // このあと elaboration で注意が必要なのは init_value.
    // 場合によってはこれが nullptrで outhead.top().init_value()
    // が空でない場合がある．
    return mFactory.new_SeqUdp(file_region,
			       udp_name,
			       port_top,
			       iohead_top,
			       init_value,
			       entry_top);
  }
  else {
    if ( init_name ) {
      // sequential primitive でなければ初期値を持てない．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      init_loc,
		      MsgType::Error,
		      "PARS",
		      "Combinational primitive can not have the initial value.");
      return nullptr;
    }
    return mFactory.new_CmbUdp(file_region,
			       udp_name,
			       port_top,
			       iohead_top,
			       entry_top);
  }
}

END_NAMESPACE_YM_VERILOG
