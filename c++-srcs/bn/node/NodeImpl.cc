
/// @file NodeImpl.cc
/// @brief NodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "NodeImpl.h"
#include "NodeImpl_sub.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス NodeImpl
//////////////////////////////////////////////////////////////////////

// @brief 外部入力ノードを作る．
NodeImpl*
NodeImpl::new_primary_input(
  SizeType input_id
)
{
  return new NodeImpl_PrimaryInput(input_id);
}

// @brief DFF出力ノードを作る．
NodeImpl*
NodeImpl::new_dff_output(
  SizeType dff_id
)
{
  return new NodeImpl_DffOutput(dff_id);
}

// @brief 論理ノードを作る．
NodeImpl*
NodeImpl::new_logic(
  SizeType func_id,
  const std::vector<SizeType>& fanin_list
)
{
  return new NodeImpl_Logic(func_id, fanin_list);
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

// @brief 関数番号を返す．
SizeType
NodeImpl::func_id() const
{
  throw std::logic_error{"not a logic node."};
}

// @brief ファンイン数を返す．
SizeType
NodeImpl::fanin_num() const
{
  return 0;
}

// @brief ファンインのノード番号を返す．
SizeType
NodeImpl::fanin_id(
  SizeType pos
) const
{
  throw std::out_of_range{"index out of range"};
}

// @brief ファンイン番号のリストを返す．
const std::vector<SizeType>&
NodeImpl::fanin_id_list() const
{
  // 空のダミー
  static std::vector<SizeType> _;
  return _;
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Input
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Input::NodeImpl_Input()
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
  SizeType iid
) : mInputId{iid}
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
  return std::unique_ptr<NodeImpl>{new NodeImpl_PrimaryInput{mInputId}};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_DffOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_DffOutput::NodeImpl_DffOutput(
  SizeType dff_id
) : mDffId{dff_id}
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
  return std::unique_ptr<NodeImpl>{new NodeImpl_DffOutput{*this}};
}


//////////////////////////////////////////////////////////////////////
// クラス NodeImpl_Logic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NodeImpl_Logic::NodeImpl_Logic(
  SizeType func_id,
  const std::vector<SizeType>& fanin_list
) : mFuncId{func_id},
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

// @brief 関数番号を返す．
SizeType
NodeImpl_Logic::func_id() const
{
  return mFuncId;
}

// @brief ファンイン数を返す．
SizeType
NodeImpl_Logic::fanin_num() const
{
  return mFaninList.size();
}

// @brief ファンインのノード番号を返す．
SizeType
NodeImpl_Logic::fanin_id(
  SizeType pos
) const
{
  if ( pos < 0 || pos >= fanin_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mFaninList[pos];
}

// @brief ファンイン番号のリストを返す．
const std::vector<SizeType>&
NodeImpl_Logic::fanin_id_list() const
{
  return mFaninList;
}

// @brief 複製を作る．
std::unique_ptr<NodeImpl>
NodeImpl_Logic::copy() const
{
  return std::unique_ptr<NodeImpl>{new NodeImpl_Logic{*this}};
}

END_NAMESPACE_YM_BN
