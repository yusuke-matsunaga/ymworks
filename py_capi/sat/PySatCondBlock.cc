
/// @file PySatCondBlock.cc
/// @brief PySatCondBlock の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySatCondBlock.h"
#include "pym/PySatSolver.h"
#include "pym/PySatLiteral.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct SatCondBlock_Object
{
  PyObject_HEAD
  SatCondBlock mVal;
};

// Python 用のタイプ定義
PyTypeObject SatCondBlock_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PySatCondBlock::init() 中で初期化する．
};

PyObject*
__enter__(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PySatCondBlock::_get_ref(self);
  try {
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
__exit__(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ex_type",
    "ex_value",
    "trace",
    nullptr
  };
  PyObject* ex_type_obj = nullptr;
  PyObject* ex_value_obj = nullptr;
  PyObject* trace_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "OOO",
                                    const_cast<char**>(kwlist),
                                    &ex_type_obj,
                                    &ex_value_obj,
                                    &trace_obj) ) {
    return nullptr;
  }
  auto& val = PySatCondBlock::_get_ref(self);
  try {
    Py_RETURN_NONE;
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
  {"__enter__",
   __enter__,
   METH_NOARGS,
   PyDoc_STR("")},
  {"__exit__",
   reinterpret_cast<PyCFunction>(__exit__),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

END_NONAMESPACE


// @brief SatCondBlock オブジェクトを使用可能にする．
bool
PySatCondBlock::init(
  PyObject* m
)
{
  SatCondBlock_Type.tp_name = "SatCondBlock";
  SatCondBlock_Type.tp_basicsize = sizeof(SatCondBlock_Object);
  SatCondBlock_Type.tp_itemsize = 0;
  SatCondBlock_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  SatCondBlock_Type.tp_doc = PyDoc_STR("Python extended object for SatCondBlock");
  SatCondBlock_Type.tp_methods = methods;
  if ( !PyModule::reg_type(m, "SatCondBlock", &SatCondBlock_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// SatCondBlock を PyObject に変換する．
PyObject*
PySatCondBlock::Conv::operator()(
  ElemType&& val ///< [in] 元の値
)
{
  auto type = PySatCondBlock::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<SatCondBlock_Object*>(obj);
  new (&my_obj->mVal) SatCondBlock(std::move(val));
  return obj;
}

// @brief PyObject が SatCondBlock タイプか調べる．
bool
PySatCondBlock::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &SatCondBlock_Type);
}

// @brief PyObject から SatCondBlock を取り出す．
SatCondBlock&
PySatCondBlock::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<SatCondBlock_Object*>(obj);
  return my_obj->mVal;
}

// @brief SatCondBlock を表すオブジェクトの型定義を返す．
PyTypeObject*
PySatCondBlock::_typeobject()
{
  return &SatCondBlock_Type;
}

END_NAMESPACE_YM
