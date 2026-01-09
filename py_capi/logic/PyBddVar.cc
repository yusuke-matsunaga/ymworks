
/// @file PyBddVar.cc
/// @brief PyBddVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBdd.h"
#include "pym/PyBddMgr.h"
#include "pym/PyBddVar.h"
#include "pym/PyBddLit.h"
#include "pym/PyBdd.h"
#include "pym/PyBddCompMap.h"
#include "pym/PyBddVarMap.h"
#include "pym/PyBddVarSet.h"
#include "pym/PyLiteral.h"
#include "pym/PyNpnMap.h"
#include "pym/PyInt.h"
#include "pym/PyLong.h"
#include "pym/PyBool.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddVar_Object
{
  PyObject_HEAD
  BddVar mVal;
};

// Python 用のタイプ定義
PyTypeObject BddVar_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddVar::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddVar_Object*>(self);
  obj->mVal.~BddVar();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVar::Check(self) ) {
      auto& val1 = PyBddVar::_get_ref(self);
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 + val2);
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
  .nb_add = nb_add
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    return val.hash();
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return 0;
  }
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    if ( PyBddVar::Check(other) ) {
      auto& val2 = PyBddVar::_get_ref(other);
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

// make invalid BddVar object
PyObject*
invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyBddVar::ToPyObject(BddVar::invalid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return True if invalid
PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVar::_get_ref(self);
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

// return positive literal of this variable
PyObject*
positive_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(val.positive_literal());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return negative literal of this variable
PyObject*
negative_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(val.negative_literal());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
bdd(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.bdd());
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
   PyDoc_STR("make invalid BddVar object")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("return True if invalid")},
  {"positive_literal",
   positive_literal,
   METH_NOARGS,
   PyDoc_STR("return positive literal of this variable")},
  {"negative_literal",
   negative_literal,
   METH_NOARGS,
   PyDoc_STR("return negative literal of this variable")},
  {"bdd",
   bdd,
   METH_NOARGS,
   PyDoc_STR("")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_varid(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    return PyInt::ToPyObject(val.varid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_mgr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddVar::_get_ref(self);
  try {
    return PyBddMgr::ToPyObject(val.mgr());
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
  {"varid", get_varid, nullptr, PyDoc_STR("var ID"), nullptr},
  {"mgr", get_mgr, nullptr, PyDoc_STR("BddMgr"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

// new 関数
PyObject*
new_func(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    nullptr
  };
  // 余分な引数を取らないことを確認しておく．
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "", const_cast<char**>(kwlist)) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<BddVar_Object*>(self);
    new (&my_obj->mVal) BddVar();
    return self;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
  catch ( std::out_of_range err ) {
    std::ostringstream buf;
    buf << "out of range" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief BddVar オブジェクトを使用可能にする．
bool
PyBddVar::init(
  PyObject* m
)
{
  BddVar_Type.tp_name = "BddVar";
  BddVar_Type.tp_basicsize = sizeof(BddVar_Object);
  BddVar_Type.tp_itemsize = 0;
  BddVar_Type.tp_dealloc = dealloc_func;
  BddVar_Type.tp_as_number = &number;
  BddVar_Type.tp_hash = hash_func;
  BddVar_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddVar_Type.tp_doc = PyDoc_STR("Python extended object for BddVar");
  BddVar_Type.tp_richcompare = richcompare_func;
  BddVar_Type.tp_methods = methods;
  BddVar_Type.tp_getset = getsets;
  BddVar_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddVar", &BddVar_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddVar を PyObject に変換する．
PyObject*
PyBddVar::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBddVar::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BddVar_Object*>(obj);
  new (&my_obj->mVal) BddVar(val);
  return obj;
}

// PyObject を BddVar に変換する．
bool
PyBddVar::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddVar::Check(obj) ) {
    val = PyBddVar::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddVar タイプか調べる．
bool
PyBddVar::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddVar_Type);
}

// @brief PyObject から BddVar を取り出す．
BddVar&
PyBddVar::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddVar_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddVar を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddVar::_typeobject()
{
  return &BddVar_Type;
}

END_NAMESPACE_YM
