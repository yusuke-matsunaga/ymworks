
/// @file PyAigLiteral.cc
/// @brief PyAigLiteral の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigLiteral.h"
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
struct AigLiteral_Object
{
  PyObject_HEAD
  AigLiteral mVal;
};

// Python 用のタイプ定義
PyTypeObject AigLiteral_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyAigLiteral::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<AigLiteral_Object*>(self);
  obj->mVal.~AigLiteral();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_multiply(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyAigLiteral::Check(self) ) {
      auto& val1 = PyAigLiteral::_get_ref(self);
      if ( PyBool::Check(other) ) {
        auto val2 = PyBool::Get(other);
        return PyAigLiteral::ToPyObject(val1 * val2);
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
  auto& val = PyAigLiteral::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(~val);
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
    if ( PyAigLiteral::Check(self) ) {
      auto& val1 = PyAigLiteral::_get_ref(self);
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
  .nb_inplace_multiply = nb_inplace_multiply
};

// hash 関数
Py_hash_t
hash_func(
  PyObject* self
)
{
  auto& val = PyAigLiteral::_get_ref(self);
  try {
    return val.index();
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
  auto& val = PyAigLiteral::_get_ref(self);
  try {
    if ( PyAigLiteral::Check(other) ) {
      auto& val2 = PyAigLiteral::_get_ref(other);
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
    return PyAigLiteral::ToPyObject(AigLiteral::invalid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
zero(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyAigLiteral::ToPyObject(AigLiteral::zero());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
one(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyAigLiteral::ToPyObject(AigLiteral::one());
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
  auto& val = PyAigLiteral::_get_ref(self);
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

PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
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
inv(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
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

PyObject*
is_zero(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
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

PyObject*
is_one(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
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

PyObject*
is_const(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
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

PyObject*
invert(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(~val);
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
   PyDoc_STR("return INVALID literal")},
  {"zero",
   zero,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("return ZERO literal")},
  {"one",
   one,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("return ONE literal")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid")},
  {"inv",
   inv,
   METH_NOARGS,
   PyDoc_STR("True if inverted")},
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
  {"invert",
   invert,
   METH_NOARGS,
   PyDoc_STR("INVERT OP")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_node_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigLiteral::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.node_id());
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
  {"node_id", get_node_id, nullptr, PyDoc_STR("node ID"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief AigLiteral オブジェクトを使用可能にする．
bool
PyAigLiteral::init(
  PyObject* m
)
{
  AigLiteral_Type.tp_name = "AigLiteral";
  AigLiteral_Type.tp_basicsize = sizeof(AigLiteral_Object);
  AigLiteral_Type.tp_itemsize = 0;
  AigLiteral_Type.tp_dealloc = dealloc_func;
  AigLiteral_Type.tp_as_number = &number;
  AigLiteral_Type.tp_hash = hash_func;
  AigLiteral_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  AigLiteral_Type.tp_doc = PyDoc_STR("Python extended object for AigLiteral");
  AigLiteral_Type.tp_richcompare = richcompare_func;
  AigLiteral_Type.tp_methods = methods;
  AigLiteral_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "AigLiteral", &AigLiteral_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// AigLiteral を PyObject に変換する．
PyObject*
PyAigLiteral::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyAigLiteral::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<AigLiteral_Object*>(obj);
  new (&my_obj->mVal) AigLiteral(val);
  return obj;
}

// PyObject を AigLiteral に変換する．
bool
PyAigLiteral::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyAigLiteral::Check(obj) ) {
    val = PyAigLiteral::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が AigLiteral タイプか調べる．
bool
PyAigLiteral::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &AigLiteral_Type);
}

// @brief PyObject から AigLiteral を取り出す．
AigLiteral&
PyAigLiteral::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<AigLiteral_Object*>(obj);
  return my_obj->mVal;
}

// @brief AigLiteral を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigLiteral::_typeobject()
{
  return &AigLiteral_Type;
}

END_NAMESPACE_YM
