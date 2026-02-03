
/// @file PyBddCube.cc
/// @brief PyBddCube の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddCube.h"
#include "pym/PyBdd.h"
#include "pym/PyBddLit.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddCube_Object
{
  PyObject_HEAD
  BddCube mVal;
};

// Python 用のタイプ定義
PyTypeObject BddCube_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddCube::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddCube_Object*>(self);
  obj->mVal.~BddCube();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddCube::Check(self) ) {
      auto& val1 = PyBddCube::_get_ref(self);
      if ( PyBddCube::Check(other) ) {
        auto& val2 = PyBddCube::_get_ref(other);
        return PyBddCube::ToPyObject(val1 & val2);
      }
      if ( PyBddLit::Check(other) ) {
        auto& val2 = PyBddLit::_get_ref(other);
        return PyBddCube::ToPyObject(val1 & val2);
      }
    }
    if ( PyBddCube::Check(other) ) {
      auto& val2 = PyBddCube::_get_ref(other);
      if ( PyBddLit::Check(self) ) {
        auto& val1 = PyBddLit::_get_ref(self);
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
nb_inplace_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddCube::Check(self) ) {
      auto& val1 = PyBddCube::_get_ref(self);
      if ( PyBddCube::Check(other) ) {
        auto& val2 = PyBddCube::_get_ref(other);
        val1 &= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PyBddLit::Check(other) ) {
        auto& val2 = PyBddLit::_get_ref(other);
        val1 &= val2;
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
  .nb_and = nb_and,
  .nb_inplace_and = nb_inplace_and
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyBddCube::_get_ref(self);
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
  auto& val = PyBddCube::_get_ref(self);
  try {
    if ( PyBddCube::Check(other) ) {
      auto& val2 = PyBddCube::_get_ref(other);
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
universal_cube(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyBddCube::ToPyObject(BddCube::universal_cube());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
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
    return PyBddCube::ToPyObject(BddCube::invalid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_universal_cube(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddCube::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_universal_cube());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddCube::_get_ref(self);
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

PyObject*
bdd(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddCube::_get_ref(self);
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
  {"universal_cube",
   universal_cube,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make an universal cube(empty cube)")},
  {"invalid",
   invalid,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make invalid BddCube object")},
  {"is_universal_cube",
   is_universal_cube,
   METH_NOARGS,
   PyDoc_STR("return True if univeral cube")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("return True if invalid")},
  {"bdd",
   bdd,
   METH_NOARGS,
   PyDoc_STR("")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
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
    auto my_obj = reinterpret_cast<BddCube_Object*>(self);
    new (&my_obj->mVal) BddCube();
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


// @brief BddCube オブジェクトを使用可能にする．
bool
PyBddCube::init(
  PyObject* m
)
{
  BddCube_Type.tp_name = "BddCube";
  BddCube_Type.tp_basicsize = sizeof(BddCube_Object);
  BddCube_Type.tp_itemsize = 0;
  BddCube_Type.tp_dealloc = dealloc_func;
  BddCube_Type.tp_as_number = &number;
  BddCube_Type.tp_hash = hash_func;
  BddCube_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddCube_Type.tp_doc = PyDoc_STR("Python extended object for BddCube");
  BddCube_Type.tp_richcompare = richcompare_func;
  BddCube_Type.tp_methods = methods;
  BddCube_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddCube", &BddCube_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddCube を PyObject に変換する．
PyObject*
PyBddCube::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBddCube::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BddCube_Object*>(obj);
  new (&my_obj->mVal) BddCube(val);
  return obj;
}

// PyObject を BddCube に変換する．
bool
PyBddCube::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddCube::Check(obj) ) {
    val = PyBddCube::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddCube タイプか調べる．
bool
PyBddCube::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddCube_Type);
}

// @brief PyObject から BddCube を取り出す．
BddCube&
PyBddCube::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddCube_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddCube を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddCube::_typeobject()
{
  return &BddCube_Type;
}

END_NAMESPACE_YM
