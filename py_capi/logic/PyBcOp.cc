
/// @file PyBcOp.cc
/// @brief PyBcOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBcOp.h"
#include "pym/PyBcEdge.h"
#include "pym/PyBcOpType.h"
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
struct BcOp_Object
{
  PyObject_HEAD
  BcOp mVal;
};

// Python 用のタイプ定義
PyTypeObject BcOp_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBcOp::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BcOp_Object*>(self);
  obj->mVal.~BcOp();
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
  auto& val = PyBcOp::_get_ref(self);
  try {
    if ( PyBcOp::Check(other) ) {
      auto& val2 = PyBcOp::_get_ref(other);
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

// return operand edge
PyObject*
operand(
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
  auto& val = PyBcOp::_get_ref(self);
  try {
    return PyBcEdge::ToPyObject(val.operand(pos));
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
  auto& val = PyBcOp::_get_ref(self);
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
  {"operand",
   reinterpret_cast<PyCFunction>(operand),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return operand edge")},
  {"to_string",
   to_string,
   METH_NOARGS,
   PyDoc_STR("return string representation")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcOp::_get_ref(self);
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
get_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcOp::_get_ref(self);
  try {
    return PyBcOpType::ToPyObject(val.type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_operand_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcOp::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.operand_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_level(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcOp::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.level());
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
  {"type", get_type, nullptr, PyDoc_STR("type(AND, OR, XOR, TV)"), nullptr},
  {"operand_num", get_operand_num, nullptr, PyDoc_STR("number of operands"), nullptr},
  {"level", get_level, nullptr, PyDoc_STR("level"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief BcOp オブジェクトを使用可能にする．
bool
PyBcOp::init(
  PyObject* m
)
{
  BcOp_Type.tp_name = "BcOp";
  BcOp_Type.tp_basicsize = sizeof(BcOp_Object);
  BcOp_Type.tp_itemsize = 0;
  BcOp_Type.tp_dealloc = dealloc_func;
  BcOp_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BcOp_Type.tp_doc = PyDoc_STR("Python extended object for BcOp");
  BcOp_Type.tp_richcompare = richcompare_func;
  BcOp_Type.tp_methods = methods;
  BcOp_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "BcOp", &BcOp_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BcOp を PyObject に変換する．
PyObject*
PyBcOp::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBcOp::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BcOp_Object*>(obj);
  new (&my_obj->mVal) BcOp(val);
  return obj;
}

// PyObject を BcOp に変換する．
bool
PyBcOp::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBcOp::Check(obj) ) {
    val = PyBcOp::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BcOp タイプか調べる．
bool
PyBcOp::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BcOp_Type);
}

// @brief PyObject から BcOp を取り出す．
BcOp&
PyBcOp::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BcOp_Object*>(obj);
  return my_obj->mVal;
}

// @brief BcOp を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBcOp::_typeobject()
{
  return &BcOp_Type;
}

END_NAMESPACE_YM
