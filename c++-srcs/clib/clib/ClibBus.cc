
/// @file ClibBus.cc
/// @brief ClibBus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibBus.h"
#include "ym/ClibBusType.h"
#include "ym/ClibPin.h"
#include "ci/CiBus.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibBus
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
std::string
ClibBus::name() const
{
  _check_valid();
  return _impl()->name();
}

// @brief バスの型の取得
ClibBusType
ClibBus::bus_type() const
{
  _check_valid();
  auto bus_type = _impl()->bus_type();
  return ClibBusType{bus_type};
}

// @brief ピン数の取得
SizeType
ClibBus::pin_num() const
{
  _check_valid();
  return _impl()->pin_num();
}

// @brief ピンの取得
ClibPin
ClibBus::pin(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->pin(pos);
  return ClibPin{_cell(), pin};
}

// @brief ピンのリストの取得
ClibPinList
ClibBus::pin_list() const
{
  _check_valid();
  return ClibPinList{_cell(), _impl()->pin_list()};
}

END_NAMESPACE_YM_CLIB
