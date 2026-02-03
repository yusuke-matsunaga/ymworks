
/// @file PyClibCapacitance.cc
/// @brief PyClibCapacitance の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCapacitance.h"
#include "pym/PyFloat.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibCapacitance_Object
{
  PyObject_HEAD
  ClibCapacitance mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCapacitance_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCapacitance::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibCapacitance_Object*>(self);
  obj->mVal.~ClibCapacitance();
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
  auto& val = PyClibCapacitance::_get_ref(self);
  try {
    if ( PyClibCapacitance::Check(other) ) {
      auto& val2 = PyClibCapacitance::_get_ref(other);
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
infinity(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyClibCapacitance::ToPyObject(ClibCapacitance::infinity());
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
  {"infinity",
   infinity,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("return Infinity Object")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_value(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCapacitance::_get_ref(self);
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


// @brief ClibCapacitance オブジェクトを使用可能にする．
bool
PyClibCapacitance::init(
  PyObject* m
)
{
  ClibCapacitance_Type.tp_name = "ClibCapacitance";
  ClibCapacitance_Type.tp_basicsize = sizeof(ClibCapacitance_Object);
  ClibCapacitance_Type.tp_itemsize = 0;
  ClibCapacitance_Type.tp_dealloc = dealloc_func;
  ClibCapacitance_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCapacitance_Type.tp_doc = PyDoc_STR("Python extended object for ClibCapacitance");
  ClibCapacitance_Type.tp_richcompare = richcompare_func;
  ClibCapacitance_Type.tp_methods = methods;
  ClibCapacitance_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibCapacitance", &ClibCapacitance_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCapacitance を PyObject に変換する．
PyObject*
PyClibCapacitance::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibCapacitance::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCapacitance_Object*>(obj);
  new (&my_obj->mVal) ClibCapacitance(val);
  return obj;
}

// PyObject を ClibCapacitance に変換する．
bool
PyClibCapacitance::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibCapacitance::Check(obj) ) {
    val = PyClibCapacitance::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCapacitance タイプか調べる．
bool
PyClibCapacitance::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCapacitance_Type);
}

// @brief PyObject から ClibCapacitance を取り出す．
ClibCapacitance&
PyClibCapacitance::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCapacitance_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCapacitance を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCapacitance::_typeobject()
{
  return &ClibCapacitance_Type;
}

END_NAMESPACE_YM
