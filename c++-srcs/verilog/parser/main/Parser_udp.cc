
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

// @brief UDP定義の開始
void
Parser::init_udp()
{
  mModuleIOHeadList.clear();
  mPortList.clear();
  push_declhead_list();

  mIOItemList.clear();
  mDeclItemList.clear();
  mUdpEntryList.clear();
}

// @brief UDP 定義の終了
void
Parser::end_udp()
{
  mCurDeclList = pop_declhead_list();
}

// UDP を生成する．(Verilog-1995)
void
Parser::new_Udp1995(
  const FileRegion& file_region,
  const char* udp_name,
  const char* init_name,
  const FileRegion& init_loc,
  const AstExpr* init_value,
  const AstAttrInstList* ai_list
)
{
  const AstIOItem* out_item = nullptr;
  bool is_seq = false;

  bool sane = true;

  // Verilog1995 の形式の時には YACC の文法では規定できないので，
  // 1. port_list に現れる名前が portdecl_list 中に存在すること．
  // 2. その最初の名前は output であること．
  // 3. それ以外はすべて input であること
  // の確認を行う．
  // まず portdecl_list の各要素を名前をキーにした連想配列に格納する．
  // ついでに output の数を数える．
  std::unordered_map<std::string, const AstIOItem*> iomap;
  for ( auto io: mModuleIOHeadList ) {
    auto item_list = io->item_list();
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

      // これは YACC の文法が正しくかけていれば成り立つはず．
      ASSERT_COND( item_list.size() == 1 );

      out_item = item_list.front();
      if ( io->aux_type() == VpiAuxType::Reg ) {
	is_seq = true;
      }
    }
    for ( auto elem: item_list ) {
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
  for ( auto port: mPortList ) {
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
  if ( mCurDeclList.size() > 1 ) {
    // 二つ以上の reg 宣言があった．
    MsgMgr::put_msg(__FILE__, __LINE__,
		    mCurDeclList[1]->file_region(),
		    MsgType::Error,
		    "PARS",
		    "More than two 'reg' declarations.");
    sane = false;
  }
  else if ( mCurDeclList.size() == 1 ) {
    auto reghead = mCurDeclList[0];
    if ( reghead ) {
      is_seq = true;
      ASSERT_COND( reghead->type() == AstDeclHead::Reg );
      ASSERT_COND( reghead->item_num() == 1 );
      auto regitem = reghead->item_list().front();
      ASSERT_COND( regitem );
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

  if ( sane ) {
    new_Udp(file_region,
	    udp_name,
	    init_name,
	    init_loc,
	    init_value,
	    ai_list,
	    is_seq,
	    out_item,
	    mPortList,
	    mModuleIOHeadList);
  }
  end_udp();
}

// UDP を生成する．(Verilog-2001)
void
Parser::new_Udp2001(
  const FileRegion& file_region,
  const char* udp_name,
  const char* init_name,
  const FileRegion& init_loc,
  const AstExpr* init_value,
  const AstAttrInstList* ai_list
)
{
  bool is_seq = false;

  // YACC の文法が正しく書かれていれば最初のヘッダが出力で
  // 要素数が1となっているはず．
  ASSERT_COND( mModuleIOHeadList.size() > 0 );
  auto out_head = mModuleIOHeadList[0];
  ASSERT_COND( out_head->direction() == VpiDir::Output );
  ASSERT_COND( out_head->item_num() == 1 );
  auto out_item = out_head->item_list().front();

  if ( out_head->aux_type() == VpiAuxType::Reg ) {
    is_seq = true;
  }
  // 残りの要素は入力になっているはず．

  // iohead_array から port_array を生成する．
  auto port_array = new_PortArray(mModuleIOHeadList);

  new_Udp(file_region,
	  udp_name,
	  init_name,
	  init_loc,
	  init_value,
	  ai_list,
	  is_seq,
	  out_item,
	  port_array,
	  mModuleIOHeadList);
  end_udp();
}

// @brief new_Udp の下請け関数
void
Parser::new_Udp(
  const FileRegion& file_region,
  const char* udp_name,
  const char* init_name,
  const FileRegion& init_loc,
  const AstExpr* init_value,
  const AstAttrInstList* ai_list,
  bool is_seq,
  const AstIOItem* out_item,
  const std::vector<PtPort*>& port_array,
  const std::vector<PtIOHead*>& iohead_array
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
	return;
      }

      if ( out_item->init_value() ) {
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
    // 場合によってはこれが nullptrで outhead->top()->init_value()
    // が空でない場合がある．
    udp = mFactory.new_SeqUdp(file_region,
			      udp_name,
			      port_array,
			      iohead_array,
			      init_value,
			      mUdpEntryList);
  }
  else {
    if ( init_name ) {
      // sequential primitive でなければ初期値を持てない．
      MsgMgr::put_msg(__FILE__, __LINE__,
		      init_loc,
		      MsgType::Error,
		      "PARS",
		      "Combinational primitive can not have the initial value.");
      return;
    }
    udp = mFactory.new_CmbUdp(file_region,
			      udp_name,
			      port_array,
			      iohead_array,
			      mUdpEntryList);
  }

  mAstMgr.reg_udp(udp);
  reg_attrinst(udp, ai_list);
}

END_NAMESPACE_YM_VERILOG
