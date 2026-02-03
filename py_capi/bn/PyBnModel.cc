
/// @file PyBnModel.cc
/// @brief PyBnModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnModel.h"
#include "pym/PyBnDff.h"
#include "pym/PyBnDffList.h"
#include "pym/PyBnNode.h"
#include "pym/PyBnNodeList.h"
#include "pym/PyBnFunc.h"
#include "pym/PyPrimType.h"
#include "pym/PySopCover.h"
#include "pym/PyExpr.h"
#include "pym/PyTvFunc.h"
#include "pym/PyBdd.h"
#include "pym/PyJsonValue.h"
#include "pym/pyfstream.h"
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
struct BnModel_Object
{
  PyObject_HEAD
  BnModel mVal;
};

// Python 用のタイプ定義
PyTypeObject BnModel_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnModel::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnModel_Object*>(self);
  obj->mVal.~BnModel();
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
  auto& val = PyBnModel::_get_ref(self);
  try {
    if ( PyBnModel::Check(other) ) {
      auto& val2 = PyBnModel::_get_ref(other);
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
    return PyBnModel::ToPyObject(BnModel::read_blif(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
    return PyBnModel::ToPyObject(BnModel::read_iscas89(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
read_truth(
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
    return PyBnModel::ToPyObject(BnModel::read_truth(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$s",
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
    return PyBnModel::ToPyObject(BnModel::read(filename, format));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
write(
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
  auto& val = PyBnModel::_get_ref(self);
  try {
    auto s = opyfstream(fout_obj);
    val.write(s);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
copy(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnModel::ToPyObject(val.copy());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
dff(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "dff_id",
    nullptr
  };
  int dff_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &dff_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnDff::ToPyObject(val.dff(dff_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  int dff_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &dff_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
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

PyObject*
dff_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnDffList::ToPyObject(val.dff_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
node(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "node_id",
    nullptr
  };
  int node_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &node_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.node(node_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  int input_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &input_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.input(input_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  int input_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &input_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
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

PyObject*
input_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNodeList::ToPyObject(val.input_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  int output_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &output_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.output(output_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  int output_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &output_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
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

PyObject*
output_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNodeList::ToPyObject(val.output_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
logic(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "id",
    nullptr
  };
  int id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.logic(id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
logic_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNodeList::ToPyObject(val.logic_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
func(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "func_id",
    nullptr
  };
  int func_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &func_id) ) {
    return nullptr;
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnFunc::ToPyObject(val.func(func_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
comment_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBnModel::_get_ref(self);
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

PyObject*
set_option(
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
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
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
  auto& val = PyBnModel::_get_ref(self);
  try {
    val.set_option(option);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
set_name(
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
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &name_tmp) ) {
    return nullptr;
  }
  std::string name;
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    val.set_name(name);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
add_comment(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "comment",
    nullptr
  };
  const char* comment_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &comment_tmp) ) {
    return nullptr;
  }
  std::string comment;
  if ( comment_tmp != nullptr ) {
    comment = std::string(comment_tmp);
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    val.add_comment(comment);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_dff(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "name",
    "reset_val",
    nullptr
  };
  const char* name_tmp = nullptr;
  int reset_val = 'X';
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$sC",
                                    const_cast<char**>(kwlist),
                                    &name_tmp,
                                    &reset_val) ) {
    return nullptr;
  }
  std::string name = {};
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnDff::ToPyObject(val.new_dff(name, reset_val));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
set_dff_src(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "dff",
    "src",
    nullptr
  };
  PyObject* dff_obj = nullptr;
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
                                    const_cast<char**>(kwlist),
                                    PyBnDff::_typeobject(), &dff_obj,
                                    PyBnNode::_typeobject(), &src_obj) ) {
    return nullptr;
  }
  BnDff dff;
  if ( dff_obj != nullptr ) {
    if ( !PyBnDff::FromPyObject(dff_obj, dff) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BnDff");
      return nullptr;
    }
  }
  BnNode src;
  if ( src_obj != nullptr ) {
    if ( !PyBnNode::FromPyObject(src_obj, src) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BnNode");
      return nullptr;
    }
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    val.set_dff_src(dff, src);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  std::string name = {};
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.new_input(name));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_output(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "src",
    "name",
    nullptr
  };
  PyObject* src_obj = nullptr;
  const char* name_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$s",
                                    const_cast<char**>(kwlist),
                                    PyBnNode::_typeobject(), &src_obj,
                                    &name_tmp) ) {
    return nullptr;
  }
  BnNode src;
  if ( src_obj != nullptr ) {
    if ( !PyBnNode::FromPyObject(src_obj, src) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to BnNode");
      return nullptr;
    }
  }
  std::string name = {};
  if ( name_tmp != nullptr ) {
    name = std::string(name_tmp);
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.new_output(src, name));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_primitive(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "primitive_type",
    "fanin_list",
    nullptr
  };
  PyObject* primitive_type_obj = nullptr;
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
                                    const_cast<char**>(kwlist),
                                    PyPrimType::_typeobject(), &primitive_type_obj,
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  PrimType primitive_type;
  if ( primitive_type_obj != nullptr ) {
    if ( !PyPrimType::FromPyObject(primitive_type_obj, primitive_type) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to PrimType");
      return nullptr;
    }
  }
  std::vector<BnNode> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<BnNode, PyBnNode>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BnNode>");
      return nullptr;
    }
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.new_primitive(primitive_type, fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_cover(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_cover",
    "output_inv",
    "fanin_list",
    nullptr
  };
  PyObject* input_cover_obj = nullptr;
  int output_inv_tmp = -1;
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!pO",
                                    const_cast<char**>(kwlist),
                                    PySopCover::_typeobject(), &input_cover_obj,
                                    &output_inv_tmp,
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  SopCover input_cover;
  if ( input_cover_obj != nullptr ) {
    if ( !PySopCover::FromPyObject(input_cover_obj, input_cover) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to SopCover");
      return nullptr;
    }
  }
  bool output_inv;
  if ( output_inv_tmp != -1 ) {
    output_inv = static_cast<bool>(output_inv_tmp);
  }
  std::vector<BnNode> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<BnNode, PyBnNode>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BnNode>");
      return nullptr;
    }
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.new_cover(input_cover, output_inv, fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "expr",
    "fanin_list",
    nullptr
  };
  PyObject* expr_obj = nullptr;
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
                                    const_cast<char**>(kwlist),
                                    PyExpr::_typeobject(), &expr_obj,
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  Expr expr;
  if ( expr_obj != nullptr ) {
    if ( !PyExpr::FromPyObject(expr_obj, expr) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Expr");
      return nullptr;
    }
  }
  std::vector<BnNode> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<BnNode, PyBnNode>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BnNode>");
      return nullptr;
    }
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.new_expr(expr, fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_tvfunc(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "func",
    "fanin_list",
    nullptr
  };
  PyObject* func_obj = nullptr;
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
                                    const_cast<char**>(kwlist),
                                    PyTvFunc::_typeobject(), &func_obj,
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  TvFunc func;
  if ( func_obj != nullptr ) {
    if ( !PyTvFunc::FromPyObject(func_obj, func) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to TvFunc");
      return nullptr;
    }
  }
  std::vector<BnNode> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<BnNode, PyBnNode>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BnNode>");
      return nullptr;
    }
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.new_tvfunc(func, fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
new_bdd(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "bdd",
    "fanin_list",
    nullptr
  };
  PyObject* bdd_obj = nullptr;
  PyObject* fanin_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &bdd_obj,
                                    &fanin_list_obj) ) {
    return nullptr;
  }
  Bdd bdd;
  if ( bdd_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(bdd_obj, bdd) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  std::vector<BnNode> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<BnNode, PyBnNode>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BnNode>");
      return nullptr;
    }
  }
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val.new_bdd(bdd, fanin_list));
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
  {"read_blif",
   reinterpret_cast<PyCFunction>(read_blif),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read BLIF file")},
  {"read_iscas89",
   reinterpret_cast<PyCFunction>(read_iscas89),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read ISCAS89(.bench) file")},
  {"read_truth",
   reinterpret_cast<PyCFunction>(read_truth),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read TRUTH(.truth) file")},
  {"read",
   reinterpret_cast<PyCFunction>(read),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("read file (format: [blif, iscas89, truth])")},
  {"write",
   reinterpret_cast<PyCFunction>(write),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("write")},
  {"copy",
   copy,
   METH_NOARGS,
   PyDoc_STR("make a DEEP copy")},
  {"dff",
   reinterpret_cast<PyCFunction>(dff),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return DFF")},
  {"dff_name",
   reinterpret_cast<PyCFunction>(dff_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return DFF name")},
  {"dff_list",
   dff_list,
   METH_NOARGS,
   PyDoc_STR("return list of DFFs")},
  {"node",
   reinterpret_cast<PyCFunction>(node),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return BnNode")},
  {"input",
   reinterpret_cast<PyCFunction>(input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Input Node")},
  {"input_name",
   reinterpret_cast<PyCFunction>(input_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Input Name")},
  {"input_list",
   input_list,
   METH_NOARGS,
   PyDoc_STR("return list of inputs")},
  {"output",
   reinterpret_cast<PyCFunction>(output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Output Node")},
  {"output_name",
   reinterpret_cast<PyCFunction>(output_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Output Name")},
  {"output_list",
   output_list,
   METH_NOARGS,
   PyDoc_STR("return list of outputs")},
  {"logic",
   reinterpret_cast<PyCFunction>(logic),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Logic Node")},
  {"logic_list",
   logic_list,
   METH_NOARGS,
   PyDoc_STR("return list of logic nodes")},
  {"func",
   reinterpret_cast<PyCFunction>(func),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return Function")},
  {"comment_list",
   comment_list,
   METH_NOARGS,
   PyDoc_STR("return list of comment lines")},
  {"set_option",
   reinterpret_cast<PyCFunction>(set_option),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set option")},
  {"set_name",
   reinterpret_cast<PyCFunction>(set_name),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set name")},
  {"add_comment",
   reinterpret_cast<PyCFunction>(add_comment),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("add comment")},
  {"new_dff",
   reinterpret_cast<PyCFunction>(new_dff),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new DFF")},
  {"set_dff_src",
   reinterpret_cast<PyCFunction>(set_dff_src),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set DFF source input")},
  {"new_input",
   reinterpret_cast<PyCFunction>(new_input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new input node")},
  {"new_output",
   reinterpret_cast<PyCFunction>(new_output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new output node")},
  {"new_primitive",
   reinterpret_cast<PyCFunction>(new_primitive),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new primitive-type node")},
  {"new_cover",
   reinterpret_cast<PyCFunction>(new_cover),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new cover-type node")},
  {"new_expr",
   reinterpret_cast<PyCFunction>(new_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new expr-type node")},
  {"new_tvfunc",
   reinterpret_cast<PyCFunction>(new_tvfunc),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new tvfunc-type node")},
  {"new_bdd",
   reinterpret_cast<PyCFunction>(new_bdd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("new BDD-type node")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnModel::_get_ref(self);
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
get_dff_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnModel::_get_ref(self);
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

PyObject*
get_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnModel::_get_ref(self);
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
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnModel::_get_ref(self);
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
  auto& val = PyBnModel::_get_ref(self);
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
get_logic_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.logic_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_func_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBnModel::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.func_num());
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
  {"dff_num", get_dff_num, nullptr, PyDoc_STR("number of DFFs"), nullptr},
  {"node_num", get_node_num, nullptr, PyDoc_STR("number of all nodes"), nullptr},
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"output_num", get_output_num, nullptr, PyDoc_STR("number of outputs"), nullptr},
  {"logic_num", get_logic_num, nullptr, PyDoc_STR("number of logic nodes"), nullptr},
  {"func_num", get_func_num, nullptr, PyDoc_STR("number of functions"), nullptr},
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
    nullptr
  };
  // 余分な引数を取らないことを確認しておく．
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "", const_cast<char**>(kwlist)) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<BnModel_Object*>(self);
    new (&my_obj->mVal) BnModel();
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


// @brief BnModel オブジェクトを使用可能にする．
bool
PyBnModel::init(
  PyObject* m
)
{
  BnModel_Type.tp_name = "BnModel";
  BnModel_Type.tp_basicsize = sizeof(BnModel_Object);
  BnModel_Type.tp_itemsize = 0;
  BnModel_Type.tp_dealloc = dealloc_func;
  BnModel_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnModel_Type.tp_doc = PyDoc_STR("Python extended object for BnModel");
  BnModel_Type.tp_richcompare = richcompare_func;
  BnModel_Type.tp_methods = methods;
  BnModel_Type.tp_getset = getsets;
  BnModel_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BnModel", &BnModel_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnModel を PyObject に変換する．
PyObject*
PyBnModel::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnModel::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnModel_Object*>(obj);
  new (&my_obj->mVal) BnModel(val);
  return obj;
}

// PyObject を BnModel に変換する．
bool
PyBnModel::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBnModel::Check(obj) ) {
    val = PyBnModel::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BnModel タイプか調べる．
bool
PyBnModel::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnModel_Type);
}

// @brief PyObject から BnModel を取り出す．
BnModel&
PyBnModel::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnModel_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnModel を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnModel::_typeobject()
{
  return &BnModel_Type;
}

END_NAMESPACE_YM
