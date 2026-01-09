
/// @file PyBnDff.cc
/// @brief PyBnDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnDff.h"
#include "pym/PyBnNode.h"
#include "pym/PyInt.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BnDff_Object
{
  PyObject_HEAD
  BnDff mVal;
};

// Python 用のタイプ定義
PyTypeObject BnDff_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnDff::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnDff_Object*>(self);
  obj->mVal.~BnDff();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
get_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnDff::_get_ref(self);
  try {
    return PyInt::ToPyObject(val.id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnDff::_get_ref(self);
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
get_output(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnDff::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.output());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_input(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnDff::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.input());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_reset_val(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnDff::_get_ref(self);
  try {
    return PyString::ToPyObject(std::string{val.reset_val()});
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
  {"id", get_id, nullptr, PyDoc_STR("ID"), nullptr},
  {"name", get_name, nullptr, PyDoc_STR("name"), nullptr},
  {"output", get_output, nullptr, PyDoc_STR("output node"), nullptr},
  {"input", get_input, nullptr, PyDoc_STR("input node"), nullptr},
  {"reset_val", get_reset_val, nullptr, PyDoc_STR("reset value"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief BnDff オブジェクトを使用可能にする．
bool
PyBnDff::init(
  PyObject* m
)
{
  BnDff_Type.tp_name = "BnDff";
  BnDff_Type.tp_basicsize = sizeof(BnDff_Object);
  BnDff_Type.tp_itemsize = 0;
  BnDff_Type.tp_dealloc = dealloc_func;
  BnDff_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnDff_Type.tp_doc = PyDoc_STR("Python extended object for BnDff");
  BnDff_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "BnDff", &BnDff_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnDff を PyObject に変換する．
PyObject*
PyBnDff::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnDff::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnDff_Object*>(obj);
  new (&my_obj->mVal) BnDff(val);
  return obj;
}

// PyObject を BnDff に変換する．
bool
PyBnDff::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBnDff::Check(obj) ) {
    val = PyBnDff::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BnDff タイプか調べる．
bool
PyBnDff::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnDff_Type);
}

// @brief PyObject から BnDff を取り出す．
BnDff&
PyBnDff::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnDff_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnDff を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnDff::_typeobject()
{
  return &BnDff_Type;
}

END_NAMESPACE_YM
