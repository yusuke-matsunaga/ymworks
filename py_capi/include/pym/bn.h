#ifndef PYM_BN_H
#define PYM_BN_H

/// @file bn.h
/// @brief bn の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief bn モジュールの初期化関数
extern "C" PyObject* PyInit_bn();

END_NAMESPACE_YM

#endif // PYM_BN_H
