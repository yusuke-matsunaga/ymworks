
/// @file PyFraigMgr.cc
/// @brief PyFraigMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyFraigMgr.h"
#include "pym/PyFraigLiteral.h"
#include "pym/PySatBool3.h"
#include "pym/PySatInitParam.h"
#include "pym/PyUlong.h"
#include "pym/PyList.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct FraigMgr_Object
{
  PyObject_HEAD
  FraigMgr mVal;
};

// Python 用のタイプ定義
PyTypeObject FraigMgr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyFraigMgr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<FraigMgr_Object*>(self);
  obj->mVal.~FraigMgr();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
new_input(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyFraigMgr::_get_ref(self);
  try {
    return PyFraigLiteral::ToPyObject(val.new_input());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  std::vector<FraigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<FraigLiteral, PyFraigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<FraigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyFraigMgr::_get_ref(self);
  try {
    return PyFraigLiteral::ToPyObject(val.new_and(fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  std::vector<FraigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<FraigLiteral, PyFraigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<FraigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyFraigMgr::_get_ref(self);
  try {
    return PyFraigLiteral::ToPyObject(val.new_or(fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

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
  std::vector<FraigLiteral> fanin_list;
  if ( fanin_list_obj != nullptr ) {
    if ( !PyList<FraigLiteral, PyFraigLiteral>::FromPyObject(fanin_list_obj, fanin_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<FraigLiteral>");
      return nullptr;
    }
  }
  auto& val = PyFraigMgr::_get_ref(self);
  try {
    return PyFraigLiteral::ToPyObject(val.new_xor(fanin_list));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
check_equiv(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "lit1",
    "lit2",
    nullptr
  };
  PyObject* lit1_obj = nullptr;
  PyObject* lit2_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!O!",
                                    const_cast<char**>(kwlist),
                                    PyFraigLiteral::_typeobject(), &lit1_obj,
                                    PyFraigLiteral::_typeobject(), &lit2_obj) ) {
    return nullptr;
  }
  FraigLiteral lit1;
  if ( lit1_obj != nullptr ) {
    if ( !PyFraigLiteral::FromPyObject(lit1_obj, lit1) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to FraigLiteral");
      return nullptr;
    }
  }
  FraigLiteral lit2;
  if ( lit2_obj != nullptr ) {
    if ( !PyFraigLiteral::FromPyObject(lit2_obj, lit2) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to FraigLiteral");
      return nullptr;
    }
  }
  auto& val = PyFraigMgr::_get_ref(self);
  try {
    return PySatBool3::ToPyObject(val.check_equiv(lit1, lit2));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
set_loglevel(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "level",
    nullptr
  };
  unsigned long level;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &level) ) {
    return nullptr;
  }
  auto& val = PyFraigMgr::_get_ref(self);
  try {
    val.set_loglevel(level);
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
  {"new_input",
   new_input,
   METH_NOARGS,
   PyDoc_STR("make new Input")},
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
  {"check_equiv",
   reinterpret_cast<PyCFunction>(check_equiv),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("do EQUIVALENCE CHECKING")},
  {"set_loglevel",
   reinterpret_cast<PyCFunction>(set_loglevel),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("set log level")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
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
    "sig_size",
    "init_param",
    nullptr
  };
  unsigned long sig_size;
  PyObject* init_param_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "$k|O!",
                                    const_cast<char**>(kwlist),
                                    &sig_size,
                                    PySatInitParam::_typeobject(), &init_param_obj) ) {
    return nullptr;
  }
  SatInitParam init_param;
  if ( init_param_obj != nullptr ) {
    if ( !PySatInitParam::FromPyObject(init_param_obj, init_param) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to SatInitParam");
      return nullptr;
    }
  }
  try {
    auto type = PyFraigMgr::_typeobject();
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<FraigMgr_Object*>(self);
    new (&my_obj->mVal) FraigMgr(sig_size, init_param);
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


// @brief FraigMgr オブジェクトを使用可能にする．
bool
PyFraigMgr::init(
  PyObject* m
)
{
  FraigMgr_Type.tp_name = "FraigMgr";
  FraigMgr_Type.tp_basicsize = sizeof(FraigMgr_Object);
  FraigMgr_Type.tp_itemsize = 0;
  FraigMgr_Type.tp_dealloc = dealloc_func;
  FraigMgr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  FraigMgr_Type.tp_doc = PyDoc_STR("Python extended object for FraigMgr");
  FraigMgr_Type.tp_methods = methods;
  FraigMgr_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "FraigMgr", &FraigMgr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// @brief PyObject が FraigMgr タイプか調べる．
bool
PyFraigMgr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &FraigMgr_Type);
}

// @brief PyObject から FraigMgr を取り出す．
FraigMgr&
PyFraigMgr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<FraigMgr_Object*>(obj);
  return my_obj->mVal;
}

// @brief FraigMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyFraigMgr::_typeobject()
{
  return &FraigMgr_Type;
}

END_NAMESPACE_YM
