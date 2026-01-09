
/// @file fraig_module.cc
/// @brief Python 用の fraig モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyFraigMgr.h"
#include "pym/PyFraigLiteral.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef fraig_module = {
  PyModuleDef_HEAD_INIT,
  "fraig",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_fraig()
{
  auto m = PyModule::init(&fraig_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyFraigMgr::init(m) ) {
    goto error;
  }
  if ( !PyFraigLiteral::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
