
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
#include "ym/BddCube.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "DdInfoMgr.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief BddBase(の派生クラス)からの変換コンストラクタ
Bdd::Bdd(
  const BddBase& src
) : BddBase(src)
{
}

// @brief 内容を指定したコンストラクタ
Bdd::Bdd(
  const BddMgrHolder& mgr,
  DdEdge edge
) : BddBase(mgr, edge)
{
  if ( !_has_valid_mgr() && edge.is_node() ) {
    throw std::invalid_argument{"null BddMgr"};
  }
}

// @brief デストラクタ
Bdd::~Bdd()
{
}

// @brief 恒儀関数を作る．
Bdd
Bdd::zero()
{
  return Bdd(BddMgrHolder{}, DdEdge::zero());
}

// @brief 恒新関数を作る．
Bdd
Bdd::one()
{
  return Bdd(BddMgrHolder{}, DdEdge::one());
}

// @brief 不正な値を作る．
Bdd
Bdd::invalid()
{
  return Bdd(BddMgrHolder{}, DdEdge::invalid());
}

// @brief 定数0の時 true を返す．
bool
Bdd::is_zero() const
{
  return root().is_zero();
}

// @brief 定数1の時 true を返す．
bool
Bdd::is_one() const
{
  return root().is_one();
}

// @brief 定数の時 true を返す．
bool
Bdd::is_const() const
{
  return root().is_const();
}

// @brief 否定した関数を返す．
Bdd
Bdd::invert() const
{
  return _bdd(~root());
}

// @brief 極性をかけ合わせる．
Bdd
Bdd::operator*(
  bool inv
) const
{
  return _bdd(root() * inv);
}

// @brief 自分自身を否定する．
Bdd&
Bdd::invert_int()
{
  // 実はこれは BddMgr を介さない．
  _change_root(~root());
  return *this;
}

// @brief 極性をかけ合わせて代入する．
Bdd&
Bdd::operator*=(
  bool inv
)
{
  // 実はこれは BddMgr を介さない．
  _change_root(root() * inv);
  return *this;
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddVar& var,
  bool inv
) const
{
  auto lit = BddLit(var, inv);
  return cofactor(lit);
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddLit& lit
) const
{
  return _cofactor(*this, lit.bdd());
}

// @brief コファクターを計算する．
Bdd
Bdd::cofactor(
  const BddCube& cube
) const
{
  return _cofactor(*this, cube.bdd());
}

// @brief (単一)compose演算
Bdd
Bdd::compose(
  const BddVar& var,
  const Bdd& cfunc
) const
{
  return multi_compose({{var, cfunc}});
}

// @brief 根の変数とコファクターを求める．
BddVar
Bdd::root_decomp(
  Bdd& f0,
  Bdd& f1
) const
{
  _check_valid();
  _check_nonterminal();

  auto node = root().node();
  auto oinv = root().inv();
  f0 = _bdd(node->edge0() * oinv);
  f1 = _bdd(node->edge1() * oinv);
  auto var = _var(node->level());
  return var;
}

// @brief 根の変数を得る．
BddVar
Bdd::root_var() const
{
  _check_valid();
  _check_nonterminal();

  auto node = root().node();
  auto var = _var(node->level());
  return var;
}

// @brief 負のコファクターを返す．
Bdd
Bdd::root_cofactor0() const
{
  _check_valid();
  _check_nonterminal();

  auto node = root().node();
  auto oinv = root().inv();
  return _bdd(node->edge0() * oinv);
}

// @brief 正のコファクターを返す．
Bdd
Bdd::root_cofactor1() const
{
  _check_valid();
  _check_nonterminal();

  auto node = root().node();
  auto oinv = root().inv();
  return _bdd(node->edge1() * oinv);
}

// @brief サポート変数のサイズを返す．
SizeType
Bdd::support_size() const
{
  return get_support().size();
}

// @brief 評価を行う．
bool
Bdd::eval(
  const std::vector<bool>& inputs
) const
{
  if ( is_invalid() ) {
    throw std::invalid_argument{"invalid BDD"};
  }

  auto edge = root();
  for ( ; ; ) {
    if ( edge.is_zero() ) {
      return false;
    }
    if ( edge.is_one() ) {
      return true;
    }
    auto node = edge.node();
    auto inv = edge.inv();
    auto level = node->level();
    if ( inputs[level] ) {
      edge = node->edge1();
    }
    else {
      edge = node->edge0();
    }
    edge *= inv;
  }
}

// @brief 非終端ノードのチェック
void
Bdd::_check_nonterminal() const
{
  if ( !root().is_node() ) {
    throw std::invalid_argument{"not a non-terminal node"};
  }
}

// @brief 複数のBDDのノード数を数える．
SizeType
bdd_size(
  const std::vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return 0;
  }
  auto mgr = Bdd::rep(bdd_list)._get();
  auto edge_list = Bdd::_conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  return info_mgr.node_num();
}

// @brief 複数のBDDの内容を出力する．
void
display(
  std::ostream& s,
  const std::vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return;
  }
  auto mgr = Bdd::rep(bdd_list)._get();
  auto edge_list = Bdd::_conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  info_mgr.display(s);
}

// @brief 構造を表す整数配列を作る．
std::vector<SizeType>
rep_data(
  const std::vector<Bdd>& bdd_list
)
{
  if ( bdd_list.empty() ) {
    return {};
  }
  auto mgr = Bdd::rep(bdd_list)._get();
  auto edge_list = Bdd::_conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  return info_mgr.rep_data();
}

// @brief BDD の内容をバイナリダンプする．
void
dump(
  BinEnc& s,
  const std::vector<Bdd>& bdd_list
)
{
  auto mgr = Bdd::rep(bdd_list)._get();
  auto edge_list = Bdd::_conv_to_edgelist(bdd_list);
  DdInfoMgr info_mgr(edge_list, mgr);
  mgr->dump(s, info_mgr);
}

END_NAMESPACE_YM_DD
