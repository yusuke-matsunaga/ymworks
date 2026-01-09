#ifndef PYM_FRAIG_H
#define PYM_FRAIG_H

/// @file fraig.h
/// @brief fraig の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief fraig モジュールの初期化関数
extern "C" PyObject* PyInit_fraig();

END_NAMESPACE_YM

#endif // PYM_FRAIG_H
