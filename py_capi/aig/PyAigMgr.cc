
/// @file PyAigMgr.cc
/// @brief PyAigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyAigMgr.h"
#include "pym/PyAigLiteral.h"
#include "pym/PyBnModel.h"
#include "pym/PyPrimType.h"
#include "pym/PyExpr.h"
#include "pym/PySopCover.h"
#include "pym/PySopCube.h"
#include "pym/pyfstream.h"
#include "pym/PyUlong.h"
#include "pym/PyString.h"
#include "pym/PyList.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct AigMgr_Object
{
  PyObject_HEAD
  AigMgr mVal;
};

// Python 用のタイプ定義
PyTypeObject AigMgr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyAigMgr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<AigMgr_Object*>(self);
  obj->mVal.~AigMgr();
  Py_TYPE(self)->tp_free(self);
}

// read AAG file
PyObject*
read_aag(
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
    return PyAigMgr::ToPyObject(AigMgr::read_aag(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read AIG file
PyObject*
read_aig(
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
    return PyAigMgr::ToPyObject(AigMgr::read_aig(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read BLIF file
PyObject*
read_blif(
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
    return PyAigMgr::ToPyObject(AigMgr::read_blif(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read ISCAS89(.bench) file
PyObject*
read_iscas89(
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
    return PyAigMgr::ToPyObject(AigMgr::read_iscas89(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// read file (format should be aag, aig, blif or iscas89)
PyObject*
read(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    "format",
    nullptr
  };
  const char* filename_tmp = nullptr;
  const char* format_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "ss",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp,
                                    &format_tmp) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  std::string format;
  if ( format_tmp != nullptr ) {
    format = std::string(format_tmp);
  }
  try {
    return PyAigMgr::ToPyObject(AigMgr::read(filename, format));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// convert from BnModel
PyObject*
from_bn(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "model",
    nullptr
  };
  PyObject* model_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyBnModel::_typeobject(), &model_obj) ) {
    return nullptr;
  }
  BnModel model;
  if ( model_obj != nullptr ) {
    if ( !PyBnModel::FromPyObject(model_obj, model) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BnModel");
      return nullptr;
    }
  }
  try {
    return PyAigMgr::ToPyObject(AigMgr::from_bn(model));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// write file in AAG format
PyObject*
write_aag(
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
  auto& val = PyAigMgr::_get_ref(self);
  try {
    auto s = opyfstream(fout_obj);
    val.write_aag(s);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// write file in AIG format
PyObject*
write_aig(
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
  auto& val = PyAigMgr::_get_ref(self);
  try {
    auto s = opyfstream(fout_obj);
    val.write_aig(s);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// write file in specified format
PyObject*
write(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fout",
    "format",
    nullptr
  };
  PyObject* fout_obj = nullptr;
  const char* format_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "Os",
                                    const_cast<char**>(kwlist),
                                    &fout_obj,
                                    &format_tmp) ) {
    return nullptr;
  }
  std::string format;
  if ( format_tmp != nullptr ) {
    format = std::string(format_tmp);
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    auto s = opyfstream(fout_obj);
    val.write(s, format);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if having valid data
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
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

// make a DEEP copy
PyObject*
copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigMgr::ToPyObject(val.copy());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Input Literal
PyObject*
input(
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
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.input(input_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Input Name
PyObject*
input_name(
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
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyString::ToPyObject(val.input_name(input_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Output Literal
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
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.output(output_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return Output Name
PyObject*
output_name(
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
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyString::ToPyObject(val.output_name(output_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return DFF Output Literal
PyObject*
dff_output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "dff_id",
    nullptr
  };
  unsigned long dff_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &dff_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.dff_output(dff_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return DFF Input Literal
PyObject*
dff_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "dff_id",
    nullptr
  };
  unsigned long dff_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &dff_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.dff_input(dff_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return DFF Name
PyObject*
dff_name(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "dff_id",
    nullptr
  };
  unsigned long dff_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &dff_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyString::ToPyObject(val.dff_name(dff_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of comments
PyObject*
comment_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyList<std::string, PyString>::ToPyObject(val.comment_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if lit is INPUT
PyObject*
is_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_input(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if lit is PRIMARY INPUT
PyObject*
is_primary_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_primary_input(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return INPUT ID of lit
PyObject*
input_id(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.input_id(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if lit is DFF OUTPUT
PyObject*
is_dff_output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_dff_output(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return DFF ID of lit
PyObject*
dff_id(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.dff_id(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if lit is AND
PyObject*
is_and(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_and(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fanin literal
PyObject*
fanin(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    "pos",
    nullptr
  };
  unsigned long node_id;
  unsigned int pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kI",
                                    const_cast<char**>(kwlist),
                                    &node_id,
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.fanin(node_id, pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fanin0 literal
PyObject*
fanin0(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.fanin0(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fanin1 literal
PyObject*
fanin1(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.fanin1(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of expanded fanin literals
PyObject*
ex_fanin_list(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  unsigned long node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyList<AigLiteral, PyAigLiteral>::ToPyObject(val.ex_fanin_list(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new Input
PyObject*
new_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "name",
    nullptr
  };
  const char* name_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$s",
                                    const_cast<char**>(kwlist),
                                    &name_tmp) ) {
    return nullptr;
  }
  std::string name;
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.new_input(name));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new Output
PyObject*
new_output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "lit",
    "name",
    nullptr
  };
  PyObject* lit_obj = nullptr;
  const char* name_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$s",
                                    const_cast<char**>(kwlist),
                                    PyAigLiteral::_typeobject(), &lit_obj,
                                    &name_tmp) ) {
    return nullptr;
  }
  AigLiteral lit;
  if ( lit_obj != nullptr ) {
    if ( !PyAigLiteral::FromPyObject(lit_obj, lit) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to AigLiteral");
      return nullptr;
    }
  }
  std::string name;
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.new_output(lit, name));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new DFF
PyObject*
new_dff(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "name",
    nullptr
  };
  const char* name_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$s",
                                    const_cast<char**>(kwlist),
                                    &name_tmp) ) {
    return nullptr;
  }
  std::string name;
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.new_dff(name));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// set DFF input
PyObject*
set_dff_input(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "dff_id",
    "lit",
    nullptr
  };
  unsigned long dff_id;
  PyObject* lit_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "kO!",
                                    const_cast<char**>(kwlist),
                                    &dff_id,
                                    PyAigLiteral::_typeobject(), &lit_obj) ) {
    return nullptr;
  }
  AigLiteral lit;
  if ( lit_obj != nullptr ) {
    if ( !PyAigLiteral::FromPyObject(lit_obj, lit) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to AigLiteral");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    val.set_dff_input(dff_id, lit);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new AND
PyObject*
new_and(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fanin_list",
    nullptr
  };
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  std::vector<AigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.new_and(fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new OR
PyObject*
new_or(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fanin_list",
    nullptr
  };
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  std::vector<AigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.new_or(fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new XOR
PyObject*
new_xor(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "fanin_list",
    nullptr
  };
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  std::vector<AigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.new_xor(fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make new PRIMITIVE
PyObject*
new_primitive(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "prim_type",
    "fanin_list",
    nullptr
  };
  PyObject* prim_type_obj = nullptr;
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
                                    const_cast<char**>(kwlist),
                                    PyPrimType::_typeobject(), &prim_type_obj,
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  PrimType prim_type;
  if ( prim_type_obj != nullptr ) {
    if ( !PyPrimType::FromPyObject(prim_type_obj, prim_type) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to PrimType");
      return nullptr;
    }
  }
  std::vector<AigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyAigLiteral::ToPyObject(val.new_primitive(prim_type, fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make AIG from EXPR
PyObject*
from_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "expr",
    "input_list",
    nullptr
  };
  PyObject* expr_obj = nullptr;
  PyObject* input_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$O",
                                    const_cast<char**>(kwlist),
                                    PyExpr::_typeobject(), &expr_obj,
                                    &input_list_obj) ) {
    return nullptr;
  }
  Expr expr;
  if ( expr_obj != nullptr ) {
    if ( !PyExpr::FromPyObject(expr_obj, expr) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Expr");
      return nullptr;
    }
  }
  std::vector<AigLiteral> input_list;
  if ( input_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(input_list_obj, input_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    if ( input_list.empty() ) {
      return PyAigLiteral::ToPyObject(val.from_expr(expr));
    }
    else {
      return PyAigLiteral::ToPyObject(val.from_expr(expr, input_list));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make AIG from EXPR list
PyObject*
from_expr_list(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "expr_list",
    "input_list",
    nullptr
  };
  PyObject* expr_list_obj = nullptr;
  PyObject* input_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O|$O",
                                    const_cast<char**>(kwlist),
                                    &expr_list_obj,
                                    &input_list_obj) ) {
    return nullptr;
  }
  std::vector<Expr> expr_list;
  if ( expr_list_obj != nullptr ) {
    if ( !PyList<Expr, PyExpr>::FromPyObject(expr_list_obj, expr_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<Expr>");
      return nullptr;
    }
  }
  std::vector<AigLiteral> input_list;
  if ( input_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(input_list_obj, input_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    if ( input_list.empty() ) {
      return PyList<AigLiteral, PyAigLiteral>::ToPyObject(val.from_expr_list(expr_list));
    }
    else {
      return PyList<AigLiteral, PyAigLiteral>::ToPyObject(val.from_expr_list(expr_list, input_list));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make AIG from COVER
PyObject*
from_cover(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "cover",
    "input_list",
    nullptr
  };
  PyObject* cover_obj = nullptr;
  PyObject* input_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$O",
                                    const_cast<char**>(kwlist),
                                    PySopCover::_typeobject(), &cover_obj,
                                    &input_list_obj) ) {
    return nullptr;
  }
  SopCover cover;
  if ( cover_obj != nullptr ) {
    if ( !PySopCover::FromPyObject(cover_obj, cover) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to SopCover");
      return nullptr;
    }
  }
  std::vector<AigLiteral> input_list;
  if ( input_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(input_list_obj, input_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    if ( input_list.empty() ) {
      return PyAigLiteral::ToPyObject(val.from_cover(cover));
    }
    else {
      return PyAigLiteral::ToPyObject(val.from_cover(cover, input_list));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make AIG from CUBE
PyObject*
from_cube(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "cube",
    "input_list",
    nullptr
  };
  PyObject* cube_obj = nullptr;
  PyObject* input_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$O",
                                    const_cast<char**>(kwlist),
                                    PySopCube::_typeobject(), &cube_obj,
                                    &input_list_obj) ) {
    return nullptr;
  }
  SopCube cube;
  if ( cube_obj != nullptr ) {
    if ( !PySopCube::FromPyObject(cube_obj, cube) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to SopCube");
      return nullptr;
    }
  }
  std::vector<AigLiteral> input_list;
  if ( input_list_obj != nullptr ) {
    if ( !PyList<AigLiteral, PyAigLiteral>::FromPyObject(input_list_obj, input_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<AigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyAigMgr::_get_ref(self);
  try {
    if ( input_list.empty() ) {
      return PyAigLiteral::ToPyObject(val.from_cube(cube));
    }
    else {
      return PyAigLiteral::ToPyObject(val.from_cube(cube, input_list));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// do LOCAL REWRITING
PyObject*
rewrite(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  try {
    val.rewrite();
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
  {"read_aag",
   reinterpret_cast<PyCFunction>(read_aag),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read AAG file")},
  {"read_aig",
   reinterpret_cast<PyCFunction>(read_aig),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read AIG file")},
  {"read_blif",
   reinterpret_cast<PyCFunction>(read_blif),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read BLIF file")},
  {"read_iscas89",
   reinterpret_cast<PyCFunction>(read_iscas89),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read ISCAS89(.bench) file")},
  {"read",
   reinterpret_cast<PyCFunction>(read),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read file (format should be aag, aig, blif or iscas89)")},
  {"from_bn",
   reinterpret_cast<PyCFunction>(from_bn),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("convert from BnModel")},
  {"write_aag",
   reinterpret_cast<PyCFunction>(write_aag),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write file in AAG format")},
  {"write_aig",
   reinterpret_cast<PyCFunction>(write_aig),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write file in AIG format")},
  {"write",
   reinterpret_cast<PyCFunction>(write),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write file in specified format")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if having valid data")},
  {"copy",
   copy,
   METH_NOARGS,
   PyDoc_STR("make a DEEP copy")},
  {"input",
   reinterpret_cast<PyCFunction>(input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Input Literal")},
  {"input_name",
   reinterpret_cast<PyCFunction>(input_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Input Name")},
  {"output",
   reinterpret_cast<PyCFunction>(output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Output Literal")},
  {"output_name",
   reinterpret_cast<PyCFunction>(output_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Output Name")},
  {"dff_output",
   reinterpret_cast<PyCFunction>(dff_output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return DFF Output Literal")},
  {"dff_input",
   reinterpret_cast<PyCFunction>(dff_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return DFF Input Literal")},
  {"dff_name",
   reinterpret_cast<PyCFunction>(dff_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return DFF Name")},
  {"comment_list",
   comment_list,
   METH_NOARGS,
   PyDoc_STR("return list of comments")},
  {"is_input",
   reinterpret_cast<PyCFunction>(is_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("True if lit is INPUT")},
  {"is_primary_input",
   reinterpret_cast<PyCFunction>(is_primary_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("True if lit is PRIMARY INPUT")},
  {"input_id",
   reinterpret_cast<PyCFunction>(input_id),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return INPUT ID of lit")},
  {"is_dff_output",
   reinterpret_cast<PyCFunction>(is_dff_output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("True if lit is DFF OUTPUT")},
  {"dff_id",
   reinterpret_cast<PyCFunction>(dff_id),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return DFF ID of lit")},
  {"is_and",
   reinterpret_cast<PyCFunction>(is_and),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("True if lit is AND")},
  {"fanin",
   reinterpret_cast<PyCFunction>(fanin),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fanin literal")},
  {"fanin0",
   reinterpret_cast<PyCFunction>(fanin0),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fanin0 literal")},
  {"fanin1",
   reinterpret_cast<PyCFunction>(fanin1),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fanin1 literal")},
  {"ex_fanin_list",
   reinterpret_cast<PyCFunction>(ex_fanin_list),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return list of expanded fanin literals")},
  {"new_input",
   reinterpret_cast<PyCFunction>(new_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new Input")},
  {"new_output",
   reinterpret_cast<PyCFunction>(new_output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new Output")},
  {"new_dff",
   reinterpret_cast<PyCFunction>(new_dff),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new DFF")},
  {"set_dff_input",
   reinterpret_cast<PyCFunction>(set_dff_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set DFF input")},
  {"new_and",
   reinterpret_cast<PyCFunction>(new_and),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new AND")},
  {"new_or",
   reinterpret_cast<PyCFunction>(new_or),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new OR")},
  {"new_xor",
   reinterpret_cast<PyCFunction>(new_xor),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new XOR")},
  {"new_primitive",
   reinterpret_cast<PyCFunction>(new_primitive),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make new PRIMITIVE")},
  {"from_expr",
   reinterpret_cast<PyCFunction>(from_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make AIG from EXPR")},
  {"from_expr_list",
   reinterpret_cast<PyCFunction>(from_expr_list),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make AIG from EXPR list")},
  {"from_cover",
   reinterpret_cast<PyCFunction>(from_cover),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make AIG from COVER")},
  {"from_cube",
   reinterpret_cast<PyCFunction>(from_cube),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make AIG from CUBE")},
  {"rewrite",
   rewrite,
   METH_NOARGS,
   PyDoc_STR("do LOCAL REWRITING")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigMgr::_get_ref(self);
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
  auto& val = PyAigMgr::_get_ref(self);
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
get_and_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.and_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_dff_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyAigMgr::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.dff_num());
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
  {"and_num", get_and_num, nullptr, PyDoc_STR("number of AND nodes"), nullptr},
  {"dff_num", get_dff_num, nullptr, PyDoc_STR("number of DFFs"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief AigMgr オブジェクトを使用可能にする．
bool
PyAigMgr::init(
  PyObject* m
)
{
  AigMgr_Type.tp_name = "AigMgr";
  AigMgr_Type.tp_basicsize = sizeof(AigMgr_Object);
  AigMgr_Type.tp_itemsize = 0;
  AigMgr_Type.tp_dealloc = dealloc_func;
  AigMgr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  AigMgr_Type.tp_doc = PyDoc_STR("Python extended object for AigMgr");
  AigMgr_Type.tp_methods = methods;
  AigMgr_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "AigMgr", &AigMgr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// AigMgr を PyObject に変換する．
PyObject*
PyAigMgr::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyAigMgr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<AigMgr_Object*>(obj);
  new (&my_obj->mVal) AigMgr(val);
  return obj;
}

// PyObject を AigMgr に変換する．
bool
PyAigMgr::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyAigMgr::Check(obj) ) {
    val = PyAigMgr::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が AigMgr タイプか調べる．
bool
PyAigMgr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &AigMgr_Type);
}

// @brief PyObject から AigMgr を取り出す．
AigMgr&
PyAigMgr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<AigMgr_Object*>(obj);
  return my_obj->mVal;
}

// @brief AigMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyAigMgr::_typeobject()
{
  return &AigMgr_Type;
}

END_NAMESPACE_YM
