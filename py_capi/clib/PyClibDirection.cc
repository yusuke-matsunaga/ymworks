
/// @file PyClibDirection.cc
/// @brief PyClibDirection の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibDirection.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibDirection_Object
{
  PyObject_HEAD
  ClibDirection mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibDirection_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibDirection::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_input = nullptr;
PyObject* Const_output = nullptr;
PyObject* Const_inout = nullptr;
PyObject* Const_internal = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibDirection val,
  PyObject*& const_obj
)
{
  auto type = PyClibDirection::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibDirection_Object*>(obj);
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
  auto& val = PyClibDirection::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibDirection::input: str_val = "input"; break;
      case ClibDirection::output: str_val = "output"; break;
      case ClibDirection::inout: str_val = "inout"; break;
      case ClibDirection::internal: str_val = "internal"; break;
      case ClibDirection::none: str_val = "None"; break;
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
  auto& val = PyClibDirection::_get_ref(self);
  try {
    if ( PyClibDirection::Check(self) && PyClibDirection::Check(other) ) {
      auto& val1 = PyClibDirection::_get_ref(self);
      auto& val2 = PyClibDirection::_get_ref(other);
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
  ClibDirection val;
  if ( val_obj != nullptr ) {
    if ( !PyClibDirection::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibDirection");
      return nullptr;
    }
  }
  try {
    return PyClibDirection::ToPyObject(val);
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


// @brief ClibDirection オブジェクトを使用可能にする．
bool
PyClibDirection::init(
  PyObject* m
)
{
  ClibDirection_Type.tp_name = "ClibDirection";
  ClibDirection_Type.tp_basicsize = sizeof(ClibDirection_Object);
  ClibDirection_Type.tp_itemsize = 0;
  ClibDirection_Type.tp_dealloc = dealloc_func;
  ClibDirection_Type.tp_repr = repr_func;
  ClibDirection_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibDirection_Type.tp_doc = PyDoc_STR("Python extended object for ClibDirection");
  ClibDirection_Type.tp_richcompare = richcompare_func;
  ClibDirection_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibDirection", &ClibDirection_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("input", ClibDirection::input, Const_input) ) {
    goto error;
  }
  if ( !reg_const_obj("output", ClibDirection::output, Const_output) ) {
    goto error;
  }
  if ( !reg_const_obj("inout", ClibDirection::inout, Const_inout) ) {
    goto error;
  }
  if ( !reg_const_obj("internal", ClibDirection::internal, Const_internal) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibDirection を PyObject に変換する．
PyObject*
PyClibDirection::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibDirection::input: obj = Const_input; break;
    case ClibDirection::output: obj = Const_output; break;
    case ClibDirection::inout: obj = Const_inout; break;
    case ClibDirection::internal: obj = Const_internal; break;
    case ClibDirection::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibDirection");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibDirection に変換する．
bool
PyClibDirection::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibDirection::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "input") == 0 ) {
      val = ClibDirection::input;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "output") == 0 ) {
      val = ClibDirection::output;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "inout") == 0 ) {
      val = ClibDirection::inout;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "internal") == 0 ) {
      val = ClibDirection::internal;
      return true;
    }
    return false;
  }
  if ( PyClibDirection::Check(obj) ) {
    val = PyClibDirection::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibDirection タイプか調べる．
bool
PyClibDirection::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibDirection_Type);
}

// @brief PyObject から ClibDirection を取り出す．
ClibDirection&
PyClibDirection::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibDirection_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibDirection を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibDirection::_typeobject()
{
  return &ClibDirection_Type;
}

END_NAMESPACE_YM
