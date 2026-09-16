
/// @file UdpGen.cc
/// @brief UdpGen の実装ファイル (UDPのインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "UdpGen.h"
#include "ElbEnv.h"

#include "ym/vl/AstUdpEntry.h"
#include "ym/vl/AstUdpValue.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/VlExpr.h"

#include "elaborator/ElbUdp.h"
#include "elaborator/ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス UdpGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UdpGen::UdpGen(
  Elaborator& elab
) : ElbProxy{elab}
{
}

// @brief デストラクタ
UdpGen::~UdpGen()
{
}

// @brief UDP定義を生成する．
void
UdpGen::instantiate_udp(
  const AstUdp& ast_udp
)
{
  const auto& file_region = ast_udp.file_region();
  auto def_name = ast_udp.name();

  log_mgr().info_udp(__FILE__, __LINE__,
		     file_region,
		     def_name);

  auto ptype = ast_udp.prim_type();
  bool is_protected{true}; // 何これ?
  auto udp = elb_mgr().new_UdpDefn(ast_udp, is_protected);

  AstIOHead outhead;
  if ( ast_udp.port_list().empty() ) {
    outhead = gen_io2001(udp, ast_udp.iohead_list());
  }
  else {
    outhead = gen_io1995(udp, ast_udp.port_list(), ast_udp.iohead_list());
  }

  // 初期化文を実体化させる．
  // initial 文がある場合と変数宣言の中に初期化式が含まれている場合がある．
  auto ast_init_value = ast_udp.init_value();
  if ( ast_init_value.is_invalid() ) {
    ast_init_value = outhead.item_list().front().init_value();
  }
  if ( ast_init_value.is_valid() ) {
    // このチェックはパース時に済んでいるはずなので念のため．
    if ( ptype != VpiPrimType::Seq ) {
      throw std::logic_error{"ptype != VpiPrimType"};
    }

    const auto& ifr = ast_init_value.file_region();

    // 初期値を設定する．
    auto val = evaluate_scalar(nullptr, ast_init_value);
    udp->set_initial(ast_init_value, val);
  }

  // テーブルの中身を作る．
  // 似た様なことをしているが単純にするために組合わせ回路と順序回路を
  // 分ける．
  SizeType io_size = ast_udp.io_num();
  auto table_list = ast_udp.table_list();
  if ( ptype == VpiPrimType::Comb ) {
    // 組合わせ回路
    gen_comb_table(udp, io_size, table_list);
  }
  else {
    // 順序回路
    gen_seq_table(udp, io_size, table_list);
  }
}

// @brief Verilog1995 スタイルのIOポート生成を行う．
AstIOHead
UdpGen::gen_io1995(
  ElbUdpDefn* udp,
  const AstPortList& port_list,
  const AstIOHeadList& iohead_list
)
{
  using IODict = std::unordered_map<std::string,
				    std::pair<AstIOHead, AstIOItem>>;

  // 最初のポート名 = 出力のポート名
  // これは Verilog-HDL のシンタックスで決まっている．
  auto outname = port_list.front().ext_name();

  // ポート名をキーにしたIOテンプレートの辞書を作る．
  IODict iodict;
  AstIOHead outhead;
  for ( auto iohead: iohead_list ) {
    for ( auto elem: iohead.item_list() ) {
      auto name = elem.name();
      iodict[name] = std::make_pair(iohead, elem);
      if ( strcmp(name, outname) == 0 ) {
	outhead = iohead;
	if ( outhead.direction() != VpiDir::Output ) {
	  throw std::logic_error{"outhead.direction() != VpiDir::Output"};
	}
      }
    }
  }

  // IOポートを実体化する．
  // ただし port_list に現れる名前の順番にしたがって実体化しなければならない．
  for ( auto port: port_list ) {
    auto name = port.ext_name();
    if ( iodict.count(name) == 0 ) {
      throw std::logic_error{"iodict.count(name) == 0"};
    }
    auto tmp = iodict.at(name);
    auto ast_header = tmp.first;
    auto ast_item = tmp.second;
    udp->add_io(ast_header, ast_item);
  }

  return outhead;
}

// @brief Verilog2001 スタイルのIOポート生成を行う．
AstIOHead
UdpGen::gen_io2001(
  ElbUdpDefn* udp,
  const AstIOHeadList& iohead_list
)
{
  AstIOHead outhead;
  for ( auto iohead: iohead_list ) {
    if ( iohead.direction() == VpiDir::Output ) {
      outhead = iohead;
      // シンタックスで唯一であることが規定されている．
    }
    for ( auto elem: iohead.item_list() ) {
      udp->add_io(iohead, elem);
    }
  }

  return outhead;
}

