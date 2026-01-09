
/// @file PyClibTechnology.cc
/// @brief PyClibTechnology の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibTechnology.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibTechnology_Object
{
  PyObject_HEAD
  ClibTechnology mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibTechnology_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibTechnology::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_cmos = nullptr;
PyObject* Const_fpga = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibTechnology val,
  PyObject*& const_obj
)
{
  auto type = PyClibTechnology::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibTechnology_Object*>(obj);
  my_obj->mVal = val;
  if ( PyDict_SetItemString(type->tp_dict, name, obj) < 0 ) {
    return false;
  }
  Py_INCREF(obj);
  const_obj = obj;
  return true;
}

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  Py_TYPE(self)->tp_free(self);
}

// repr 関数
PyObject*
repr_func(
  PyObject* self
)
{
  auto& val = PyClibTechnology::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibTechnology::cmos: str_val = "cmos"; break;
      case ClibTechnology::fpga: str_val = "fpga"; break;
      case ClibTechnology::none: str_val = "None"; break;
    }
    return PyString::ToPyObject(str_val);
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyClibTechnology::_get_ref(self);
  try {
    if ( PyClibTechnology::Check(self) && PyClibTechnology::Check(other) ) {
      auto& val1 = PyClibTechnology::_get_ref(self);
      auto& val2 = PyClibTechnology::_get_ref(other);
      if ( op == Py_EQ ) {
        return PyBool_FromLong(val1 == val2);
      }
      if ( op == Py_NE ) {
        return PyBool_FromLong(val1 != val2);
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

// new 関数
PyObject*
new_func(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "",
    nullptr
  };
  PyObject* val_obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "O",
                                    const_cast<char**>(kwlist),
                                    &val_obj) ) {
    return nullptr;
  }
  ClibTechnology val;
  if ( val_obj != nullptr ) {
    if ( !PyClibTechnology::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibTechnology");
      return nullptr;
    }
  }
  try {
    return PyClibTechnology::ToPyObject(val);
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


// @brief ClibTechnology オブジェクトを使用可能にする．
bool
PyClibTechnology::init(
  PyObject* m
)
{
  ClibTechnology_Type.tp_name = "ClibTechnology";
  ClibTechnology_Type.tp_basicsize = sizeof(ClibTechnology_Object);
  ClibTechnology_Type.tp_itemsize = 0;
  ClibTechnology_Type.tp_dealloc = dealloc_func;
  ClibTechnology_Type.tp_repr = repr_func;
  ClibTechnology_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibTechnology_Type.tp_doc = PyDoc_STR("Python extended object for ClibTechnology");
  ClibTechnology_Type.tp_richcompare = richcompare_func;
  ClibTechnology_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibTechnology", &ClibTechnology_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("cmos", ClibTechnology::cmos, Const_cmos) ) {
    goto error;
  }
  if ( !reg_const_obj("fpga", ClibTechnology::fpga, Const_fpga) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibTechnology を PyObject に変換する．
PyObject*
PyClibTechnology::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibTechnology::cmos: obj = Const_cmos; break;
    case ClibTechnology::fpga: obj = Const_fpga; break;
    case ClibTechnology::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibTechnology");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibTechnology に変換する．
bool
PyClibTechnology::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibTechnology::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "cmos") == 0 ) {
      val = ClibTechnology::cmos;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "fpga") == 0 ) {
      val = ClibTechnology::fpga;
      return true;
    }
    return false;
  }
  if ( PyClibTechnology::Check(obj) ) {
    val = PyClibTechnology::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibTechnology タイプか調べる．
bool
PyClibTechnology::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibTechnology_Type);
}

// @brief PyObject から ClibTechnology を取り出す．
ClibTechnology&
PyClibTechnology::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibTechnology_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibTechnology を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibTechnology::_typeobject()
{
  return &ClibTechnology_Type;
}

END_NAMESPACE_YM
