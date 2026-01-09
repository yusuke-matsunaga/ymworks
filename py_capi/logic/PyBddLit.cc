
/// @file PyBddLit.cc
/// @brief PyBddLit の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddLit.h"
#include "pym/PyBddMgr.h"
#include "pym/PyBddVar.h"
#include "pym/PyBddCube.h"
#include "pym/PyBdd.h"
#include "pym/PyBool.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddLit_Object
{
  PyObject_HEAD
  BddLit mVal;
};

// Python 用のタイプ定義
PyTypeObject BddLit_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddLit::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddLit_Object*>(self);
  obj->mVal.~BddLit();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddLit::Check(self) ) {
      auto& val1 = PyBddLit::_get_ref(self);
      if ( PyBool::Check(other) ) {
        auto val2 = PyBool::Get(other);
        return PyBddLit::ToPyObject(val1 * val2);
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
nb_invert(
  PyObject* self
)
{
  auto& val = PyBddLit::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(~val);
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

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddLit::Check(self) ) {
      auto& val1 = PyBddLit::_get_ref(self);
      if ( PyBddLit::Check(other) ) {
        auto& val2 = PyBddLit::_get_ref(other);
        return PyBddCube::ToPyObject(val1 & val2);
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
nb_inplace_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddLit::Check(self) ) {
      auto& val1 = PyBddLit::_get_ref(self);
      if ( PyBool::Check(other) ) {
        auto val2 = PyBool::Get(other);
        val1 *= val2;
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
  .nb_multiply = nb_multiply,
  .nb_invert = nb_invert,
  .nb_and = nb_and,
  .nb_inplace_multiply = nb_inplace_multiply
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyBddLit::_get_ref(self);
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
  auto& val = PyBddLit::_get_ref(self);
  try {
    if ( PyBddLit::Check(other) ) {
      auto& val2 = PyBddLit::_get_ref(other);
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

// make invalid BddLit object
PyObject*
invalid(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyBddLit::ToPyObject(BddLit::invalid());
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
  auto& val = PyBddLit::_get_ref(self);
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

// return True if positive literal
PyObject*
is_positive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddLit::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_positive());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return True if negative literal
PyObject*
is_negative(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddLit::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_negative());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return positive literal of the same variable
PyObject*
positive_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddLit::_get_ref(self);
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

// return negative literal of the same variable
PyObject*
negative_literal(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddLit::_get_ref(self);
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
  auto& val = PyBddLit::_get_ref(self);
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

// return the complement of this literal
PyObject*
complement(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddLit::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(val.complement());
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
   PyDoc_STR("make invalid BddLit object")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("return True if invalid")},
  {"is_positive",
   is_positive,
   METH_NOARGS,
   PyDoc_STR("return True if positive literal")},
  {"is_negative",
   is_negative,
   METH_NOARGS,
   PyDoc_STR("return True if negative literal")},
  {"positive_literal",
   positive_literal,
   METH_NOARGS,
   PyDoc_STR("return positive literal of the same variable")},
  {"negative_literal",
   negative_literal,
   METH_NOARGS,
   PyDoc_STR("return negative literal of the same variable")},
  {"bdd",
   bdd,
   METH_NOARGS,
   PyDoc_STR("")},
  {"complement",
   complement,
   METH_NOARGS,
   PyDoc_STR("return the complement of this literal")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_var(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddLit::_get_ref(self);
  try {
    return PyBddVar::ToPyObject(val.var());
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
  {"var", get_var, nullptr, PyDoc_STR("BddVar"), nullptr},
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
    auto my_obj = reinterpret_cast<BddLit_Object*>(self);
    new (&my_obj->mVal) BddLit();
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


// @brief BddLit オブジェクトを使用可能にする．
bool
PyBddLit::init(
  PyObject* m
)
{
  BddLit_Type.tp_name = "BddLit";
  BddLit_Type.tp_basicsize = sizeof(BddLit_Object);
  BddLit_Type.tp_itemsize = 0;
  BddLit_Type.tp_dealloc = dealloc_func;
  BddLit_Type.tp_as_number = &number;
  BddLit_Type.tp_hash = hash_func;
  BddLit_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddLit_Type.tp_doc = PyDoc_STR("Python extended object for BddLit");
  BddLit_Type.tp_richcompare = richcompare_func;
  BddLit_Type.tp_methods = methods;
  BddLit_Type.tp_getset = getsets;
  BddLit_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddLit", &BddLit_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddLit を PyObject に変換する．
PyObject*
PyBddLit::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBddLit::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BddLit_Object*>(obj);
  new (&my_obj->mVal) BddLit(val);
  return obj;
}

// PyObject を BddLit に変換する．
bool
PyBddLit::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddLit::Check(obj) ) {
    val = PyBddLit::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddLit タイプか調べる．
bool
PyBddLit::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddLit_Type);
}

// @brief PyObject から BddLit を取り出す．
BddLit&
PyBddLit::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddLit_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddLit を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddLit::_typeobject()
{
  return &BddLit_Type;
}

END_NAMESPACE_YM
