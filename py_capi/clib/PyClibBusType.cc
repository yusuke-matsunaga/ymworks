
/// @file PyClibBusType.cc
/// @brief PyClibBusType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibBusType.h"
#include "pym/PyString.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibBusType_Object
{
  PyObject_HEAD
  ClibBusType mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibBusType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibBusType::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibBusType_Object*>(self);
  obj->mVal.~ClibBusType();
  Py_TYPE(self)->tp_free(self);
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    if ( PyClibBusType::Check(other) ) {
      auto& val2 = PyClibBusType::_get_ref(other);
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
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_valid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_invalid());
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
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyString::ToPyObject(val.name());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_base_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyString::ToPyObject("ArrayType");
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_data_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyString::ToPyObject("BitType");
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bit_width(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.bit_width());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bit_from(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.bit_from());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bit_to(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.bit_to());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_downto(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBusType::_get_ref(self);
  try {
    return PyBool_FromLong(val.downto());
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
  {"name", get_name, nullptr, PyDoc_STR("name"), nullptr},
  {"base_type", get_base_type, nullptr, PyDoc_STR("base type(\"ArrayType\" only)"), nullptr},
  {"data_type", get_data_type, nullptr, PyDoc_STR("data type"), nullptr},
  {"bit_width", get_bit_width, nullptr, PyDoc_STR("bit witdh"), nullptr},
  {"bit_from", get_bit_from, nullptr, PyDoc_STR("bit from position"), nullptr},
  {"bit_to", get_bit_to, nullptr, PyDoc_STR("bit to position"), nullptr},
  {"downto", get_downto, nullptr, PyDoc_STR("True if down-to direction"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibBusType オブジェクトを使用可能にする．
bool
PyClibBusType::init(
  PyObject* m
)
{
  ClibBusType_Type.tp_name = "ClibBusType";
  ClibBusType_Type.tp_basicsize = sizeof(ClibBusType_Object);
  ClibBusType_Type.tp_itemsize = 0;
  ClibBusType_Type.tp_dealloc = dealloc_func;
  ClibBusType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibBusType_Type.tp_doc = PyDoc_STR("Python extended object for ClibBusType");
  ClibBusType_Type.tp_richcompare = richcompare_func;
  ClibBusType_Type.tp_methods = methods;
  ClibBusType_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibBusType", &ClibBusType_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibBusType を PyObject に変換する．
PyObject*
PyClibBusType::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibBusType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibBusType_Object*>(obj);
  new (&my_obj->mVal) ClibBusType(val);
  return obj;
}

// PyObject を ClibBusType に変換する．
bool
PyClibBusType::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibBusType::Check(obj) ) {
    val = PyClibBusType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibBusType タイプか調べる．
bool
PyClibBusType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibBusType_Type);
}

// @brief PyObject から ClibBusType を取り出す．
ClibBusType&
PyClibBusType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibBusType_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibBusType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibBusType::_typeobject()
{
  return &ClibBusType_Type;
}

END_NAMESPACE_YM
