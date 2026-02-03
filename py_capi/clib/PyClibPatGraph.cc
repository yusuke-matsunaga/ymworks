
/// @file PyClibPatGraph.cc
/// @brief PyClibPatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibPatGraph.h"
#include "pym/PyClibCellClass.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibPatGraph_Object
{
  PyObject_HEAD
  ClibPatGraph mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibPatGraph_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibPatGraph::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibPatGraph_Object*>(self);
  obj->mVal.~ClibPatGraph();
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
  auto& val = PyClibPatGraph::_get_ref(self);
  try {
    if ( PyClibPatGraph::Check(other) ) {
      auto& val2 = PyClibPatGraph::_get_ref(other);
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
edge(
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
  auto& val = PyClibPatGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.edge(pos));
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
  auto& val = PyClibPatGraph::_get_ref(self);
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

// メソッド定義
PyMethodDef methods[] = {
  {"edge",
   reinterpret_cast<PyCFunction>(edge),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return EDGE ID")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_rep_class(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPatGraph::_get_ref(self);
  try {
    return PyClibCellClass::ToPyObject(val.rep_class());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_root_id(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPatGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.root_id());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_root_inv(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPatGraph::_get_ref(self);
  try {
    return PyBool_FromLong(val.root_inv());
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
  auto& val = PyClibPatGraph::_get_ref(self);
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
get_edge_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibPatGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.edge_num());
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
  {"rep_class", get_rep_class, nullptr, PyDoc_STR("representative class"), nullptr},
  {"root_id", get_root_id, nullptr, PyDoc_STR("root ID"), nullptr},
  {"root_inv", get_root_inv, nullptr, PyDoc_STR("True if root is inverted"), nullptr},
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"edge_num", get_edge_num, nullptr, PyDoc_STR("number of edges"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibPatGraph オブジェクトを使用可能にする．
bool
PyClibPatGraph::init(
  PyObject* m
)
{
  ClibPatGraph_Type.tp_name = "ClibPatGraph";
  ClibPatGraph_Type.tp_basicsize = sizeof(ClibPatGraph_Object);
  ClibPatGraph_Type.tp_itemsize = 0;
  ClibPatGraph_Type.tp_dealloc = dealloc_func;
  ClibPatGraph_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibPatGraph_Type.tp_doc = PyDoc_STR("Python extended object for ClibPatGraph");
  ClibPatGraph_Type.tp_richcompare = richcompare_func;
  ClibPatGraph_Type.tp_methods = methods;
  ClibPatGraph_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibPatGraph", &ClibPatGraph_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibPatGraph を PyObject に変換する．
PyObject*
PyClibPatGraph::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibPatGraph::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibPatGraph_Object*>(obj);
  new (&my_obj->mVal) ClibPatGraph(val);
  return obj;
}

// PyObject を ClibPatGraph に変換する．
bool
PyClibPatGraph::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibPatGraph::Check(obj) ) {
    val = PyClibPatGraph::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibPatGraph タイプか調べる．
bool
PyClibPatGraph::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibPatGraph_Type);
}

// @brief PyObject から ClibPatGraph を取り出す．
ClibPatGraph&
PyClibPatGraph::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibPatGraph_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibPatGraph を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibPatGraph::_typeobject()
{
  return &ClibPatGraph_Type;
}

END_NAMESPACE_YM
