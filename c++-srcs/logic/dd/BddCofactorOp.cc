
/// @file BddCofactorOp.cc
/// @brief BddCofactorOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "DdEdge.h"
#include "BddCofactorOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief コファクターを計算する．
Bdd
Bdd::_cofactor(
  const Bdd& left,
  const Bdd& right
)
{
  _check_valid(left, right);
  auto edge0 = left.root();
  auto edge1 = right.root();
  if ( edge0.is_const() ) {
    // 定数なら不変
    return left;
  }
  // この時点で left は終端ではない．
  BddCofactorOp op(left.get());
  auto edge = op.op_step(edge0, edge1);
  return left._bdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス BddCofactorOp
//////////////////////////////////////////////////////////////////////

// @brief コファクターを計算する．
DdEdge
BddCofactorOp::op_step(
  DdEdge edge,
  DdEdge cedge
)
{
  // 終端ならそのまま返す．
  if ( edge.is_const() ) {
    return edge;
  }

  // cedge が 0 なら 0 を返す．
  if ( cedge.is_zero() ) {
    return DdEdge::zero();
  }
  // cedge が 1 なら edge を返す．
  if ( cedge.is_one() ) {
    return edge;
  }

  // 正規化する．
  auto oinv = edge.inv();
  edge.make_positive();

  Apply2Key key{edge, cedge};
  if ( mTable.count(key) > 0 ) {
    return mTable.at(key) * oinv;
  }

  auto node = edge.node();
  auto level = node->level();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();

  auto cnode = cedge.node();
  auto clevel = cnode->level();
  auto cinv = cedge.inv();
  DdEdge result;
  if ( level == clevel ) {
    auto cedge0 = cnode->edge0() * cinv;
    auto cedge1 = cnode->edge1() * cinv;
    if ( cedge0.is_zero() ) {
      result = op_step(edge1, cedge1);
    }
    else if ( cedge1.is_zero() ) {
      result = op_step(edge0, cedge0);
    }
    else {
      ASSERT_NOT_REACHED;
    }
  }
  else if ( level < clevel ) {
    auto ans0 = op_step(edge0, cedge);
    auto ans1 = op_step(edge1, cedge);
    result = new_node(level, ans0, ans1);;
  }
  else { // level > clevel
    auto cedge0 = cnode->edge0() * cinv;
    auto cedge1 = cnode->edge1() * cinv;
    if ( cedge0.is_zero() ) {
      cedge = cedge1;
    }
    else if ( cedge1.is_zero() ) {
      cedge = cedge0;
    }
    else {
      ASSERT_NOT_REACHED;
    }
    result = op_step(edge, cedge);
  }
  mTable.emplace(key, result);
  return result * oinv;
}

END_NAMESPACE_YM_DD
