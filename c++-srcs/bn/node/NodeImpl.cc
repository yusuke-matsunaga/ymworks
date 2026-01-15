
/// @file NodeImpl.cc
/// @brief NodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "NodeImpl.h"
#include "NodeImpl_sub.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス NodeImpl
//////////////////////////////////////////////////////////////////////

// @brief 外部入力ノードを作る．
NodeImpl*
NodeImpl::new_primary_input(
  const ModelImpl* model,
  SizeType id,
  SizeType input_id
)
{
  return new NodeImpl_PrimaryInput(model, id, input_id);
}

// @brief DFF出力ノードを作る．
NodeImpl*
NodeImpl::new_dff_output(
  const ModelImpl* model,
  SizeType id,
  SizeType dff_id
)
{
  return new NodeImpl_DffOutput(model, id, dff_id);
}

// @brief 論理ノードを作る．
NodeImpl*
NodeImpl::new_logic(
  const ModelImpl* model,
  SizeType id,
  const FuncImpl* func,
  const std::vector<const NodeImpl*>& fanin_list
)
{
  return new NodeImpl_Logic(model, id, func, fanin_list);
}

// @brief 入力ノードの時 true を返す．
bool
NodeImpl::is_input() const
{
  return false;
}

// @brief 論理ノードの時 true を返す．
bool
NodeImpl::is_logic() const
{
  return false;
}

// @brief 外部入力ノードの時 true を返す．
bool
NodeImpl::is_primary_input() const
{
  return false;
}

// @brief DFFの出力の時 true を返す．
bool
NodeImpl::is_dff_output() const
{
  return false;
}

// @brief 入力番号を返す．
SizeType
NodeImpl::input_id() const
{
  throw std::logic_error{"not an input."};
}

// @brief DFF番号を返す．
SizeType
NodeImpl::dff_id() const
{
  throw std::logic_error{"not a DFF output."};
}

// @brief 関数を返す．
const FuncImpl*
NodeImpl::func() const
{
  throw std::logic_error{"not a logic node."};
}

// @brief ファンイン数を返す．
SizeType
NodeImpl::fanin_num() const
{
  return 0;
}

// @brief ファンインのノードを返す．
const NodeImpl*
NodeImpl::fanin(
  SizeType pos
) const
{
  throw std::out_of_range{"index out of range"};
}

// @brief ファンインのリストを返す．
const std::vector<const NodeImpl*>&
NodeImpl::fanin_list() const
{
  // 空のダミー
  static std::vector<const NodeImpl*> _;
  return _;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Input
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Input::NodeImpl_Input(
  const ModelImpl* model,
  SizeType id
) : NodeImpl(model, id)
{
}

// @brief デストラクタ
NodeImpl_Input::~NodeImpl_Input()
{
}

// @brief ノードの種類を返す．
BnNode::Type
NodeImpl_Input::type() const
{
  return BnNode::INPUT;
}

// @brief 入力ノードの時 true を返す．
bool
NodeImpl_Input::is_input() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_PrimaryInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_PrimaryInput::NodeImpl_PrimaryInput(
  const ModelImpl* model,
  SizeType id,
  SizeType iid
) : NodeImpl_Input(model, id),
    mInputId{iid}
{
}

// @brief デストラクタ
NodeImpl_PrimaryInput::~NodeImpl_PrimaryInput()
{
}

// @brief 外部入力ノードの時 true を返す．
bool
NodeImpl_PrimaryInput::is_primary_input() const
{
  return true;
}

// @brief 入力番号を返す．
SizeType
NodeImpl_PrimaryInput::input_id() const
{
  return mInputId;
}

// @brief 複製を作る．
std::unique_ptr<NodeImpl>
NodeImpl_PrimaryInput::copy() const
{
  auto new_node = new NodeImpl_PrimaryInput(_model(), id(), input_id());
  return std::unique_ptr<NodeImpl>{new_node};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_DffOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_DffOutput::NodeImpl_DffOutput(
  const ModelImpl* model,
  SizeType id,
  SizeType dff_id
) : NodeImpl_Input(model, id),
    mDffId{dff_id}
{
}

// @brief デストラクタ
NodeImpl_DffOutput::~NodeImpl_DffOutput()
{
}

// @brief DFFの出力の時 true を返す．
bool
NodeImpl_DffOutput::is_dff_output() const
{
  return true;
}

// @brief DFF番号を返す．
SizeType
NodeImpl_DffOutput::dff_id() const
{
  return mDffId;
}

// @brief 複製を作る．
std::unique_ptr<NodeImpl>
NodeImpl_DffOutput::copy() const
{
  auto new_node = new NodeImpl_DffOutput(_model(), id(), dff_id());
  return std::unique_ptr<NodeImpl>{new_node};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Logic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Logic::NodeImpl_Logic(
  const ModelImpl* model,
  SizeType id,
  const FuncImpl* func,
  const std::vector<const NodeImpl*>& fanin_list
) : NodeImpl(model, id),
    mFunc{func},
    mFaninList{fanin_list}
{
}

// @brief デストラクタ
NodeImpl_Logic::~NodeImpl_Logic()
{
}

// @brief ノードの種類を返す．
BnNode::Type
NodeImpl_Logic::type() const
{
  return BnNode::LOGIC;
}

// @brief 論理ノードの時 true を返す．
bool
NodeImpl_Logic::is_logic() const
{
  return true;
}

// @brief 関数を返す．
const FuncImpl*
NodeImpl_Logic::func() const
{
  return mFunc;
}

// @brief ファンイン数を返す．
SizeType
NodeImpl_Logic::fanin_num() const
{
  return mFaninList.size();
}

// @brief ファンインのノード番号を返す．
const NodeImpl*
NodeImpl_Logic::fanin(
  SizeType pos
) const
{
  if ( pos < 0 || pos >= fanin_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mFaninList[pos];
}

// @brief ファンイン番号のリストを返す．
const std::vector<const NodeImpl*>&
NodeImpl_Logic::fanin_list() const
{
  return mFaninList;
}

// @brief 複製を作る．
std::unique_ptr<NodeImpl>
NodeImpl_Logic::copy() const
{
  auto new_node = new NodeImpl_Logic(_model(), id(), func(), fanin_list());
  return std::unique_ptr<NodeImpl>{new_node};
}

END_NAMESPACE_YM_BN
