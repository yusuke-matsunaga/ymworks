
/// @file PyTruthTable.cc
/// @brief PyTruthTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyTruthTable.h"
#include "pym/PyVal3.h"
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
struct TruthTable_Object
{
  PyObject_HEAD
  TruthTable mVal;
};

// Python 用のタイプ定義
PyTypeObject TruthTable_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyTruthTable::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<TruthTable_Object*>(self);
  obj->mVal.~TruthTable();
  Py_TYPE(self)->tp_free(self);
}

// make TruthTable from file
PyObject*
read_file(
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
    return PyTruthTable::ToPyObject(TruthTable::read_file(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// name TruthTable from BIN string
PyObject*
from_bin(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "str",
    nullptr
  };
  const char* str_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &str_tmp) ) {
    return nullptr;
  }
  std::string str;
  if ( str_tmp != nullptr ) {
    str = std::string(str_tmp);
  }
  try {
    return PyTruthTable::ToPyObject(TruthTable::from_bin(str));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// name TruthTable from HEX string
PyObject*
from_hex(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "str",
    nullptr
  };
  const char* str_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &str_tmp) ) {
    return nullptr;
  }
  std::string str;
  if ( str_tmp != nullptr ) {
    str = std::string(str_tmp);
  }
  try {
    return PyTruthTable::ToPyObject(TruthTable::from_hex(str));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// get value
PyObject*
get_value(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "i_index",
    "o_index",
    nullptr
  };
  unsigned long i_index;
  unsigned long o_index;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kk",
                                    const_cast<char**>(kwlist),
                                    &i_index,
                                    &o_index) ) {
    return nullptr;
  }
  auto& val = PyTruthTable::_get_ref(self);
  try {
    return PyVal3::ToPyObject(val.get_value(i_index, o_index));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// set value
PyObject*
set_value(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "i_index",
    "o_index",
    "val",
    nullptr
  };
  unsigned long i_index;
  unsigned long o_index;
  PyObject* v_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kkO!",
                                    const_cast<char**>(kwlist),
                                    &i_index,
                                    &o_index,
                                    PyVal3::_typeobject(), &v_obj) ) {
    return nullptr;
  }
  Val3 v;
  if ( v_obj != nullptr ) {
    if ( !PyVal3::FromPyObject(v_obj, v) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Val3");
      return nullptr;
    }
  }
  auto& val = PyTruthTable::_get_ref(self);
  try {
    val.set_value(i_index, o_index, v);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return BIN format string
PyObject*
bin_str(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTruthTable::_get_ref(self);
  try {
    return PyString::ToPyObject(val.bin_str());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return HEX format string
PyObject*
hex_str(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyTruthTable::_get_ref(self);
  try {
    return PyString::ToPyObject(val.bin_str());
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
  {"read_file",
   reinterpret_cast<PyCFunction>(read_file),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make TruthTable from file")},
  {"from_bin",
   reinterpret_cast<PyCFunction>(from_bin),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("name TruthTable from BIN string")},
  {"from_hex",
   reinterpret_cast<PyCFunction>(from_hex),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("name TruthTable from HEX string")},
  {"get_value",
   reinterpret_cast<PyCFunction>(get_value),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("get value")},
  {"set_value",
   reinterpret_cast<PyCFunction>(set_value),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set value")},
  {"bin_str",
   bin_str,
   METH_NOARGS,
   PyDoc_STR("return BIN format string")},
  {"hex_str",
   hex_str,
   METH_NOARGS,
   PyDoc_STR("return HEX format string")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_input_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyTruthTable::_get_ref(self);
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
  auto& val = PyTruthTable::_get_ref(self);
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

// getter/setter定義
PyGetSetDef getsets[] = {
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of inputs"), nullptr},
  {"output_num", get_output_num, nullptr, PyDoc_STR("number of outputs"), nullptr},
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
    "output_num",
    nullptr
  };
  unsigned long input_num;
  unsigned long output_num;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kk",
                                    const_cast<char**>(kwlist),
                                    &input_num,
                                    &output_num) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<TruthTable_Object*>(self);
    new (&my_obj->mVal) TruthTable(input_num, output_num);
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


// @brief TruthTable オブジェクトを使用可能にする．
bool
PyTruthTable::init(
  PyObject* m
)
{
  TruthTable_Type.tp_name = "TruthTable";
  TruthTable_Type.tp_basicsize = sizeof(TruthTable_Object);
  TruthTable_Type.tp_itemsize = 0;
  TruthTable_Type.tp_dealloc = dealloc_func;
  TruthTable_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  TruthTable_Type.tp_doc = PyDoc_STR("Python extended object for TruthTable");
  TruthTable_Type.tp_methods = methods;
  TruthTable_Type.tp_getset = getsets;
  TruthTable_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "TruthTable", &TruthTable_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// TruthTable を PyObject に変換する．
PyObject*
PyTruthTable::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyTruthTable::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<TruthTable_Object*>(obj);
  new (&my_obj->mVal) TruthTable(val);
  return obj;
}

// PyObject を TruthTable に変換する．
bool
PyTruthTable::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyTruthTable::Check(obj) ) {
    val = PyTruthTable::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が TruthTable タイプか調べる．
bool
PyTruthTable::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &TruthTable_Type);
}

// @brief PyObject から TruthTable を取り出す．
TruthTable&
PyTruthTable::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<TruthTable_Object*>(obj);
  return my_obj->mVal;
}

// @brief TruthTable を表すオブジェクトの型定義を返す．
PyTypeObject*
PyTruthTable::_typeobject()
{
  return &TruthTable_Type;
}

END_NAMESPACE_YM
