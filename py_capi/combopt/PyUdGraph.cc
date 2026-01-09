
/// @file PyUdGraph.cc
/// @brief PyUdGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyUdGraph.h"
#include "pym/PyUdEdge.h"
#include "pym/PyUdEdgeList.h"
#include "pym/PyJsonValue.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct UdGraph_Object
{
  PyObject_HEAD
  UdGraph mVal;
};

// Python 用のタイプ定義
PyTypeObject UdGraph_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyUdGraph::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  Py_TYPE(self)->tp_free(self);
}

// resize
PyObject*
resize(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_num",
    nullptr
  };
  unsigned long node_num;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_num) ) {
    return nullptr;
  }
  auto& val = PyUdGraph::_get_ref(self);
  try {
    val.resize(node_num);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// connect
PyObject*
connect(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "id1",
    "id2",
    nullptr
  };
  unsigned long id1;
  unsigned long id2;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kk",
                                    const_cast<char**>(kwlist),
                                    &id1,
                                    &id2) ) {
    return nullptr;
  }
  auto& val = PyUdGraph::_get_ref(self);
  try {
    val.connect(id1, id2);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if reflective graph
PyObject*
is_reflective(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyUdGraph::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_reflective());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Edge
PyObject*
edge(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "id",
    nullptr
  };
  unsigned long id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &id) ) {
    return nullptr;
  }
  auto& val = PyUdGraph::_get_ref(self);
  try {
    return PyUdEdge::ToPyObject(val.edge(id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read DIMACS format
PyObject*
read_dimacs(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };
  const char* filename_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  try {
    return PyUdGraph::ToPyObject(UdGraph::read_dimacs(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// write in DIMCAS format
PyObject*
write_dimacs(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };
  const char* filename_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  auto& val = PyUdGraph::_get_ref(self);
  try {
    val.write_dimacs(filename);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// do minimum coloring
PyObject*
coloring(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "option",
    nullptr
  };
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$O",
                                    const_cast<char**>(kwlist),
                                    &option_obj) ) {
    return nullptr;
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    if ( !PyJsonValue::FromPyObject(option_obj, option) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to JsonValue");
      return nullptr;
    }
  }
  auto& val = PyUdGraph::_get_ref(self);
  try {
    std::vector<SizeType> color_map;
    auto nc = val.coloring(color_map, option);
    auto cmap_obj = PyList<SizeType, PyUlong>::ToPyObject(color_map);
    return Py_BuildValue("(Ok)", cmap_obj, nc);
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get max clique
PyObject*
max_clique(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "option",
    nullptr
  };
  PyObject* option_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$O",
                                    const_cast<char**>(kwlist),
                                    &option_obj) ) {
    return nullptr;
  }
  JsonValue option;
  if ( option_obj != nullptr ) {
    if ( !PyJsonValue::FromPyObject(option_obj, option) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to JsonValue");
      return nullptr;
    }
  }
  auto& val = PyUdGraph::_get_ref(self);
  try {
    return PyList<SizeType, PyUlong>::ToPyObject(val.max_clique(option));
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
  {"resize",
   reinterpret_cast<PyCFunction>(resize),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("resize")},
  {"connect",
   reinterpret_cast<PyCFunction>(connect),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("connect")},
  {"is_reflective",
   is_reflective,
   METH_NOARGS,
   PyDoc_STR("True if reflective graph")},
  {"edge",
   reinterpret_cast<PyCFunction>(edge),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Edge")},
  {"read_dimacs",
   reinterpret_cast<PyCFunction>(read_dimacs),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read DIMACS format")},
  {"write_dimacs",
   reinterpret_cast<PyCFunction>(write_dimacs),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write in DIMCAS format")},
  {"coloring",
   reinterpret_cast<PyCFunction>(coloring),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("do minimum coloring")},
  {"max_clique",
   reinterpret_cast<PyCFunction>(max_clique),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get max clique")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyUdGraph::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.node_num());
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
  auto& val = PyUdGraph::_get_ref(self);
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

PyObject*
get_edge_list(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyUdGraph::_get_ref(self);
  try {
    return PyUdEdgeList::ToPyObject(val.edge_list());
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
  {"node_num", get_node_num, nullptr, PyDoc_STR(""), nullptr},
  {"edge_num", get_edge_num, nullptr, PyDoc_STR(""), nullptr},
  {"edge_list", get_edge_list, nullptr, PyDoc_STR(""), nullptr},
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
    "node_num",
    nullptr
  };
  unsigned long node_num = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$k",
                                    const_cast<char**>(kwlist),
                                    &node_num) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<UdGraph_Object*>(self);
    new (&my_obj->mVal) UdGraph(node_num);
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


// @brief UdGraph オブジェクトを使用可能にする．
bool
PyUdGraph::init(
  PyObject* m
)
{
  UdGraph_Type.tp_name = "UdGraph";
  UdGraph_Type.tp_basicsize = sizeof(UdGraph_Object);
  UdGraph_Type.tp_itemsize = 0;
  UdGraph_Type.tp_dealloc = dealloc_func;
  UdGraph_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  UdGraph_Type.tp_doc = PyDoc_STR("Python extended object for UdGraph");
  UdGraph_Type.tp_methods = methods;
  UdGraph_Type.tp_getset = getsets;
  UdGraph_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "UdGraph", &UdGraph_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// UdGraph を PyObject に変換する．
PyObject*
PyUdGraph::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyUdGraph::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<UdGraph_Object*>(obj);
  new (&my_obj->mVal) UdGraph(val);
  return obj;
}

// PyObject を UdGraph に変換する．
bool
PyUdGraph::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyUdGraph::Check(obj) ) {
    val = PyUdGraph::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が UdGraph タイプか調べる．
bool
PyUdGraph::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &UdGraph_Type);
}

// @brief PyObject から UdGraph を取り出す．
UdGraph&
PyUdGraph::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<UdGraph_Object*>(obj);
  return my_obj->mVal;
}

// @brief UdGraph を表すオブジェクトの型定義を返す．
PyTypeObject*
PyUdGraph::_typeobject()
{
  return &UdGraph_Type;
}

END_NAMESPACE_YM
