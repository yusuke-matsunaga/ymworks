
/// @file ymworks_module.cc
/// @brief Python 用の ymworks モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/misc.h"
#include "pym/combopt.h"
#include "pym/logic.h"
#include "pym/clib.h"
#include "pym/bn.h"
#include "pym/aig.h"
#include "pym/sat.h"
#include "pym/fraig.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef ymworks_module = {
  PyModuleDef_HEAD_INIT,
  "ymworks",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymworks()
{
  auto m = PyModule::init(&ymworks_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyModule::reg_submodule(m, "misc", PyInit_misc()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "combopt", PyInit_combopt()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "logic", PyInit_logic()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "clib", PyInit_clib()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "bn", PyInit_bn()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "aig", PyInit_aig()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "sat", PyInit_sat()) ) {
    goto error;
  }
  if ( !PyModule::reg_submodule(m, "fraig", PyInit_fraig()) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
