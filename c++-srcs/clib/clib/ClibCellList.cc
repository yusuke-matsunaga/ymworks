
/// @file ClibCellList.cc
/// @brief ClibCellList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellList.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibCellListIter
//////////////////////////////////////////////////////////////////////

// @brief 内容を取り出す．
ClibCell
ClibCellListIter::operator*() const
{
  if ( mPtr.is_valid() ) {
    auto ptr = mPtr->cell(mPos);
    return ClibCell{ptr};
  }
  return ClibCell{};
}


END_NAMESPACE_YM_CLIB
