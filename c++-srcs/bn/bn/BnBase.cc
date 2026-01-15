
/// @file BnBase.cc
/// @brief BnBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnBase.h"
#include "ym/BnDff.h"
#include "ym/BnDffList.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

// @brief 空のコンストラクタ
BnBase::BnBase()
{
}

// @brief 内容を指定したコンストラクタ
BnBase::BnBase(
  const std::shared_ptr<ModelImpl>& impl
) : mImpl{impl}
{
}

// @brief 内容を指定したコンストラクタ
BnBase::BnBase(
  std::shared_ptr<ModelImpl>&& impl
) : mImpl{impl}
{
}

// @brief デストラクタ
BnBase::~BnBase()
{
}

// @brief ModelImpl のオブジェクトをセットする．
void
BnBase::_set_impl(
  ModelImpl* impl
)
{
  mImpl = std::shared_ptr<ModelImpl>{impl};
}

// @brief DFF番号を BnDff に変換する．
BnDff
BnBase::_id2dff(
  SizeType id
) const
{
  if ( id >= mImpl->dff_num() ) {
    throw std::out_of_range{"id is out of range"};
  }
  return BnDff(mImpl, id);
}

// @brief DFF番号のリストを BnDff のリストに変換する．
BnDffList
BnBase::_id2dff_list(
  const std::vector<SizeType>& id_list
) const
{
  BnDffList dff_list;
  dff_list.reserve(id_list.size());
  for ( auto id: id_list ) {
    dff_list.push_back(_id2dff(id));
  }
  return dff_list;
}

// @brief ノード番号を BnNode に変換する．
BnNode
BnBase::_id2node(
  SizeType id
) const
{
  if ( id >= mImpl->node_num() ) {
    throw std::out_of_range{"id is out of range"};
  }
  return BnNode(mImpl, id);
}

// @brief ノード番号のリストを BnNode のリストに変換する．
BnNodeList
BnBase::_id2node_list(
  const std::vector<SizeType>& id_list
) const
{
  BnNodeList node_list;
  node_list.reserve(id_list.size());
  for ( auto id: id_list ) {
    node_list.push_back(_id2node(id));
  }
  return node_list;
}

// @brief BnNode をノード番号に変換する．
SizeType
BnBase::_node2id(
  const BnNode& node
) const
{
  if ( !_check_impl(node) ) {
    throw std::invalid_argument{"node does not belong to this Model"};
  }
  return node.id();
}

// @brief 関数番号を BnFunc に変換する．
BnFunc
BnBase::_id2func(
  SizeType id
) const
{
  if ( id >= mImpl->func_num() ) {
    throw std::out_of_range{"id is out of range"};
  }
  return BnFunc(mImpl, id);
}

// @brief 同じ ModelImpl を持つオブジェクトかチェックする．
bool
BnBase::_check_impl(
  const BnBase& base
) const
{
  return mImpl == base.mImpl;
}

// @brief BnDff のチェックを行う．
void
BnBase::_check_dff(
  const BnDff& dff
) const
{
  if ( !dff.is_valid() ) {
    throw std::invalid_argument{"'dff' is invalid."};
  }
  if ( !_check_impl(dff) ) {
    throw std::invalid_argument{"'dff' does not belong to this model."};
  }
}

// @brief BnNode のチェックを行う．
void
BnBase::_check_node(
  const BnNode& node
) const
{
  if ( !node.is_valid() ) {
    throw std::invalid_argument{"'node' is invalid."};
  }
  if ( !_check_impl(node) ) {
    throw std::invalid_argument{"'node' does not belong to this model."};
  }
}

// @brief BnFunc のチェックを行う．
void
BnBase::_check_func(
  const BnFunc& func
) const
{
  if ( !func.is_valid() ) {
    throw std::invalid_argument{"'func' is invalid."};
  }
  if ( !_check_impl(func) ) {
    throw std::invalid_argument{"'func' does not belong to this model."};
  }
}

// @brief ModelImpl を返す．
const ModelImpl&
BnBase::_model_impl() const
{
  return *mImpl;
}

// @brief ModelImpl を返す．
ModelImpl&
BnBase::_model_impl()
{
  return *mImpl;
}

const std::shared_ptr<ModelImpl>&
BnBase::_model() const
{
  return mImpl;
}

END_NAMESPACE_YM_BN
