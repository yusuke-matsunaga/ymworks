
/// @file BddMgrHolder.cc
/// @brief BddMgrHolder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgrHolder.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddVarSet.h"
#include "ym/BddCube.h"
#include "BddMgrImpl.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

//////////////////////////////////////////////////////////////////////
// クラス BddMgrHolder
//////////////////////////////////////////////////////////////////////

// @brief 生のポインタを指定したコンストラクタ
BddMgrHolder::BddMgrHolder(
  BddMgrImpl* ptr
) : mPtr{ptr}
{
}

// @brief コピーコンストラクタ
BddMgrHolder::BddMgrHolder(
  const BddMgrHolder& src
) : mPtr{src.mPtr}
{
}

// @brief コピー代入演算子
BddMgrHolder&
BddMgrHolder::operator=(
  const BddMgrHolder& src
)
{
  mPtr = src.mPtr;

  return *this;
}

// @brief デストラクタ
BddMgrHolder::~BddMgrHolder()
{
}

// @brief DdEdge を Bdd に変換する．
Bdd
BddMgrHolder::_bdd(
  DdEdge edge
) const
{
  return Bdd(*this, edge);
}

// @brief DdEdge を BddVar に変換する．
BddVar
BddMgrHolder::_var(
  DdEdge edge
) const
{
  return BddVar(*this, edge);
}

// @brief レベルを BddVar に変換する．
BddVar
BddMgrHolder::_var(
  SizeType level
) const
{
  auto edge = get()->new_node(level, DdEdge::zero(), DdEdge::one());
  return _var(edge);
}

// @brief DdEdge を BddLit に変換する．
BddLit
BddMgrHolder::_lit(
  DdEdge edge
) const
{
  return BddLit(*this, edge);
}

// @brief DdEdge を BddVarSet に変換する．
BddVarSet
BddMgrHolder::_varset(
  DdEdge edge
) const
{
  return BddVarSet(*this, edge);
}

// @brief DdEdge を BddCube に変換する．
BddCube
BddMgrHolder::_cube(
  DdEdge edge
) const
{
  if ( edge.is_zero() ) {
    // 特例: 0 は invalid に置き換える．
    edge = DdEdge::invalid();
  }
  return BddCube(*this, edge);
}

// @brief BddVar のレベルを返す．
SizeType
BddMgrHolder::_var2level(
  const BddVar& var
)
{
  return var.level();
}

// @brief BddVar のリストから DdEdge のリストに変換する．
std::vector<DdEdge>
BddMgrHolder::_var2edge_list(
  const std::vector<BddVar>& var_list
)
{
  std::vector<DdEdge> edge_list;
  edge_list.reserve(var_list.size());
  for ( auto var: var_list ) {
    auto edge = var.root();
    edge_list.push_back(edge);
  }
  return edge_list;
}

// @brief BddVar のリストからレベルのリストに変換する．
std::vector<SizeType>
BddMgrHolder::_var2level_list(
  const std::vector<BddVar>& var_list
)
{
  std::vector<SizeType> level_list;
  level_list.reserve(var_list.size());
  for ( auto var: var_list ) {
    auto level = var.level();
    level_list.push_back(level);
  }
  return level_list;
}

END_NAMESPACE_YM_DD
