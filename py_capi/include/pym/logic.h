#ifndef PYM_LOGIC_H
#define PYM_LOGIC_H

/// @file logic.h
/// @brief logic の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief logic モジュールの初期化関数
extern "C" PyObject* PyInit_logic();

END_NAMESPACE_YM

#endif // PYM_LOGIC_H
