#ifndef PYUDEDGELIST_H
#define PYUDEDGELIST_H

/// @file PyUdEdgeList.h
/// @brief PyUdEdgeList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/UdGraph.h"
#include "pym/PyList.h"


BEGIN_NAMESPACE_YM

using PyUdEdgeList = PyList<UdEdge, PyUdEdge>;

END_NAMESPACE_YM

#endif // PYUDEDGELIST_H
