
/// @file BddLit.cc
/// @brief BddLit の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddLit.h"
#include "ym/BddCube.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddLit
//////////////////////////////////////////////////////////////////////

// @brief マネージャと変数番号を指定したコンストラクタ
BddLit::BddLit(
  BddMgrHolder& mgr,
  SizeType varid,
  bool inv
) : BddBase{mgr, DdEdge::zero()}
{
  if ( !_has_valid_mgr() ) {
    throw std::invalid_argument{"null BddMgr"};
  }

  auto edge = get()->variable(varid) * inv;
  _change_root(edge);
}

// @brief BddVar と極性を引数にとるコンストラクタ
BddLit::BddLit(
  const BddVar& var,
  bool inv
) : BddBase{var.positive_literal() * inv}
{
  // var が適正なマネージャを持っていない場合には
  // var.positive_literal() が例外を送出する．
}

// @brief 内容を指定したコンストラクタ
BddLit::BddLit(
  const BddMgrHolder& mgr,
  DdEdge root
) : BddBase(mgr, root)
{
  if ( !is_literal() ) {
    throw std::invalid_argument{"is not a literal"};
  }
}

// @brief 変数を取り出す．
BddVar
BddLit::var() const
{
  _check_valid();
  auto edge = root().positive_edge();
  return _var(edge);
}

// @brief 同じ変数の肯定のリテラルを返す．
BddLit
BddLit::positive_literal() const
{
  _check_valid();
  auto edge = root().positive_edge();
  return _lit(edge);
}

// @brief 同じ変数の否定のリテラルを返す．
BddLit
BddLit::negative_literal() const
{
  _check_valid();
  auto edge = root().negative_edge();
  return _lit(edge);
}

// @brief 極性を反転したリテラルを返す．
BddLit
BddLit::complement() const
{
  _check_valid();
  auto edge = ~root();
  return _lit(edge);
}

// @brief 反転属性との掛け算
BddLit
BddLit::operator*(
  bool inv
) const
{
  _check_valid();
  auto edge = root() * inv;
  return _lit(edge);
}

// @brief 反転属性との掛け算付き代入
BddLit&
BddLit::operator*=(
  bool inv
)
{
  _check_valid();
  auto edge = root() * inv;
  _change_root(edge);
  return *this;
}

// @brief 論理積を計算する．
BddCube
BddLit::operator&(
  const BddLit& right
) const
{
  return _cube_and(*this, right);
}

// @brief キューブとの論理積
BddCube
BddLit::operator&(
  const BddCube& right
) const
{
  return _cube_and(*this, right);
}

END_NAMESPACE_YM_DD
