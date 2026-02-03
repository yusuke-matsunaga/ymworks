
/// @file PyClibPin.cc
/// @brief PyClibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibPin.h"
#include "pym/PyClibDirection.h"
#include "pym/PyClibCapacitance.h"
#include "pym/PyExpr.h"
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
struct ClibPin_Object
{
  PyObject_HEAD
  ClibPin mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibPin_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibPin::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibPin_Object*>(self);
  obj->mVal.~ClibPin();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_input());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_output(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_output());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_inout(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_inout());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_internal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_internal());
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
  {"is_input",
   is_input,
   METH_NOARGS,
   PyDoc_STR("True if INPUT")},
  {"is_output",
   is_output,
   METH_NOARGS,
   PyDoc_STR("True if OUTPUT")},
  {"is_inout",
   is_inout,
   METH_NOARGS,
   PyDoc_STR("True if INOUT")},
  {"is_internal",
   is_internal,
   METH_NOARGS,
   PyDoc_STR("True if INTERNAL")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
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
get_direction(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibDirection::ToPyObject(val.direction());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pin_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pin_id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_input_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.input_id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_capacitance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.capacitance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_rise_capacitance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.rise_capacitance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_fall_capacitance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.fall_capacitance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_output_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.output_id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_max_fanout(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.max_fanout());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_min_fanout(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.min_fanout());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_max_capacitance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.max_capacitance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_min_capacitance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyClibCapacitance::ToPyObject(val.min_capacitance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_function(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.function());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_tristate(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.tristate());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_internal_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPin::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.internal_id());
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
  {"direction", get_direction, nullptr, PyDoc_STR("direction"), nullptr},
  {"pin_id", get_pin_id, nullptr, PyDoc_STR("pin ID"), nullptr},
  {"input_id", get_input_id, nullptr, PyDoc_STR("input ID"), nullptr},
  {"capacitance", get_capacitance, nullptr, PyDoc_STR("capacitance"), nullptr},
  {"rise_capacitance", get_rise_capacitance, nullptr, PyDoc_STR("rise capacitance"), nullptr},
  {"fall_capacitance", get_fall_capacitance, nullptr, PyDoc_STR("fall capacitance"), nullptr},
  {"output_id", get_output_id, nullptr, PyDoc_STR("output ID"), nullptr},
  {"max_fanout", get_max_fanout, nullptr, PyDoc_STR("max fanout"), nullptr},
  {"min_fanout", get_min_fanout, nullptr, PyDoc_STR("min fanout"), nullptr},
  {"max_capacitance", get_max_capacitance, nullptr, PyDoc_STR("max capacitance"), nullptr},
  {"min_capacitance", get_min_capacitance, nullptr, PyDoc_STR("min capacitance"), nullptr},
  {"function", get_function, nullptr, PyDoc_STR("function"), nullptr},
  {"tristate", get_tristate, nullptr, PyDoc_STR("tristate condition"), nullptr},
  {"internal_id", get_internal_id, nullptr, PyDoc_STR("internal ID"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibPin オブジェクトを使用可能にする．
bool
PyClibPin::init(
  PyObject* m
)
{
  ClibPin_Type.tp_name = "ClibPin";
  ClibPin_Type.tp_basicsize = sizeof(ClibPin_Object);
  ClibPin_Type.tp_itemsize = 0;
  ClibPin_Type.tp_dealloc = dealloc_func;
  ClibPin_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibPin_Type.tp_doc = PyDoc_STR("Python extended object for ClibPin");
  ClibPin_Type.tp_methods = methods;
  ClibPin_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibPin", &ClibPin_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibPin を PyObject に変換する．
PyObject*
PyClibPin::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibPin::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibPin_Object*>(obj);
  new (&my_obj->mVal) ClibPin(val);
  return obj;
}

// PyObject を ClibPin に変換する．
bool
PyClibPin::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibPin::Check(obj) ) {
    val = PyClibPin::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibPin タイプか調べる．
bool
PyClibPin::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibPin_Type);
}

// @brief PyObject から ClibPin を取り出す．
ClibPin&
PyClibPin::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibPin_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibPin を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibPin::_typeobject()
{
  return &ClibPin_Type;
}

END_NAMESPACE_YM
