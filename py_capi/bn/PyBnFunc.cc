
/// @file PyBnFunc.cc
/// @brief PyBnFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnFunc.h"
#include "pym/PyPrimType.h"
#include "pym/PySopCover.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyBdd.h"
#include "pym/PyUlong.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BnFunc_Object
{
  PyObject_HEAD
  BnFunc mVal;
};

// Python 用のタイプ定義
PyTypeObject BnFunc_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnFunc::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnFunc_Object*>(self);
  obj->mVal.~BnFunc();
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
  auto& val = PyBnFunc::_get_ref(self);
  try {
    if ( PyBnFunc::Check(other) ) {
      auto& val2 = PyBnFunc::_get_ref(other);
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

// True if PRIMITIVE type
PyObject*
is_primitive(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_primitive());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if COVER type
PyObject*
is_cover(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_cover());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if EXPR type
PyObject*
is_expr(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if TVFUNC type
PyObject*
is_tvfunc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_tvfunc());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if BDD type
PyObject*
is_bdd(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_bdd());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// print contents
PyObject*
print(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    val.print(std::cout);
    Py_RETURN_NONE;
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
  {"is_primitive",
   is_primitive,
   METH_NOARGS,
   PyDoc_STR("True if PRIMITIVE type")},
  {"is_cover",
   is_cover,
   METH_NOARGS,
   PyDoc_STR("True if COVER type")},
  {"is_expr",
   is_expr,
   METH_NOARGS,
   PyDoc_STR("True if EXPR type")},
  {"is_tvfunc",
   is_tvfunc,
   METH_NOARGS,
   PyDoc_STR("True if TVFUNC type")},
  {"is_bdd",
   is_bdd,
   METH_NOARGS,
   PyDoc_STR("True if BDD type")},
  {"print",
   print,
   METH_NOARGS,
   PyDoc_STR("print contents")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
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
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.input_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyString::ToPyObject(val.type_str());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_primitive_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyPrimType::ToPyObject(val.primitive_type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_input_cover(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PySopCover::ToPyObject(val.input_cover());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_output_inv(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyBool_FromLong(val.output_inv());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_tvfunc(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
  try {
    return PyTvFunc::ToPyObject(val.tvfunc());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bdd(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnFunc::_get_ref(self);
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

// getter/setter定義
PyGetSetDef getsets[] = {
  {"id", get_id, nullptr, PyDoc_STR("ID"), nullptr},
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"type", get_type, nullptr, PyDoc_STR("type(\"PRIMITIVE\", \"COVER\", \"EXPR\", \"TVFUNC\", \"BDD\")"), nullptr},
  {"primitive_type", get_primitive_type, nullptr, PyDoc_STR("primitive type"), nullptr},
  {"input_cover", get_input_cover, nullptr, PyDoc_STR("input cover"), nullptr},
  {"output_inv", get_output_inv, nullptr, PyDoc_STR("True if output is inverted"), nullptr},
  {"expr", get_expr, nullptr, PyDoc_STR("Expr"), nullptr},
  {"tvfunc", get_tvfunc, nullptr, PyDoc_STR("TvFunc"), nullptr},
  {"bdd", get_bdd, nullptr, PyDoc_STR("Bdd"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief BnFunc オブジェクトを使用可能にする．
bool
PyBnFunc::init(
  PyObject* m
)
{
  BnFunc_Type.tp_name = "BnFunc";
  BnFunc_Type.tp_basicsize = sizeof(BnFunc_Object);
  BnFunc_Type.tp_itemsize = 0;
  BnFunc_Type.tp_dealloc = dealloc_func;
  BnFunc_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnFunc_Type.tp_doc = PyDoc_STR("Python extended object for BnFunc");
  BnFunc_Type.tp_richcompare = richcompare_func;
  BnFunc_Type.tp_methods = methods;
  BnFunc_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "BnFunc", &BnFunc_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnFunc を PyObject に変換する．
PyObject*
PyBnFunc::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnFunc::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnFunc_Object*>(obj);
  new (&my_obj->mVal) BnFunc(val);
  return obj;
}

// PyObject を BnFunc に変換する．
bool
PyBnFunc::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBnFunc::Check(obj) ) {
    val = PyBnFunc::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BnFunc タイプか調べる．
bool
PyBnFunc::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnFunc_Type);
}

// @brief PyObject から BnFunc を取り出す．
BnFunc&
PyBnFunc::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnFunc_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnFunc を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnFunc::_typeobject()
{
  return &BnFunc_Type;
}

END_NAMESPACE_YM
