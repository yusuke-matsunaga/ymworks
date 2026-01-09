
/// @file logic_module.cc
/// @brief Python 用の logic モジュールを定義する．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "pym/PyVal3.h"
#include "pym/PyPrimType.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PySopCube.h"
#include "pym/PySopCover.h"
#include "pym/PyBdd.h"
#include "pym/PyBddVar.h"
#include "pym/PyBddLit.h"
#include "pym/PyBddVarSet.h"
#include "pym/PyBddCube.h"
#include "pym/PyBddMgr.h"
#include "pym/PyBddCompMap.h"
#include "pym/PyBddVarMap.h"
#include "pym/PyTruthTable.h"
#include "pym/PyBcOpType.h"
#include "pym/PyBcOp.h"
#include "pym/PyBcEdge.h"
#include "pym/PyBcGraph.h"
#include "pym/PyDjDec.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// メソッド定義
PyMethodDef methods[] = {
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

// モジュール定義構造体
PyModuleDef logic_module = {
  PyModuleDef_HEAD_INIT,
  "logic",
  PyDoc_STR(""),
  -1,
  methods,
};

END_NONAMESPACE

PyMODINIT_FUNC
PyInit_logic()
{
  auto m = PyModule::init(&logic_module);
  if ( m == nullptr ) {
    return nullptr;
  }

  if ( !PyVal3::init(m) ) {
    goto error;
  }
  if ( !PyPrimType::init(m) ) {
    goto error;
  }
  if ( !PyLiteral::init(m) ) {
    goto error;
  }
  if ( !PyNpnMap::init(m) ) {
    goto error;
  }
  if ( !PyExpr::init(m) ) {
    goto error;
  }
  if ( !PyTvFunc::init(m) ) {
    goto error;
  }
  if ( !PySopCube::init(m) ) {
    goto error;
  }
  if ( !PySopCover::init(m) ) {
    goto error;
  }
  if ( !PyBdd::init(m) ) {
    goto error;
  }
  if ( !PyBddVar::init(m) ) {
    goto error;
  }
  if ( !PyBddLit::init(m) ) {
    goto error;
  }
  if ( !PyBddVarSet::init(m) ) {
    goto error;
  }
  if ( !PyBddCube::init(m) ) {
    goto error;
  }
  if ( !PyBddMgr::init(m) ) {
    goto error;
  }
  if ( !PyBddCompMap::init(m) ) {
    goto error;
  }
  if ( !PyBddVarMap::init(m) ) {
    goto error;
  }
  if ( !PyTruthTable::init(m) ) {
    goto error;
  }
  if ( !PyBcOpType::init(m) ) {
    goto error;
  }
  if ( !PyBcOp::init(m) ) {
    goto error;
  }
  if ( !PyBcEdge::init(m) ) {
    goto error;
  }
  if ( !PyBcGraph::init(m) ) {
    goto error;
  }
  if ( !PyDjDec::init(m) ) {
    goto error;
  }

  return m;

 error:
  Py_DECREF(m);
  return nullptr;
}

END_NAMESPACE_YM
