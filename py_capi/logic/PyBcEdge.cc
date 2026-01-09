
/// @file PyBcEdge.cc
/// @brief PyBcEdge の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBcEdge.h"
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
struct BcEdge_Object
{
  PyObject_HEAD
  BcEdge mVal;
};

// Python 用のタイプ定義
PyTypeObject BcEdge_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBcEdge::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BcEdge_Object*>(self);
  obj->mVal.~BcEdge();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_invert(
  PyObject* self
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    return PyBcEdge::ToPyObject(~val);
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

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_invert = nb_invert
};

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    if ( PyBcEdge::Check(other) ) {
      auto& val2 = PyBcEdge::_get_ref(other);
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

// make ZERO edge
PyObject*
zero(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyBcEdge::ToPyObject(BcEdge::zero());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make ONE edge
PyObject*
one(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyBcEdge::ToPyObject(BcEdge::one());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make INPUT edge
PyObject*
input(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_id",
    "inv",
    nullptr
  };
  unsigned long input_id;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$p",
                                    const_cast<char**>(kwlist),
                                    &input_id,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  try {
    return PyBcEdge::ToPyObject(BcEdge::input(input_id, inv));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make OP edge
PyObject*
op(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "op_id",
    "inv",
    nullptr
  };
  unsigned long op_id;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k|$p",
                                    const_cast<char**>(kwlist),
                                    &op_id,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv = false;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  try {
    return PyBcEdge::ToPyObject(BcEdge::op(op_id, inv));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if INPUT edge
PyObject*
is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_input());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if OP edge
PyObject*
is_op(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_op());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if ZERO edge
PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcEdge::_get_ref(self);
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

// True if ONE edge
PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcEdge::_get_ref(self);
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

// return string representation
PyObject*
to_string(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    return PyString::ToPyObject(val.to_string());
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
   PyDoc_STR("make ZERO edge")},
  {"one",
   one,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make ONE edge")},
  {"input",
   reinterpret_cast<PyCFunction>(input),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make INPUT edge")},
  {"op",
   reinterpret_cast<PyCFunction>(op),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make OP edge")},
  {"is_input",
   is_input,
   METH_NOARGS,
   PyDoc_STR("True if INPUT edge")},
  {"is_op",
   is_op,
   METH_NOARGS,
   PyDoc_STR("True if OP edge")},
  {"is_zero",
   is_zero,
   METH_NOARGS,
   PyDoc_STR("True if ZERO edge")},
  {"is_one",
   is_one,
   METH_NOARGS,
   PyDoc_STR("True if ONE edge")},
  {"to_string",
   to_string,
   METH_NOARGS,
   PyDoc_STR("return string representation")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_input_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.input_id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_op_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcEdge::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.op_id());
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
  auto& val = PyBcEdge::_get_ref(self);
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
  {"input_id", get_input_id, nullptr, PyDoc_STR(""), nullptr},
  {"op_id", get_op_id, nullptr, PyDoc_STR(""), nullptr},
  {"inv", get_inv, nullptr, PyDoc_STR(""), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief BcEdge オブジェクトを使用可能にする．
bool
PyBcEdge::init(
  PyObject* m
)
{
  BcEdge_Type.tp_name = "BcEdge";
  BcEdge_Type.tp_basicsize = sizeof(BcEdge_Object);
  BcEdge_Type.tp_itemsize = 0;
  BcEdge_Type.tp_dealloc = dealloc_func;
  BcEdge_Type.tp_as_number = &number;
  BcEdge_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BcEdge_Type.tp_doc = PyDoc_STR("Python extended object for BcEdge");
  BcEdge_Type.tp_richcompare = richcompare_func;
  BcEdge_Type.tp_methods = methods;
  BcEdge_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "BcEdge", &BcEdge_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BcEdge を PyObject に変換する．
PyObject*
PyBcEdge::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBcEdge::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BcEdge_Object*>(obj);
  new (&my_obj->mVal) BcEdge(val);
  return obj;
}

// PyObject を BcEdge に変換する．
bool
PyBcEdge::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBcEdge::Check(obj) ) {
    val = PyBcEdge::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BcEdge タイプか調べる．
bool
PyBcEdge::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BcEdge_Type);
}

// @brief PyObject から BcEdge を取り出す．
BcEdge&
PyBcEdge::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BcEdge_Object*>(obj);
  return my_obj->mVal;
}

// @brief BcEdge を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBcEdge::_typeobject()
{
  return &BcEdge_Type;
}

END_NAMESPACE_YM
