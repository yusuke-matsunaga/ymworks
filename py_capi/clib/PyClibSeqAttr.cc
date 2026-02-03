
/// @file PyClibSeqAttr.cc
/// @brief PyClibSeqAttr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibSeqAttr.h"
#include "pym/PyClibCPV.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibSeqAttr_Object
{
  PyObject_HEAD
  ClibSeqAttr mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibSeqAttr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibSeqAttr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibSeqAttr_Object*>(self);
  obj->mVal.~ClibSeqAttr();
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
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    if ( PyClibSeqAttr::Check(other) ) {
      auto& val2 = PyClibSeqAttr::_get_ref(other);
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
has_slave_clock(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    return PyBool_FromLong(val.has_slave_clock());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
has_clear(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    return PyBool_FromLong(val.has_clear());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
has_preset(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    return PyBool_FromLong(val.has_preset());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
max_index(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyUlong::ToPyObject(ClibSeqAttr::max_index());
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
  {"has_slave_clock",
   has_slave_clock,
   METH_NOARGS,
   PyDoc_STR("True if having slave clock")},
  {"has_clear",
   has_clear,
   METH_NOARGS,
   PyDoc_STR("True if having clear pin")},
  {"has_preset",
   has_preset,
   METH_NOARGS,
   PyDoc_STR("True if having preset pin")},
  {"max_index",
   max_index,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("return MAX INDEX")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_cpv1(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    return PyClibCPV::ToPyObject(val.cpv1());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cpv2(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    return PyClibCPV::ToPyObject(val.cpv2());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_index(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibSeqAttr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.index());
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
  {"cpv1", get_cpv1, nullptr, PyDoc_STR("clear preset var1"), nullptr},
  {"cpv2", get_cpv2, nullptr, PyDoc_STR("clear preset var2"), nullptr},
  {"index", get_index, nullptr, PyDoc_STR("index"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibSeqAttr オブジェクトを使用可能にする．
bool
PyClibSeqAttr::init(
  PyObject* m
)
{
  ClibSeqAttr_Type.tp_name = "ClibSeqAttr";
  ClibSeqAttr_Type.tp_basicsize = sizeof(ClibSeqAttr_Object);
  ClibSeqAttr_Type.tp_itemsize = 0;
  ClibSeqAttr_Type.tp_dealloc = dealloc_func;
  ClibSeqAttr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibSeqAttr_Type.tp_doc = PyDoc_STR("Python extended object for ClibSeqAttr");
  ClibSeqAttr_Type.tp_richcompare = richcompare_func;
  ClibSeqAttr_Type.tp_methods = methods;
  ClibSeqAttr_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibSeqAttr", &ClibSeqAttr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibSeqAttr を PyObject に変換する．
PyObject*
PyClibSeqAttr::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibSeqAttr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibSeqAttr_Object*>(obj);
  new (&my_obj->mVal) ClibSeqAttr(val);
  return obj;
}

// PyObject を ClibSeqAttr に変換する．
bool
PyClibSeqAttr::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibSeqAttr::Check(obj) ) {
    val = PyClibSeqAttr::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibSeqAttr タイプか調べる．
bool
PyClibSeqAttr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibSeqAttr_Type);
}

// @brief PyObject から ClibSeqAttr を取り出す．
ClibSeqAttr&
PyClibSeqAttr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibSeqAttr_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibSeqAttr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibSeqAttr::_typeobject()
{
  return &ClibSeqAttr_Type;
}

END_NAMESPACE_YM
