
/// @file PyClibIOMap.cc
/// @brief PyClibIOMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibIOMap.h"
#include "pym/PyClibPinMap.h"
#include "pym/PyUlong.h"
#include "pym/PyList.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibIOMap_Object
{
  PyObject_HEAD
  ClibIOMap mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibIOMap_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibIOMap::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibIOMap_Object*>(self);
  obj->mVal.~ClibIOMap();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibIOMap::Check(self) ) {
      auto& val1 = PyClibIOMap::_get_ref(self);
      if ( PyClibIOMap::Check(other) ) {
        auto& val2 = PyClibIOMap::_get_ref(other);
        return PyClibIOMap::ToPyObject(val1 * val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibIOMap::Check(self) ) {
      auto& val1 = PyClibIOMap::_get_ref(self);
      if ( PyClibIOMap::Check(other) ) {
        auto& val2 = PyClibIOMap::_get_ref(other);
        val1 *= val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_multiply = nb_multiply,
  .nb_inplace_multiply = nb_inplace_multiply
};

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    if ( PyClibIOMap::Check(other) ) {
      auto& val2 = PyClibIOMap::_get_ref(other);
      if ( op == Py_EQ ) {
        return PyBool_FromLong(val == val2);
      }
      if ( op == Py_NE ) {
        return PyBool_FromLong(val != val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
identity(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    "no",
    "nb",
    nullptr
  };
  unsigned long ni;
  unsigned long no = 1;
  unsigned long nb = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|kk",
                                    const_cast<char**>(kwlist),
                                    &ni,
                                    &no,
                                    &nb) ) {
    return nullptr;
  }
  try {
    return PyClibIOMap::ToPyObject(ClibIOMap::identity(ni, no, nb));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
input_map(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyClibPinMap::ToPyObject(val.input_map(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_input_map_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyList<ClibPinMap, PyClibPinMap>::ToPyObject(val.input_map_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
output_map(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyClibPinMap::ToPyObject(val.output_map(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_output_map_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyList<ClibPinMap, PyClibPinMap>::ToPyObject(val.output_map_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
inout_map(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyClibPinMap::ToPyObject(val.inout_map(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_inout_map_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyList<ClibPinMap, PyClibPinMap>::ToPyObject(val.inout_map_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
inverse(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyClibIOMap::ToPyObject(val.inverse());
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
  {"identity",
   reinterpret_cast<PyCFunction>(identity),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make IDENTITY map")},
  {"input_map",
   reinterpret_cast<PyCFunction>(input_map),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return input map")},
  {"meth_input_map_list",
   meth_input_map_list,
   METH_NOARGS,
   PyDoc_STR("return list of input maps")},
  {"output_map",
   reinterpret_cast<PyCFunction>(output_map),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return output map")},
  {"meth_output_map_list",
   meth_output_map_list,
   METH_NOARGS,
   PyDoc_STR("return list of output maps")},
  {"inout_map",
   reinterpret_cast<PyCFunction>(inout_map),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return inout map")},
  {"meth_inout_map_list",
   meth_inout_map_list,
   METH_NOARGS,
   PyDoc_STR("return list of inout maps")},
  {"inverse",
   inverse,
   METH_NOARGS,
   PyDoc_STR("INVERSE operation")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.input_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_output_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.output_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_inout_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibIOMap::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.inout_num());
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
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"output_num", get_output_num, nullptr, PyDoc_STR("number of outputs"), nullptr},
  {"inout_num", get_inout_num, nullptr, PyDoc_STR("number inouts"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibIOMap オブジェクトを使用可能にする．
bool
PyClibIOMap::init(
  PyObject* m
)
{
  ClibIOMap_Type.tp_name = "ClibIOMap";
  ClibIOMap_Type.tp_basicsize = sizeof(ClibIOMap_Object);
  ClibIOMap_Type.tp_itemsize = 0;
  ClibIOMap_Type.tp_dealloc = dealloc_func;
  ClibIOMap_Type.tp_as_number = &number;
  ClibIOMap_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibIOMap_Type.tp_doc = PyDoc_STR("Python extended object for ClibIOMap");
  ClibIOMap_Type.tp_richcompare = richcompare_func;
  ClibIOMap_Type.tp_methods = methods;
  ClibIOMap_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibIOMap", &ClibIOMap_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibIOMap を PyObject に変換する．
PyObject*
PyClibIOMap::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibIOMap::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibIOMap_Object*>(obj);
  new (&my_obj->mVal) ClibIOMap(val);
  return obj;
}

// PyObject を ClibIOMap に変換する．
bool
PyClibIOMap::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibIOMap::Check(obj) ) {
    val = PyClibIOMap::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibIOMap タイプか調べる．
bool
PyClibIOMap::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibIOMap_Type);
}

// @brief PyObject から ClibIOMap を取り出す．
ClibIOMap&
PyClibIOMap::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibIOMap_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibIOMap を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibIOMap::_typeobject()
{
  return &ClibIOMap_Type;
}

END_NAMESPACE_YM
