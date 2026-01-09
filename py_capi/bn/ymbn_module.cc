
/// @file ymbn_module.cc
/// @brief Python 用の bn モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyBnModel.h"
#include "pym/PyBnNode.h"
#include "pym/PyBnFunc.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymbn_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymbn_module = {
  PyModuleDef_HEAD_INIT,
  "ymbn",
  PyDoc_STR("ymbn: Extension module for bn"),
  -1,
  ymbn_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymbn()
{
  auto m = PyModule::init(&ymbn_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyBnModel::init(m) ) {
    goto error;
  }

  if ( !PyBnNode::init(m) ) {
    goto error;
  }

  if ( !PyBnFunc::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
