
/// @file PyBddVarSet.cc
/// @brief PyBddVarSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddVarSet.h"
#include "pym/PyBddVar.h"
#include "pym/PyBdd.h"
#include "pym/PyBddMgr.h"
#include "pym/PyList.h"
#include "pym/PyLong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddVarSet_Object
{
  PyObject_HEAD
  BddVarSet mVal;
};

// Python 用のタイプ定義
PyTypeObject BddVarSet_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddVarSet::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddVarSet_Object*>(self);
  obj->mVal.~BddVarSet();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
nb_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 + val2);
      }
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 + val2);
      }
    }
    if ( PyBddVarSet::Check(other) ) {
      auto& val2 = PyBddVarSet::_get_ref(other);
      if ( PyBddVar::Check(self) ) {
        auto& val1 = PyBddVar::_get_ref(self);
        return PyBddVarSet::ToPyObject(val1 + val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 - val2);
      }
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 - val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 & val2);
      }
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        return PyBddVarSet::ToPyObject(val1 & val2);
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_add(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        val1 += val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        val1 += val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_subtract(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        val1 -= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        val1 -= val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
nb_inplace_and(
  PyObject* self,
  PyObject* other
)
{
  try {
    if ( PyBddVarSet::Check(self) ) {
      auto& val1 = PyBddVarSet::_get_ref(self);
      if ( PyBddVarSet::Check(other) ) {
        auto& val2 = PyBddVarSet::_get_ref(other);
        val1 &= val2;
        Py_XINCREF(self);
        return self;
      }
      if ( PyBddVar::Check(other) ) {
        auto& val2 = PyBddVar::_get_ref(other);
        val1 &= val2;
        Py_XINCREF(self);
        return self;
      }
    }
    Py_RETURN_NOTIMPLEMENTED;
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// Numberオブジェクト構造体
PyNumberMethods number = {
  .nb_add = nb_add,
  .nb_subtract = nb_subtract,
  .nb_and = nb_and,
  .nb_inplace_add = nb_inplace_add,
  .nb_inplace_subtract = nb_inplace_subtract,
  .nb_inplace_and = nb_inplace_and
};

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  try {
    if ( PyBddVarSet::Check(other) ) {
      auto& val2 = PyBddVarSet::_get_ref(other);
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
is_empty_set(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_empty_set());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
size(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  try {
    return PyLong::ToPyObject(val.size());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
var_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  try {
    return PyList<BddVar, PyBddVar>::ToPyObject(val.var_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
bdd(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyBddVarSet::_get_ref(self);
  try {
    return PyBdd::ToPyObject(val.bdd());
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
  {"is_empty_set",
   is_empty_set,
   METH_NOARGS,
   PyDoc_STR("True if empty set")},
  {"size",
   size,
   METH_NOARGS,
   PyDoc_STR("return size")},
  {"var_list",
   var_list,
   METH_NOARGS,
   PyDoc_STR("convert to list of 'BddVar's")},
  {"bdd",
   bdd,
   METH_NOARGS,
   PyDoc_STR("")},
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
    "arg",
    nullptr
  };
  PyObject* arg_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O",
                                    const_cast<char**>(kwlist),
                                    &arg_obj) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<BddVarSet_Object*>(self);
    if ( arg_obj == nullptr ) {
      new (&my_obj->mVal) BddVarSet();
    }
    else if ( PyBddVar::Check(arg_obj) ) {
      auto& var = PyBddVar::_get_ref(arg_obj);
      new (&my_obj->mVal) BddVarSet(var);
    }
    else if ( PyList<BddVar, PyBddVar>::Check(arg_obj) ) {
      auto var_list = PyList<BddVar, PyBddVar>::Get(arg_obj);
      new (&my_obj->mVal) BddVarSet(var_list);
    }
    else {
      PyErr_SetString(PyExc_TypeError, "argument 2 should be a sequence of 'BddVar'");
      return nullptr;
    }
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


// @brief BddVarSet オブジェクトを使用可能にする．
bool
PyBddVarSet::init(
  PyObject* m
)
{
  BddVarSet_Type.tp_name = "BddVarSet";
  BddVarSet_Type.tp_basicsize = sizeof(BddVarSet_Object);
  BddVarSet_Type.tp_itemsize = 0;
  BddVarSet_Type.tp_dealloc = dealloc_func;
  BddVarSet_Type.tp_as_number = &number;
  BddVarSet_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddVarSet_Type.tp_doc = PyDoc_STR("Python extended object for BddVarSet");
  BddVarSet_Type.tp_richcompare = richcompare_func;
  BddVarSet_Type.tp_methods = methods;
  BddVarSet_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddVarSet", &BddVarSet_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BddVarSet を PyObject に変換する．
PyObject*
PyBddVarSet::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBddVarSet::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BddVarSet_Object*>(obj);
  new (&my_obj->mVal) BddVarSet(val);
  return obj;
}

// PyObject を BddVarSet に変換する．
bool
PyBddVarSet::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddVarSet::Check(obj) ) {
    val = PyBddVarSet::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BddVarSet タイプか調べる．
bool
PyBddVarSet::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddVarSet_Type);
}

// @brief PyObject から BddVarSet を取り出す．
BddVarSet&
PyBddVarSet::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddVarSet_Object*>(obj);
  return my_obj->mVal;
}

// @brief BddVarSet を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddVarSet::_typeobject()
{
  return &BddVarSet_Type;
}

END_NAMESPACE_YM
