
/// @file PyBddMgr.cc
/// @brief PyBddMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddMgr.h"
#include "pym/PyBdd.h"
#include "pym/PyBddVar.h"
#include "pym/PyBddLit.h"
#include "pym/PyExpr.h"
#include "pym/PyLong.h"
#include "pym/PyJsonValue.h"
#include "pym/PyList.h"
#include "ym/BddVar.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddMgr_Object
{
  PyObject_HEAD
  BddMgr mVal;
};

// Python 用のタイプ定義
PyTypeObject BddMgr_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddMgr::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddMgr_Object*>(self);
  obj->mVal.~BddMgr();
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
  auto& val = PyBddMgr::_get_ref(self);
  try {
    if ( PyBddMgr::Check(other) ) {
      auto& val2 = PyBddMgr::_get_ref(other);
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

// True if valid mgr
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
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

// make a variable
PyObject*
variable(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBddVar::ToPyObject(val.variable(varid));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make a variable BDD
PyObject*
variable_bdd(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.variable_bdd(varid));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
literal(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    "inv",
    nullptr
  };
  int varid;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$p",
                                    const_cast<char**>(kwlist),
                                    &varid,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(val.literal(varid, inv));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
literal_bdd(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    "inv",
    nullptr
  };
  int varid;
  int inv_tmp = -1;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i|$p",
                                    const_cast<char**>(kwlist),
                                    &varid,
                                    &inv_tmp) ) {
    return nullptr;
  }
  bool inv;
  if ( inv_tmp != -1 ) {
    inv = static_cast<bool>(inv_tmp);
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.literal_bdd(varid, inv));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
positive_literal(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(val.positive_literal(varid));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
positive_literal_bdd(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.positive_literal_bdd(varid));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
negative_literal(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBddLit::ToPyObject(val.negative_literal(varid));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

//
PyObject*
negative_literal_bdd(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "varid",
    nullptr
  };
  int varid;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "i",
                                    const_cast<char**>(kwlist),
                                    &varid) ) {
    return nullptr;
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.negative_literal_bdd(varid));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make a copy of BDD
PyObject*
copy(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "src",
    nullptr
  };
  PyObject* src_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!",
                                    const_cast<char**>(kwlist),
                                    PyBdd::_typeobject(), &src_obj) ) {
    return nullptr;
  }
  Bdd src;
  if ( src_obj != nullptr ) {
    if ( !PyBdd::FromPyObject(src_obj, src) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Bdd");
      return nullptr;
    }
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.copy(src));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make a BDD from truth table
PyObject*
from_truth(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "func_str",
    "var_list",
    nullptr
  };
  const char* func_str_tmp = nullptr;
  PyObject* var_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s|$O",
                                    const_cast<char**>(kwlist),
                                    &func_str_tmp,
                                    &var_list_obj) ) {
    return nullptr;
  }
  std::string func_str;
  if ( func_str_tmp != nullptr ) {
    func_str = std::string(func_str_tmp);
  }
  std::vector<BddVar> var_list;
  if ( var_list_obj != nullptr ) {
    if ( !PyList<BddVar, PyBddVar>::FromPyObject(var_list_obj, var_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BddVar>");
      return nullptr;
    }
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    if ( var_list.empty() ) {
      return PyBdd::ToPyObject(val.from_truth(func_str));
    }
    else {
      return PyBdd::ToPyObject(val.from_truth(func_str, var_list));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make a BDD from Expr
PyObject*
from_expr(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "expr",
    "var_list",
    nullptr
  };
  PyObject* expr_obj = nullptr;
  PyObject* var_list_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O!|$O",
                                    const_cast<char**>(kwlist),
                                    PyExpr::_typeobject(), &expr_obj,
                                    &var_list_obj) ) {
    return nullptr;
  }
  Expr expr;
  if ( expr_obj != nullptr ) {
    if ( !PyExpr::FromPyObject(expr_obj, expr) ) {
      PyErr_SetString(PyExc_TypeError, "could not convert to Expr");
      return nullptr;
    }
  }
  std::vector<BddVar> var_list;
  if ( var_list_obj != nullptr ) {
    if ( !PyList<BddVar, PyBddVar>::FromPyObject(var_list_obj, var_list) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to std::vector<BddVar>");
      return nullptr;
    }
  }
  auto& val = PyBddMgr::_get_ref(self);
  try {
    if ( var_list.empty() ) {
      return PyBdd::ToPyObject(val.from_expr(expr));
    }
    else {
      return PyBdd::ToPyObject(val.from_expr(expr, var_list));
    }
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// enable GC
PyObject*
enable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  try {
    val.enable_gc();
    Py_RETURN_NONE;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// disable GC
PyObject*
disable_gc(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  try {
    val.enable_gc();
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
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid mgr")},
  {"variable",
   reinterpret_cast<PyCFunction>(variable),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a variable")},
  {"variable_bdd",
   reinterpret_cast<PyCFunction>(variable_bdd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a variable BDD")},
  {"literal",
   reinterpret_cast<PyCFunction>(literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"literal_bdd",
   reinterpret_cast<PyCFunction>(literal_bdd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"positive_literal",
   reinterpret_cast<PyCFunction>(positive_literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"positive_literal_bdd",
   reinterpret_cast<PyCFunction>(positive_literal_bdd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"negative_literal",
   reinterpret_cast<PyCFunction>(negative_literal),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"negative_literal_bdd",
   reinterpret_cast<PyCFunction>(negative_literal_bdd),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {"copy",
   reinterpret_cast<PyCFunction>(copy),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a copy of BDD")},
  {"from_truth",
   reinterpret_cast<PyCFunction>(from_truth),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a BDD from truth table")},
  {"from_expr",
   reinterpret_cast<PyCFunction>(from_expr),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("make a BDD from Expr")},
  {"enable_gc",
   enable_gc,
   METH_NOARGS,
   PyDoc_STR("enable GC")},
  {"disable_gc",
   disable_gc,
   METH_NOARGS,
   PyDoc_STR("disable GC")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_node_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyLong::ToPyObject(val.node_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_gc_limit(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  try {
    return PyLong::ToPyObject(val.gc_limit());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

int
set_gc_limit(
  PyObject* self,
  PyObject* obj,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyBddMgr::_get_ref(self);
  try {
    if ( !PyLong::Check(obj) ) {
      PyErr_SetString(PyExc_TypeError, "integer type is expected");
      return -1;
    }
    auto gc_limit = PyLong::Get(obj);
    val.set_gc_limit(gc_limit);
    return 0;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return -1;
  }
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"node_num", get_node_num, nullptr, PyDoc_STR("number of nodes"), nullptr},
  {"gc_limit", get_gc_limit, set_gc_limit, PyDoc_STR("garbage collection limit"), nullptr},
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
    auto my_obj = reinterpret_cast<BddMgr_Object*>(self);
    new (&my_obj->mVal) BddMgr();
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


// @brief BddMgr オブジェクトを使用可能にする．
bool
PyBddMgr::init(
  PyObject* m
)
{
  BddMgr_Type.tp_name = "BddMgr";
  BddMgr_Type.tp_basicsize = sizeof(BddMgr_Object);
  BddMgr_Type.tp_itemsize = 0;
  BddMgr_Type.tp_dealloc = dealloc_func;
  BddMgr_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddMgr_Type.tp_doc = PyDoc_STR("Python extended object for BddMgr");
  BddMgr_Type.tp_richcompare = richcompare_func;
  BddMgr_Type.tp_methods = methods;
  BddMgr_Type.tp_getset = getsets;
  BddMgr_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddMgr", &BddMgr_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddMgr を PyObject に変換する．
PyObject*
PyBddMgr::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBddMgr::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BddMgr_Object*>(obj);
  new (&my_obj->mVal) BddMgr(val);
  return obj;
}

// PyObject を BddMgr に変換する．
bool
PyBddMgr::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddMgr::Check(obj) ) {
    val = PyBddMgr::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddMgr タイプか調べる．
bool
PyBddMgr::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddMgr_Type);
}

// @brief PyObject から BddMgr を取り出す．
BddMgr&
PyBddMgr::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddMgr_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddMgr を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddMgr::_typeobject()
{
  return &BddMgr_Type;
}

END_NAMESPACE_YM
