
/// @file PyDjDec.cc
/// @brief PyDjDec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyDjDec.h"
#include "pym/PyBdd.h"
#include "pym/PyTvFunc.h"
#include "pym/PyBcGraph.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct DjDec_Object
{
  PyObject_HEAD
  DjDec mVal;
};

// Python 用のタイプ定義
PyTypeObject DjDec_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyDjDec::init() 中で初期化する．
};

// do Disjoint Support Decomposition
PyObject*
decompose(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "func",
    nullptr
  };
  PyObject* func_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &func_obj) ) {
    return nullptr;
  }
  try {
    Bdd bdd_func;
    if ( PyBdd::FromPyObject(func_obj, bdd_func) ) {
      return PyBcGraph::ToPyObject(DjDec::decompose(bdd_func));
    }
    TvFunc tv_func;
    if ( PyTvFunc::FromPyObject(func_obj, tv_func) ) {
      return PyBcGraph::ToPyObject(DjDec::decompose(tv_func));
    }
    PyErr_SetString(PyExc_TypeError, "argument 1 must be either Bdd or TvFunc");
    return nullptr;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef methods[] = {
  {"decompose",
   reinterpret_cast<PyCFunction>(decompose),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("do Disjoint Support Decomposition")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

END_NONAMESPACE


// @brief DjDec オブジェクトを使用可能にする．
bool
PyDjDec::init(
  PyObject* m
)
{
  DjDec_Type.tp_name = "DjDec";
  DjDec_Type.tp_basicsize = sizeof(DjDec_Object);
  DjDec_Type.tp_itemsize = 0;
  DjDec_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  DjDec_Type.tp_doc = PyDoc_STR("Python extended object for DjDec");
  DjDec_Type.tp_methods = methods;
  if ( !PyModule::reg_type(m, "DjDec", &DjDec_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が DjDec タイプか調べる．
bool
PyDjDec::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &DjDec_Type);
}

// @brief PyObject から DjDec を取り出す．
DjDec&
PyDjDec::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<DjDec_Object*>(obj);
  return my_obj->mVal;
}

// @brief DjDec を表すオブジェクトの型定義を返す．
PyTypeObject*
PyDjDec::_typeobject()
{
  return &DjDec_Type;
}

END_NAMESPACE_YM
