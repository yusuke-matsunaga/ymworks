
/// @file PyClibTime.cc
/// @brief PyClibTime の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibTime.h"
#include "pym/PyFloat.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibTime_Object
{
  PyObject_HEAD
  ClibTime mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibTime_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibTime::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibTime_Object*>(self);
  obj->mVal.~ClibTime();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibTime::Check(self) ) {
      auto& val1 = PyClibTime::_get_ref(self);
      if ( PyClibTime::Check(other) ) {
        auto& val2 = PyClibTime::_get_ref(other);
        return PyClibTime::ToPyObject(val1 + val2);
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
nb_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibTime::Check(self) ) {
      auto& val1 = PyClibTime::_get_ref(self);
      if ( PyClibTime::Check(other) ) {
        auto& val2 = PyClibTime::_get_ref(other);
        return PyClibTime::ToPyObject(val1 - val2);
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
nb_inplace_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibTime::Check(self) ) {
      auto& val1 = PyClibTime::_get_ref(self);
      if ( PyClibTime::Check(other) ) {
        auto& val2 = PyClibTime::_get_ref(other);
        val1 += val2;
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

PyObject*
nb_inplace_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibTime::Check(self) ) {
      auto& val1 = PyClibTime::_get_ref(self);
      if ( PyClibTime::Check(other) ) {
        auto& val2 = PyClibTime::_get_ref(other);
        val1 -= val2;
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
  .nb_add = nb_add,
  .nb_subtract = nb_subtract,
  .nb_inplace_add = nb_inplace_add,
  .nb_inplace_subtract = nb_inplace_subtract
};

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyClibTime::_get_ref(self);
  try {
    if ( PyClibTime::Check(other) ) {
      auto& val2 = PyClibTime::_get_ref(other);
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

// make INIFITY object
PyObject*
infty(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyClibTime::ToPyObject(ClibTime::infty());
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
  {"infty",
   infty,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make INIFITY object")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_value(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTime::_get_ref(self);
  try {
    return PyFloat::ToPyObject(val.value());
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
  {"value", get_value, nullptr, PyDoc_STR("value"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibTime オブジェクトを使用可能にする．
bool
PyClibTime::init(
  PyObject* m
)
{
  ClibTime_Type.tp_name = "ClibTime";
  ClibTime_Type.tp_basicsize = sizeof(ClibTime_Object);
  ClibTime_Type.tp_itemsize = 0;
  ClibTime_Type.tp_dealloc = dealloc_func;
  ClibTime_Type.tp_as_number = &number;
  ClibTime_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibTime_Type.tp_doc = PyDoc_STR("Python extended object for ClibTime");
  ClibTime_Type.tp_richcompare = richcompare_func;
  ClibTime_Type.tp_methods = methods;
  ClibTime_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibTime", &ClibTime_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibTime を PyObject に変換する．
PyObject*
PyClibTime::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibTime::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibTime_Object*>(obj);
  new (&my_obj->mVal) ClibTime(val);
  return obj;
}

// PyObject を ClibTime に変換する．
bool
PyClibTime::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibTime::Check(obj) ) {
    val = PyClibTime::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibTime タイプか調べる．
bool
PyClibTime::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibTime_Type);
}

// @brief PyObject から ClibTime を取り出す．
ClibTime&
PyClibTime::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibTime_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibTime を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibTime::_typeobject()
{
  return &ClibTime_Type;
}

END_NAMESPACE_YM
