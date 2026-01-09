
/// @file PyClibTimingType.cc
/// @brief PyClibTimingType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibTimingType.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibTimingType_Object
{
  PyObject_HEAD
  ClibTimingType mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibTimingType_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibTimingType::init() 中で初期化する．
};

// 定数を表すオブジェクト
PyObject* Const_combinational = nullptr;
PyObject* Const_combinational_rise = nullptr;
PyObject* Const_combinational_fall = nullptr;
PyObject* Const_three_state_enable = nullptr;
PyObject* Const_three_state_disable = nullptr;
PyObject* Const_three_state_enable_rise = nullptr;
PyObject* Const_three_state_enable_fall = nullptr;
PyObject* Const_three_state_disable_rise = nullptr;
PyObject* Const_three_state_disable_fall = nullptr;
PyObject* Const_rising_edge = nullptr;
PyObject* Const_falling_edge = nullptr;
PyObject* Const_preset = nullptr;
PyObject* Const_clear = nullptr;
PyObject* Const_hold_rising = nullptr;
PyObject* Const_hold_falling = nullptr;
PyObject* Const_setup_rising = nullptr;
PyObject* Const_setup_falling = nullptr;
PyObject* Const_recovery_rising = nullptr;
PyObject* Const_recovery_falling = nullptr;
PyObject* Const_skew_rising = nullptr;
PyObject* Const_skew_falling = nullptr;
PyObject* Const_removal_rising = nullptr;
PyObject* Const_removal_falling = nullptr;
PyObject* Const_non_seq_setup_rising = nullptr;
PyObject* Const_non_seq_setup_falling = nullptr;
PyObject* Const_non_seq_hold_rising = nullptr;
PyObject* Const_non_seq_hold_falling = nullptr;
PyObject* Const_nochange_high_high = nullptr;
PyObject* Const_nochange_high_low = nullptr;
PyObject* Const_nochange_low_high = nullptr;
PyObject* Const_nochange_low_low = nullptr;

