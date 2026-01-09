
/// @file XformInfo.cc
/// @brief XformInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "XformInfo.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief TruthTable を指定したコンストラクタ
XformInfo::XformInfo(
  const TruthTable& tt
) : mInputInfoArray(tt.input_num()),
    mOutputInfoArray(tt.output_num()),
    mLiteralArray(tt.output_num()),
    mDomArray(tt.output_num()),
    mOutputMap(tt.output_num())
{
  SizeType ni = tt.input_num();
  SizeType ni_exp = 1 << ni;
  SizeType no = tt.output_num();

  // まず最初に定数・リテラル関数のチェックを行う．
  std::vector<SizeType> output_list;
  output_list.reserve(no);
  for ( SizeType o = 0; o < no; ++ o ) {
    // 定数・リテラル関数(入力)のチェック
    bool has0 = false;
    bool has1 = false;
    std::vector<bool> has0_array(ni, false);
    std::vector<bool> has1_array(ni, false);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      auto val = tt.get_value(b, o);
      switch ( val ) {
      case Val3::ZERO: has0 = true; break;
      case Val3::ONE:  has1 = true; break;
      case Val3::X: break;
      }
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto val1 = val;
	if ( b & (1 << (ni - i - 1)) ) {
	  // Walsh の 1次の係数を計算する．
	  val1 = ~val1;
	}
	switch ( val1 ) {
	case Val3::ZERO: has0_array[i] = true; break;
	case Val3::ONE:  has1_array[i] = true; break;
	case Val3::X: break;
	}
      }
    }
    if ( !has1 ) {
      // 1 の出力がない．-> 定数0
      // 全部ドントケアの時は定数0と見なす(if 文の順番的に)．
      mOutputInfoArray[o] = ZERO;
    }
    else if ( !has0 ) {
      // 0 の出力がない．-> 定数1
      // 定数1は定数0が反転したものと考える．
      mOutputInfoArray[o] = ONE;
    }
    else {
      bool done = false;
      for ( SizeType i = 0; i < ni; ++ i ) {
	if ( !has1_array[i] ) {
	  // 1 がない．-> 入力i
	  mOutputInfoArray[o] = POSI_LIT;
	  mLiteralArray[o] = i;
	  done = true;
	  break;
	}
	if ( !has0_array[i] ) {
	  // 0 がない．-> 入力iの否定
	  mOutputInfoArray[o] = NEGA_LIT;
	  mLiteralArray[o] = i;
	  done = true;
	  break;
	}
      }
      if ( !done ) {
	// mOutputInfoArray は極性が確定したときに設定される．
	output_list.push_back(o);
      }
    }
  }
  // 支配関係のチェックを行う．
  // まずドントケアの少ない順に並べる．
  std::vector<SizeType> dc_array(no, 0);
  for ( auto o: output_list ) {
    SizeType c = 0;
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      auto val = tt.get_value(b, o);
      if ( val == Val3::X ) {
	++ c;
      }
    }
    dc_array[o] = c;
  }
  std::sort(output_list.begin(), output_list.end(),
	    [&](SizeType o1, SizeType o2) -> bool {
	      return dc_array[o1] < dc_array[o2];
	    });

  // これで自分より後をしらべればよい．
  SizeType no1 = output_list.size();
  std::vector<bool> done_mark(no, false);
  std::vector<SizeType> output_list2;
  output_list2.reserve(no1);
  for ( SizeType i1 = 0; i1 < no1; ++ i1 ) {
    auto o1 = output_list[i1];
    if ( done_mark[o1] ) {
      continue;
    }
    auto oid = output_list2.size();
    output_list2.push_back(o1);
    mOutputMap[o1] = oid;
    for ( SizeType i2 = i1 + 1; i2 < no1; ++ i2 ) {
      auto o2 = output_list[i2];
      bool noinv_dom = true;
      bool inv_dom = true;
      for ( SizeType b = 0; b < ni_exp; ++ b ) {
	auto val1 = tt.get_value(b, o1);
	auto val2 = tt.get_value(b, o2);
	switch ( val1 ) {
	case Val3::ZERO:
	  if ( val2 == Val3::ONE ) {
	    noinv_dom = false;
	  }
	  if ( val2 == Val3::ZERO ) {
	    inv_dom = false;
	  }
	  break;
	case Val3::ONE:
	  if ( val2 == Val3::ZERO ) {
	    noinv_dom = false;
	  }
	  if ( val2 == Val3::ONE ) {
	    inv_dom = false;
	  }
	  break;
	case Val3::X:
	  if ( val2 != Val3::X ) {
	    noinv_dom = false;
	    inv_dom = false;
	  }
	  break;
	}
	if ( !noinv_dom && !inv_dom ) {
	  break;
	}
      }
      if ( noinv_dom ) {
	// o2 は o1 に支配されている．
	mOutputInfoArray[o2] = NOINV_DOM;
	mDomArray[o2] = o1;
	done_mark[o2] = true;
      }
      else if ( inv_dom ) {
	// o2 は o1 の否定に支配されている．
	mOutputInfoArray[o2] = INV_DOM;
	mDomArray[o2] = o1;
	done_mark[o2] = true;
      }
    }
  }

  // サポートのチェック
  // ただし上で確定した出力は除く
  std::vector<bool> support_mark(ni, false);
  mInputList.reserve(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    if ( support_mark[i] ) {
      continue;
    }
    bool found = false;
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      // b の i番目の入力を反転させたインデックス
      auto b1 = b ^ (1 << (ni - i - 1));
      for ( auto o: output_list2 ) {
	auto val = tt.get_value(b, o);
	auto val1 = tt.get_value(b1, o);
	if ( val != Val3::X &&
	     val1 != Val3::X &&
	     val1 != val ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	support_mark[i] = true;
	mInputList.push_back(i);
	break;
      }
    }
  }

  // ピボットを決める．
  SizeType min_b;
  SizeType min_dc = no + 1;
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    // サポートでない変数に関するインデックスはスキップする．
    bool skip = false;
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( !support_mark[i] ) {
	if ( b & (1 << (ni - i - 1)) ) {
	  skip = true;
	  break;
	}
      }
    }
    if ( skip ) {
      continue;
    }
    // ドントケアとなっている出力の数を数える．
    SizeType dc = 0;
    for ( auto o: output_list2 ) {
      auto val = tt.get_value(b, o);
      if ( val == Val3::X ) {
	++ dc;
      }
    }
    if ( min_dc > dc ) {
      // 最小値となる入力インデックスを記録する．
      min_dc = dc;
      min_b = b;
      if ( min_dc == 0 ) {
	// 残りは調べる必要なし．
	break;
      }
    }
  }

  // 入力の反転属性を求める．
  for ( SizeType i = 0; i < ni; ++ i ) {
    if ( support_mark[i] ) {
      if ( min_b & (1 << (ni - i - 1)) ) {
	mInputInfoArray[i] = INV;
      }
      else {
	mInputInfoArray[i] = NOINV;
      }
    }
    else {
      mInputInfoArray[i] = UNUSED;
    }
  }

  // 出力の極性を決める．
  for ( auto o: output_list2 ) {
    auto val0 = tt.get_value(min_b, o);
    // 極性のチェック
    switch ( val0 ) {
    case Val3::ZERO: mOutputInfoArray[o] = NOINV_OP; break;
    case Val3::ONE:  mOutputInfoArray[o] = INV_OP; break;
    case Val3::X:    mOutputInfoArray[o] = NOINV_OP; break;
    }
  }

  // 正規化された関数を作る．
  SizeType ni2 = mInputList.size();
  SizeType ni_exp2 = 1 << ni2;
  SizeType no2 = output_list2.size();
  mNormalFunc = TruthTable(ni2, no2);
  for ( SizeType b = 0; b < ni_exp2; ++ b ) {
    // もとの入力ベクトル
    std::vector<bool> tmp_vect(ni, false);
    for ( SizeType i2 = 0; i2 < ni2; ++ i2 ) {
      auto i1 = mInputList[i2];
      if ( b & (1 << (ni2 - i2 - 1)) ) {
	tmp_vect[i1] = true;
      }
    }
    auto index1 = tt.encode_index(tmp_vect);
    for ( SizeType o2 = 0; o2 < no2; ++ o2 ) {
      SizeType o1 = output_list[o2];
      auto oinfo = output_info(o1);
      auto val = tt.get_value(index1, o1);
      if ( oinfo == INV_OP ) {
	val = ~val;
      }
      mNormalFunc.set_value(b, o2, val);
    }
  }
}

