
/// @file PyClibBus.cc
/// @brief PyClibBus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibBus.h"
#include "pym/PyClibBusType.h"
#include "pym/PyClibPin.h"
#include "pym/PyClibList.h"
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
struct ClibBus_Object
{
  PyObject_HEAD
  ClibBus mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibBus_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibBus::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibBus_Object*>(self);
  obj->mVal.~ClibBus();
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
  auto& val = PyClibBus::_get_ref(self);
  try {
    if ( PyClibBus::Check(other) ) {
      auto& val2 = PyClibBus::_get_ref(other);
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

// return PIN
PyObject*
pin(
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
  auto& val = PyClibBus::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.pin(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of pins
PyObject*
meth_pin_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibBus::_get_ref(self);
  try {
    return PyClibPinList::ToPyObject(val.pin_list());
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
  {"pin",
   reinterpret_cast<PyCFunction>(pin),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return PIN")},
  {"meth_pin_list",
   meth_pin_list,
   METH_NOARGS,
   PyDoc_STR("return list of pins")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBus::_get_ref(self);
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
get_bus_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBus::_get_ref(self);
  try {
    return PyClibBusType::ToPyObject(val.bus_type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pin_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibBus::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pin_num());
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
  {"bus_type", get_bus_type, nullptr, PyDoc_STR("bus type"), nullptr},
  {"pin_num", get_pin_num, nullptr, PyDoc_STR("number of pins"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibBus オブジェクトを使用可能にする．
bool
PyClibBus::init(
  PyObject* m
)
{
  ClibBus_Type.tp_name = "ClibBus";
  ClibBus_Type.tp_basicsize = sizeof(ClibBus_Object);
  ClibBus_Type.tp_itemsize = 0;
  ClibBus_Type.tp_dealloc = dealloc_func;
  ClibBus_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibBus_Type.tp_doc = PyDoc_STR("Python extended object for ClibBus");
  ClibBus_Type.tp_richcompare = richcompare_func;
  ClibBus_Type.tp_methods = methods;
  ClibBus_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibBus", &ClibBus_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibBus を PyObject に変換する．
PyObject*
PyClibBus::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibBus::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibBus_Object*>(obj);
  new (&my_obj->mVal) ClibBus(val);
  return obj;
}

// PyObject を ClibBus に変換する．
bool
PyClibBus::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibBus::Check(obj) ) {
    val = PyClibBus::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibBus タイプか調べる．
bool
PyClibBus::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibBus_Type);
}

// @brief PyObject から ClibBus を取り出す．
ClibBus&
PyClibBus::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibBus_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibBus を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibBus::_typeobject()
{
  return &ClibBus_Type;
}

END_NAMESPACE_YM
