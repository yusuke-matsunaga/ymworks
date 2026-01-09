
/// @file PyFraigLiteral.cc
/// @brief PyFraigLiteral の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyFraigLiteral.h"
#include "pym/PyUlong.h"
#include "pym/PyBool.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct FraigLiteral_Object
{
  PyObject_HEAD
  FraigLiteral mVal;
};

// Python 用のタイプ定義
PyTypeObject FraigLiteral_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyFraigLiteral::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<FraigLiteral_Object*>(self);
  obj->mVal.~FraigLiteral();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyFraigLiteral::Check(self) ) {
      auto& val1 = PyFraigLiteral::_get_ref(self);
      if ( PyBool::Check(other) ) {
        auto val2 = PyBool::Get(other);
        return PyFraigLiteral::ToPyObject(val1 * val2);
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
  auto& val = PyFraigLiteral::_get_ref(self);
  try {
    return PyFraigLiteral::ToPyObject(~val);
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
nb_inplace_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyFraigLiteral::Check(self) ) {
      auto& val1 = PyFraigLiteral::_get_ref(self);
      if ( PyBool::Check(other) ) {
        auto val2 = PyBool::Get(other);
        return PyFraigLiteral::ToPyObject(val1 * val2);
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
  .nb_inplace_multiply = nb_inplace_multiply
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyFraigLiteral::_get_ref(self);
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
  auto& val = PyFraigLiteral::_get_ref(self);
  try {
    if ( PyFraigLiteral::Check(other) ) {
      auto& val2 = PyFraigLiteral::_get_ref(other);
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

// return ZERO literal
PyObject*
zero(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyFraigLiteral::ToPyObject(FraigLiteral::zero());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return ONE literal
PyObject*
one(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyFraigLiteral::ToPyObject(FraigLiteral::one());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if ZERO
PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyFraigLiteral::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_zero());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if ONE
PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyFraigLiteral::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_one());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if constant(ZERO or ONE)
PyObject*
is_const(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyFraigLiteral::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_const());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if AND node
PyObject*
is_and(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyFraigLiteral::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_and());
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
  {"zero",
   zero,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("return ZERO literal")},
  {"one",
   one,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("return ONE literal")},
  {"is_zero",
   is_zero,
   METH_NOARGS,
   PyDoc_STR("True if ZERO")},
  {"is_one",
   is_one,
   METH_NOARGS,
   PyDoc_STR("True if ONE")},
  {"is_const",
   is_const,
   METH_NOARGS,
   PyDoc_STR("True if constant(ZERO or ONE)")},
  {"is_and",
   is_and,
   METH_NOARGS,
   PyDoc_STR("True if AND node")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

END_NONAMESPACE


// @brief FraigLiteral オブジェクトを使用可能にする．
bool
PyFraigLiteral::init(
  PyObject* m
)
{
  FraigLiteral_Type.tp_name = "FraigLiteral";
  FraigLiteral_Type.tp_basicsize = sizeof(FraigLiteral_Object);
  FraigLiteral_Type.tp_itemsize = 0;
  FraigLiteral_Type.tp_dealloc = dealloc_func;
  FraigLiteral_Type.tp_as_number = &number;
  FraigLiteral_Type.tp_hash = hash_func;
  FraigLiteral_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  FraigLiteral_Type.tp_doc = PyDoc_STR("Python extended object for FraigLiteral");
  FraigLiteral_Type.tp_richcompare = richcompare_func;
  FraigLiteral_Type.tp_methods = methods;
  if ( !PyModule::reg_type(m, "FraigLiteral", &FraigLiteral_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// FraigLiteral を PyObject に変換する．
PyObject*
PyFraigLiteral::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyFraigLiteral::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<FraigLiteral_Object*>(obj);
  new (&my_obj->mVal) FraigLiteral(val);
  return obj;
}

// PyObject を FraigLiteral に変換する．
bool
PyFraigLiteral::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyFraigLiteral::Check(obj) ) {
    val = PyFraigLiteral::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が FraigLiteral タイプか調べる．
bool
PyFraigLiteral::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &FraigLiteral_Type);
}

// @brief PyObject から FraigLiteral を取り出す．
FraigLiteral&
PyFraigLiteral::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<FraigLiteral_Object*>(obj);
  return my_obj->mVal;
}

// @brief FraigLiteral を表すオブジェクトの型定義を返す．
PyTypeObject*
PyFraigLiteral::_typeobject()
{
  return &FraigLiteral_Type;
}

END_NAMESPACE_YM
