
/// @file PyClibTimingSense.cc
/// @brief PyClibTimingSense の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibTimingSense.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibTimingSense_Object
{
  PyObject_HEAD
  ClibTimingSense mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibTimingSense_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibTimingSense::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_positive_unate = nullptr;
PyObject* Const_negative_unate = nullptr;
PyObject* Const_non_unate = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibTimingSense val,
  PyObject*& const_obj
)
{
  auto type = PyClibTimingSense::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibTimingSense_Object*>(obj);
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
  auto& val = PyClibTimingSense::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibTimingSense::positive_unate: str_val = "positive_unate"; break;
      case ClibTimingSense::negative_unate: str_val = "negative_unate"; break;
      case ClibTimingSense::non_unate: str_val = "non_unate"; break;
      case ClibTimingSense::none: str_val = "None"; break;
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
  auto& val = PyClibTimingSense::_get_ref(self);
  try {
    if ( PyClibTimingSense::Check(self) && PyClibTimingSense::Check(other) ) {
      auto& val1 = PyClibTimingSense::_get_ref(self);
      auto& val2 = PyClibTimingSense::_get_ref(other);
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
  ClibTimingSense val;
  if ( val_obj != nullptr ) {
    if ( !PyClibTimingSense::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibTimingSense");
      return nullptr;
    }
  }
  try {
    return PyClibTimingSense::ToPyObject(val);
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


// @brief ClibTimingSense オブジェクトを使用可能にする．
bool
PyClibTimingSense::init(
  PyObject* m
)
{
  ClibTimingSense_Type.tp_name = "ClibTimingSense";
  ClibTimingSense_Type.tp_basicsize = sizeof(ClibTimingSense_Object);
  ClibTimingSense_Type.tp_itemsize = 0;
  ClibTimingSense_Type.tp_dealloc = dealloc_func;
  ClibTimingSense_Type.tp_repr = repr_func;
  ClibTimingSense_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibTimingSense_Type.tp_doc = PyDoc_STR("Python extended object for ClibTimingSense");
  ClibTimingSense_Type.tp_richcompare = richcompare_func;
  ClibTimingSense_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibTimingSense", &ClibTimingSense_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("positive_unate", ClibTimingSense::positive_unate, Const_positive_unate) ) {
    goto error;
  }
  if ( !reg_const_obj("negative_unate", ClibTimingSense::negative_unate, Const_negative_unate) ) {
    goto error;
  }
  if ( !reg_const_obj("non_unate", ClibTimingSense::non_unate, Const_non_unate) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibTimingSense を PyObject に変換する．
PyObject*
PyClibTimingSense::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibTimingSense::positive_unate: obj = Const_positive_unate; break;
    case ClibTimingSense::negative_unate: obj = Const_negative_unate; break;
    case ClibTimingSense::non_unate: obj = Const_non_unate; break;
    case ClibTimingSense::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibTimingSense");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibTimingSense に変換する．
bool
PyClibTimingSense::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibTimingSense::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "positive_unate") == 0 ) {
      val = ClibTimingSense::positive_unate;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "negative_unate") == 0 ) {
      val = ClibTimingSense::negative_unate;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "non_unate") == 0 ) {
      val = ClibTimingSense::non_unate;
      return true;
    }
    return false;
  }
  if ( PyClibTimingSense::Check(obj) ) {
    val = PyClibTimingSense::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibTimingSense タイプか調べる．
bool
PyClibTimingSense::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibTimingSense_Type);
}

// @brief PyObject から ClibTimingSense を取り出す．
ClibTimingSense&
PyClibTimingSense::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibTimingSense_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibTimingSense を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibTimingSense::_typeobject()
{
  return &ClibTimingSense_Type;
}

END_NAMESPACE_YM
