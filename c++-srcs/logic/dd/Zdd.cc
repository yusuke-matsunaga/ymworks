
/// @file Zdd.cc
/// @brief Zdd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Zdd.h"
#include "ym/ZddMgr.h"
#include "ym/ZddMgrHolder.h"
#include "ZddMgrImpl.h"
#include "DdEdge.h"
#include "DdNode.h"
#include "DdInfoMgr.h"


BEGIN_NAMESPACE_YM_DD

// @brief 内容を指定したコンストラクタ
Zdd::Zdd(
  const ZddMgrHolder& mgr,
  DdEdge edge
) : ZddBase(mgr, edge)
{
  if ( !has_valid_mgr() && edge.is_node() ) {
    throw std::invalid_argument{"null ZddMgr"};
  }
}

// @brief デストラクタ
Zdd::~Zdd()
{
}

// @brief 空集合(定数0)を作る．
Zdd
Zdd::empty_set()
{
  return Zdd(ZddMgrHolder{}, DdEdge::zero());
}

// @brief 全集合(定数1)を作る．
Zdd
Zdd::universal_set()
{
  return Zdd(ZddMgrHolder{}, DdEdge::one());
}

// @brief 不正な値を作る．
Zdd
Zdd::invalid()
{
  return Zdd(ZddMgrHolder{}, DdEdge::invalid());
}

// @brief 否定した関数を返す．
Zdd
Zdd::invert() const
{
  return _zdd(~root());
}

// @brief 自分自身を否定する．
Zdd&
Zdd::invert_int()
{
  _change_root(~root());
  return *this;
}

// @brief 共通集合演算
Zdd
Zdd::cap(
  const Zdd& right
) const
{
  auto edge = _cap(right);
  return _zdd(edge);
}

// @brief CAP演算つき代入
Zdd&
Zdd::cap_int(
  const Zdd& right
)
{
  auto edge = _cap(right);
  _change_root(edge);
  return *this;
}

// @brief ユニオン演算
Zdd
Zdd::cup(
  const Zdd& right ///< [in] オペランド
) const
{
  auto edge = _cup(right);
  return _zdd(edge);
}

// @brief CUP演算つき代入
Zdd&
Zdd::cup_int(
  const Zdd& right
)
{
  auto edge = _diff(right);
  _change_root(edge);
  return *this;
}

// @brief 集合差演算
Zdd
Zdd::diff(
  const Zdd& right
) const
{
  auto edge = _diff(right);
  return _zdd(edge);
}

// @brief DIFF演算つき代入
Zdd&
Zdd::diff_int(
  const Zdd& right
)
{
  auto edge = _diff(right);
  _change_root(edge);
  return *this;
}

// @brief 直積演算
Zdd
Zdd::product(
  const Zdd& right
) const
{
  auto edge = _product(right);
  return _zdd(edge);
}

// @brief 直積を計算して代入する．
Zdd&
Zdd::product_int(
  const Zdd& right
)
{
  auto edge = _product(right);
  _change_root(edge);
  return *this;
}

// @brief 変数を含む集合を求める．
Zdd
Zdd::onset(
  const ZddItem& item
) const
{
  auto edge = _onset(item);
  return _zdd(edge);
}

// @brief 変数を含む集合を求める．
Zdd&
Zdd::onset_int(
  const ZddItem& item
)
{
  auto edge = _onset(item);
  _change_root(edge);
  return *this;
}

// @brief 変数を含まない集合を求める．
Zdd
Zdd::offset(
  const ZddItem& item
) const
{
  auto edge = _offset(item);
  return _zdd(edge);
}

// @brief offset を計算して代入する．
Zdd&
Zdd::offset_int(
  const ZddItem& item
)
{
  auto edge = _offset(item);
  _change_root(edge);
  return *this;
}

// @brief 根の変数とコファクターを求める．
ZddItem
Zdd::root_decomp(
  Zdd& f0,
  Zdd& f1
) const
{
  _check_valid();

  if ( !root().is_node() ) {
    throw std::invalid_argument{"not a non-terminal node"};
  }

  auto node = root().node();
  f0 = _zdd(node->edge0());
  f1 = _zdd(node->edge1());
  auto item = _item(node->level());
  return item;
}

// @brief 根の変数を得る．
ZddItem
Zdd::root_item() const
{
  _check_valid();

  if ( !root().is_node() ) {
    throw std::invalid_argument{"not a non-terminal node"};
  }

  auto node = root().node();
  auto item = _item(node->level());
  return item;
}

// @brief 負のコファクターを返す．
Zdd
Zdd::root_cofactor0() const
{
  _check_valid();

  if ( !root().is_node() ) {
    throw std::invalid_argument{"not a non-terminal node"};
  }

  auto node = root().node();
  return _zdd(node->edge0());
}

// @brief 正のコファクターを返す．
Zdd
Zdd::root_cofactor1() const
{
  _check_valid();

  if ( !root().is_node() ) {
    throw std::invalid_argument{"not a non-terminal node"};
  }

  auto node = root().node();
  return _zdd(node->edge1());
}


//////////////////////////////////////////////////////////////////////
// クラス ZddItem
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
ZddItem::ZddItem(
  const ZddMgrHolder& holder,
  DdEdge root
) : ZddBase(holder, root)
{
  _check_valid();
}

// @brief 要素番号を返す．
SizeType
ZddItem::id() const
{
  return get()->level_to_varid(level());
}

// @brief レベルを返す．
SizeType
ZddItem::level() const
{
  _check_valid();
  auto node = root().node();
  return node->level();
}

#if 0
// @brief レベルのリストに変換する．
std::vector<SizeType>
ZddItem::conv_to_levellist(
  const std::vector<ZddItem>& item_list
)
{
  std::vector<SizeType> level_list;
  level_list.reserve(item_list.size());
  for ( auto& item: item_list ) {
    auto level = item.level();
    level_list.push_back(level);
  }
  std::sort(level_list.begin(), level_list.end());
  return level_list;
}
#endif

END_NAMESPACE_YM_DD
