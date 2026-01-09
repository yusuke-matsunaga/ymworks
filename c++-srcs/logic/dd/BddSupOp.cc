
/// @file BddSupOp.cc
/// @brief BddSupOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddVarSet.h"
#include "ym/BddLit.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddSupOp.h"


BEGIN_NAMESPACE_YM_DD

// @brief サポートを表すBDDを返す．
BddVarSet
Bdd::get_support() const
{
  _check_valid();
  BddSupOp op(get());
  auto edge = op.get_step(root());
  return _varset(edge);
}

// @brief サポート変数のリスト(vector)を得る．
std::vector<BddVar>
Bdd::get_support_list() const
{
  return get_support().var_list();
}


//////////////////////////////////////////////////////////////////////
// クラス Bdd_SupOp
//////////////////////////////////////////////////////////////////////

// @brief サポートを表すBDDを返す．
DdEdge
BddSupOp::get_step(
  DdEdge edge
)
{
  if ( edge.is_const() ) {
    return DdEdge::one();
  }

  auto node = edge.node();
  if ( mTable.count(node) > 0 ) {
    return mTable.at(node);
  }
  auto level = node->level();
  auto edge0 = node->edge0();
  auto edge1 = node->edge1();
  auto r0 = get_step(edge0);
  auto r1 = get_step(edge1);
  auto tmp = cup_step(r0, r1);
  auto result = new_node(level, tmp);
  mTable.emplace(node, result);
  return result;
}

END_NAMESPACE_YM_DD
