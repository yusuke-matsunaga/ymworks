
/// @file BddDec2Bc.cc
/// @brief BddDec2Bc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "BddDec2Bc.h"
#include "BddDecNode.h"
#include "BddDecEdge.h"
#include "ym/BddMgr.h"
#include "ym/BddVar.h"


BEGIN_NAMESPACE_YM_DJDEC

// @brief コンストラクタ
BddDec2Bc::BddDec2Bc(
  const std::vector<BddVar>& sup_list
) : mG(sup_list.size())
{
#if 1
  SizeType vid = 0;
  for ( auto var: sup_list ) {
    mVarMap.emplace(var, vid);
    ++ vid;
  }
#endif
}

/// @brief BcGraph に変換する．
BcGraph
BddDec2Bc::make_bcgraph(
  BddDecEdge root
)
{
  auto e = convert(root);
  mG.add_output(e);
  return mG;
}

BEGIN_NONAMESPACE

std::vector<BcBvType>
bdd2tv(
  const Bdd& f,
  SizeType input_num
)
{
  SizeType vlen = 1 << input_num;
  SizeType nblk = (vlen + 63) / 64; // BcBvType は64ビット固定
  std::vector<BcBvType> tv(nblk);
  auto mgr = f.mgr();
  // ベタなやり方
  std::vector<BddLit> lit_list(input_num);
  for ( SizeType i = 0; i < input_num; ++ i ) {
    lit_list[i] = mgr.positive_literal(i);
  }
  for ( SizeType b = 0; b < vlen; ++ b ) {
    auto cube = BddCube::universal_cube();
    for ( SizeType i = 0; i < input_num; ++ i ) {
      if ( b & (1 << (input_num - i - 1)) ) {
	cube &= lit_list[i];
      }
      else {
	cube &= ~lit_list[i];
      }
    }
    auto tmp = f / cube;
    if ( tmp.is_one() ) {
      SizeType blk = b / 64;
      SizeType sft = b % 64;
      tv[blk] |= 1 << sft;
    }
  }
  return tv;
}

END_NONAMESPACE

// @brief make_bcgraph の下請け関数
BcEdge
BddDec2Bc::convert(
  BddDecEdge edge
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
    auto var = node->top();
    if ( mVarMap.count(var) == 0 ) {
      throw std::logic_error{"var is not registered"};
    }
    auto vid = mVarMap.at(var);
    return BcEdge::input(vid, inv);
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
    auto tv = bdd2tv(lf, nc);
    op_id = mG.add_tvop(tv, child_list);
  }
  return BcEdge::op(op_id, inv);
}

END_NAMESPACE_YM_DJDEC
