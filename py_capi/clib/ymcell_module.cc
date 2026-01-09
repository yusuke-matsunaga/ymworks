
/// @file cell_module.cc
/// @brief Python 用の cell モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyClibTechnology.h"
#include "pym/PyClibDelayModel.h"
#include "pym/PyClibDirection.h"
#include "pym/PyClibTimingSense.h"
#include "pym/PyClibTimingType.h"
#include "pym/PyClibCellType.h"
#include "pym/PyClibCPV.h"
#include "pym/PyClibVarType.h"
#include "pym/PyClibCellLibrary.h"
#include "pym/PyClibCell.h"
#include "pym/PyClibPin.h"
#include "pym/PyClibTiming.h"
#include "pym/ymlogic.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義構造体
PyMethodDef ymcell_methods[] = {
  {nullptr, nullptr, 0, nullptr},
};

// モジュール定義構造体
PyModuleDef ymcell_module = {
  PyModuleDef_HEAD_INIT,
  "ymcell",
  PyDoc_STR("ymcell: Extension module for cell"),
  -1,
  ymcell_methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_ymcell()
{
  auto m = PyModule::init(&ymcell_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyModule::reg_submodule(m, "ymlogic", PyInit_ymlogic()) ) {
    goto error;
  }

  if ( !PyClibTechnology::init(m) ) {
    goto error;
  }

  if ( !PyClibDelayModel::init(m) ) {
    goto error;
  }

  if ( !PyClibDirection::init(m) ) {
    goto error;
  }

  if ( !PyClibTimingSense::init(m) ) {
    goto error;
  }

  if ( !PyClibTimingType::init(m) ) {
    goto error;
  }

  if ( !PyClibCellType::init(m) ) {
    goto error;
  }

  if ( !PyClibCPV::init(m) ) {
    goto error;
  }

  if ( !PyClibVarType::init(m) ) {
    goto error;
  }

  if ( !PyClibCellLibrary::init(m) ) {
    goto error;
  }

  if ( !PyClibCell::init(m) ) {
    goto error;
  }

  if ( !PyClibPin::init(m) ) {
    goto error;
  }

  if ( !PyClibTiming::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
