
/// @file BddCompOp.cc
/// @brief BddCompOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddMgrHolder.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddCompOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief 複合compose演算
Bdd
Bdd::_compose(
  const Bdd& left,
  const std::unordered_map<BddVar, Bdd>& compose_map
)
{
  left._check_valid();
  if ( left.is_const() ) {
    // 定数なら不変
    return left;
  }
  std::unordered_map<SizeType, DdEdge> cmap;
  for ( auto& p: compose_map ) {
    auto var = p.first;
    auto level = _var2level(var);
    auto& bdd = p.second;
    _check_valid(left, bdd);
    auto cedge = bdd.root();
    cmap.emplace(level, cedge);
  }
  BddCompOp op(left.get(), cmap);
  auto edge = op.comp_op(left.root());
  return left._bdd(edge);
}

// @brief 変数順を入れ替える演算
Bdd
Bdd::_remap_vars(
  const Bdd& left,
  const std::unordered_map<BddVar, BddLit>& varmap
)
{
  left._check_valid();
  if ( left.is_const() ) {
    // 定数なら不変
    return left;
  }
  std::unordered_map<SizeType, DdEdge> cmap;
  for ( auto& p: varmap ) {
    auto var = p.first;
    auto level = _var2level(var);
    auto lit = p.second;
    _check_valid(left, lit);
    auto cedge = lit.bdd().root();
    cmap.emplace(level, cedge);
  }
  BddCompOp op(left.get(), cmap);
  auto edge = op.comp_op(left.root());
  return left._bdd(edge);
}


//////////////////////////////////////////////////////////////////////
// クラス BddCompOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BddCompOp::BddCompOp(
  BddMgrImpl* mgr,
  const std::unordered_map<SizeType, DdEdge>& comp_map
) : BddOpBase{mgr},
    mIteOp{mgr}
{
  mCompList.reserve(comp_map.size());
  for ( auto& p: comp_map ) {
    mCompList.push_back(p);
  }
  sort(mCompList.begin(), mCompList.end(),
       [](const std::pair<SizeType, DdEdge>& a,
	  const std::pair<SizeType, DdEdge>& b)
       { return a.first < b.first; });
}

// @brief 複数のcompose演算を行う．
DdEdge
BddCompOp::comp_step(
  DdEdge edge,
  SizeType pos
)
{
  if ( edge.is_const() ) {
    return edge;
  }

  auto node = edge.node();
  auto level = node->level();

  DdEdge result;
  if ( mTable.count(node) > 0 ) {
    result = mTable.at(node);
  }
  else {
    auto clevel = mCompList[pos].first;
    while ( clevel < level ) {
      ++ pos;
      if ( pos == mCompList.size() ) {
	return edge;
      }
      clevel = mCompList[pos].first;
    }
    ASSERT_COND( clevel >= level );

    auto edge0 = node->edge0();
    auto edge1 = node->edge1();
    if ( level < clevel ) {
      auto r0 = comp_step(edge0, pos);
      auto r1 = comp_step(edge1, pos);
      result = new_node(level, r0, r1);
    }
    else { // level == clevel
      auto r0 = comp_step(edge0, pos + 1);
      auto r1 = comp_step(edge1, pos + 1);
      auto cedge = mCompList[pos].second;
      result = mIteOp.ite_step(cedge, r1, r0);
    }
    mTable.emplace(node, result);
  }
  auto inv = edge.inv();
  return result * inv;
}

END_NAMESPACE_YM_DD
