
/// @file Bdd.cc
/// @brief Bdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddVarSet.h"
#include "DdEdge.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddVar
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BddVar::BddVar(
  const BddMgrHolder& mgr,
  DdEdge root
) : BddBase(mgr, root)
{
  if ( !is_variable() ) {
    throw std::invalid_argument{"not a variable"};
  }
}

// @brief 変数番号を返す．
SizeType
BddVar::varid() const
{
  return get()->level_to_varid(level());
}

// @brief レベルを返す．
SizeType
BddVar::level() const
{
  _check_valid();
  auto node = root().node();
  return node->level();
}

// @brief 肯定のリテラルを返す．
BddLit
BddVar::positive_literal() const
{
  // 実は自身と同じ
  return _lit(root());
}

// @brief 否定のリテラルを返す．
BddLit
BddVar::negative_literal() const
{
  return _lit(~root());
}

// @brief 他の変数との集合を作る．
BddVarSet
BddVar::operator+(
  const BddVar& right
) const
{
  return _varset_cup(*this, right);
}

// @brief BddVarSet に追加する．
BddVarSet
BddVar::operator+(
  const BddVarSet& right
) const
{
  return _varset_cup(*this, right);
}

// @brief 比較演算子
bool
BddVar::operator<(
  const BddVar& right
) const
{
  return level() < right.level();
}

// @brief 比較演算子
bool
BddVar::operator<=(
  const BddVar& right
) const
{
  return level() <= right.level();
}

END_NAMESPACE_YM_DD
