
/// @file BnNode.cc
/// @brief BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"
#include "NodeImpl.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnNode::BnNode(
  const NodeImpl* ptr
) : mPtr{ptr}
{
  if ( is_valid() ) {
    mPtr->inc_ref();
  }
}

// @brief コピーコンストラクタ
BnNode::BnNode(
  const BnNode& src
) : BnNode(src.mPtr)
{
}

// @brief コピー代入演算子
BnNode&
BnNode::operator=(
  const BnNode& src
)
{
  // mPtr == src.mPtr でも正しく動く
  if ( src.is_valid() ) {
    src.mPtr->inc_ref();
  }
  if ( is_valid() ) {
    mPtr->dec_ref();
  }
  mPtr = src.mPtr;
  return *this;
}

// @brief デストラクタ
BnNode::~BnNode()
{
  if ( is_valid() ) {
    mPtr->dec_ref();
  }
}

// @brief ノード番号を返す．
SizeType
BnNode::id() const
{
  auto& node = _node_impl();
  return node.id();
}

// @brief ノードの種類を返す．
BnNode::Type
BnNode::type() const
{
  auto& node = _node_impl();
  return node.type();
}

// @brief ノードの種類を表す文字列を返す．
std::string
BnNode::type_str() const
{
  switch ( type() ) {
  case INPUT: return "INPUT";
  case LOGIC: return "LOGIC";
  }
}

// @brief 入力ノードの時 true を返す．
bool
BnNode::is_input() const
{
  auto& node = _node_impl();
  return node.is_input();
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  auto& node = _node_impl();
  return node.is_logic();
}

// @brief 外部入力ノードの時 true を返す．
bool
BnNode::is_primary_input() const
{
  auto& node = _node_impl();
  return node.is_primary_input();
}

// @brief DFFの出力ノードの時 true を返す．
bool
BnNode::is_dff_output() const
{
  auto& node = _node_impl();
  return node.is_dff_output();
}

// @brief 入力番号を返す．
SizeType
BnNode::input_id() const
{
  auto& node = _node_impl();
  return node.input_id();
}

// @brief DFF番号を返す．
SizeType
BnNode::dff_id() const
{
  auto& node = _node_impl();
  return node.dff_id();
}

// @brief 関数情報を返す．
BnFunc
BnNode::func() const
{
  auto& node = _node_impl();
  auto func= node.func();
  return BnFunc(func);
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  auto& node = _node_impl();
  return node.fanin_num();
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  auto& node = _node_impl();
  return node.fanin(pos);
}

// @brief ノードのファンインのノードのリストを返す．
BnNodeList
BnNode::fanin_list() const
{
  auto& node = _node_impl();
  auto& ptr_list = node.fanin_list();
  return BnNodeList(ptr_list);
}

// @brief ノードの実体を返す．
const NodeImpl&
BnNode::_node_impl() const
{
  if ( !is_valid() ) {
    throw std::logic_error{"BnNode: invalid data"};
  }
  return *mPtr;
}

END_NAMESPACE_YM_BN
