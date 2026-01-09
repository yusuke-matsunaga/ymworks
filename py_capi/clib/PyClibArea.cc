
/// @file PyClibArea.cc
/// @brief PyClibArea の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibArea.h"
#include "pym/PyFloat.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibArea_Object
{
  PyObject_HEAD
  ClibArea mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibArea_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibArea::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibArea_Object*>(self);
  obj->mVal.~ClibArea();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyClibArea::Check(self) ) {
      auto& val1 = PyClibArea::_get_ref(self);
      if ( PyClibArea::Check(other) ) {
        auto& val2 = PyClibArea::_get_ref(other);
        return PyClibArea::ToPyObject(val1 + val2);
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
    if ( PyClibArea::Check(self) ) {
      auto& val1 = PyClibArea::_get_ref(self);
      if ( PyClibArea::Check(other) ) {
        auto& val2 = PyClibArea::_get_ref(other);
        return PyClibArea::ToPyObject(val1 - val2);
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
    if ( PyClibArea::Check(self) ) {
      auto& val1 = PyClibArea::_get_ref(self);
      if ( PyClibArea::Check(other) ) {
        auto& val2 = PyClibArea::_get_ref(other);
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
    if ( PyClibArea::Check(self) ) {
      auto& val1 = PyClibArea::_get_ref(self);
      if ( PyClibArea::Check(other) ) {
        auto& val2 = PyClibArea::_get_ref(other);
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

PyObject*
get_value(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibArea::_get_ref(self);
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


// @brief ClibArea オブジェクトを使用可能にする．
bool
PyClibArea::init(
  PyObject* m
)
{
  ClibArea_Type.tp_name = "ClibArea";
  ClibArea_Type.tp_basicsize = sizeof(ClibArea_Object);
  ClibArea_Type.tp_itemsize = 0;
  ClibArea_Type.tp_dealloc = dealloc_func;
  ClibArea_Type.tp_as_number = &number;
  ClibArea_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibArea_Type.tp_doc = PyDoc_STR("Python extended object for ClibArea");
  ClibArea_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibArea", &ClibArea_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibArea を PyObject に変換する．
PyObject*
PyClibArea::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibArea::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibArea_Object*>(obj);
  new (&my_obj->mVal) ClibArea(val);
  return obj;
}

// PyObject を ClibArea に変換する．
bool
PyClibArea::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibArea::Check(obj) ) {
    val = PyClibArea::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibArea タイプか調べる．
bool
PyClibArea::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibArea_Type);
}

// @brief PyObject から ClibArea を取り出す．
ClibArea&
PyClibArea::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibArea_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibArea を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibArea::_typeobject()
{
  return &ClibArea_Type;
}

END_NAMESPACE_YM
