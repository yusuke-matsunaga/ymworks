
/// @file BddMgr.cc
/// @brief BddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddMgr.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief コンストラクタ
BddMgr::BddMgr(
) : BddMgrHolder(new BddMgrImpl)
{
}

// @brief BddMgrImpl を指定したコンストラクタ
BddMgr::BddMgr(
  const BddMgrHolder& holder
) : BddMgrHolder(holder)
{
}

// @brief コピーコンストラクタ
BddMgr::BddMgr(
  const BddMgr& src
) : BddMgrHolder(src)
{
}

// @brief デストラクタ
BddMgr::~BddMgr()
{
}

// @breif 変数の数を返す．
SizeType
BddMgr::variable_num() const
{
  return get()->variable_num();
}

// @brief 変数を返す．
BddVar
BddMgr::variable(
  SizeType varid
)
{
  auto edge = get()->variable(varid);
  return _var(edge);
}

// @brief 変数を表すBDDを返す．
Bdd
BddMgr::variable_bdd(
  SizeType varid
)
{
  auto var = variable(varid);
  auto bdd = var.bdd();
  return bdd;
}

// @brief リテラルを返す．
BddLit
BddMgr::literal(
  SizeType varid,
  bool inv
)
{
  auto edge = get()->variable(varid) * inv;
  return _lit(edge);
}

// @brief リテラルを返す．
Bdd
BddMgr::literal_bdd(
  SizeType varid,
  bool inv
)
{
  return literal(varid, inv).bdd();
}

// @brief 正のリテラルを返す．
BddLit
BddMgr::positive_literal(
  SizeType varid
)
{
  return literal(varid, false);
}

// @brief 負のリテラルを返す．
BddLit
BddMgr::negative_literal(
  SizeType varid
)
{
  return literal(varid, true);
}

// @brief 変数のリストを返す．
std::vector<BddVar>
BddMgr::variable_list() const
{
  auto& edge_list = get()->variable_list();
  return conv_to_varlist(edge_list);
}

// @brief 変数順を表す変数のリストを返す．
std::vector<BddVar>
BddMgr::variable_order() const
{
  auto edge_list = get()->variable_order();
  return conv_to_varlist(edge_list);
}

// @brief 変数順を設定する．
void
BddMgr::set_variable_order(
  const std::vector<BddVar>& order_list
)
{
  auto edge_list = _var2edge_list(order_list);
  get()->set_variable_order(edge_list);
}

// @brief バイナリダンプから復元する．
std::vector<Bdd>
BddMgr::restore(
  BinDec& s
)
{
  auto edge_list = get()->restore(s);
  return conv_to_bddlist(edge_list);
}

// @brief ガーベージコレクションを行う．
void
BddMgr::garbage_collection()
{
  get()->garbage_collection();
}

// @brief ノード数を返す．
SizeType
BddMgr::node_num() const
{
  return get()->node_num();
}

// @brief GC を起動するしきい値を返す．
SizeType
BddMgr::gc_limit() const
{
  return get()->gc_limit();
}

// @brief GC を起動するしきい値を設定する．
void
BddMgr::set_gc_limit(
  SizeType limit
)
{
  get()->set_gc_limit(limit);
}

// @brief GC を許可する．
void
BddMgr::enable_gc()
{
  get()->enable_gc();
}

// @brief GC を禁止する．
void
BddMgr::disable_gc()
{
  get()->disable_gc();
}

// @brief 枝のリストをBddのリストに変換する．
std::vector<Bdd>
BddMgr::conv_to_bddlist(
  const std::vector<DdEdge>& edge_list
) const
{
  std::vector<Bdd> bdd_list;
  bdd_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto bdd = _bdd(edge);
    bdd_list.push_back(bdd);
  }
  return bdd_list;
}

// @brief 枝のリストを変数のリストに変換する．
std::vector<BddVar>
BddMgr::conv_to_varlist(
  const std::vector<DdEdge>& edge_list
) const
{
  std::vector<BddVar> var_list;
  var_list.reserve(edge_list.size());
  for ( auto edge: edge_list ) {
    auto var = _var(edge);
    var_list.push_back(var);
  }
  return var_list;
}

END_NAMESPACE_YM_DD