// 定数の登録を行う関数
bool
reg_const_obj(
  const char* name,
  ClibTimingType val,
  PyObject*& const_obj
)
{
  auto type = PyClibTimingType::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibTimingType_Object*>(obj);
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
  auto& val = PyClibTimingType::_get_ref(self);
  try {
    std::string str_val;
    switch ( val ) {
      case ClibTimingType::combinational: str_val = "combinational"; break;
      case ClibTimingType::combinational_rise: str_val = "combinational_rise"; break;
      case ClibTimingType::combinational_fall: str_val = "combinational_fall"; break;
      case ClibTimingType::three_state_enable: str_val = "three_state_enable"; break;
      case ClibTimingType::three_state_disable: str_val = "three_state_disable"; break;
      case ClibTimingType::three_state_enable_rise: str_val = "three_state_enable_rise"; break;
      case ClibTimingType::three_state_enable_fall: str_val = "three_state_enable_fall"; break;
      case ClibTimingType::three_state_disable_rise: str_val = "three_state_disable_rise"; break;
      case ClibTimingType::three_state_disable_fall: str_val = "three_state_disable_fall"; break;
      case ClibTimingType::rising_edge: str_val = "rising_edge"; break;
      case ClibTimingType::falling_edge: str_val = "falling_edge"; break;
      case ClibTimingType::preset: str_val = "preset"; break;
      case ClibTimingType::clear: str_val = "clear"; break;
      case ClibTimingType::hold_rising: str_val = "hold_rising"; break;
      case ClibTimingType::hold_falling: str_val = "hold_falling"; break;
      case ClibTimingType::setup_rising: str_val = "setup_rising"; break;
      case ClibTimingType::setup_falling: str_val = "setup_falling"; break;
      case ClibTimingType::recovery_rising: str_val = "recovery_rising"; break;
      case ClibTimingType::recovery_falling: str_val = "recovery_falling"; break;
      case ClibTimingType::skew_rising: str_val = "skew_rising"; break;
      case ClibTimingType::skew_falling: str_val = "skew_falling"; break;
      case ClibTimingType::removal_rising: str_val = "removal_rising"; break;
      case ClibTimingType::removal_falling: str_val = "removal_falling"; break;
      case ClibTimingType::non_seq_setup_rising: str_val = "non_seq_setup_rising"; break;
      case ClibTimingType::non_seq_setup_falling: str_val = "non_seq_setup_falling"; break;
      case ClibTimingType::non_seq_hold_rising: str_val = "non_seq_hold_rising"; break;
      case ClibTimingType::non_seq_hold_falling: str_val = "non_seq_hold_falling"; break;
      case ClibTimingType::nochange_high_high: str_val = "nochange_high_high"; break;
      case ClibTimingType::nochange_high_low: str_val = "nochange_high_low"; break;
      case ClibTimingType::nochange_low_high: str_val = "nochange_low_high"; break;
      case ClibTimingType::nochange_low_low: str_val = "nochange_low_low"; break;
      case ClibTimingType::none: str_val = "None"; break;
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
  auto& val = PyClibTimingType::_get_ref(self);
  try {
    if ( PyClibTimingType::Check(self) && PyClibTimingType::Check(other) ) {
      auto& val1 = PyClibTimingType::_get_ref(self);
      auto& val2 = PyClibTimingType::_get_ref(other);
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
  ClibTimingType val;
  if ( val_obj != nullptr ) {
    if ( !PyClibTimingType::FromPyObject(val_obj, val) ) {
      PyErr_SetString(PyExc_ValueError, "could not convert to ClibTimingType");
      return nullptr;
    }
  }
  try {
    return PyClibTimingType::ToPyObject(val);
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


// @brief ClibTimingType オブジェクトを使用可能にする．
bool
PyClibTimingType::init(
  PyObject* m
)
{
  ClibTimingType_Type.tp_name = "ClibTimingType";
  ClibTimingType_Type.tp_basicsize = sizeof(ClibTimingType_Object);
  ClibTimingType_Type.tp_itemsize = 0;
  ClibTimingType_Type.tp_dealloc = dealloc_func;
  ClibTimingType_Type.tp_repr = repr_func;
  ClibTimingType_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibTimingType_Type.tp_doc = PyDoc_STR("Python extended object for ClibTimingType");
  ClibTimingType_Type.tp_richcompare = richcompare_func;
  ClibTimingType_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "ClibTimingType", &ClibTimingType_Type) ) {
    goto error;
  }
  // 定数オブジェクトの生成・登録
  if ( !reg_const_obj("combinational", ClibTimingType::combinational, Const_combinational) ) {
    goto error;
  }
  if ( !reg_const_obj("combinational_rise", ClibTimingType::combinational_rise, Const_combinational_rise) ) {
    goto error;
  }
  if ( !reg_const_obj("combinational_fall", ClibTimingType::combinational_fall, Const_combinational_fall) ) {
    goto error;
  }
  if ( !reg_const_obj("three_state_enable", ClibTimingType::three_state_enable, Const_three_state_enable) ) {
    goto error;
  }
  if ( !reg_const_obj("three_state_disable", ClibTimingType::three_state_disable, Const_three_state_disable) ) {
    goto error;
  }
  if ( !reg_const_obj("three_state_enable_rise", ClibTimingType::three_state_enable_rise, Const_three_state_enable_rise) ) {
    goto error;
  }
  if ( !reg_const_obj("three_state_enable_fall", ClibTimingType::three_state_enable_fall, Const_three_state_enable_fall) ) {
    goto error;
  }
  if ( !reg_const_obj("three_state_disable_rise", ClibTimingType::three_state_disable_rise, Const_three_state_disable_rise) ) {
    goto error;
  }
  if ( !reg_const_obj("three_state_disable_fall", ClibTimingType::three_state_disable_fall, Const_three_state_disable_fall) ) {
    goto error;
  }
  if ( !reg_const_obj("rising_edge", ClibTimingType::rising_edge, Const_rising_edge) ) {
    goto error;
  }
  if ( !reg_const_obj("falling_edge", ClibTimingType::falling_edge, Const_falling_edge) ) {
    goto error;
  }
  if ( !reg_const_obj("preset", ClibTimingType::preset, Const_preset) ) {
    goto error;
  }
  if ( !reg_const_obj("clear", ClibTimingType::clear, Const_clear) ) {
    goto error;
  }
  if ( !reg_const_obj("hold_rising", ClibTimingType::hold_rising, Const_hold_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("hold_falling", ClibTimingType::hold_falling, Const_hold_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("setup_rising", ClibTimingType::setup_rising, Const_setup_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("setup_falling", ClibTimingType::setup_falling, Const_setup_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("recovery_rising", ClibTimingType::recovery_rising, Const_recovery_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("recovery_falling", ClibTimingType::recovery_falling, Const_recovery_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("skew_rising", ClibTimingType::skew_rising, Const_skew_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("skew_falling", ClibTimingType::skew_falling, Const_skew_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("removal_rising", ClibTimingType::removal_rising, Const_removal_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("removal_falling", ClibTimingType::removal_falling, Const_removal_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("non_seq_setup_rising", ClibTimingType::non_seq_setup_rising, Const_non_seq_setup_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("non_seq_setup_falling", ClibTimingType::non_seq_setup_falling, Const_non_seq_setup_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("non_seq_hold_rising", ClibTimingType::non_seq_hold_rising, Const_non_seq_hold_rising) ) {
    goto error;
  }
  if ( !reg_const_obj("non_seq_hold_falling", ClibTimingType::non_seq_hold_falling, Const_non_seq_hold_falling) ) {
    goto error;
  }
  if ( !reg_const_obj("nochange_high_high", ClibTimingType::nochange_high_high, Const_nochange_high_high) ) {
    goto error;
  }
  if ( !reg_const_obj("nochange_high_low", ClibTimingType::nochange_high_low, Const_nochange_high_low) ) {
    goto error;
  }
  if ( !reg_const_obj("nochange_low_high", ClibTimingType::nochange_low_high, Const_nochange_low_high) ) {
    goto error;
  }
  if ( !reg_const_obj("nochange_low_low", ClibTimingType::nochange_low_low, Const_nochange_low_low) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibTimingType を PyObject に変換する．
PyObject*
PyClibTimingType::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
    case ClibTimingType::combinational: obj = Const_combinational; break;
    case ClibTimingType::combinational_rise: obj = Const_combinational_rise; break;
    case ClibTimingType::combinational_fall: obj = Const_combinational_fall; break;
    case ClibTimingType::three_state_enable: obj = Const_three_state_enable; break;
    case ClibTimingType::three_state_disable: obj = Const_three_state_disable; break;
    case ClibTimingType::three_state_enable_rise: obj = Const_three_state_enable_rise; break;
    case ClibTimingType::three_state_enable_fall: obj = Const_three_state_enable_fall; break;
    case ClibTimingType::three_state_disable_rise: obj = Const_three_state_disable_rise; break;
    case ClibTimingType::three_state_disable_fall: obj = Const_three_state_disable_fall; break;
    case ClibTimingType::rising_edge: obj = Const_rising_edge; break;
    case ClibTimingType::falling_edge: obj = Const_falling_edge; break;
    case ClibTimingType::preset: obj = Const_preset; break;
    case ClibTimingType::clear: obj = Const_clear; break;
    case ClibTimingType::hold_rising: obj = Const_hold_rising; break;
    case ClibTimingType::hold_falling: obj = Const_hold_falling; break;
    case ClibTimingType::setup_rising: obj = Const_setup_rising; break;
    case ClibTimingType::setup_falling: obj = Const_setup_falling; break;
    case ClibTimingType::recovery_rising: obj = Const_recovery_rising; break;
    case ClibTimingType::recovery_falling: obj = Const_recovery_falling; break;
    case ClibTimingType::skew_rising: obj = Const_skew_rising; break;
    case ClibTimingType::skew_falling: obj = Const_skew_falling; break;
    case ClibTimingType::removal_rising: obj = Const_removal_rising; break;
    case ClibTimingType::removal_falling: obj = Const_removal_falling; break;
    case ClibTimingType::non_seq_setup_rising: obj = Const_non_seq_setup_rising; break;
    case ClibTimingType::non_seq_setup_falling: obj = Const_non_seq_setup_falling; break;
    case ClibTimingType::non_seq_hold_rising: obj = Const_non_seq_hold_rising; break;
    case ClibTimingType::non_seq_hold_falling: obj = Const_non_seq_hold_falling; break;
    case ClibTimingType::nochange_high_high: obj = Const_nochange_high_high; break;
    case ClibTimingType::nochange_high_low: obj = Const_nochange_high_low; break;
    case ClibTimingType::nochange_low_high: obj = Const_nochange_low_high; break;
    case ClibTimingType::nochange_low_low: obj = Const_nochange_low_low; break;
    case ClibTimingType::none: Py_RETURN_NONE;
  }
  if ( obj == nullptr ) {
    PyErr_SetString(PyExc_ValueError, "invalid value for ClibTimingType");
    return nullptr;
  }
  Py_INCREF(obj);
  return obj;
}

// PyObject を ClibTimingType に変換する．
bool
PyClibTimingType::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( obj == Py_None ) {
    val = ClibTimingType::none;
    return true;
  }
  if ( PyString::Check(obj) ) {
    auto str_val = PyString::Get(obj);
    if ( strcasecmp(str_val.c_str(), "combinational") == 0 ) {
      val = ClibTimingType::combinational;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "combinational_rise") == 0 ) {
      val = ClibTimingType::combinational_rise;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "combinational_fall") == 0 ) {
      val = ClibTimingType::combinational_fall;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "three_state_enable") == 0 ) {
      val = ClibTimingType::three_state_enable;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "three_state_disable") == 0 ) {
      val = ClibTimingType::three_state_disable;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "three_state_enable_rise") == 0 ) {
      val = ClibTimingType::three_state_enable_rise;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "three_state_enable_fall") == 0 ) {
      val = ClibTimingType::three_state_enable_fall;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "three_state_disable_rise") == 0 ) {
      val = ClibTimingType::three_state_disable_rise;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "three_state_disable_fall") == 0 ) {
      val = ClibTimingType::three_state_disable_fall;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "rising_edge") == 0 ) {
      val = ClibTimingType::rising_edge;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "falling_edge") == 0 ) {
      val = ClibTimingType::falling_edge;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "preset") == 0 ) {
      val = ClibTimingType::preset;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "clear") == 0 ) {
      val = ClibTimingType::clear;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "hold_rising") == 0 ) {
      val = ClibTimingType::hold_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "hold_falling") == 0 ) {
      val = ClibTimingType::hold_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "setup_rising") == 0 ) {
      val = ClibTimingType::setup_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "setup_falling") == 0 ) {
      val = ClibTimingType::setup_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "recovery_rising") == 0 ) {
      val = ClibTimingType::recovery_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "recovery_falling") == 0 ) {
      val = ClibTimingType::recovery_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "skew_rising") == 0 ) {
      val = ClibTimingType::skew_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "skew_falling") == 0 ) {
      val = ClibTimingType::skew_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "removal_rising") == 0 ) {
      val = ClibTimingType::removal_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "removal_falling") == 0 ) {
      val = ClibTimingType::removal_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "non_seq_setup_rising") == 0 ) {
      val = ClibTimingType::non_seq_setup_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "non_seq_setup_falling") == 0 ) {
      val = ClibTimingType::non_seq_setup_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "non_seq_hold_rising") == 0 ) {
      val = ClibTimingType::non_seq_hold_rising;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "non_seq_hold_falling") == 0 ) {
      val = ClibTimingType::non_seq_hold_falling;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "nochange_high_high") == 0 ) {
      val = ClibTimingType::nochange_high_high;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "nochange_high_low") == 0 ) {
      val = ClibTimingType::nochange_high_low;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "nochange_low_high") == 0 ) {
      val = ClibTimingType::nochange_low_high;
      return true;
    }
    else if ( strcasecmp(str_val.c_str(), "nochange_low_low") == 0 ) {
      val = ClibTimingType::nochange_low_low;
      return true;
    }
    return false;
  }
  if ( PyClibTimingType::Check(obj) ) {
    val = PyClibTimingType::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibTimingType タイプか調べる．
bool
PyClibTimingType::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibTimingType_Type);
}

// @brief PyObject から ClibTimingType を取り出す．
ClibTimingType&
PyClibTimingType::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibTimingType_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibTimingType を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibTimingType::_typeobject()
{
  return &ClibTimingType_Type;
}

END_NAMESPACE_YM
