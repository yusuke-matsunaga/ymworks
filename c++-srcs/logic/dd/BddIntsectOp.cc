
/// @file BddIntsectOp.cc
/// @brief BddIntsectOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdEdge.h"
#include "BddIntsectOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 交差をチェックする．
bool
check_intersect(
  const Bdd& left,
  const Bdd& right
)
{
  Bdd::_check_valid(left, right);
  auto edge0 = left.root();
  auto edge1 = right.root();
  if ( edge0.is_zero() || edge1.is_zero() ) {
    // 一方が０なら交差しない．
    return false;
  }
  if ( edge0.is_one() || edge1.is_one() ) {
    // 一方が1なら交差している．
    return true;
  }
  // この時点で left は終端ではない．
  BddIntsectOp op(left.get());
  auto res = op.op_step(edge0, edge1);
  return res;
}


//////////////////////////////////////////////////////////////////////
// クラス BddIntsectOp
//////////////////////////////////////////////////////////////////////

// @brief 交差チェックを行う．
bool
BddIntsectOp::op_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  if ( edge0.is_zero() || edge1.is_zero() ) {
    // 一方が０なら交差しない．
    return false;
  }
  if ( edge0.is_one() || edge1.is_one() ) {
    // 一方が1なら交差している．
    return true;
  }

  // 枝の順序を正規化する．
  if ( edge0.body() > edge1.body() ) {
    std::swap(edge0, edge1);
  }

  Apply2Key key{edge0, edge1};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key);
  }

  auto node0 = edge0.node();
  auto inv0 = edge0.inv();
  auto level0 = node0->level();

  auto node1 = edge1.node();
  auto inv1 = edge1.inv();
  auto level1 = node1->level();

  bool result;
  if ( level0 == level1 ) {
    auto edge0_0 = node0->edge0() * inv0;
    auto edge0_1 = node0->edge1() * inv0;
    auto edge1_0 = node1->edge0() * inv1;
    auto edge1_1 = node1->edge1() * inv1;
    result = op_step(edge0_0, edge1_0) || op_step(edge0_1, edge1_1);
  }
  else if ( level0 < level1 ) {
    auto edge0_0 = node0->edge0() * inv0;
    auto edge0_1 = node0->edge1() * inv0;
    result = op_step(edge0_0, edge1) || op_step(edge0_1, edge1);
  }
  else { // level > clevel
    auto edge1_0 = node1->edge0() * inv1;
    auto edge1_1 = node1->edge1() * inv1;
    result = op_step(edge0, edge1_0) || op_step(edge0, edge1_1);
  }
  mTable.emplace(key, result);
  return result;
}

END_NAMESPACE_YM_DD
