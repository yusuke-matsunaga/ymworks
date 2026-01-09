
/// @file PyClibDelayModel.cc
/// @brief PyClibDelayModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibDelayModel.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibDelayModel_Object
{
  PyObject_HEAD
  ClibDelayModel mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibDelayModel_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibDelayModel::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_generic_cmos = nullptr;
PyObject* Const_table_lookup = nullptr;
PyObject* Const_piecewise_cmos = nullptr;
PyObject* Const_cmos2 = nullptr;
PyObject* Const_dcm = nullptr;
PyObject* Const_polynomial = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibDelayModel val,
  PyObject*& const_obj
)
{
  auto type = PyClibDelayModel::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibDelayModel_Object*>(obj);
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
  auto& val = PyClibDelayModel::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibDelayModel::generic_cmos: str_val = "generic_cmos"; break;
      case ClibDelayModel::table_lookup: str_val = "table_lookup"; break;
      case ClibDelayModel::piecewise_cmos: str_val = "piecewise_cmos"; break;
      case ClibDelayModel::cmos2: str_val = "cmos2"; break;
      case ClibDelayModel::dcm: str_val = "dcm"; break;
      case ClibDelayModel::polynomial: str_val = "polynomial"; break;
      case ClibDelayModel::none: str_val = "None"; break;
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
  auto& val = PyClibDelayModel::_get_ref(self);
  try {
    if ( PyClibDelayModel::Check(self) && PyClibDelayModel::Check(other) ) {
      auto& val1 = PyClibDelayModel::_get_ref(self);
      auto& val2 = PyClibDelayModel::_get_ref(other);
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
  ClibDelayModel val;
  if ( val_obj != nullptr ) {
    if ( !PyClibDelayModel::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibDelayModel");
      return nullptr;
    }
  }
  try {
    return PyClibDelayModel::ToPyObject(val);
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


// @brief ClibDelayModel オブジェクトを使用可能にする．
bool
PyClibDelayModel::init(
  PyObject* m
)
{
  ClibDelayModel_Type.tp_name = "ClibDelayModel";
  ClibDelayModel_Type.tp_basicsize = sizeof(ClibDelayModel_Object);
  ClibDelayModel_Type.tp_itemsize = 0;
  ClibDelayModel_Type.tp_dealloc = dealloc_func;
  ClibDelayModel_Type.tp_repr = repr_func;
  ClibDelayModel_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibDelayModel_Type.tp_doc = PyDoc_STR("Python extended object for ClibDelayModel");
  ClibDelayModel_Type.tp_richcompare = richcompare_func;
  ClibDelayModel_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibDelayModel", &ClibDelayModel_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("generic_cmos", ClibDelayModel::generic_cmos, Const_generic_cmos) ) {
    goto error;
  }
  if ( !reg_const_obj("table_lookup", ClibDelayModel::table_lookup, Const_table_lookup) ) {
    goto error;
  }
  if ( !reg_const_obj("piecewise_cmos", ClibDelayModel::piecewise_cmos, Const_piecewise_cmos) ) {
    goto error;
  }
  if ( !reg_const_obj("cmos2", ClibDelayModel::cmos2, Const_cmos2) ) {
    goto error;
  }
  if ( !reg_const_obj("dcm", ClibDelayModel::dcm, Const_dcm) ) {
    goto error;
  }
  if ( !reg_const_obj("polynomial", ClibDelayModel::polynomial, Const_polynomial) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibDelayModel を PyObject に変換する．
PyObject*
PyClibDelayModel::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibDelayModel::generic_cmos: obj = Const_generic_cmos; break;
    case ClibDelayModel::table_lookup: obj = Const_table_lookup; break;
    case ClibDelayModel::piecewise_cmos: obj = Const_piecewise_cmos; break;
    case ClibDelayModel::cmos2: obj = Const_cmos2; break;
    case ClibDelayModel::dcm: obj = Const_dcm; break;
    case ClibDelayModel::polynomial: obj = Const_polynomial; break;
    case ClibDelayModel::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibDelayModel");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibDelayModel に変換する．
bool
PyClibDelayModel::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibDelayModel::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "generic_cmos") == 0 ) {
      val = ClibDelayModel::generic_cmos;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "table_lookup") == 0 ) {
      val = ClibDelayModel::table_lookup;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "piecewise_cmos") == 0 ) {
      val = ClibDelayModel::piecewise_cmos;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "cmos2") == 0 ) {
      val = ClibDelayModel::cmos2;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "dcm") == 0 ) {
      val = ClibDelayModel::dcm;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "polynomial") == 0 ) {
      val = ClibDelayModel::polynomial;
      return true;
    }
    return false;
  }
  if ( PyClibDelayModel::Check(obj) ) {
    val = PyClibDelayModel::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibDelayModel タイプか調べる．
bool
PyClibDelayModel::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibDelayModel_Type);
}

// @brief PyObject から ClibDelayModel を取り出す．
ClibDelayModel&
PyClibDelayModel::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibDelayModel_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibDelayModel を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibDelayModel::_typeobject()
{
  return &ClibDelayModel_Type;
}

END_NAMESPACE_YM
