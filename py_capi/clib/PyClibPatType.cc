
/// @file PyClibPatType.cc
/// @brief PyClibPatType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibPatType.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibPatType_Object
{
  PyObject_HEAD
  ClibPatType mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibPatType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibPatType::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_Input = nullptr;
PyObject* Const_And = nullptr;
PyObject* Const_Xor = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibPatType val,
  PyObject*& const_obj
)
{
  auto type = PyClibPatType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibPatType_Object*>(obj);
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
  auto& val = PyClibPatType::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibPatType::Input: str_val = "Input"; break;
      case ClibPatType::And: str_val = "And"; break;
      case ClibPatType::Xor: str_val = "Xor"; break;
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
  auto& val = PyClibPatType::_get_ref(self);
  try {
    if ( PyClibPatType::Check(self) && PyClibPatType::Check(other) ) {
      auto& val1 = PyClibPatType::_get_ref(self);
      auto& val2 = PyClibPatType::_get_ref(other);
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
  ClibPatType val;
  if ( val_obj != nullptr ) {
    if ( !PyClibPatType::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibPatType");
      return nullptr;
    }
  }
  try {
    return PyClibPatType::ToPyObject(val);
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


// @brief ClibPatType オブジェクトを使用可能にする．
bool
PyClibPatType::init(
  PyObject* m
)
{
  ClibPatType_Type.tp_name = "ClibPatType";
  ClibPatType_Type.tp_basicsize = sizeof(ClibPatType_Object);
  ClibPatType_Type.tp_itemsize = 0;
  ClibPatType_Type.tp_dealloc = dealloc_func;
  ClibPatType_Type.tp_repr = repr_func;
  ClibPatType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibPatType_Type.tp_doc = PyDoc_STR("Python extended object for ClibPatType");
  ClibPatType_Type.tp_richcompare = richcompare_func;
  ClibPatType_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibPatType", &ClibPatType_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("Input", ClibPatType::Input, Const_Input) ) {
    goto error;
  }
  if ( !reg_const_obj("And", ClibPatType::And, Const_And) ) {
    goto error;
  }
  if ( !reg_const_obj("Xor", ClibPatType::Xor, Const_Xor) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibPatType を PyObject に変換する．
PyObject*
PyClibPatType::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibPatType::Input: obj = Const_Input; break;
    case ClibPatType::And: obj = Const_And; break;
    case ClibPatType::Xor: obj = Const_Xor; break;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibPatType");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibPatType に変換する．
bool
PyClibPatType::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "Input") == 0 ) {
      val = ClibPatType::Input;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "And") == 0 ) {
      val = ClibPatType::And;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Xor") == 0 ) {
      val = ClibPatType::Xor;
      return true;
    }
    return false;
  }
  if ( PyClibPatType::Check(obj) ) {
    val = PyClibPatType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibPatType タイプか調べる．
bool
PyClibPatType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibPatType_Type);
}

// @brief PyObject から ClibPatType を取り出す．
ClibPatType&
PyClibPatType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibPatType_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibPatType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibPatType::_typeobject()
{
  return &ClibPatType_Type;
}

END_NAMESPACE_YM
