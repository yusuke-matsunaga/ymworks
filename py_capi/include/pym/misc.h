#ifndef PYM_MISC_H
#define PYM_MISC_H

/// @file misc.h
/// @brief misc の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief misc モジュールの初期化関数
extern "C" PyObject* PyInit_misc();

END_NAMESPACE_YM

#endif // PYM_MISC_H
