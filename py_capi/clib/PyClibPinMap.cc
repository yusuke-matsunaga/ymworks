
/// @file PyClibPinMap.cc
/// @brief PyClibPinMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibPinMap.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibPinMap_Object
{
  PyObject_HEAD
  ClibPinMap mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibPinMap_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibPinMap::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibPinMap_Object*>(self);
  obj->mVal.~ClibPinMap();
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
  auto& val = PyClibPinMap::_get_ref(self);
  try {
    if ( PyClibPinMap::Check(other) ) {
      auto& val2 = PyClibPinMap::_get_ref(other);
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
invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyClibPinMap::ToPyObject(ClibPinMap::invalid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
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
  auto& val = PyClibPinMap::_get_ref(self);
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

// メソッド定義
PyMethodDef methods[] = {
  {"invalid",
   invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make INVALID object")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPinMap::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_inv(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPinMap::_get_ref(self);
  try {
    return PyBool_FromLong(val.inv());
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
  {"pin_id", get_id, nullptr, PyDoc_STR("pin ID"), nullptr},
  {"inv", get_inv, nullptr, PyDoc_STR("True if inverted"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibPinMap オブジェクトを使用可能にする．
bool
PyClibPinMap::init(
  PyObject* m
)
{
  ClibPinMap_Type.tp_name = "ClibPinMap";
  ClibPinMap_Type.tp_basicsize = sizeof(ClibPinMap_Object);
  ClibPinMap_Type.tp_itemsize = 0;
  ClibPinMap_Type.tp_dealloc = dealloc_func;
  ClibPinMap_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibPinMap_Type.tp_doc = PyDoc_STR("Python extended object for ClibPinMap");
  ClibPinMap_Type.tp_richcompare = richcompare_func;
  ClibPinMap_Type.tp_methods = methods;
  ClibPinMap_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibPinMap", &ClibPinMap_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibPinMap を PyObject に変換する．
PyObject*
PyClibPinMap::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibPinMap::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibPinMap_Object*>(obj);
  new (&my_obj->mVal) ClibPinMap(val);
  return obj;
}

// PyObject を ClibPinMap に変換する．
bool
PyClibPinMap::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibPinMap::Check(obj) ) {
    val = PyClibPinMap::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibPinMap タイプか調べる．
bool
PyClibPinMap::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibPinMap_Type);
}

// @brief PyObject から ClibPinMap を取り出す．
ClibPinMap&
PyClibPinMap::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibPinMap_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibPinMap を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibPinMap::_typeobject()
{
  return &ClibPinMap_Type;
}

END_NAMESPACE_YM
