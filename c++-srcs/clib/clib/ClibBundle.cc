
/// @file ClibBundle.cc
/// @brief ClibBundle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibBundle.h"
#include "ym/ClibPin.h"
#include "ci/CiBundle.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibBundle
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
std::string
ClibBundle::name() const
{
  _check_valid();
  return _impl()->name();
}

// @brief ピン数の取得
SizeType
ClibBundle::pin_num() const
{
  _check_valid();
  return _impl()->pin_num();
}

// @brief ピンの取得
ClibPin
ClibBundle::pin(
  SizeType pos
) const
{
  _check_valid();
  auto pin = _impl()->pin(pos);
  return ClibPin{_cell(), pin};
}

// @brief ピンのリストの取得
ClibPinList
ClibBundle::pin_list() const
{
  _check_valid();
  return ClibPinList{_cell(), _impl()->pin_list()};
}

END_NAMESPACE_YM_CLIB
