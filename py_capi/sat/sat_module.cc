
/// @file sat_module.cc
/// @brief Python 用の sat モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyCnfSize.h"
#include "pym/PySatBool3.h"
#include "pym/PySatInitParam.h"
#include "pym/PySatLiteral.h"
#include "pym/PySatLiteralMap.h"
#include "pym/PySatModel.h"
#include "pym/PySatSolver.h"
#include "pym/PySatCondBlock.h"
#include "pym/PySatStats.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef sat_module = {
  PyModuleDef_HEAD_INIT,
  "sat",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_sat()
{
  auto m = PyModule::init(&sat_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyCnfSize::init(m) ) {
    goto error;
  }
  if ( !PySatBool3::init(m) ) {
    goto error;
  }
  if ( !PySatInitParam::init(m) ) {
    goto error;
  }
  if ( !PySatLiteral::init(m) ) {
    goto error;
  }
  if ( !PySatLiteralMap::init(m) ) {
    goto error;
  }
  if ( !PySatModel::init(m) ) {
    goto error;
  }
  if ( !PySatSolver::init(m) ) {
    goto error;
  }
  if ( !PySatCondBlock::init(m) ) {
    goto error;
  }
  if ( !PySatStats::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
