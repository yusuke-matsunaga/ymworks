
/// @file clib_module.cc
/// @brief Python 用の clib モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyClibArea.h"
#include "pym/PyClibTechnology.h"
#include "pym/PyClibDelayModel.h"
#include "pym/PyClibDirection.h"
#include "pym/PyClibTimingSense.h"
#include "pym/PyClibTimingType.h"
#include "pym/PyClibVarType.h"
#include "pym/PyClibCellType.h"
#include "pym/PyClibCPV.h"
#include "pym/PyClibPatType.h"
#include "pym/PyClibBundle.h"
#include "pym/PyClibBus.h"
#include "pym/PyClibBusType.h"
#include "pym/PyClibCapacitance.h"
#include "pym/PyClibCell.h"
#include "pym/PyClibCellClass.h"
#include "pym/PyClibCellGroup.h"
#include "pym/PyClibCellLibrary.h"
#include "pym/PyClibLut.h"
#include "pym/PyClibIOMap.h"
#include "pym/PyClibPatGraph.h"
#include "pym/PyClibPin.h"
#include "pym/PyClibPinMap.h"
#include "pym/PyClibResistance.h"
#include "pym/PyClibSeqAttr.h"
#include "pym/PyClibTime.h"
#include "pym/PyClibTiming.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef clib_module = {
  PyModuleDef_HEAD_INIT,
  "clib",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_clib()
{
  auto m = PyModule::init(&clib_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyClibArea::init(m) ) {
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
  if ( !PyClibVarType::init(m) ) {
    goto error;
  }
  if ( !PyClibCellType::init(m) ) {
    goto error;
  }
  if ( !PyClibCPV::init(m) ) {
    goto error;
  }
  if ( !PyClibPatType::init(m) ) {
    goto error;
  }
  if ( !PyClibBundle::init(m) ) {
    goto error;
  }
  if ( !PyClibBus::init(m) ) {
    goto error;
  }
  if ( !PyClibBusType::init(m) ) {
    goto error;
  }
  if ( !PyClibCapacitance::init(m) ) {
    goto error;
  }
  if ( !PyClibCell::init(m) ) {
    goto error;
  }
  if ( !PyClibCellClass::init(m) ) {
    goto error;
  }
  if ( !PyClibCellGroup::init(m) ) {
    goto error;
  }
  if ( !PyClibCellLibrary::init(m) ) {
    goto error;
  }
  if ( !PyClibLut::init(m) ) {
    goto error;
  }
  if ( !PyClibIOMap::init(m) ) {
    goto error;
  }
  if ( !PyClibPatGraph::init(m) ) {
    goto error;
  }
  if ( !PyClibPin::init(m) ) {
    goto error;
  }
  if ( !PyClibPinMap::init(m) ) {
    goto error;
  }
  if ( !PyClibResistance::init(m) ) {
    goto error;
  }
  if ( !PyClibSeqAttr::init(m) ) {
    goto error;
  }
  if ( !PyClibTime::init(m) ) {
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
