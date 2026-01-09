
/// @file PyClibTiming.cc
/// @brief PyClibTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibTiming.h"
#include "pym/PyClibTime.h"
#include "pym/PyClibCapacitance.h"
#include "pym/PyClibResistance.h"
#include "pym/PyClibTimingType.h"
#include "pym/PyClibLut.h"
#include "pym/PyExpr.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibTiming_Object
{
  PyObject_HEAD
  ClibTiming mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibTiming_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibTiming::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibTiming_Object*>(self);
  obj->mVal.~ClibTiming();
  Py_TYPE(self)->tp_free(self);
}

// calculate RISE delay
PyObject*
calc_rise_delay(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_transition",
    "output_capacitance",
    nullptr
  };
  PyObject* input_transition_obj = nullptr;
  PyObject* output_capacitance_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
                                    const_cast<char**>(kwlist),
                                    PyClibTime::_typeobject(), &input_transition_obj,
                                    PyClibCapacitance::_typeobject(), &output_capacitance_obj) ) {
    return nullptr;
  }
  ClibTime input_transition;
  if ( input_transition_obj != nullptr ) {
    if ( !PyClibTime::FromPyObject(input_transition_obj, input_transition) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibTime");
      return nullptr;
    }
  }
  ClibCapacitance output_capacitance;
  if ( output_capacitance_obj != nullptr ) {
    if ( !PyClibCapacitance::FromPyObject(output_capacitance_obj, output_capacitance) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibCapacitance");
      return nullptr;
    }
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.calc_rise_delay(input_transition, output_capacitance));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// calculate FALL delay
PyObject*
calc_fall_delay(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_transition",
    "output_capacitance",
    nullptr
  };
  PyObject* input_transition_obj = nullptr;
  PyObject* output_capacitance_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
                                    const_cast<char**>(kwlist),
                                    PyClibTime::_typeobject(), &input_transition_obj,
                                    PyClibCapacitance::_typeobject(), &output_capacitance_obj) ) {
    return nullptr;
  }
  ClibTime input_transition;
  if ( input_transition_obj != nullptr ) {
    if ( !PyClibTime::FromPyObject(input_transition_obj, input_transition) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibTime");
      return nullptr;
    }
  }
  ClibCapacitance output_capacitance;
  if ( output_capacitance_obj != nullptr ) {
    if ( !PyClibCapacitance::FromPyObject(output_capacitance_obj, output_capacitance) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibCapacitance");
      return nullptr;
    }
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.calc_fall_delay(input_transition, output_capacitance));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// calculate RISE transition
PyObject*
calc_rise_transition(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_transition",
    "output_capacitance",
    nullptr
  };
  PyObject* input_transition_obj = nullptr;
  PyObject* output_capacitance_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
                                    const_cast<char**>(kwlist),
                                    PyClibTime::_typeobject(), &input_transition_obj,
                                    PyClibCapacitance::_typeobject(), &output_capacitance_obj) ) {
    return nullptr;
  }
  ClibTime input_transition;
  if ( input_transition_obj != nullptr ) {
    if ( !PyClibTime::FromPyObject(input_transition_obj, input_transition) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibTime");
      return nullptr;
    }
  }
  ClibCapacitance output_capacitance;
  if ( output_capacitance_obj != nullptr ) {
    if ( !PyClibCapacitance::FromPyObject(output_capacitance_obj, output_capacitance) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibCapacitance");
      return nullptr;
    }
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.calc_rise_transition(input_transition, output_capacitance));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// calculate FALL transition
PyObject*
calc_fall_transition(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "input_transition",
    "output_capacitance",
    nullptr
  };
  PyObject* input_transition_obj = nullptr;
  PyObject* output_capacitance_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
                                    const_cast<char**>(kwlist),
                                    PyClibTime::_typeobject(), &input_transition_obj,
                                    PyClibCapacitance::_typeobject(), &output_capacitance_obj) ) {
    return nullptr;
  }
  ClibTime input_transition;
  if ( input_transition_obj != nullptr ) {
    if ( !PyClibTime::FromPyObject(input_transition_obj, input_transition) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibTime");
      return nullptr;
    }
  }
  ClibCapacitance output_capacitance;
  if ( output_capacitance_obj != nullptr ) {
    if ( !PyClibCapacitance::FromPyObject(output_capacitance_obj, output_capacitance) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to ClibCapacitance");
      return nullptr;
    }
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.calc_fall_transition(input_transition, output_capacitance));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return rise pin resistance
PyObject*
rise_pin_resistance(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "piece_id",
    nullptr
  };
  unsigned long piece_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &piece_id) ) {
    return nullptr;
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibResistance::ToPyObject(val.rise_pin_resistance(piece_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fall pin resistance
PyObject*
fall_pin_resistance(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "piece_id",
    nullptr
  };
  unsigned long piece_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &piece_id) ) {
    return nullptr;
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibResistance::ToPyObject(val.fall_pin_resistance(piece_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return rise delay intercept
PyObject*
rise_delay_intercept(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "piece_id",
    nullptr
  };
  unsigned long piece_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &piece_id) ) {
    return nullptr;
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.rise_delay_intercept(piece_id));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return fall delay intercept
PyObject*
fall_delay_intercept(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "piece_id",
    nullptr
  };
  unsigned long piece_id;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &piece_id) ) {
    return nullptr;
  }
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.fall_delay_intercept(piece_id));
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
  {"calc_rise_delay",
   reinterpret_cast<PyCFunction>(calc_rise_delay),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("calculate RISE delay")},
  {"calc_fall_delay",
   reinterpret_cast<PyCFunction>(calc_fall_delay),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("calculate FALL delay")},
  {"calc_rise_transition",
   reinterpret_cast<PyCFunction>(calc_rise_transition),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("calculate RISE transition")},
  {"calc_fall_transition",
   reinterpret_cast<PyCFunction>(calc_fall_transition),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("calculate FALL transition")},
  {"rise_pin_resistance",
   reinterpret_cast<PyCFunction>(rise_pin_resistance),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return rise pin resistance")},
  {"fall_pin_resistance",
   reinterpret_cast<PyCFunction>(fall_pin_resistance),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fall pin resistance")},
  {"rise_delay_intercept",
   reinterpret_cast<PyCFunction>(rise_delay_intercept),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return rise delay intercept")},
  {"fall_delay_intercept",
   reinterpret_cast<PyCFunction>(fall_delay_intercept),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return fall delay intercept")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTimingType::ToPyObject(val.type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_timing_cond(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyExpr::ToPyObject(val.timing_cond());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_intrinsic_rise(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.intrinsic_rise());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_intrinsic_fall(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.intrinsic_fall());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_slope_rise(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.slope_rise());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_slope_fall(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibTime::ToPyObject(val.slope_fall());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_rise_resistance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibResistance::ToPyObject(val.rise_resistance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_fall_resistance(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibResistance::ToPyObject(val.fall_resistance());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_rise_transition(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibLut::ToPyObject(val.rise_transition());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_fall_transition(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibLut::ToPyObject(val.fall_transition());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_rise_propagation(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibLut::ToPyObject(val.rise_propagation());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_fall_propagation(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibLut::ToPyObject(val.fall_propagation());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_rise(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibLut::ToPyObject(val.cell_rise());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_fall(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibTiming::_get_ref(self);
  try {
    return PyClibLut::ToPyObject(val.cell_fall());
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
  {"type", get_type, nullptr, PyDoc_STR("type(TimingType)"), nullptr},
  {"timing_cond", get_timing_cond, nullptr, PyDoc_STR("timing condition expression"), nullptr},
  {"intrinsic_rise", get_intrinsic_rise, nullptr, PyDoc_STR("intrinsic rise time"), nullptr},
  {"intrinsic_fall", get_intrinsic_fall, nullptr, PyDoc_STR("intrinsic fall time"), nullptr},
  {"slope_rise", get_slope_rise, nullptr, PyDoc_STR("slope rise time"), nullptr},
  {"slope_fall", get_slope_fall, nullptr, PyDoc_STR("slpe fall time"), nullptr},
  {"rise_resistance", get_rise_resistance, nullptr, PyDoc_STR("rise resistance"), nullptr},
  {"fall_resistance", get_fall_resistance, nullptr, PyDoc_STR("fall resistance"), nullptr},
  {"rise_transition", get_rise_transition, nullptr, PyDoc_STR("rise transition"), nullptr},
  {"fall_transition", get_fall_transition, nullptr, PyDoc_STR("fall transition"), nullptr},
  {"rise_propagation", get_rise_propagation, nullptr, PyDoc_STR("rise propagation"), nullptr},
  {"fall_propagation", get_fall_propagation, nullptr, PyDoc_STR("fall propagation"), nullptr},
  {"cell_rise", get_cell_rise, nullptr, PyDoc_STR("cell rise"), nullptr},
  {"cell_fall", get_cell_fall, nullptr, PyDoc_STR("cell fall"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibTiming オブジェクトを使用可能にする．
bool
PyClibTiming::init(
  PyObject* m
)
{
  ClibTiming_Type.tp_name = "ClibTiming";
  ClibTiming_Type.tp_basicsize = sizeof(ClibTiming_Object);
  ClibTiming_Type.tp_itemsize = 0;
  ClibTiming_Type.tp_dealloc = dealloc_func;
  ClibTiming_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibTiming_Type.tp_doc = PyDoc_STR("Python extended object for ClibTiming");
  ClibTiming_Type.tp_methods = methods;
  ClibTiming_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibTiming", &ClibTiming_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibTiming を PyObject に変換する．
PyObject*
PyClibTiming::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibTiming::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibTiming_Object*>(obj);
  new (&my_obj->mVal) ClibTiming(val);
  return obj;
}

// PyObject を ClibTiming に変換する．
bool
PyClibTiming::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibTiming::Check(obj) ) {
    val = PyClibTiming::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibTiming タイプか調べる．
bool
PyClibTiming::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibTiming_Type);
}

// @brief PyObject から ClibTiming を取り出す．
ClibTiming&
PyClibTiming::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibTiming_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibTiming を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibTiming::_typeobject()
{
  return &ClibTiming_Type;
}

END_NAMESPACE_YM