// @brief normal_func 用の BcGraph を元に戻す．
BcGraph
XformInfo::denormalize(
  const BcGraph& normal_graph
) const
{
  SizeType ni = input_num();
  SizeType no = output_num();

  BcGraph g(ni);
  SizeType op_num = normal_graph.op_num();
  for ( SizeType op_id = 0; op_id < op_num; ++ op_id ) {
    // 演算ノードをコピーする．
    // ただし入力のマッピングを行う必要がある．
    auto& src_op = normal_graph.op(op_id);
    auto op_type = src_op.type();
    auto nop = src_op.operand_num();
    std::vector<BcEdge> opr_list(nop);
    for ( SizeType i = 0; i < nop; ++ i ) {
      auto src_edge = src_op.operand(i);
      auto dst_edge = make_edge(src_edge);
      opr_list[i] = dst_edge;
    }
    if ( op_type == BcOpType::TV ) {
      g.add_tvop(src_op.tv(), opr_list);
    }
    else {
      g.add_op(op_type, opr_list);
    }
  }

  for ( SizeType o = 0; o < no; ++ o ) {
    auto o_info = output_info(o);
    switch ( o_info ) {
    case ZERO:
      g.add_zero_output();
      break;
    case ONE:
      g.add_one_output();
      break;
    case POSI_LIT:
      g.add_output(BcEdge::input(output_literal(o), false));
      break;
    case NEGA_LIT:
      g.add_output(BcEdge::input(output_literal(o), true));
      break;
    case NOINV_OP:
      {
	auto src_o = output_map(o);
	auto src_edge = normal_graph.output(src_o);
	g.add_output(make_edge(src_edge));
      }
      break;
    case INV_OP:
      {
	auto src_o = output_map(o);
	auto src_edge = normal_graph.output(src_o);
	g.add_output(~make_edge(src_edge));
      }
      break;
    case NOINV_DOM:
      {
	auto o1 = mDomArray[o];
	auto src_o = output_map(o1);
	auto src_edge = normal_graph.output(src_o);
	g.add_output(make_edge(src_edge));
      }
      break;
    case INV_DOM:
      {
	auto o1 = mDomArray[o];
	auto src_o = output_map(o1);
	auto src_edge = normal_graph.output(src_o);
	g.add_output(~make_edge(src_edge));
      }
      break;
    }
  }
  return g;
}

// @brief 変換情報を参照して枝を作る．
BcEdge
XformInfo::make_edge(
  BcEdge src_edge
) const
{
  if ( src_edge.is_input() ) {
    auto src_input_id = src_edge.input_id();
    auto input_id = input_map(src_input_id);
    auto iinfo = input_info(input_id);
    // iinfo が UNUSED のはずはない．
    if ( iinfo == UNUSED ) {
      throw std::logic_error{"UNUSED input is used"};
    }
    bool inv1 = (iinfo == INV);
    bool inv = (inv1 != src_edge.inv());
    return BcEdge::input(input_id, inv);
  }
  else {
    // 演算ノードを指す枝の場合はそのままコピーする．
    auto op_id = src_edge.op_id();
    return BcEdge::op(op_id, src_edge.inv());
  }
}

END_NAMESPACE_YM_EXSYN
