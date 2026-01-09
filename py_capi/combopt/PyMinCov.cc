
/// @file PyMinCov.cc
/// @brief PyMinCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyMinCov.h"
#include "pym/PyJsonValue.h"
#include "pym/PyList.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct MinCov_Object
{
  PyObject_HEAD
  MinCov mVal;
};

// Python 用のタイプ定義
PyTypeObject MinCov_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyMinCov::init() 中で初期化する．
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
    "row_size",
    "col_size",
    nullptr
  };
  unsigned long row_size;
  unsigned long col_size;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kk",
                                    const_cast<char**>(kwlist),
                                    &row_size,
                                    &col_size) ) {
    return nullptr;
  }
  auto& val = PyMinCov::_get_ref(self);
  try {
    val.resize(row_size, col_size);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// set column cost
PyObject*
set_col_cost(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "col_pos",
    "cost",
    nullptr
  };
  unsigned long col_pos;
  unsigned long cost;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kk",
                                    const_cast<char**>(kwlist),
                                    &col_pos,
                                    &cost) ) {
    return nullptr;
  }
  auto& val = PyMinCov::_get_ref(self);
  try {
    val.set_col_cost(col_pos, cost);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// insert element
PyObject*
insert_elem(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "row_pos",
    "col_pos",
    nullptr
  };
  unsigned long row_pos;
  unsigned long col_pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kk",
                                    const_cast<char**>(kwlist),
                                    &row_pos,
                                    &col_pos) ) {
    return nullptr;
  }
  auto& val = PyMinCov::_get_ref(self);
  try {
    val.insert_elem(row_pos, col_pos);
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return column cost
PyObject*
col_cost(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "col_pos",
    nullptr
  };
  unsigned long col_pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &col_pos) ) {
    return nullptr;
  }
  auto& val = PyMinCov::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.col_cost(col_pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// solve minimum covering problem
PyObject*
solve(
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
  auto& val = PyMinCov::_get_ref(self);
  try {
    std::vector<SizeType> solution;
    auto cost = val.solve(solution, option);
    auto solution_obj = PyList<SizeType, PyUlong>::ToPyObject(solution);
    return Py_BuildValue("(Ok)", solution_obj, cost);
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
  {"set_col_cost",
   reinterpret_cast<PyCFunction>(set_col_cost),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set column cost")},
  {"insert_elem",
   reinterpret_cast<PyCFunction>(insert_elem),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("insert element")},
  {"col_cost",
   reinterpret_cast<PyCFunction>(col_cost),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return column cost")},
  {"solve",
   reinterpret_cast<PyCFunction>(solve),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("solve minimum covering problem")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_row_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyMinCov::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.row_size());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_col_size(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyMinCov::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.col_size());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_col_cost_array(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyMinCov::_get_ref(self);
  try {
    return PyList<SizeType, PyUlong>::ToPyObject(val.col_cost_array());
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
  {"row_size", get_row_size, nullptr, PyDoc_STR(""), nullptr},
  {"col_size", get_col_size, nullptr, PyDoc_STR(""), nullptr},
  {"col_cost_array", get_col_cost_array, nullptr, PyDoc_STR(""), nullptr},
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
    "row_size",
    "col_size",
    nullptr
  };
  unsigned long row_size = 0;
  unsigned long col_size = 0;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|$kk",
                                    const_cast<char**>(kwlist),
                                    &row_size,
                                    &col_size) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<MinCov_Object*>(self);
    new (&my_obj->mVal) MinCov(row_size, col_size);
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


// @brief MinCov オブジェクトを使用可能にする．
bool
PyMinCov::init(
  PyObject* m
)
{
  MinCov_Type.tp_name = "MinCov";
  MinCov_Type.tp_basicsize = sizeof(MinCov_Object);
  MinCov_Type.tp_itemsize = 0;
  MinCov_Type.tp_dealloc = dealloc_func;
  MinCov_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  MinCov_Type.tp_doc = PyDoc_STR("Python extended object for MinCov");
  MinCov_Type.tp_methods = methods;
  MinCov_Type.tp_getset = getsets;
  MinCov_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "MinCov", &MinCov_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が MinCov タイプか調べる．
bool
PyMinCov::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &MinCov_Type);
}

// @brief PyObject から MinCov を取り出す．
MinCov&
PyMinCov::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<MinCov_Object*>(obj);
  return my_obj->mVal;
}

// @brief MinCov を表すオブジェクトの型定義を返す．
PyTypeObject*
PyMinCov::_typeobject()
{
  return &MinCov_Type;
}

END_NAMESPACE_YM
