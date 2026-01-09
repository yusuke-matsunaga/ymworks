
/// @file BddBase_varset.cc
/// @brief VarSet 関連の下請け関数の実装
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/BddVarSet.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddMgrImpl.h"
#include "BddSupOp.h"


BEGIN_NAMESPACE_YM_DD


//////////////////////////////////////////////////////////////////////
// クラス BddBase
//////////////////////////////////////////////////////////////////////

// @brief サポート集合のユニオンを計算する．
BddVarSet
BddBase::_varset_cup(
  const BddBase& left,
  const BddBase& right
)
{
  _check_valid(left, right);
  auto ledge = left.root();
  auto redge = right.root();
  if ( ledge.is_one() ) {
    return right._varset(redge);
  }
  if ( redge.is_one() ) {
    return left._varset(ledge);
  }
  // この時点で left, right は終端ではない．
  BddSupOp op(left.get());
  auto edge = op.cup_step(ledge, redge);
  return left._varset(edge);
}

// @brief サポート集合のインターセクションを計算する．
BddVarSet
BddBase::_varset_cap(
  const BddBase& left,
  const BddBase& right
)
{
  _check_valid(left, right);
  auto ledge = left.root();
  auto redge = right.root();
  if ( ledge.is_one() || redge.is_one() ) {
    return BddVarSet::empty_set();
  }
  // この時点では left, right は終端ではない．
  BddSupOp op(left.get());
  auto edge = op.cap_step(ledge, redge);
  return left._varset(edge);
}

// @brief サポート集合の差を計算する．
BddVarSet
BddBase::_varset_diff(
  const BddBase& left,
  const BddBase& right
)
{
  _check_valid(left, right);
  auto ledge = left.root();
  auto redge = right.root();
  if ( ledge.is_one() || redge.is_one() ) {
    return left._varset(ledge);
  }
  // この時点では left, right は終端ではない．
  BddSupOp op(left.get());
  auto edge = op.diff_step(ledge, redge);
  return left._varset(edge);
}

// @brief 共通部分を持つかチェックする．
bool
BddBase::_varset_check(
  const BddBase& left,
  const BddBase& right
)
{
  _check_valid(left, right);
  auto e1 = left.root();
  auto e2 = right.root();
  while ( !e1.is_one() && !e2.is_one() ) {
    auto node1 = e1.node();
    auto node2 = e2.node();
    auto level1 = node1->level();
    auto level2 = node2->level();
    if ( level1 == level2 ) {
      return true;
    }
    else if ( level1 < level2 ) {
      e1 = node1->edge1();
    }
    else { // level > level2
      e2 = node2->edge1();
    }
  }
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス BddSupOp
//////////////////////////////////////////////////////////////////////

// @brief サポートのユニオンを求める．
DdEdge
BddSupOp::cup_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return edge1;
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  auto top = std::min(level0, level1);
  if ( level0 < level1 ) {
    auto tmp = cup_step(node0->edge1(), edge1);
    return new_node(level0, tmp);
  }
  else if ( level0 == level1 ) {
    auto tmp = cup_step(node0->edge1(), node1->edge1());
    return new_node(level0, tmp);
  }
  else {
    auto tmp = cup_step(edge0, node1->edge1());
    return new_node(level1, tmp);
  }
}

// @brief サポートのインターセクションを求める．
DdEdge
BddSupOp::cap_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() || edge1.is_one() ) {
    return DdEdge::one();
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  auto top = std::min(level0, level1);
  if ( level0 < level1 ) {
    return cap_step(node0->edge1(), edge1);
  }
  else if ( level0 == level1 ) {
    auto tmp = cap_step(node0->edge1(), node1->edge1());
    return new_node(level0, tmp);
  }
  else {
    return cap_step(edge0, node1->edge1());
  }
}

// @brief サポートのユニオンを求める．
DdEdge
BddSupOp::diff_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  ASSERT_COND( !edge0.is_zero() );
  ASSERT_COND( !edge1.is_zero() );

  if ( edge0.is_one() ) {
    return DdEdge::one();
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  if ( level0 < level1 ) {
    auto tmp = diff_step(node0->edge1(), edge1);
    return new_node(level0, tmp);
  }
  else if ( level0 == level1 ) {
    return diff_step(node0->edge1(), node1->edge1());
  }
  else { // level0 > level1
    return diff_step(edge0, node1->edge1());
  }
}

END_NAMESPACE_YM_DD
