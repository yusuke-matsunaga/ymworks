
/// @file BddVarSet.cc
/// @brief BddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"


BEGIN_NAMESPACE_YM_DD

// @brief 内容を指定したコンストラクタ
BddVarSet::BddVarSet(
  const BddMgrHolder& holder,
  DdEdge root
) : BddBase(holder, root)
{
  if ( !is_invalid() && !is_posicube() ) {
    throw std::invalid_argument{"not a positive cube"};
  }
}

// @brief 要素のリストを指定したコンストラクタ
BddVarSet::BddVarSet(
  const std::vector<BddVar>& var_set
) : BddBase(rep(var_set), DdEdge::one())
{
  for ( auto var: var_set ) {
    *this = _varset_cup(*this, var);
  }
}

// @brief BddVar からの変換コンストラクタ
BddVarSet::BddVarSet(
  const BddVar& var
) : BddBase(var)
{
}

// @brief 空集合を返す．
BddVarSet
BddVarSet::empty_set()
{
  return BddVarSet(BddMgrHolder{}, DdEdge::one());
}

// @brief 不正値を返す．
BddVarSet
BddVarSet::invalid()
{
  return BddVarSet(BddMgrHolder{}, DdEdge::invalid());
}

// @brief 空集合の時 true を返す．
bool
BddVarSet::is_empty_set() const
{
  return root().is_one();
}

// @brief 変数の数を返す．
SizeType
BddVarSet::size() const
{
  auto dummy = var_list();
  return dummy.size();
}

// @brief 変数のリストに変換する．
std::vector<BddVar>
BddVarSet::var_list() const
{
  _check_valid();
  auto edge = root();
  std::vector<BddVar> var_list;
  while ( !edge.is_const() ) {
    auto node = edge.node();
    auto var = _var(node->level());
    var_list.push_back(var);
    edge = node->edge1();
  }
  return var_list;
}

/// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  const BddVarSet& varset
)
{
  const char* sep = "";
  for ( auto var: varset.var_list() ) {
    s << sep << var;
    sep = " ";
  }
  return s;
}

END_NAMESPACE_YM_DD
