#ifndef PYM_SAT_H
#define PYM_SAT_H

/// @file sat.h
/// @brief sat の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief sat モジュールの初期化関数
extern "C" PyObject* PyInit_sat();

END_NAMESPACE_YM

#endif // PYM_SAT_H
