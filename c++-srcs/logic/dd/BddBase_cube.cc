
/// @file BddBase_cube.cc
/// @brief BddBase キューブ関係の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/BddCube.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddCubeOp.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddBase
//////////////////////////////////////////////////////////////////////

// @brief 論理積を計算する下請け関数
BddCube
BddBase::_cube_and(
  const BddBase& left,
  const BddBase& right
)
{
  _check_valid(left, right);
  auto edge0 = left.root();
  auto edge1 = right.root();
  if ( edge0.is_one() ) {
    return right._cube(edge1);
  }
  if ( edge1.is_one() ) {
    return left._cube(edge0);
  }
  // この時点で left, right は共に終端ではない．
  BddCubeOp op(left.get());
  auto edge = op.and_step(edge0, edge1);
  return left._cube(edge);
}

// @brief 集合差を計算する下請け関数
BddCube
BddBase::_cube_diff(
  const BddBase& left,
  const BddBase& right
)
{
  _check_valid(left, right);
  auto edge0 = left.root();
  auto edge1 = right.root();
  if ( edge0.is_one() ||
       edge1.is_one() ) {
    return left._cube(edge0);
  }
  // この時点では left, right は終端ではない．
  BddCubeOp op(left.get());
  auto edge = op.diff_step(edge0, edge1);
  return left._cube(edge);
}

// @brief 相反するリテラルを持つ時 true を返す．
bool
BddBase::_cube_conflict(
  const BddBase& left,
  const BddBase& right
)
{
  auto edge0 = left.root();
  auto edge1 = right.root();
  if ( edge0.is_zero() || edge1.is_zero() ) {
    return false;
  }
  while ( true ) {
    if ( edge0.is_one() ) {
      return false;
    }
    if ( edge1.is_one() ) {
      return false;
    }

    auto inv0 = edge0.inv();
    auto inv1 = edge1.inv();
    auto node0 = edge0.node();
    auto node1 = edge1.node();
    auto level0 = node0->level();
    auto level1 = node1->level();
    if ( level0 < level1 ) {
      auto edge0_0 = node0->edge0() * inv0;
      auto edge0_1 = node0->edge1() * inv0;
      if ( edge0_0.is_zero() ) {
	edge0 = edge0_1;
      }
      else {
	edge0 = edge0_0;
      }
    }
    else if ( level0 > level1 ) {
      auto edge1_0 = node1->edge0() * inv1;
      auto edge1_1 = node1->edge1() * inv1;
      if ( edge1_0.is_zero() ) {
	edge1 = edge1_1;
      }
      else {
	edge1 = edge1_0;
      }
    }
    else { // level0 == level1
      auto edge0_0 = node0->edge0() * inv0;
      auto edge0_1 = node0->edge1() * inv0;
      auto edge1_0 = node1->edge0() * inv1;
      auto edge1_1 = node1->edge1() * inv1;
      if ( edge0_0.is_zero() ) {
	if ( edge1_0.is_zero() ) {
	  edge0 = edge0_1;
	  edge1 = edge1_1;
	}
	else {
	  return true;
	}
      }
      else { // edge0_1.is_zero()
	if ( edge1_1.is_zero() ) {
	  edge0 = edge0_0;
	  edge1 = edge1_0;
	}
	else {
	  return true;
	}
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス BddCubeOp
//////////////////////////////////////////////////////////////////////

// @brief キューブの論理積を求める．
DdEdge
BddCubeOp::and_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  if ( edge0.is_one() ) {
    return edge1;
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto inv0 = edge0.inv();
  auto inv1 = edge1.inv();
  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  auto top = std::min(level0, level1);
  if ( level0 < level1 ) {
    auto edge0_0 = node0->edge0() * inv0;
    auto edge0_1 = node0->edge1() * inv0;
    if ( edge0_0.is_zero() ) {
      auto tmp = and_step(edge0_1, edge1);
      return new_node(level0, DdEdge::zero(), tmp);
    }
    else {
      auto tmp = and_step(edge0_0, edge1);
      return new_node(level0, tmp, DdEdge::zero());
    }
  }
  else if ( level0 > level1 ) {
    auto edge1_0 = node1->edge0() * inv1;
    auto edge1_1 = node1->edge1() * inv1;
    if ( edge1_0.is_zero() ) {
      auto tmp = and_step(edge0, edge1_1);
      return new_node(level1, DdEdge::zero(), tmp);
    }
    else {
      auto tmp = and_step(edge0, edge1_0);
      return new_node(level1, tmp, DdEdge::zero());
    }
  }
  else {
    auto edge0_0 = node0->edge0() * inv0;
    auto edge0_1 = node0->edge1() * inv0;
    auto edge1_0 = node1->edge0() * inv1;
    auto edge1_1 = node1->edge1() * inv1;
    if ( edge0_0.is_zero() ) {
      if ( edge1_0.is_zero() ) {
	auto tmp = and_step(edge0_1, edge1_1);
	return new_node(level0, DdEdge::zero(), tmp);
      }
    }
    else { // edge0_1.is_zero()
      if ( edge1_1.is_zero() ) {
	auto tmp = and_step(edge0_0, edge1_0);
	return new_node(level0, tmp, DdEdge::zero());
      }
    }
    // 相反するリテラルの積
    return DdEdge::zero();
  }
}

// @brief キューブの集合差を求める．
DdEdge
BddCubeOp::diff_step(
  DdEdge edge0,
  DdEdge edge1
)
{
  if ( edge0.is_one() ) {
    return DdEdge::one();
  }
  if ( edge1.is_one() ) {
    return edge0;
  }

  auto inv0 = edge0.inv();
  auto inv1 = edge1.inv();
  auto node0 = edge0.node();
  auto node1 = edge1.node();
  auto level0 = node0->level();
  auto level1 = node1->level();
  if ( level0 < level1 ) {
    auto edge0_0 = node0->edge0() * inv0;
    auto edge0_1 = node0->edge1() * inv0;
    if ( edge0_0.is_zero() ) {
      auto tmp = diff_step(edge0_1, edge1);
      return new_node(level0, DdEdge::zero(), tmp);
    }
    else {
      auto tmp = diff_step(edge0_0, edge1);
      return new_node(level0, tmp, DdEdge::zero());
    }
  }
  else if ( level0 > level1 ) {
    // node1 はスキップ
    auto edge1_0 = node1->edge0() * inv1;
    auto edge1_1 = node1->edge1() * inv1;
    if ( edge1_0.is_zero() ) {
      return diff_step(edge0, edge1_1);
    }
    else {
      return diff_step(edge0, edge1_0);
    }
  }
  else { // level0 == level1
    auto edge0_0 = node0->edge0() * inv0;
    auto edge0_1 = node0->edge1() * inv0;
    auto edge1_0 = node1->edge0() * inv1;
    auto edge1_1 = node1->edge1() * inv1;
    if ( edge0_0.is_zero() ) {
      if ( edge1_0.is_zero() ) {
	return diff_step(edge0_1, edge1_1);
      }
      else {
	// 相反するリテラルは無視
	auto tmp = diff_step(edge0_1, edge1_0);
	return new_node(level0, DdEdge::zero(), tmp);
      }
    }
    else { // edge0_1.is_zero()
      if ( edge1_1.is_zero() ) {
	return diff_step(edge0_0, edge1_0);
      }
      else {
	// 相反するリテラルは無視
	auto tmp = diff_step(edge0_0, edge1_1);
	return new_node(level0, tmp, DdEdge::zero());
      }
    }
  }
}

END_NAMESPACE_YM_DD
