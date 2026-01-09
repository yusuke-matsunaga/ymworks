
/// @file bn_module.cc
/// @brief Python 用の bn モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyBnModel.h"
#include "pym/PyBnDff.h"
#include "pym/PyBnDffList.h"
#include "pym/PyBnDffIter2.h"
#include "pym/PyBnNode.h"
#include "pym/PyBnNodeList.h"
#include "pym/PyBnNodeIter2.h"
#include "pym/PyBnFunc.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef bn_module = {
  PyModuleDef_HEAD_INIT,
  "bn",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_bn()
{
  auto m = PyModule::init(&bn_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyBnModel::init(m) ) {
    goto error;
  }
  if ( !PyBnDff::init(m) ) {
    goto error;
  }
  if ( !PyBnDffList::init(m) ) {
    goto error;
  }
  if ( !PyBnDffIter2::init(m) ) {
    goto error;
  }
  if ( !PyBnNode::init(m) ) {
    goto error;
  }
  if ( !PyBnNodeList::init(m) ) {
    goto error;
  }
  if ( !PyBnNodeIter2::init(m) ) {
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
