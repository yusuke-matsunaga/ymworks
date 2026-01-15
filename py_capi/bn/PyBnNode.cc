
/// @file PyBnNode.cc
/// @brief PyBnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnNode.h"
#include "pym/PyBnNodeList.h"
#include "pym/PyBnFunc.h"
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
struct BnNode_Object
{
  PyObject_HEAD
  BnNode mVal;
};

// Python 用のタイプ定義
PyTypeObject BnNode_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnNode::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnNode_Object*>(self);
  obj->mVal.~BnNode();
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
  auto& val = PyBnNode::_get_ref(self);
  try {
    if ( PyBnNode::Check(other) ) {
      auto& val2 = PyBnNode::_get_ref(other);
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

// True if INPUT type
PyObject*
is_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnNode::_get_ref(self);
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

// True if LOGIC type
PyObject*
is_logic(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_logic());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if PRIMARY INPUT type
PyObject*
is_primary_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_primary_input());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if DFF OUTPUT type
PyObject*
is_dff_output(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_dff_output());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fanin node
PyObject*
fanin(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  int pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.fanin(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of fanin nodes
PyObject*
fanin_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyList<BnNode, PyBnNode>::ToPyObject(val.fanin_list());
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
  {"is_input",
   is_input,
   METH_NOARGS,
   PyDoc_STR("True if INPUT type")},
  {"is_logic",
   is_logic,
   METH_NOARGS,
   PyDoc_STR("True if LOGIC type")},
  {"is_primary_input",
   is_primary_input,
   METH_NOARGS,
   PyDoc_STR("True if PRIMARY INPUT type")},
  {"is_dff_output",
   is_dff_output,
   METH_NOARGS,
   PyDoc_STR("True if DFF OUTPUT type")},
  {"fanin",
   reinterpret_cast<PyCFunction>(fanin),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fanin node")},
  {"fanin_list",
   fanin_list,
   METH_NOARGS,
   PyDoc_STR("return list of fanin nodes")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnNode::_get_ref(self);
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
  auto& val = PyBnNode::_get_ref(self);
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
get_input_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnNode::_get_ref(self);
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
get_dff_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.dff_id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyBnFunc::ToPyObject(val.func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_fanin_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnNode::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.fanin_num());
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
  {"type", get_type, nullptr, PyDoc_STR("type(\"INPUT\", \"LOGIC\")"), nullptr},
  {"input_id", get_input_id, nullptr, PyDoc_STR("input ID"), nullptr},
  {"dff_id", get_dff_id, nullptr, PyDoc_STR("DFF ID"), nullptr},
  {"func", get_func, nullptr, PyDoc_STR("function"), nullptr},
  {"fanin_num", get_fanin_num, nullptr, PyDoc_STR("number of fanins"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief BnNode オブジェクトを使用可能にする．
bool
PyBnNode::init(
  PyObject* m
)
{
  BnNode_Type.tp_name = "BnNode";
  BnNode_Type.tp_basicsize = sizeof(BnNode_Object);
  BnNode_Type.tp_itemsize = 0;
  BnNode_Type.tp_dealloc = dealloc_func;
  BnNode_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnNode_Type.tp_doc = PyDoc_STR("Python extended object for BnNode");
  BnNode_Type.tp_richcompare = richcompare_func;
  BnNode_Type.tp_methods = methods;
  BnNode_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "BnNode", &BnNode_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnNode を PyObject に変換する．
PyObject*
PyBnNode::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnNode::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnNode_Object*>(obj);
  new (&my_obj->mVal) BnNode(val);
  return obj;
}

// PyObject を BnNode に変換する．
bool
PyBnNode::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBnNode::Check(obj) ) {
    val = PyBnNode::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BnNode タイプか調べる．
bool
PyBnNode::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnNode_Type);
}

// @brief PyObject から BnNode を取り出す．
BnNode&
PyBnNode::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnNode_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnNode を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnNode::_typeobject()
{
  return &BnNode_Type;
}

END_NAMESPACE_YM
