#ifndef CGPOLINFO_H
#define CGPOLINFO_H

/// @file CgPolInfo.h
/// @brief CgPolInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

/// @brief 極性情報を表す列挙型
enum class CgPolInfo {
  Positive, ///< 正極性
  Negative, ///< 負極性
  Both      ///< 未定
};

END_NAMESPACE_YM_CLIB

#endif // CGPOLINFO_H
