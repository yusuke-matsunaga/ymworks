
/// @file TvDec2Bc.cc
/// @brief TvDec2Bc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "TvDec2Bc.h"
#include "TvDecNode.h"
#include "TvDecEdge.h"


BEGIN_NAMESPACE_YM_DJDEC

// @brief コンストラクタ
TvDec2Bc::TvDec2Bc(
  SizeType input_num
) : mG(input_num)
{
}

/// @brief BcGraph に変換する．
BcGraph
TvDec2Bc::make_bcgraph(
  TvDecEdge root
)
{
  auto e = convert(root);
  mG.add_output(e);
  return mG;
}

BEGIN_NONAMESPACE

std::vector<BcBvType>
make_tv(
  const TvFunc& func
)
{
  SizeType ni = func.input_num();
  SizeType ni_exp = 1 << ni;
  SizeType nblk = (ni_exp + 63) / 64;
  std::vector<BcBvType> tv(nblk);
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto v = func.value(b);
    if ( v ) {
      SizeType index = 0;
      for ( SizeType i = 0; i < ni; ++ i ) {
	if ( b & (1 << i) ) {
	  index |= (1 << (ni - i - 1));
	}
      }
      auto blk = index / 64;
      auto sft = index % 64;
      tv[blk] |= (1 << sft);
    }
  }
  return tv;
}

END_NONAMESPACE

// @brief make_bcgraph の下請け関数
BcEdge
TvDec2Bc::convert(
  TvDecEdge edge
)
{
  if ( edge.is_zero() ) {
    return BcEdge::zero();
  }
  if ( edge.is_one() ) {
    return BcEdge::one();
  }
  auto node = edge.node();
  bool inv = edge.inv();
  if ( node->is_lit() ) {
    auto var = node->top_var();
    return BcEdge::input(var, inv);
  }

  auto nc = node->child_num();
  std::vector<BcEdge> child_list(nc);
  for ( SizeType i = 0; i < nc; ++ i ) {
    child_list[i] = convert(node->child(i));
  }
  SizeType op_id;
  if ( node->is_or() ) {
    // 入力の極性を調節する．
    bool any_inv = false;
    for ( auto cedge: child_list ) {
      if ( cedge.inv() ) {
	any_inv = true;
	break;
      }
    }
    if ( any_inv ) {
      // child_list を反転して AND にする．
      std::vector<BcEdge> child_list2(nc);
      for ( SizeType i = 0; i < nc; ++ i ) {
	child_list2[i] = ~child_list[i];
      }
      op_id = mG.add_op(BcOpType::AND, child_list2);
      inv = !inv;
    }
    else {
      op_id = mG.add_op(BcOpType::OR, child_list);
    }
  }
  else if ( node->is_xor() ) {
    op_id = mG.add_op(BcOpType::XOR, child_list);
  }
  else if ( node->is_cplx() ) {
    auto lf = node->local_func();
    // lf を真理値表ベクタに変換する．
    auto tv = make_tv(lf);
    op_id = mG.add_tvop(tv, child_list);
  }
  return BcEdge::op(op_id, inv);
}

END_NAMESPACE_YM_DJDEC
