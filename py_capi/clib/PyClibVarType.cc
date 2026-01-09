
/// @file PyClibVarType.cc
/// @brief PyClibVarType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibVarType.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibVarType_Object
{
  PyObject_HEAD
  ClibVarType mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibVarType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibVarType::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_input_net_transition = nullptr;
PyObject* Const_total_output_net_capacitance = nullptr;
PyObject* Const_output_net_length = nullptr;
PyObject* Const_output_net_wire_cap = nullptr;
PyObject* Const_output_net_pin_cap = nullptr;
PyObject* Const_equal_or_opposite_output_net_capacitance = nullptr;
PyObject* Const_input_transition_time = nullptr;
PyObject* Const_related_out_total_output_net_capacitance = nullptr;
PyObject* Const_related_out_output_net_length = nullptr;
PyObject* Const_related_out_output_net_wire_cap = nullptr;
PyObject* Const_related_out_output_net_pin_cap = nullptr;
PyObject* Const_constrained_pin_transition = nullptr;
PyObject* Const_related_pin_transition = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibVarType val,
  PyObject*& const_obj
)
{
  auto type = PyClibVarType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibVarType_Object*>(obj);
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
  auto& val = PyClibVarType::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibVarType::input_net_transition: str_val = "input_net_transition"; break;
      case ClibVarType::total_output_net_capacitance: str_val = "total_output_net_capacitance"; break;
      case ClibVarType::output_net_length: str_val = "output_net_length"; break;
      case ClibVarType::output_net_wire_cap: str_val = "output_net_wire_cap"; break;
      case ClibVarType::output_net_pin_cap: str_val = "output_net_pin_cap"; break;
      case ClibVarType::equal_or_opposite_output_net_capacitance: str_val = "equal_or_opposite_output_net_capacitance"; break;
      case ClibVarType::input_transition_time: str_val = "input_transition_time"; break;
      case ClibVarType::related_out_total_output_net_capacitance: str_val = "related_out_total_output_net_capacitance"; break;
      case ClibVarType::related_out_output_net_length: str_val = "related_out_output_net_length"; break;
      case ClibVarType::related_out_output_net_wire_cap: str_val = "related_out_output_net_wire_cap"; break;
      case ClibVarType::related_out_output_net_pin_cap: str_val = "related_out_output_net_pin_cap"; break;
      case ClibVarType::constrained_pin_transition: str_val = "constrained_pin_transition"; break;
      case ClibVarType::related_pin_transition: str_val = "related_pin_transition"; break;
      case ClibVarType::none: str_val = "None"; break;
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
  auto& val = PyClibVarType::_get_ref(self);
  try {
    if ( PyClibVarType::Check(self) && PyClibVarType::Check(other) ) {
      auto& val1 = PyClibVarType::_get_ref(self);
      auto& val2 = PyClibVarType::_get_ref(other);
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
  ClibVarType val;
  if ( val_obj != nullptr ) {
    if ( !PyClibVarType::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibVarType");
      return nullptr;
    }
  }
  try {
    return PyClibVarType::ToPyObject(val);
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


// @brief ClibVarType オブジェクトを使用可能にする．
bool
PyClibVarType::init(
  PyObject* m
)
{
  ClibVarType_Type.tp_name = "ClibVarType";
  ClibVarType_Type.tp_basicsize = sizeof(ClibVarType_Object);
  ClibVarType_Type.tp_itemsize = 0;
  ClibVarType_Type.tp_dealloc = dealloc_func;
  ClibVarType_Type.tp_repr = repr_func;
  ClibVarType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibVarType_Type.tp_doc = PyDoc_STR("Python extended object for ClibVarType");
  ClibVarType_Type.tp_richcompare = richcompare_func;
  ClibVarType_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibVarType", &ClibVarType_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("input_net_transition", ClibVarType::input_net_transition, Const_input_net_transition) ) {
    goto error;
  }
  if ( !reg_const_obj("total_output_net_capacitance", ClibVarType::total_output_net_capacitance, Const_total_output_net_capacitance) ) {
    goto error;
  }
  if ( !reg_const_obj("output_net_length", ClibVarType::output_net_length, Const_output_net_length) ) {
    goto error;
  }
  if ( !reg_const_obj("output_net_wire_cap", ClibVarType::output_net_wire_cap, Const_output_net_wire_cap) ) {
    goto error;
  }
  if ( !reg_const_obj("output_net_pin_cap", ClibVarType::output_net_pin_cap, Const_output_net_pin_cap) ) {
    goto error;
  }
  if ( !reg_const_obj("equal_or_opposite_output_net_capacitance", ClibVarType::equal_or_opposite_output_net_capacitance, Const_equal_or_opposite_output_net_capacitance) ) {
    goto error;
  }
  if ( !reg_const_obj("input_transition_time", ClibVarType::input_transition_time, Const_input_transition_time) ) {
    goto error;
  }
  if ( !reg_const_obj("related_out_total_output_net_capacitance", ClibVarType::related_out_total_output_net_capacitance, Const_related_out_total_output_net_capacitance) ) {
    goto error;
  }
  if ( !reg_const_obj("related_out_output_net_length", ClibVarType::related_out_output_net_length, Const_related_out_output_net_length) ) {
    goto error;
  }
  if ( !reg_const_obj("related_out_output_net_wire_cap", ClibVarType::related_out_output_net_wire_cap, Const_related_out_output_net_wire_cap) ) {
    goto error;
  }
  if ( !reg_const_obj("related_out_output_net_pin_cap", ClibVarType::related_out_output_net_pin_cap, Const_related_out_output_net_pin_cap) ) {
    goto error;
  }
  if ( !reg_const_obj("constrained_pin_transition", ClibVarType::constrained_pin_transition, Const_constrained_pin_transition) ) {
    goto error;
  }
  if ( !reg_const_obj("related_pin_transition", ClibVarType::related_pin_transition, Const_related_pin_transition) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibVarType を PyObject に変換する．
PyObject*
PyClibVarType::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibVarType::input_net_transition: obj = Const_input_net_transition; break;
    case ClibVarType::total_output_net_capacitance: obj = Const_total_output_net_capacitance; break;
    case ClibVarType::output_net_length: obj = Const_output_net_length; break;
    case ClibVarType::output_net_wire_cap: obj = Const_output_net_wire_cap; break;
    case ClibVarType::output_net_pin_cap: obj = Const_output_net_pin_cap; break;
    case ClibVarType::equal_or_opposite_output_net_capacitance: obj = Const_equal_or_opposite_output_net_capacitance; break;
    case ClibVarType::input_transition_time: obj = Const_input_transition_time; break;
    case ClibVarType::related_out_total_output_net_capacitance: obj = Const_related_out_total_output_net_capacitance; break;
    case ClibVarType::related_out_output_net_length: obj = Const_related_out_output_net_length; break;
    case ClibVarType::related_out_output_net_wire_cap: obj = Const_related_out_output_net_wire_cap; break;
    case ClibVarType::related_out_output_net_pin_cap: obj = Const_related_out_output_net_pin_cap; break;
    case ClibVarType::constrained_pin_transition: obj = Const_constrained_pin_transition; break;
    case ClibVarType::related_pin_transition: obj = Const_related_pin_transition; break;
    case ClibVarType::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibVarType");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibVarType に変換する．
bool
PyClibVarType::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibVarType::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "input_net_transition") == 0 ) {
      val = ClibVarType::input_net_transition;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "total_output_net_capacitance") == 0 ) {
      val = ClibVarType::total_output_net_capacitance;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "output_net_length") == 0 ) {
      val = ClibVarType::output_net_length;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "output_net_wire_cap") == 0 ) {
      val = ClibVarType::output_net_wire_cap;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "output_net_pin_cap") == 0 ) {
      val = ClibVarType::output_net_pin_cap;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "equal_or_opposite_output_net_capacitance") == 0 ) {
      val = ClibVarType::equal_or_opposite_output_net_capacitance;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "input_transition_time") == 0 ) {
      val = ClibVarType::input_transition_time;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "related_out_total_output_net_capacitance") == 0 ) {
      val = ClibVarType::related_out_total_output_net_capacitance;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "related_out_output_net_length") == 0 ) {
      val = ClibVarType::related_out_output_net_length;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "related_out_output_net_wire_cap") == 0 ) {
      val = ClibVarType::related_out_output_net_wire_cap;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "related_out_output_net_pin_cap") == 0 ) {
      val = ClibVarType::related_out_output_net_pin_cap;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "constrained_pin_transition") == 0 ) {
      val = ClibVarType::constrained_pin_transition;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "related_pin_transition") == 0 ) {
      val = ClibVarType::related_pin_transition;
      return true;
    }
    return false;
  }
  if ( PyClibVarType::Check(obj) ) {
    val = PyClibVarType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibVarType タイプか調べる．
bool
PyClibVarType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibVarType_Type);
}

// @brief PyObject から ClibVarType を取り出す．
ClibVarType&
PyClibVarType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibVarType_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibVarType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibVarType::_typeobject()
{
  return &ClibVarType_Type;
}

END_NAMESPACE_YM
