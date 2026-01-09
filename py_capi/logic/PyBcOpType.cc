
/// @file PyBcOpType.cc
/// @brief PyBcOpType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBcOpType.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BcOpType_Object
{
  PyObject_HEAD
  BcOpType mVal;
};

// Python 用のタイプ定義
PyTypeObject BcOpType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBcOpType::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_AND = nullptr;
PyObject* Const_OR = nullptr;
PyObject* Const_XOR = nullptr;
PyObject* Const_TV = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  BcOpType val,
  PyObject*& const_obj
)
{
  auto type = PyBcOpType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BcOpType_Object*>(obj);
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
  auto& val = PyBcOpType::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case BcOpType::AND: str_val = "AND"; break;
      case BcOpType::OR: str_val = "OR"; break;
      case BcOpType::XOR: str_val = "XOR"; break;
      case BcOpType::TV: str_val = "TV"; break;
      case BcOpType::NONE: str_val = "None"; break;
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
  auto& val = PyBcOpType::_get_ref(self);
  try {
    if ( PyBcOpType::Check(self) && PyBcOpType::Check(other) ) {
      auto& val1 = PyBcOpType::_get_ref(self);
      auto& val2 = PyBcOpType::_get_ref(other);
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
  BcOpType val;
  if ( val_obj != nullptr ) {
    if ( !PyBcOpType::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to BcOpType");
      return nullptr;
    }
  }
  try {
    return PyBcOpType::ToPyObject(val);
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


// @brief BcOpType オブジェクトを使用可能にする．
bool
PyBcOpType::init(
  PyObject* m
)
{
  BcOpType_Type.tp_name = "BcOpType";
  BcOpType_Type.tp_basicsize = sizeof(BcOpType_Object);
  BcOpType_Type.tp_itemsize = 0;
  BcOpType_Type.tp_dealloc = dealloc_func;
  BcOpType_Type.tp_repr = repr_func;
  BcOpType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BcOpType_Type.tp_doc = PyDoc_STR("Python extended object for BcOpType");
  BcOpType_Type.tp_richcompare = richcompare_func;
  BcOpType_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BcOpType", &BcOpType_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("AND", BcOpType::AND, Const_AND) ) {
    goto error;
  }
  if ( !reg_const_obj("OR", BcOpType::OR, Const_OR) ) {
    goto error;
  }
  if ( !reg_const_obj("XOR", BcOpType::XOR, Const_XOR) ) {
    goto error;
  }
  if ( !reg_const_obj("TV", BcOpType::TV, Const_TV) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BcOpType を PyObject に変換する．
PyObject*
PyBcOpType::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case BcOpType::AND: obj = Const_AND; break;
    case BcOpType::OR: obj = Const_OR; break;
    case BcOpType::XOR: obj = Const_XOR; break;
    case BcOpType::TV: obj = Const_TV; break;
    case BcOpType::NONE: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for BcOpType");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を BcOpType に変換する．
bool
PyBcOpType::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = BcOpType::NONE;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( str_val == "AND" ) {
      val = BcOpType::AND;
      return true;
    }
    else if ( str_val == "OR" ) {
      val = BcOpType::OR;
      return true;
    }
    else if ( str_val == "XOR" ) {
      val = BcOpType::XOR;
      return true;
    }
    else if ( str_val == "TV" ) {
      val = BcOpType::TV;
      return true;
    }
    return false;
  }
  if ( PyBcOpType::Check(obj) ) {
    val = PyBcOpType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が BcOpType タイプか調べる．
bool
PyBcOpType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BcOpType_Type);
}

// @brief PyObject から BcOpType を取り出す．
BcOpType&
PyBcOpType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BcOpType_Object*>(obj);
  return my_obj->mVal;
}

// @brief BcOpType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBcOpType::_typeobject()
{
  return &BcOpType_Type;
}

END_NAMESPACE_YM
