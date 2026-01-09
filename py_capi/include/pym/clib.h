#ifndef PYM_CLIB_H
#define PYM_CLIB_H

/// @file clib.h
/// @brief clib の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief clib モジュールの初期化関数
extern "C" PyObject* PyInit_clib();

END_NAMESPACE_YM

#endif // PYM_CLIB_H
