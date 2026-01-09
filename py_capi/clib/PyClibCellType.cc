
/// @file PyClibCellType.cc
/// @brief PyClibCellType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCellType.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibCellType_Object
{
  PyObject_HEAD
  ClibCellType mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCellType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCellType::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_Logic = nullptr;
PyObject* Const_FF = nullptr;
PyObject* Const_Latch = nullptr;
PyObject* Const_FSM = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibCellType val,
  PyObject*& const_obj
)
{
  auto type = PyClibCellType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCellType_Object*>(obj);
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
  auto& val = PyClibCellType::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibCellType::Logic: str_val = "Logic"; break;
      case ClibCellType::FF: str_val = "FF"; break;
      case ClibCellType::Latch: str_val = "Latch"; break;
      case ClibCellType::FSM: str_val = "FSM"; break;
      case ClibCellType::none: str_val = "None"; break;
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
  auto& val = PyClibCellType::_get_ref(self);
  try {
    if ( PyClibCellType::Check(self) && PyClibCellType::Check(other) ) {
      auto& val1 = PyClibCellType::_get_ref(self);
      auto& val2 = PyClibCellType::_get_ref(other);
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
  ClibCellType val;
  if ( val_obj != nullptr ) {
    if ( !PyClibCellType::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibCellType");
      return nullptr;
    }
  }
  try {
    return PyClibCellType::ToPyObject(val);
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


// @brief ClibCellType オブジェクトを使用可能にする．
bool
PyClibCellType::init(
  PyObject* m
)
{
  ClibCellType_Type.tp_name = "ClibCellType";
  ClibCellType_Type.tp_basicsize = sizeof(ClibCellType_Object);
  ClibCellType_Type.tp_itemsize = 0;
  ClibCellType_Type.tp_dealloc = dealloc_func;
  ClibCellType_Type.tp_repr = repr_func;
  ClibCellType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCellType_Type.tp_doc = PyDoc_STR("Python extended object for ClibCellType");
  ClibCellType_Type.tp_richcompare = richcompare_func;
  ClibCellType_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibCellType", &ClibCellType_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("Logic", ClibCellType::Logic, Const_Logic) ) {
    goto error;
  }
  if ( !reg_const_obj("FF", ClibCellType::FF, Const_FF) ) {
    goto error;
  }
  if ( !reg_const_obj("Latch", ClibCellType::Latch, Const_Latch) ) {
    goto error;
  }
  if ( !reg_const_obj("FSM", ClibCellType::FSM, Const_FSM) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCellType を PyObject に変換する．
PyObject*
PyClibCellType::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibCellType::Logic: obj = Const_Logic; break;
    case ClibCellType::FF: obj = Const_FF; break;
    case ClibCellType::Latch: obj = Const_Latch; break;
    case ClibCellType::FSM: obj = Const_FSM; break;
    case ClibCellType::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibCellType");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibCellType に変換する．
bool
PyClibCellType::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibCellType::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "Logic") == 0 ) {
      val = ClibCellType::Logic;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "FF") == 0 ) {
      val = ClibCellType::FF;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "Latch") == 0 ) {
      val = ClibCellType::Latch;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "FSM") == 0 ) {
      val = ClibCellType::FSM;
      return true;
    }
    return false;
  }
  if ( PyClibCellType::Check(obj) ) {
    val = PyClibCellType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCellType タイプか調べる．
bool
PyClibCellType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCellType_Type);
}

// @brief PyObject から ClibCellType を取り出す．
ClibCellType&
PyClibCellType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCellType_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCellType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCellType::_typeobject()
{
  return &ClibCellType_Type;
}

END_NAMESPACE_YM
