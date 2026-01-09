#ifndef PYM_COMBOPT_H
#define PYM_COMBOPT_H

/// @file combopt.h
/// @brief combopt の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief combopt モジュールの初期化関数
extern "C" PyObject* PyInit_combopt();

END_NAMESPACE_YM

#endif // PYM_COMBOPT_H
