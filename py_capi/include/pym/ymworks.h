#ifndef PYM_YMWORKS_H
#define PYM_YMWORKS_H

/// @file ymworks.h
/// @brief ymworks の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymworks モジュールの初期化関数
extern "C" PyObject* PyInit_ymworks();

END_NAMESPACE_YM

#endif // PYM_YMWORKS_H
