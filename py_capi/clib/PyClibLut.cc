
/// @file PyClibLut.cc
/// @brief PyClibLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibLut.h"
#include "pym/PyClibVarType.h"
#include "pym/PyList.h"
#include "pym/PyFloat.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibLut_Object
{
  PyObject_HEAD
  ClibLut mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibLut_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibLut::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibLut_Object*>(self);
  obj->mVal.~ClibLut();
  Py_TYPE(self)->tp_free(self);
}

// return variable type
PyObject*
variable_type(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    nullptr
  };
  unsigned long var;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &var) ) {
    return nullptr;
  }
  auto& val = PyClibLut::_get_ref(self);
  try {
    return PyClibVarType::ToPyObject(val.variable_type(var));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return index size
PyObject*
index_num(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    nullptr
  };
  unsigned long var;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &var) ) {
    return nullptr;
  }
  auto& val = PyClibLut::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.index_num(var));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return index value
PyObject*
index(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "var",
    "pos",
    nullptr
  };
  unsigned long var;
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk",
                                    const_cast<char**>(kwlist),
                                    &var,
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibLut::_get_ref(self);
  try {
    return PyFloat::ToPyObject(val.index(var, pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return grid_value
PyObject*
grid_value(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos_array",
    nullptr
  };
  PyObject* tmp_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &tmp_obj) ) {
    return nullptr;
  }
  auto& val = PyClibLut::_get_ref(self);
  try {
    std::vector<SizeType> pos_array;
    if ( PyList<SizeType, PyUlong>::FromPyObject(tmp_obj, pos_array) ) {
      return PyFloat::ToPyObject(val.grid_value(pos_array));
    }
    else {
      PyErr_SetString(PyExc_TypeError, "1st argument should be a list of integers");
      return nullptr;
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return value
PyObject*
value(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "val_array",
    nullptr
  };
  PyObject* tmp_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &tmp_obj) ) {
    return nullptr;
  }
  auto& val = PyClibLut::_get_ref(self);
  try {
    std::vector<double> val_array;
    if ( PyList<double, PyFloat>::FromPyObject(tmp_obj, val_array) ) {
      return PyFloat::ToPyObject(val.value(val_array));
    }
    else {
      PyErr_SetString(PyExc_TypeError, "1st argument should be a list of floats");
      return nullptr;
    }
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
  {"variable_type",
   reinterpret_cast<PyCFunction>(variable_type),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return variable type")},
  {"index_num",
   reinterpret_cast<PyCFunction>(index_num),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return index size")},
  {"index",
   reinterpret_cast<PyCFunction>(index),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return index value")},
  {"grid_value",
   reinterpret_cast<PyCFunction>(grid_value),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return grid_value")},
  {"value",
   reinterpret_cast<PyCFunction>(value),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return value")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_dimension(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibLut::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.dimension());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"dimension", get_dimension, nullptr, PyDoc_STR("dimension"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibLut オブジェクトを使用可能にする．
bool
PyClibLut::init(
  PyObject* m
)
{
  ClibLut_Type.tp_name = "ClibLut";
  ClibLut_Type.tp_basicsize = sizeof(ClibLut_Object);
  ClibLut_Type.tp_itemsize = 0;
  ClibLut_Type.tp_dealloc = dealloc_func;
  ClibLut_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibLut_Type.tp_doc = PyDoc_STR("Python extended object for ClibLut");
  ClibLut_Type.tp_methods = methods;
  ClibLut_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibLut", &ClibLut_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibLut を PyObject に変換する．
PyObject*
PyClibLut::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibLut::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibLut_Object*>(obj);
  new (&my_obj->mVal) ClibLut(val);
  return obj;
}

// PyObject を ClibLut に変換する．
bool
PyClibLut::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibLut::Check(obj) ) {
    val = PyClibLut::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibLut タイプか調べる．
bool
PyClibLut::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibLut_Type);
}

// @brief PyObject から ClibLut を取り出す．
ClibLut&
PyClibLut::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibLut_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibLut を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibLut::_typeobject()
{
  return &ClibLut_Type;
}

END_NAMESPACE_YM
