
/// @file PyBcGraph.cc
/// @brief PyBcGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBcGraph.h"
#include "pym/PyBcEdge.h"
#include "pym/PyBcOp.h"
#include "pym/PyBcOpType.h"
#include "pym/PyList.h"
#include "pym/PyUlong.h"
#include "pym/PyUint64.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BcGraph_Object
{
  PyObject_HEAD
  BcGraph mVal;
};

// Python 用のタイプ定義
PyTypeObject BcGraph_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBcGraph::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BcGraph_Object*>(self);
  obj->mVal.~BcGraph();
  Py_TYPE(self)->tp_free(self);
}

// return OP node
PyObject*
op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "op_id",
    nullptr
  };
  unsigned long op_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &op_id) ) {
    return nullptr;
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyBcOp::ToPyObject(val.op(op_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return output edge
PyObject*
output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "output_id",
    nullptr
  };
  unsigned long output_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &output_id) ) {
    return nullptr;
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyBcEdge::ToPyObject(val.output(output_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return output level
PyObject*
output_level(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "output_id",
    nullptr
  };
  unsigned long output_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &output_id) ) {
    return nullptr;
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.output_level(output_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return edge level
PyObject*
edge_level(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "edge",
    nullptr
  };
  PyObject* edge_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyBcEdge::_typeobject(), &edge_obj) ) {
    return nullptr;
  }
  BcEdge edge;
  if ( edge_obj != nullptr ) {
    if ( !PyBcEdge::FromPyObject(edge_obj, edge) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BcEdge");
      return nullptr;
    }
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.edge_level(edge));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// add OP
PyObject*
add_op(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "type",
    "opr_list",
    nullptr
  };
  PyObject* type_obj = nullptr;
  PyObject* opr_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
                                    const_cast<char**>(kwlist),
                                    PyBcOpType::_typeobject(), &type_obj,
                                    &opr_list_obj) ) {
    return nullptr;
  }
  BcOpType type;
  if ( type_obj != nullptr ) {
    if ( !PyBcOpType::FromPyObject(type_obj, type) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BcOpType");
      return nullptr;
    }
  }
  std::vector<BcEdge> opr_list;
  if ( opr_list_obj != nullptr ) {
    if ( !PyList<BcEdge, PyBcEdge>::FromPyObject(opr_list_obj, opr_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BcEdge>");
      return nullptr;
    }
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.add_op(type, opr_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// add TVOP
PyObject*
add_tvop(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "tv",
    "opr_list",
    nullptr
  };
  PyObject* tv_obj = nullptr;
  PyObject* opr_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "OO",
                                    const_cast<char**>(kwlist),
                                    &tv_obj,
                                    &opr_list_obj) ) {
    return nullptr;
  }
  std::vector<BcBvType> tv;
  if ( tv_obj != nullptr ) {
    if ( !PyList<BcBvType, PyUint64>::FromPyObject(tv_obj, tv) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BcBvType>");
      return nullptr;
    }
  }
  std::vector<BcEdge> opr_list;
  if ( opr_list_obj != nullptr ) {
    if ( !PyList<BcEdge, PyBcEdge>::FromPyObject(opr_list_obj, opr_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BcEdge>");
      return nullptr;
    }
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.add_tvop(tv, opr_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// add output
PyObject*
add_output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "edge",
    nullptr
  };
  PyObject* edge_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyBcEdge::_typeobject(), &edge_obj) ) {
    return nullptr;
  }
  BcEdge edge;
  if ( edge_obj != nullptr ) {
    if ( !PyBcEdge::FromPyObject(edge_obj, edge) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BcEdge");
      return nullptr;
    }
  }
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.add_output(edge));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// add output of constant 0
PyObject*
add_zero_output(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.add_zero_output());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// add output of constant 1
PyObject*
add_one_output(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.add_one_output());
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
  {"op",
   reinterpret_cast<PyCFunction>(op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return OP node")},
  {"output",
   reinterpret_cast<PyCFunction>(output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return output edge")},
  {"output_level",
   reinterpret_cast<PyCFunction>(output_level),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return output level")},
  {"edge_level",
   reinterpret_cast<PyCFunction>(edge_level),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return edge level")},
  {"add_op",
   reinterpret_cast<PyCFunction>(add_op),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("add OP")},
  {"add_tvop",
   reinterpret_cast<PyCFunction>(add_tvop),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("add TVOP")},
  {"add_output",
   reinterpret_cast<PyCFunction>(add_output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("add output")},
  {"add_zero_output",
   add_zero_output,
   METH_NOARGS,
   PyDoc_STR("add output of constant 0")},
  {"add_one_output",
   add_one_output,
   METH_NOARGS,
   PyDoc_STR("add output of constant 1")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcGraph::_get_ref(self);
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
get_output_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.output_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_op_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.op_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_max_level(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBcGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.max_level());
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
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"output_num", get_output_num, nullptr, PyDoc_STR("number of outputs"), nullptr},
  {"op_num", get_op_num, nullptr, PyDoc_STR("number OP nodes"), nullptr},
  {"max_level", get_max_level, nullptr, PyDoc_STR("max level"), nullptr},
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
    "input_num",
    nullptr
  };
  unsigned long input_num = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$k",
                                    const_cast<char**>(kwlist),
                                    &input_num) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<BcGraph_Object*>(self);
    new (&my_obj->mVal) BcGraph(input_num);
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


// @brief BcGraph オブジェクトを使用可能にする．
bool
PyBcGraph::init(
  PyObject* m
)
{
  BcGraph_Type.tp_name = "BcGraph";
  BcGraph_Type.tp_basicsize = sizeof(BcGraph_Object);
  BcGraph_Type.tp_itemsize = 0;
  BcGraph_Type.tp_dealloc = dealloc_func;
  BcGraph_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BcGraph_Type.tp_doc = PyDoc_STR("Python extended object for BcGraph");
  BcGraph_Type.tp_methods = methods;
  BcGraph_Type.tp_getset = getsets;
  BcGraph_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BcGraph", &BcGraph_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BcGraph を PyObject に変換する．
PyObject*
PyBcGraph::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBcGraph::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BcGraph_Object*>(obj);
  new (&my_obj->mVal) BcGraph(val);
  return obj;
}

// PyObject を BcGraph に変換する．
bool
PyBcGraph::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBcGraph::Check(obj) ) {
    val = PyBcGraph::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BcGraph タイプか調べる．
bool
PyBcGraph::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BcGraph_Type);
}

// @brief PyObject から BcGraph を取り出す．
BcGraph&
PyBcGraph::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BcGraph_Object*>(obj);
  return my_obj->mVal;
}

// @brief BcGraph を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBcGraph::_typeobject()
{
  return &BcGraph_Type;
}

END_NAMESPACE_YM