// @brief 組み合わせ回路用のテーブルを生成する．
void
UdpGen::gen_comb_table(
  ElbUdpDefn* udp,
  SizeType io_size,
  const AstUdpEntryList& table_list
)
{
  // 一行のサイズは入出力数と一致する．
  SizeType row_size = io_size;

  // 入力数
  SizeType isize = io_size - 1;

  // 出力値の位置
  SizeType opos = row_size - 1;

  for ( auto ast_udp_entry: table_list ) {
    const auto& tfr = ast_udp_entry.file_region();
    auto input_list = ast_udp_entry.input_list();
    if ( input_list.size() != isize ) {
      // サイズが合わない．
      log_mgr().error_udp_tablesize_mismatch(__FILE__, __LINE__, tfr);
    }

    // 一行文のデータを保持しておくためのバッファ
    std::vector<VlUdpVal> row_data;
    row_data.reserve(row_size);
    // 入力
    for ( auto ast_v: input_list ) {
      auto symbol = ast_v.symbol();
      if ( symbol.is_edge_symbol() ) {
	// 組合せ回路の場合にはエッジタイプの値は使えない．
	log_mgr().error_udp_trans_sym(__FILE__, __LINE__, ast_v);
      }
      if ( symbol.is_nc_symbol() ) {
	// NC は状態出力にしか使えない
	log_mgr().error_udp_nc_sym_in_input(__FILE__, __LINE__, ast_v);
      }
      row_data.push_back(symbol);
    }

    { // 組み合わせ回路は現状態を持たない．
      if ( ast_udp_entry.current().is_valid() ) {
	log_mgr().error_udp_wrong_cur_state(__FILE__, __LINE__,
					    ast_udp_entry);
      }
    }

    { // 出力
      auto ast_v = ast_udp_entry.output();
      auto symbol = ast_v.symbol();
      if ( symbol.is_composite_symbol() ) {
	// 出力には複合値は使えない
	log_mgr().error_udp_composite_sym_in_output(__FILE__, __LINE__, ast_v);
      }

      row_data.push_back(symbol);
    }

    // 一行文のデータを設定する．
    udp->add_tableentry(ast_udp_entry, row_data);
  }
}

// @brief 順序回路用のテーブルを生成する．
void
UdpGen::gen_seq_table(
  ElbUdpDefn* udp,
  SizeType io_size,
  const AstUdpEntryList& table_list
)
{

  // 一行のサイズは入出力 + 現状態
  SizeType row_size = io_size + 1;

  // 入力数
  SizeType isize = io_size - 1;

  // 現状態値の位置
  SizeType cpos = isize;

  // 出力値の位置
  SizeType opos = io_size;

  for ( auto ast_udp_entry: table_list ) {
    const auto& tfr = ast_udp_entry.file_region();
    if ( ast_udp_entry.input_list().size() != isize ) {
      // サイズが合わない．
      log_mgr().error_udp_tablesize_mismatch(__FILE__, __LINE__, tfr);
    }

    // 一行中に含まれるエッジタイプのシンボルの数
    SizeType nt = 0;

    // 一行文のデータを保持しておくためのバッファ
    std::vector<VlUdpVal> row_data;
    row_data.reserve(row_size);

    // 入力
    for ( auto ast_v: ast_udp_entry.input_list() ) {
      auto symbol = ast_v.symbol();
      if ( symbol.is_edge_symbol() ) {
	++ nt;
	if ( nt > 1 ) {
	  // 1行に複数の遷移シンボルがある．
	  log_mgr().error_udp_dup_trans_sym(__FILE__, __LINE__,
					    ast_udp_entry);
	}
      }

      // 順序回路の入力は全ての値/シンボルが使用可
      row_data.push_back(symbol);
    }

    { // 現状態
      auto ast_v = ast_udp_entry.current();
      if ( ast_v.is_invalid() ) {
	// 現状態がない．
	log_mgr().error_udp_no_cur_state(__FILE__, __LINE__,
					 ast_udp_entry);
      }

      auto symbol = ast_v.symbol();
      if ( symbol.is_edge_symbol() ) {
	// エッジタイプの値は使えない．
	log_mgr().error_udp_trans_sym_in_cur_state(__FILE__, __LINE__,
						   ast_v);
      }
      if ( symbol.is_nc_symbol() ) {
	// NC は状態出力にしか使えない
	log_mgr().error_udp_nc_sym_in_cur_state(__FILE__, __LINE__,
						ast_v);
      }

      row_data.push_back(symbol);
    }

    { // 出力
      auto ast_v = ast_udp_entry.output();
      auto symbol = ast_v.symbol();
      if ( !symbol.is_nc_symbol() && symbol.is_edge_symbol() ) {
	// エッジタイプの値は使えない．
	log_mgr().error_udp_trans_sym_in_output(__FILE__, __LINE__,
						ast_v);
      }
      if ( symbol.is_composite_symbol() ) {
	// 出力には複合値は使えない
	log_mgr().error_udp_composite_sym_in_output(__FILE__, __LINE__,
						    ast_v);
      }

      row_data.push_back(symbol);
    }

    // 一行文のデータを追加する．
    udp->add_tableentry(ast_udp_entry, row_data);
  }
}

END_NAMESPACE_YM_VERILOG
