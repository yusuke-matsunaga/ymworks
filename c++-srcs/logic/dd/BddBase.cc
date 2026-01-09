
/// @file BddBase.cc
/// @brief BddBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BddBase.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddMgr.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "DdInfoMgr.h"
#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_DD

// @brief 空のコンストラクタ
BddBase::BddBase(
) : mRoot{2}
{
}

// @brief 内容を指定したコンストラクタ
BddBase::BddBase(
  const BddMgrHolder& holder,
  DdEdge edge
) : BddMgrHolder(holder),
    mRoot{edge.body()}
{
  if ( _has_valid_mgr() ) {
    get()->activate(root());
  }
}

// @brief コピーコンストラクタ
BddBase::BddBase(
  const BddBase& src
) : BddBase(src, src.root())
{
  // delegate constructor パタン
}

// @brief コピー代入演算子
BddBase&
BddBase::operator=(
  const BddBase& src
)
{
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src._has_valid_mgr() ) {
    src.get()->activate(src.root());
  }
  if ( _has_valid_mgr() ) {
    get()->deactivate(root());
  }
  BddMgrHolder::operator=(src);
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
BddBase::~BddBase()
{
  if ( _has_valid_mgr() ) {
    get()->deactivate(root());
  }
}

// @brief 親のマネージャを返す．
BddMgr
BddBase::mgr() const
{
  return BddMgr(*this);
}

// @brief Bdd に変換する．
Bdd
BddBase::bdd() const
{
  return _bdd(root());
}

// @brief 不正値の時 true を返す．
bool
BddBase::is_invalid() const
{
  return root().is_invalid();
}

// @brief 終端の時 true を返す．
bool
BddBase::is_terminal() const
{
  return root().is_terminal();
}

// @brief 非終端ノードの時 true を返す．
bool
BddBase::is_node() const
{
  return root().is_node();
}

// @brief 変数の時 true を返す．
bool
BddBase::is_variable() const
{
  return is_positive_literal();
}

// @brief リテラルの時 true を返す．
bool
BddBase::is_literal() const
{
  if ( is_invalid() ) {
    return false;
  }
  auto node = root().node();
  auto e0 = node->edge0();
  if ( !e0.is_const() ) {
    return false;
  }
  auto e1 = node->edge1();
  if ( !e1.is_const() ) {
    return false;
  }
  return true;
}

// @brief 肯定のリテラルの時 true を返す．
bool
BddBase::is_positive_literal() const
{
  if ( !is_literal() ) {
    return false;
  }
  return !root().inv();
}

// @brief 否定のリテラルの時 true を返す．
bool
BddBase::is_negative_literal() const
{
  if ( !is_literal() ) {
    return false;
  }
  return root().inv();
}

// @brief 積項の時 true を返す．
bool
BddBase::is_cube() const
{
  if ( is_invalid() ) {
    return false;
  }

  auto e = root();
  if ( e.is_zero() ) {
    return false;
  }
  while ( !e.is_one() ) {
    auto node = e.node();
    auto inv = e.inv();
    auto e0 = node->edge0() * inv;
    auto e1 = node->edge1() * inv;
    if ( e0.is_zero() ) {
      e = e1;
    }
    else if ( e1.is_zero() ) {
      e = e0;
    }
    else {
      return false;
    }
  }
  return true;
}

// @brief 正リテラルの積項の時 true を返す．
bool
BddBase::is_posicube() const
{
  if ( is_invalid() ) {
    return false;
  }

  auto e = root();
  if ( e.is_zero() ) {
    return false;
  }
  while ( !e.is_one() ) {
    auto node = e.node();
    auto inv = e.inv();
    auto e0 = node->edge0() * inv;
    if ( e0.is_zero() ) {
      e = node->edge1() * inv;
    }
    else {
      return false;
    }
  }
  return true;
}

// @brief 根が否定されている時 true を返す．
bool
BddBase::root_inv() const
{
  return !is_invalid() && root().inv();
}

// @brief 等価比較演算
bool
BddBase::operator==(
  const BddBase& right ///< [in] オペランド
) const
{
  // 本当はマネージャも比較する必要があるが，
  // そもそもマネージャが異なって根のポインタが同じはずがない．
  return mRoot == right.mRoot;
}

// @brief ノード数を返す．
SizeType
BddBase::size() const
{
  if ( !_has_valid_mgr() ) {
    return 0;
  }
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  return info_mgr.node_num();
}

// @brief ハッシュ値を返す．
SizeType
BddBase::hash() const
{
  return root().hash();
}

// @brief 根の枝を返す．
DdEdge
BddBase::root() const
{
  return DdEdge{mRoot};
}

// @brief 根の枝を変更する．
void
BddBase::_change_root(
  DdEdge new_root
)
{
  if ( _has_valid_mgr() ) {
    // この順序なら new_root と mRoot が等しくても
    // 正しく動く
    auto mgr = get();
    mgr->activate(new_root);
    mgr->deactivate(root());
  }
  mRoot = new_root.body();
}

// @brief 根のノードのレベルを得る．
SizeType
BddBase::root_level() const
{
  auto node = root().node();
  if ( node == nullptr ) {
    throw std::logic_error{"constant node has no level"};
  }
  return node->level();
}

END_NAMESPACE_YM_DD
