
/// @file PyClibCPV.cc
/// @brief PyClibCPV の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCPV.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibCPV_Object
{
  PyObject_HEAD
  ClibCPV mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCPV_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCPV::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_L = nullptr;
PyObject* Const_H = nullptr;
PyObject* Const_N = nullptr;
PyObject* Const_T = nullptr;
PyObject* Const_X = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibCPV val,
  PyObject*& const_obj
)
{
  auto type = PyClibCPV::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCPV_Object*>(obj);
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
  auto& val = PyClibCPV::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibCPV::L: str_val = "L"; break;
      case ClibCPV::H: str_val = "H"; break;
      case ClibCPV::N: str_val = "N"; break;
      case ClibCPV::T: str_val = "T"; break;
      case ClibCPV::X: str_val = "X"; break;
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
  auto& val = PyClibCPV::_get_ref(self);
  try {
    if ( PyClibCPV::Check(self) && PyClibCPV::Check(other) ) {
      auto& val1 = PyClibCPV::_get_ref(self);
      auto& val2 = PyClibCPV::_get_ref(other);
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
  ClibCPV val;
  if ( val_obj != nullptr ) {
    if ( !PyClibCPV::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibCPV");
      return nullptr;
    }
  }
  try {
    return PyClibCPV::ToPyObject(val);
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


// @brief ClibCPV オブジェクトを使用可能にする．
bool
PyClibCPV::init(
  PyObject* m
)
{
  ClibCPV_Type.tp_name = "ClibCPV";
  ClibCPV_Type.tp_basicsize = sizeof(ClibCPV_Object);
  ClibCPV_Type.tp_itemsize = 0;
  ClibCPV_Type.tp_dealloc = dealloc_func;
  ClibCPV_Type.tp_repr = repr_func;
  ClibCPV_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCPV_Type.tp_doc = PyDoc_STR("Python extended object for ClibCPV");
  ClibCPV_Type.tp_richcompare = richcompare_func;
  ClibCPV_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibCPV", &ClibCPV_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("L", ClibCPV::L, Const_L) ) {
    goto error;
  }
  if ( !reg_const_obj("H", ClibCPV::H, Const_H) ) {
    goto error;
  }
  if ( !reg_const_obj("N", ClibCPV::N, Const_N) ) {
    goto error;
  }
  if ( !reg_const_obj("T", ClibCPV::T, Const_T) ) {
    goto error;
  }
  if ( !reg_const_obj("X", ClibCPV::X, Const_X) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCPV を PyObject に変換する．
PyObject*
PyClibCPV::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibCPV::L: obj = Const_L; break;
    case ClibCPV::H: obj = Const_H; break;
    case ClibCPV::N: obj = Const_N; break;
    case ClibCPV::T: obj = Const_T; break;
    case ClibCPV::X: obj = Const_X; break;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibCPV");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibCPV に変換する．
bool
PyClibCPV::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "L") == 0 ) {
      val = ClibCPV::L;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "H") == 0 ) {
      val = ClibCPV::H;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "N") == 0 ) {
      val = ClibCPV::N;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "T") == 0 ) {
      val = ClibCPV::T;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "X") == 0 ) {
      val = ClibCPV::X;
      return true;
    }
    return false;
  }
  if ( PyClibCPV::Check(obj) ) {
    val = PyClibCPV::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCPV タイプか調べる．
bool
PyClibCPV::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCPV_Type);
}

// @brief PyObject から ClibCPV を取り出す．
ClibCPV&
PyClibCPV::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCPV_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCPV を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCPV::_typeobject()
{
  return &ClibCPV_Type;
}

END_NAMESPACE_YM
