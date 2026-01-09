
/// @file ClibLibraryPtr.cc
/// @brief ClibLibraryPtr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibLibraryPtr.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB

// @brief 値を指定したコンストラクタ
ClibLibraryPtr::ClibLibraryPtr(
  const CiCellLibrary* library
) : mPtr{library}
{
  if ( mPtr != nullptr ) {
    mPtr->inc_ref();
  }
}

// @brief コピーコンストラクタ
ClibLibraryPtr::ClibLibraryPtr(
  const ClibLibraryPtr& src
) : mPtr{src.mPtr}
{
  if ( mPtr != nullptr ) {
    mPtr->inc_ref();
  }
}

// @brief 代入演算子
ClibLibraryPtr&
ClibLibraryPtr::operator=(
  const ClibLibraryPtr& src
)
{
  if ( src.mPtr != mPtr ) {
    if ( mPtr != nullptr ) {
      mPtr->dec_ref();
    }
    mPtr = src.mPtr;
    if ( mPtr != nullptr ) {
      mPtr->inc_ref();
    }
  }
  return *this;
}

// @brief デストラクタ
ClibLibraryPtr::~ClibLibraryPtr()
{
  if ( mPtr != nullptr ) {
    mPtr->dec_ref();
  }
}

END_NAMESPACE_YM_CLIB
