#ifndef PYM_AIG_H
#define PYM_AIG_H

/// @file aig.h
/// @brief aig の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief aig モジュールの初期化関数
extern "C" PyObject* PyInit_aig();

END_NAMESPACE_YM

#endif // PYM_AIG_H
