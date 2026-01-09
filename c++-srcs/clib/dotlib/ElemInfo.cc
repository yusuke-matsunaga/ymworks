
/// @file ElemInfo.cc
/// @brief ElemInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/ElemInfo.h"
#include "dotlib/LibraryInfo.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス ElemInfo
//////////////////////////////////////////////////////////////////////

// @brief ライブラリを取り出す．
CiCellLibrary*
ElemInfo::library() const
{
  return mLibraryInfo.library();
}

END_NAMESPACE_YM_DOTLIB
