
/// @file PyClibCellLibrary.cc
/// @brief PyClibCellLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCellLibrary.h"
#include "pym/PyClibCellClass.h"
#include "pym/PyClibCellGroup.h"
#include "pym/PyClibCell.h"
#include "pym/PyClibCellType.h"
#include "pym/PyClibIOMap.h"
#include "pym/PyClibSeqAttr.h"
#include "pym/PyClibPatGraph.h"
#include "pym/PyClibPatType.h"
#include "pym/PyClibTechnology.h"
#include "pym/PyClibDelayModel.h"
#include "pym/PyClibVarType.h"
#include "pym/PyClibList.h"
#include "pym/pyfstream.h"
#include "pym/PyUlong.h"
#include "pym/PyFloat.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibCellLibrary_Object
{
  PyObject_HEAD
  ClibCellLibrary mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCellLibrary_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCellLibrary::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibCellLibrary_Object*>(self);
  obj->mVal.~ClibCellLibrary();
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    if ( PyClibCellLibrary::Check(other) ) {
      auto& val2 = PyClibCellLibrary::_get_ref(other);
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

// read mislig(genlib)
PyObject*
read_mislib(
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
    return PyClibCellLibrary::ToPyObject(ClibCellLibrary::read_mislib(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read liberty(.lib)
PyObject*
read_liberty(
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
    return PyClibCellLibrary::ToPyObject(ClibCellLibrary::read_liberty(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if valid
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
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

// return list of piece-defines
PyObject*
meth_piece_define(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyList<double, PyFloat>::ToPyObject(val.piece_define());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return cell
PyObject*
cell(
  PyObject* self,
  PyObject* args
)
{
  PyObject* arg_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &arg_obj) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    if ( PyUlong::Check(arg_obj) ) {
      auto cell_id = PyUlong::Get(arg_obj);
      return PyClibCell::ToPyObject(val.cell(cell_id));
    }
    else if ( PyString::Check(arg_obj) ) {
      auto name = PyString::Get(arg_obj);
      return PyClibCell::ToPyObject(val.cell(name));
    }
    else {
      PyErr_SetString(PyExc_TypeError, "1st argument should be integer or string");
      return nullptr;
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of cells
PyObject*
meth_cell_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellList::ToPyObject(val.cell_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return cell group
PyObject*
cell_group(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.cell_group(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of cell groups
PyObject*
meth_cell_group_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroupList::ToPyObject(val.cell_group_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return cell class
PyObject*
npn_class(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellClass::ToPyObject(val.npn_class(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of cell classes
PyObject*
meth_npn_class_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellClassList::ToPyObject(val.npn_class_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get AND function cell group
PyObject*
and_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    nullptr
  };
  unsigned long ni;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.and_func(ni));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get NAND function cell group
PyObject*
nand_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    nullptr
  };
  unsigned long ni;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.nand_func(ni));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get OR function cell group
PyObject*
or_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    nullptr
  };
  unsigned long ni;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.or_func(ni));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get NOR function cell group
PyObject*
nor_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    nullptr
  };
  unsigned long ni;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.nor_func(ni));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get XOR function cell group
PyObject*
xor_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    nullptr
  };
  unsigned long ni;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.xor_func(ni));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get XNOR function cell group
PyObject*
xnor_func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ni",
    nullptr
  };
  unsigned long ni;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &ni) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.xnor_func(ni));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// find FF cell class
PyObject*
find_ff_class(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "attr",
    nullptr
  };
  PyObject* attr_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyClibSeqAttr::_typeobject(), &attr_obj) ) {
    return nullptr;
  }
  ClibSeqAttr attr;
  if ( attr_obj != nullptr ) {
    if ( !PyClibSeqAttr::FromPyObject(attr_obj, attr) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibSeqAttr");
      return nullptr;
    }
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellClassList::ToPyObject(val.find_ff_class(attr));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// find LATCH cell class
PyObject*
find_latch_class(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "attr",
    nullptr
  };
  PyObject* attr_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyClibSeqAttr::_typeobject(), &attr_obj) ) {
    return nullptr;
  }
  ClibSeqAttr attr;
  if ( attr_obj != nullptr ) {
    if ( !PyClibSeqAttr::FromPyObject(attr_obj, attr) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibSeqAttr");
      return nullptr;
    }
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellClassList::ToPyObject(val.find_latch_class(attr));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return PatGraph
PyObject*
pg_pat(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibPatGraph::ToPyObject(val.pg_pat(id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Node Type
PyObject*
pg_node_type(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibPatType::ToPyObject(val.pg_node_type(id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Input ID of the specified node
PyObject*
pg_input_id(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pg_input_id(id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Node ID of the specified input
PyObject*
pg_input_node(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_id",
    nullptr
  };
  unsigned long input_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &input_id) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pg_input_node(input_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Edge information(souce-id, sink-id, fanin-pos, inv)
PyObject*
pg_edge(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    auto val1 = val.pg_edge_from(id);
    auto val2 = val.pg_edge_to(id);
    auto val3 = val.pg_edge_pos(id);
    auto val4 = val.pg_edge_inv(id);
    return Py_BuildValue("(OOOO)", val1, val2, val3, val4);
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// dump
PyObject*
dump(
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
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    val.dump(filename);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// restore
PyObject*
restore(
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
    return PyClibCellLibrary::ToPyObject(ClibCellLibrary::restore(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// display contents
PyObject*
display(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fout",
    nullptr
  };
  PyObject* fout_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &fout_obj) ) {
    return nullptr;
  }
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    auto s = opyfstream(fout_obj);
    val.display(s);
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
  {"read_mislib",
   reinterpret_cast<PyCFunction>(read_mislib),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read mislig(genlib)")},
  {"read_liberty",
   reinterpret_cast<PyCFunction>(read_liberty),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read liberty(.lib)")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"meth_piece_define",
   meth_piece_define,
   METH_NOARGS,
   PyDoc_STR("return list of piece-defines")},
  {"cell",
   cell,
   METH_VARARGS,
   PyDoc_STR("return cell")},
  {"meth_cell_list",
   meth_cell_list,
   METH_NOARGS,
   PyDoc_STR("return list of cells")},
  {"cell_group",
   reinterpret_cast<PyCFunction>(cell_group),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return cell group")},
  {"meth_cell_group_list",
   meth_cell_group_list,
   METH_NOARGS,
   PyDoc_STR("return list of cell groups")},
  {"npn_class",
   reinterpret_cast<PyCFunction>(npn_class),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return cell class")},
  {"meth_npn_class_list",
   meth_npn_class_list,
   METH_NOARGS,
   PyDoc_STR("return list of cell classes")},
  {"and_func",
   reinterpret_cast<PyCFunction>(and_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get AND function cell group")},
  {"nand_func",
   reinterpret_cast<PyCFunction>(nand_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get NAND function cell group")},
  {"or_func",
   reinterpret_cast<PyCFunction>(or_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get OR function cell group")},
  {"nor_func",
   reinterpret_cast<PyCFunction>(nor_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get NOR function cell group")},
  {"xor_func",
   reinterpret_cast<PyCFunction>(xor_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get XOR function cell group")},
  {"xnor_func",
   reinterpret_cast<PyCFunction>(xnor_func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get XNOR function cell group")},
  {"find_ff_class",
   reinterpret_cast<PyCFunction>(find_ff_class),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("find FF cell class")},
  {"find_latch_class",
   reinterpret_cast<PyCFunction>(find_latch_class),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("find LATCH cell class")},
  {"pg_pat",
   reinterpret_cast<PyCFunction>(pg_pat),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return PatGraph")},
  {"pg_node_type",
   reinterpret_cast<PyCFunction>(pg_node_type),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Node Type")},
  {"pg_input_id",
   reinterpret_cast<PyCFunction>(pg_input_id),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Input ID of the specified node")},
  {"pg_input_node",
   reinterpret_cast<PyCFunction>(pg_input_node),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Node ID of the specified input")},
  {"pg_edge",
   reinterpret_cast<PyCFunction>(pg_edge),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Edge information(souce-id, sink-id, fanin-pos, inv)")},
  {"dump",
   reinterpret_cast<PyCFunction>(dump),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("dump")},
  {"restore",
   reinterpret_cast<PyCFunction>(restore),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("restore")},
  {"display",
   reinterpret_cast<PyCFunction>(display),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("display contents")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.name());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_technology(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibTechnology::ToPyObject(val.technology());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_delay_model(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibDelayModel::ToPyObject(val.delay_model());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_piece_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibVarType::ToPyObject(val.piece_type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bus_naming_style(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.bus_naming_style());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_date(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.date());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_revision(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.revision());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_comment(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.comment());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_time_unit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.time_unit().val());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_voltage_unit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.voltage_unit().val());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_current_unit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.current_unit().val());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pulling_resistance_unit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.pulling_resistance_unit().val());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_capacitive_load_unit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    auto val1 = PyFloat::ToPyObject(val.capacitive_load_unit().float_val());
    auto val2 = PyString::ToPyObject(val.capacitive_load_unit().str_val());
    return Py_BuildValue("(OO)", val1, val2);
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_leakage_power_unit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyString::ToPyObject(val.leakage_power_unit().val());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.cell_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_group_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.cell_group_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_npn_class_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.npn_class_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_const0_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.const0_func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_const1_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.const1_func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_buf_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.buf_func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_inv_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.inv_func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_mux2_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.mux2_func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_mux4_func(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.mux4_func());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pg_pat_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pg_pat_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pg_max_input(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pg_max_input());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pg_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pg_node_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pg_edge_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellLibrary::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pg_edge_num());
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
  {"name", get_name, nullptr, PyDoc_STR("name"), nullptr},
  {"technology", get_technology, nullptr, PyDoc_STR("technology(cmos, fpga)"), nullptr},
  {"delay_model", get_delay_model, nullptr, PyDoc_STR("delay model(generic_cmos, table_lookup, piecewise_cmos, cmos2, dcm, polynomial)"), nullptr},
  {"piece_type", get_piece_type, nullptr, PyDoc_STR("ClibVarType for pice_define"), nullptr},
  {"bus_naming_style", get_bus_naming_style, nullptr, PyDoc_STR("bus naming style"), nullptr},
  {"date", get_date, nullptr, PyDoc_STR("date"), nullptr},
  {"revision", get_revision, nullptr, PyDoc_STR("revision"), nullptr},
  {"comment", get_comment, nullptr, PyDoc_STR("comment"), nullptr},
  {"time_unit", get_time_unit, nullptr, PyDoc_STR("time unit"), nullptr},
  {"voltage_unit", get_voltage_unit, nullptr, PyDoc_STR("valtage unit"), nullptr},
  {"current_unit", get_current_unit, nullptr, PyDoc_STR("current unit"), nullptr},
  {"pulling_resistance_unit", get_pulling_resistance_unit, nullptr, PyDoc_STR("pulling resistance unit"), nullptr},
  {"capacitive_load_unit", get_capacitive_load_unit, nullptr, PyDoc_STR("capacitive load unit"), nullptr},
  {"leakage_power_unit", get_leakage_power_unit, nullptr, PyDoc_STR("leakage power unit"), nullptr},
  {"cell_num", get_cell_num, nullptr, PyDoc_STR("number of cells"), nullptr},
  {"cell_group_num", get_cell_group_num, nullptr, PyDoc_STR("number of cell groups"), nullptr},
  {"npn_class_num", get_npn_class_num, nullptr, PyDoc_STR("number of cell classes"), nullptr},
  {"const0_func", get_const0_func, nullptr, PyDoc_STR("Const0 cell group"), nullptr},
  {"const1_func", get_const1_func, nullptr, PyDoc_STR("Const1 cell group"), nullptr},
  {"buf_func", get_buf_func, nullptr, PyDoc_STR("Buffer cell group"), nullptr},
  {"inv_func", get_inv_func, nullptr, PyDoc_STR("Inveter cell gropu"), nullptr},
  {"mux2_func", get_mux2_func, nullptr, PyDoc_STR("MUX2 function cell group"), nullptr},
  {"mux4_func", get_mux4_func, nullptr, PyDoc_STR("MUX4 function cell group"), nullptr},
  {"pg_pat_num", get_pg_pat_num, nullptr, PyDoc_STR("number of PatGraphs"), nullptr},
  {"pg_max_input", get_pg_max_input, nullptr, PyDoc_STR("max number of PatGraph inputs"), nullptr},
  {"pg_node_num", get_pg_node_num, nullptr, PyDoc_STR("number PatGraph nodes"), nullptr},
  {"pg_edge_num", get_pg_edge_num, nullptr, PyDoc_STR("number of PatGraph edges"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibCellLibrary オブジェクトを使用可能にする．
bool
PyClibCellLibrary::init(
  PyObject* m
)
{
  ClibCellLibrary_Type.tp_name = "ClibCellLibrary";
  ClibCellLibrary_Type.tp_basicsize = sizeof(ClibCellLibrary_Object);
  ClibCellLibrary_Type.tp_itemsize = 0;
  ClibCellLibrary_Type.tp_dealloc = dealloc_func;
  ClibCellLibrary_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCellLibrary_Type.tp_doc = PyDoc_STR("Python extended object for ClibCellLibrary");
  ClibCellLibrary_Type.tp_richcompare = richcompare_func;
  ClibCellLibrary_Type.tp_methods = methods;
  ClibCellLibrary_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibCellLibrary", &ClibCellLibrary_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCellLibrary を PyObject に変換する．
PyObject*
PyClibCellLibrary::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibCellLibrary::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCellLibrary_Object*>(obj);
  new (&my_obj->mVal) ClibCellLibrary(val);
  return obj;
}

// PyObject を ClibCellLibrary に変換する．
bool
PyClibCellLibrary::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibCellLibrary::Check(obj) ) {
    val = PyClibCellLibrary::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCellLibrary タイプか調べる．
bool
PyClibCellLibrary::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCellLibrary_Type);
}

// @brief PyObject から ClibCellLibrary を取り出す．
ClibCellLibrary&
PyClibCellLibrary::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCellLibrary_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCellLibrary を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCellLibrary::_typeobject()
{
  return &ClibCellLibrary_Type;
}

END_NAMESPACE_YM
