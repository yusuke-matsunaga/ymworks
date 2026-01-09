
/// @file ZddBase.cc
/// @brief ZddBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ZddBase.h"
#include "ym/ZddMgr.h"
#include "ZddMgrImpl.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

// @brief 空のコンストラクタ
ZddBase::ZddBase(
) : mRoot{2}
{
}

// @brief 内容を指定したコンストラクタ
ZddBase::ZddBase(
  const ZddMgrHolder& holder,
  DdEdge edge
) : ZddMgrHolder(holder),
    mRoot{edge.body()}
{
  if ( has_valid_mgr() ) {
    get()->activate(root());
  }
}

// @brief コピーコンストラクタ
ZddBase::ZddBase(
  const ZddBase& src
) : ZddBase(src, src.root())
{
  // delegate constructor パタン
}

// @brief コピー代入演算子
ZddBase&
ZddBase::operator=(
  const ZddBase& src
)
{
  // この順序なら自分自身に代入しても正しく動作する．
  if ( src.has_valid_mgr() ) {
    src.get()->activate(src.root());
  }
  if ( has_valid_mgr() ) {
    get()->deactivate(root());
  }
  ZddMgrHolder::operator=(src);
  mRoot = src.mRoot;
  return *this;
}

// @brief デストラクタ
ZddBase::~ZddBase()
{
  if ( has_valid_mgr() ) {
    get()->deactivate(root());
  }
}

// @brief 親のマネージャを返す．
ZddMgr
ZddBase::mgr() const
{
  return ZddMgr(*this);
}

// @brief Zdd に変換する
Zdd
ZddBase::zdd() const
{
  return _zdd(root());
}

// @brief 定数0の時 true を返す．
bool
ZddBase::is_zero() const
{
  return root().is_zero();
}

// @brief 定数1の時 true を返す．
bool
ZddBase::is_one() const
{
  return root().is_one();
}

// @brief 定数の時 true を返す．
bool
ZddBase::is_const() const
{
  return root().is_const();
}

// @brief 不正値の時 true を返す．
bool
ZddBase::is_invalid() const
{
  return root().is_invalid();
}

// @brief 終端の時 true を返す．
bool
ZddBase::is_terminal() const
{
  return root().is_terminal();
}

// @brief シングルトンの時 true を返す．
bool
ZddBase::is_singleton() const
{
  if ( is_invalid() ) {
    return false;
  }
  auto node = root().node();
  auto e0 = node->edge0();
  if ( !e0.is_zero() ) {
    return false;
  }
  auto e1 = node->edge1();
  if ( !e1.is_one() ) {
    return false;
  }
  return true;
}

// @brief 等価比較演算
bool
ZddBase::operator==(
  const ZddBase& right ///< [in] オペランド
) const
{
  return has_same_mgr(right) && mRoot == right.mRoot;
}

// @brief ノード数を返す．
SizeType
ZddBase::size() const
{
  if ( is_invalid() ) {
    return 0;
  }
  auto edge = root();
  auto mgr = get();
  DdInfoMgr info_mgr({edge}, mgr);
  return info_mgr.node_num();
}

// @brief ハッシュ値を返す．
SizeType
ZddBase::hash() const
{
  _check_valid();

  return root().hash();
}

// @brief 根の枝を返す．
DdEdge
ZddBase::root() const
{
  return DdEdge{mRoot};
}

// @brief 根の枝を変更する．
void
ZddBase::_change_root(
  DdEdge new_root
)
{
  if ( has_valid_mgr() ) {
    auto mgr = _get();
    // この順序なら new_root と mRoot が等しくても
    // 正しく動く
    mgr->activate(new_root);
    mgr->deactivate(root());
  }
  mRoot = new_root.body();
}

#if 0
// @brief ZDDのリストから枝のリストに変換する．
std::vector<DdEdge>
Zdd::_conv_to_edgelist(
  const std::vector<Zdd>& zdd_list
)
{
  if ( zdd_list.empty() ) {
    return {};
  }

  std::vector<DdEdge> edge_list;
  edge_list.reserve(zdd_list.size());
  for ( auto& zdd: zdd_list ) {
    auto edge = zdd.root();
    edge_list.push_back(edge);
  }
  return edge_list;
}
#endif

END_NAMESPACE_YM_DD
