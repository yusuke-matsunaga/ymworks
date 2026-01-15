
/// @file BnDff.cc
/// @brief BnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnDff.h"
#include "ym/BnNode.h"
#include "DffImpl.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス BnDff
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
BnDff::BnDff(
  const DffImpl* ptr
) : mPtr{ptr}
{
  if ( is_valid() ) {
    mPtr->inc_ref();
  }
}

// @brief コピーコンストラクタ
BnDff::BnDff(
  const BnDff& src
) : BnDff(src.mPtr)
{
}

// @brief デストラクタ
BnDff::~BnDff()
{
  if ( is_valid() ) {
    mPtr->dec_ref();
  }
}

// @brief DFF番号を返す．
SizeType
BnDff::id() const
{
  if ( is_valid() ) {
    return mPtr->id();
  }
  return BAD_ID;
}

// @brief 名前を返す．
const std::string&
BnDff::name() const
{
  auto& dff = _dff_impl();
  return dff.name();
}

// @brief 出力ノードを返す．
BnNode
BnDff::output() const
{
  auto& dff = _dff_impl();
  return BnNode(dff.output());
}

// @brief 入力ノードを返す．
BnNode
BnDff::input() const
{
  auto& dff = _dff_impl();
  return BnNode(dff.src());
}

// @brief リセット値
char
BnDff::reset_val() const
{
  auto& dff = _dff_impl();
  return dff.reset_val();
}

// @brief DFFの実体を返す．
const DffImpl&
BnDff::_dff_impl() const
{
  if ( !is_valid() ) {
    throw std::logic_error{"BnDff: invalid data"};
  }
  return *mPtr;
}

END_NAMESPACE_YM_BN
