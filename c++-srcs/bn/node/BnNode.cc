
/// @file BnNode.cc
/// @brief BnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnModel.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BnNode::BnNode(
  const std::shared_ptr<ModelImpl>& model,
  SizeType id
) : BnBase(model),
    mId{id}
{
  if ( is_invalid() ) {
    mId = BAD_ID;
  }
}

// @brief デストラクタ
BnNode::~BnNode()
{
}

// @brief ノードの種類を返す．
BnNode::Type
BnNode::type() const
{
  return _node_impl().type();
}

// @brief ノードの種類を表す文字列を返す．
std::string
BnNode::type_str() const
{
  switch ( type() ) {
  case NONE:  return "NONE";
  case INPUT: return "INPUT";
  case LOGIC: return "LOGIC";
  }
}

// @brief 入力ノードの時 true を返す．
bool
BnNode::is_input() const
{
  return _node_impl().is_input();
}

// @brief 論理ノードの時 true を返す．
bool
BnNode::is_logic() const
{
  return _node_impl().is_logic();
}

// @brief 外部入力ノードの時 true を返す．
bool
BnNode::is_primary_input() const
{
  return _node_impl().is_primary_input();
}

// @brief DFFの出力ノードの時 true を返す．
bool
BnNode::is_dff_output() const
{
  return _node_impl().is_dff_output();
}

// @brief 入力番号を返す．
SizeType
BnNode::input_id() const
{
  return _node_impl().input_id();
}

// @brief DFF番号を返す．
SizeType
BnNode::dff_id() const
{
  return _node_impl().dff_id();
}

// @brief 関数情報を返す．
BnFunc
BnNode::func() const
{
  auto id = _node_impl().func_id();
  return _id2func(id);
}

// @brief ノードのファンイン数を返す．
SizeType
BnNode::fanin_num() const
{
  return _node_impl().fanin_num();
}

// @brief ノードのファンインのノードを返す．
BnNode
BnNode::fanin(
  SizeType pos
) const
{
  auto id = _node_impl().fanin_id(pos);
  return _id2node(id);
}

// @brief ノードのファンインのノードのリストを返す．
BnNodeList
BnNode::fanin_list() const
{
  auto& id_list = _node_impl().fanin_id_list();
  return _id2node_list(id_list);
}

// @brief ノードの実体を返す．
const NodeImpl&
BnNode::_node_impl() const
{
  if ( !is_valid() ) {
    throw std::logic_error{"BnNode: invalid data"};
  }
  return _model_impl().node_impl(mId);
}

END_NAMESPACE_YM_BN
