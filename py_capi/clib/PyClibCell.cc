
/// @file PyClibCell.cc
/// @brief PyClibCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCell.h"
#include "pym/PyClibCellLibrary.h"
#include "pym/PyClibCellGroup.h"
#include "pym/PyClibCellType.h"
#include "pym/PyClibCPV.h"
#include "pym/PyClibPin.h"
#include "pym/PyClibBus.h"
#include "pym/PyClibBundle.h"
#include "pym/PyClibTiming.h"
#include "pym/PyClibTimingSense.h"
#include "pym/PyClibList.h"
#include "pym/PyClibArea.h"
#include "pym/PyExpr.h"
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
struct ClibCell_Object
{
  PyObject_HEAD
  ClibCell mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCell_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCell::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibCell_Object*>(self);
  obj->mVal.~ClibCell();
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    if ( PyClibCell::Check(other) ) {
      auto& val2 = PyClibCell::_get_ref(other);
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
pin(
  PyObject* self,
  PyObject* args
)
{
  PyObject* arg_obj = nullptr;
  if ( !PyArg_ParseTuple(args, "O",
                         &arg_obj) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    if ( PyUlong::Check(arg_obj) ) {
      auto pos = PyUlong::Get(arg_obj);
      return PyClibPin::ToPyObject(val.pin(pos));
    }
    else if ( PyString::Check(arg_obj) ) {
      auto name = PyString::Get(arg_obj);
      return PyClibPin::ToPyObject(val.pin(name));
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

PyObject*
meth_pin_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPinList::ToPyObject(val.pin_list());
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
    "pos",
    nullptr
  };
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.input(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_input_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPinList::ToPyObject(val.input_list());
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
    "pos",
    nullptr
  };
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.output(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_output_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPinList::ToPyObject(val.output_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
inout(
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.inout(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_inout_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPinList::ToPyObject(val.inout_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
input2(
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.input2(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
output2(
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.output2(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
internal(
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPin::ToPyObject(val.internal(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_internal_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibPinList::ToPyObject(val.internal_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
bus(
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibBus::ToPyObject(val.bus(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_bus_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibBusList::ToPyObject(val.bus_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
bundle(
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibBundle::ToPyObject(val.bundle(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_bundle_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibBundleList::ToPyObject(val.bundle_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
timing_list(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "ipos",
    "opos",
    "sense",
    nullptr
  };
  unsigned long ipos;
  unsigned long opos;
  PyObject* sense_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$kkO!",
                                    const_cast<char**>(kwlist),
                                    &ipos,
                                    &opos,
                                    PyClibTimingSense::_typeobject(), &sense_obj) ) {
    return nullptr;
  }
  ClibTimingSense sense;
  if ( sense_obj != nullptr ) {
    if ( !PyClibTimingSense::FromPyObject(sense_obj, sense) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibTimingSense");
      return nullptr;
    }
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibTimingList::ToPyObject(val.timing_list(ipos, opos, sense));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_logic(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
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

PyObject*
is_ff(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_ff());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
is_latch(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_latch());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
has_logic(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pin_id",
    nullptr
  };
  long pin_id = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|l",
                                    const_cast<char**>(kwlist),
                                    &pin_id) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    if ( pin_id == -1 ) {
      return PyBool_FromLong(val.has_logic());
    }
    else {
      return PyBool_FromLong(val.has_logic(pin_id));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
logic_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pin_id",
    nullptr
  };
  unsigned long pin_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pin_id) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.logic_expr(pin_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
has_tristate(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pin_id",
    nullptr
  };
  unsigned long pin_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pin_id) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyBool_FromLong(val.has_tristate(pin_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
tristate_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pin_id",
    nullptr
  };
  unsigned long pin_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pin_id) ) {
    return nullptr;
  }
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.tristate_expr(pin_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
has_clear(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyBool_FromLong(val.has_clear());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
has_preset(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyBool_FromLong(val.has_preset());
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
  {"pin",
   pin,
   METH_VARARGS,
   PyDoc_STR("return pin")},
  {"meth_pin_list",
   meth_pin_list,
   METH_NOARGS,
   PyDoc_STR("return list of pins")},
  {"input",
   reinterpret_cast<PyCFunction>(input),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return input pin")},
  {"meth_input_list",
   meth_input_list,
   METH_NOARGS,
   PyDoc_STR("return list of input pins")},
  {"output",
   reinterpret_cast<PyCFunction>(output),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return output pin")},
  {"meth_output_list",
   meth_output_list,
   METH_NOARGS,
   PyDoc_STR("return list of output pins")},
  {"inout",
   reinterpret_cast<PyCFunction>(inout),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return inout pin")},
  {"meth_inout_list",
   meth_inout_list,
   METH_NOARGS,
   PyDoc_STR("return list of inout pins")},
  {"input2",
   reinterpret_cast<PyCFunction>(input2),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return input/inout pin")},
  {"output2",
   reinterpret_cast<PyCFunction>(output2),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return output/inout pin")},
  {"internal",
   reinterpret_cast<PyCFunction>(internal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return internal pin")},
  {"meth_internal_list",
   meth_internal_list,
   METH_NOARGS,
   PyDoc_STR("return list of internal pins")},
  {"bus",
   reinterpret_cast<PyCFunction>(bus),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return bus")},
  {"meth_bus_list",
   meth_bus_list,
   METH_NOARGS,
   PyDoc_STR("return list of buses")},
  {"bundle",
   reinterpret_cast<PyCFunction>(bundle),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return bundle")},
  {"meth_bundle_list",
   meth_bundle_list,
   METH_NOARGS,
   PyDoc_STR("return list of bundles")},
  {"timing_list",
   reinterpret_cast<PyCFunction>(timing_list),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return list of timing information")},
  {"is_logic",
   is_logic,
   METH_NOARGS,
   PyDoc_STR("True if LOGIC cell")},
  {"is_ff",
   is_ff,
   METH_NOARGS,
   PyDoc_STR("True if FF cell")},
  {"is_latch",
   is_latch,
   METH_NOARGS,
   PyDoc_STR("True if LATCH cell")},
  {"has_logic",
   reinterpret_cast<PyCFunction>(has_logic),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("True if the output has logic expression")},
  {"logic_expr",
   reinterpret_cast<PyCFunction>(logic_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return logic expression of the output")},
  {"has_tristate",
   reinterpret_cast<PyCFunction>(has_tristate),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("True if the output has tristate expression")},
  {"tristate_expr",
   reinterpret_cast<PyCFunction>(tristate_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return tristate expression of the output")},
  {"has_clear",
   has_clear,
   METH_NOARGS,
   PyDoc_STR("True if having CLEAR pin")},
  {"has_preset",
   has_preset,
   METH_NOARGS,
   PyDoc_STR("True if having PRESET pin")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_library(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibCellLibrary::ToPyObject(val.library());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_group(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.group());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_name(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
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
get_area(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibArea::ToPyObject(val.area());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_pin_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.pin_num());
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
  auto& val = PyClibCell::_get_ref(self);
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
  auto& val = PyClibCell::_get_ref(self);
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
get_inout_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.inout_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_input2_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.input2_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_output2_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.output2_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_internal_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.internal_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bus_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.bus_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_bundle_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.bundle_num());
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
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibCellType::ToPyObject(val.type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_qvar1(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyString::ToPyObject(val.qvar1());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_qvar2(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyString::ToPyObject(val.qvar2());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_clear_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.clear_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_preset_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.preset_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cpv1(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibCPV::ToPyObject(val.clear_preset_var1());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cpv2(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyClibCPV::ToPyObject(val.clear_preset_var2());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_clock_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.clock_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_clock2_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.clock2_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_next_state_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.next_state_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_enable_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.enable_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_enable2_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.enable2_expr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_data_in_expr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCell::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.data_in_expr());
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
  {"library", get_library, nullptr, PyDoc_STR("cell library"), nullptr},
  {"group", get_group, nullptr, PyDoc_STR("cell group"), nullptr},
  {"name", get_name, nullptr, PyDoc_STR("name"), nullptr},
  {"area", get_area, nullptr, PyDoc_STR("area"), nullptr},
  {"pin_num", get_pin_num, nullptr, PyDoc_STR("number of pins"), nullptr},
  {"input_num", get_input_num, nullptr, PyDoc_STR("number of input pins"), nullptr},
  {"output_num", get_output_num, nullptr, PyDoc_STR("number of output pins"), nullptr},
  {"inout_num", get_inout_num, nullptr, PyDoc_STR("number of inout pins"), nullptr},
  {"input2_num", get_input2_num, nullptr, PyDoc_STR("number of input/inout pins"), nullptr},
  {"output2_num", get_output2_num, nullptr, PyDoc_STR("number of output/inout pins"), nullptr},
  {"internal_num", get_internal_num, nullptr, PyDoc_STR("number of internal pins"), nullptr},
  {"bus_num", get_bus_num, nullptr, PyDoc_STR("number of buses"), nullptr},
  {"bundle_num", get_bundle_num, nullptr, PyDoc_STR("number of bundles"), nullptr},
  {"type", get_type, nullptr, PyDoc_STR("type(Logic, FF, Latch, FSM)"), nullptr},
  {"qvar1", get_qvar1, nullptr, PyDoc_STR("qvar1"), nullptr},
  {"qvar2", get_qvar2, nullptr, PyDoc_STR("qvar2"), nullptr},
  {"clear_expr", get_clear_expr, nullptr, PyDoc_STR("clear expression"), nullptr},
  {"preset_expr", get_preset_expr, nullptr, PyDoc_STR("preset expression"), nullptr},
  {"clear_preset_var1", get_cpv1, nullptr, PyDoc_STR("clear preset var1"), nullptr},
  {"clear_preset_var2", get_cpv2, nullptr, PyDoc_STR("clear preset var2"), nullptr},
  {"clock_expr", get_clock_expr, nullptr, PyDoc_STR("clock expression"), nullptr},
  {"clock2_expr", get_clock2_expr, nullptr, PyDoc_STR("clock2 expression"), nullptr},
  {"next_state_expr", get_next_state_expr, nullptr, PyDoc_STR("next state expression"), nullptr},
  {"enable_expr", get_enable_expr, nullptr, PyDoc_STR("enable expression"), nullptr},
  {"enable2_expr", get_enable2_expr, nullptr, PyDoc_STR("enable2 expression"), nullptr},
  {"data_in_expr", get_data_in_expr, nullptr, PyDoc_STR("data-in expression"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibCell オブジェクトを使用可能にする．
bool
PyClibCell::init(
  PyObject* m
)
{
  ClibCell_Type.tp_name = "ClibCell";
  ClibCell_Type.tp_basicsize = sizeof(ClibCell_Object);
  ClibCell_Type.tp_itemsize = 0;
  ClibCell_Type.tp_dealloc = dealloc_func;
  ClibCell_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCell_Type.tp_doc = PyDoc_STR("Python extended object for ClibCell");
  ClibCell_Type.tp_richcompare = richcompare_func;
  ClibCell_Type.tp_methods = methods;
  ClibCell_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibCell", &ClibCell_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCell を PyObject に変換する．
PyObject*
PyClibCell::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibCell::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCell_Object*>(obj);
  new (&my_obj->mVal) ClibCell(val);
  return obj;
}

// PyObject を ClibCell に変換する．
bool
PyClibCell::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibCell::Check(obj) ) {
    val = PyClibCell::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCell タイプか調べる．
bool
PyClibCell::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCell_Type);
}

// @brief PyObject から ClibCell を取り出す．
ClibCell&
PyClibCell::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCell_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCell を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCell::_typeobject()
{
  return &ClibCell_Type;
}

END_NAMESPACE_YM
