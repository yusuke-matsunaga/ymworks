#ifndef PYSATLITERALLIST_H
#define PYSATLITERALLIST_H

/// @file PySatLiteralList.h
/// @brief PySatLiteralList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/SatLiteral.h"
#include "pym/PyList.h"


BEGIN_NAMESPACE_YM

using PySatLiteralList = PyList<SatLiteral, PySatLiteral>;

END_NAMESPACE_YM

#endif // PYSATLITERALLIST_H
