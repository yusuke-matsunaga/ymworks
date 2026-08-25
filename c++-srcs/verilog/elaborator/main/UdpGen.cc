
/// @file UdpGen.cc
/// @brief UdpGen の実装ファイル (UDPのインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "UdpGen.h"
#include "ElbEnv.h"

#include "ym/vl/AstUdp.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstDecl.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/VlExpr.h"

#include "elaborator/ElbUdp.h"
#include "elaborator/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス UdpGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UdpGen::UdpGen(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy{elab, elb_mgr}
{
}

// @brief デストラクタ
UdpGen::~UdpGen()
{
}

// @brief UDP定義を生成する．
void
UdpGen::instantiate_udp(
  const AstUdp* ast_udp
)
{
  using IODict = std::unordered_map<std::string,
				    std::pair<const AstIOHead*, const AstIOItem*>>;

  const auto& file_region = ast_udp->file_region();
  auto def_name = ast_udp->name();

  {
    std::ostringstream buf;
    buf << "instantiating UDP \"" << def_name << "\".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    file_region,
		    MsgType::Info,
		    "ELAB",
		    buf.str());
  }

  SizeType io_size = ast_udp->port_list().size();

  SizeType isize = io_size - 1;
  auto ptype = ast_udp->prim_type();
  bool is_protected{true}; // 何これ?

  auto udp = mgr().new_UdpDefn(ast_udp, is_protected);

  // 最初のポート名 = 出力のポート名
  auto outname = ast_udp->port_list().front()->ext_name();

  // ポート名をキーにしたIOテンプレートの辞書を作る．
  IODict iodict;
  const AstIOHead* outhead{nullptr};
  for ( auto iohead: ast_udp->iohead_list() ) {
    for ( auto elem: iohead->item_list() ) {
      auto name = elem->name();
      iodict[name] = std::make_pair(iohead, elem);
      if ( strcmp(name, outname) == 0 ) {
	outhead = iohead;
      }
    }
  }

  // IOポートを実体化する．
  // ただし port_list に現れる名前の順番にしたがって実体化しなければならない．
  SizeType index{0};
  for ( auto port: ast_udp->port_list() ) {
    auto name = port->ext_name();
    if ( iodict.count(name) == 0 ) {
      throw std::logic_error{"iodict.count(name) == 0"};
    }
    auto tmp = iodict.at(name);
    auto ast_header = tmp.first;
    auto ast_item = tmp.second;
    udp->set_io(index, ast_header, ast_item);
    ++ index;
  }

  // 初期化文を実体化させる．
  // initial 文がある場合と変数宣言の中に初期化式が含まれている場合がある．
  auto ast_init_value = ast_udp->init_value();
  if ( ast_init_value == nullptr ) {
    ast_init_value = outhead->item_list().front()->init_value();
  }
  if ( ast_init_value ) {
    // このチェックはパース時に済んでいるはずなので念のため．
    if ( ptype != VpiPrimType::Seq ) {
      throw std::logic_error{"ptype != VpiPrimType"};
    }

    const auto& ifr = ast_init_value->file_region();

    // 初期値を設定する．
    auto val = evaluate_scalar(nullptr, ast_init_value);
    udp->set_initial(ast_init_value, val);
  }

  // テーブルの中身を作る．
  // 似た様なことをしているが単純にするために組合わせ回路と順序回路を
  // 分ける．
  if ( ptype == VpiPrimType::Comb ) {
    // 組合わせ回路

    // 一行のサイズは入出力数と一致する．
    SizeType row_size = io_size;

    // 出力値の位置
    SizeType opos = row_size - 1;

    SizeType pos{0};
    for ( auto ast_udp_entry: ast_udp->table_list() ) {
      const auto& tfr = ast_udp_entry->file_region();
      auto input_list = ast_udp_entry->input_list();
      if ( input_list.size() != isize ) {
	// サイズが合わない．
	MsgMgr::put_msg(__FILE__, __LINE__,
			tfr,
			MsgType::Error,
			"ELAB",
			"Number of input symbols mimatch.");
	return;
      }


      // 一行文のデータを保持しておくためのバッファ
      std::vector<VlUdpVal> row_data;
      row_data.reserve(row_size);
      // 入力
      for ( auto ast_v: input_list ) {
	auto symbol = ast_v->symbol();
	if ( symbol.is_edge_symbol() ) {
	  // 組合せ回路の場合にはエッジタイプの値は使えない．
	  std::ostringstream buf;
	  buf << symbol.to_string()
	      << " : transition symbol for combinational UDP";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}
	if ( symbol.is_nc_symbol() ) {
	  // NC は状態出力にしか使えない
	  std::ostringstream buf;
	  buf << symbol.to_string() << " : illegal symbol for input field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data.push_back(symbol);
      }

      { // 現状態
	if ( ast_udp_entry->current() ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_udp_entry->file_region(),
			  MsgType::Error,
			  "ELAB",
			  "Combinational UDP should not have "
			  "\'current state\' value.");
	  return;
	}
      }

      { // 出力
	auto ast_v = ast_udp_entry->output();
	auto symbol = ast_v->symbol();
	if ( symbol.is_composite_symbol() ) {
	  // 出力には複合値は使えない
	  std::ostringstream buf;
	  buf << symbol.to_string() << " : illegal symbol for output field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data.push_back(symbol);
      }

      // 一行文のデータを設定する．
      udp->set_tableentry(pos, ast_udp_entry, row_data);
      ++ pos;
    }
  }
  else {
    // 順序回路

    // 一行のサイズは入出力 + 現状態
    SizeType row_size = io_size + 1;

    // 現状態値の位置
    SizeType cpos = isize;

    // 出力値の位置
    SizeType opos = io_size;

    SizeType pos = 0;
    for ( auto ast_udp_entry: ast_udp->table_list() ) {
      const auto& tfr = ast_udp_entry->file_region();
      if ( ast_udp_entry->input_list().size() != isize ) {
	// サイズが合わない．
	MsgMgr::put_msg(__FILE__, __LINE__,
			tfr,
			MsgType::Error,
			"ELAB",
			"Number of input symbols mimatch.");
	return;
      }

      // 一行中に含まれるエッジタイプのシンボルの数
      SizeType nt = 0;

      // 一行文のデータを保持しておくためのバッファ
      std::vector<VlUdpVal> row_data;
      row_data.reserve(row_size);

      // 入力
      for ( auto ast_v: ast_udp_entry->input_list() ) {
	auto symbol = ast_v->symbol();
	if ( symbol.is_edge_symbol() ) {
	  ++ nt;
	  if ( nt > 1 ) {
	    MsgMgr::put_msg(__FILE__, __LINE__,
			    ast_v->file_region(),
			    MsgType::Error,
			    "ELAB",
			    "More than one transition symbols "
			    "in the same row.");
	    return;
	  }
	}

	// 順序回路の入力は全ての値/シンボルが使用可
	row_data.push_back(symbol);
      }

      { // 現状態
	auto ast_v = ast_udp_entry->current();
	if ( !ast_v ) {
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  tfr,
			  MsgType::Error,
			  "ELAB",
			  "Sequential UDP requires \'current state\' value.");
	  return;
	}

	auto symbol = ast_v->symbol();
	if ( symbol.is_edge_symbol() ) {
	  // エッジタイプの値は使えない．
	  std::ostringstream buf;
	  buf << symbol.to_string()
	      << " : transition symbol for current state field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}
	if ( symbol.is_nc_symbol() ) {
	  // NC は状態出力にしか使えない
	  std::ostringstream buf;
	  buf << symbol.to_string()
	      << " : illegal symbol for current state field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data.push_back(symbol);
      }

      { // 出力
	auto ast_v = ast_udp_entry->output();
	auto symbol = ast_v->symbol();
	if ( !symbol.is_nc_symbol() && symbol.is_edge_symbol() ) {
	  // エッジタイプの値は使えない．
	  std::ostringstream buf;
	  buf << symbol.to_string()
	      << " : transition symbol for output field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}
	if ( symbol.is_composite_symbol() ) {
	  // 出力には複合値は使えない
	  std::ostringstream buf;
	  buf << symbol.to_string()
	      << " : illegal symbol for output field.";
	  MsgMgr::put_msg(__FILE__, __LINE__,
			  ast_v->file_region(),
			  MsgType::Error,
			  "ELAB",
			  buf.str());
	  return;
	}

	row_data.push_back(symbol);
      }

      // 一行文のデータを設定する．
      udp->set_tableentry(pos, ast_udp_entry, row_data);
      ++ pos;
    }
  }
}

END_NAMESPACE_YM_VERILOG
