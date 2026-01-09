
/// @file ClibBusType.cc
/// @brief ClibBusType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibBusType.h"
#include "ci/CiBusType.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibBusType
//////////////////////////////////////////////////////////////////////

// @brief 内容を指定したコンストラクタ
ClibBusType::ClibBusType(
  const CiBusType* impl
) : mImpl{impl}
{
}

// @brief デストラクタ
ClibBusType::~ClibBusType()
{
}

// @brief 名前の取得
std::string
ClibBusType::name() const
{
  _check_valid();
  return mImpl->name();
}

// @brief base_type の取得
ClibBusType::BaseType
ClibBusType::base_type() const
{
  _check_valid();
  return mImpl->base_type();
}

// @brief data_type の取得
ClibBusType::DataType
ClibBusType::data_type() const
{
  _check_valid();
  return mImpl->data_type();
}

// @brief ビット幅の取得
SizeType
ClibBusType::bit_width() const
{
  _check_valid();
  return mImpl->bit_width();
}

// @brief 開始ビットの取得
SizeType
ClibBusType::bit_from() const
{
  _check_valid();
  return mImpl->bit_from();
}

// @brief 終了ビットの取得
SizeType
ClibBusType::bit_to() const
{
  _check_valid();
  return mImpl->bit_to();
}

/// @brief 向きの取得
bool
ClibBusType::downto() const
{
  _check_valid();
  return mImpl->downto();
}

END_NAMESPACE_YM_CLIB
