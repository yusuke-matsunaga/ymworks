
/// @file PySatStats.cc
/// @brief PySatStats の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySatStats.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct SatStats_Object
{
  PyObject_HEAD
  SatStats mVal;
};

// Python 用のタイプ定義
PyTypeObject SatStats_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PySatStats::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<SatStats_Object*>(self);
  obj->mVal.~SatStats();
  Py_TYPE(self)->tp_free(self);
}

END_NONAMESPACE


// @brief SatStats オブジェクトを使用可能にする．
bool
PySatStats::init(
  PyObject* m
)
{
  SatStats_Type.tp_name = "SatStats";
  SatStats_Type.tp_basicsize = sizeof(SatStats_Object);
  SatStats_Type.tp_itemsize = 0;
  SatStats_Type.tp_dealloc = dealloc_func;
  SatStats_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  SatStats_Type.tp_doc = PyDoc_STR("Python extended object for SatStats");
  if ( !PyModule::reg_type(m, "SatStats", &SatStats_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// SatStats を PyObject に変換する．
PyObject*
PySatStats::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PySatStats::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<SatStats_Object*>(obj);
  new (&my_obj->mVal) SatStats(val);
  return obj;
}

// PyObject を SatStats に変換する．
bool
PySatStats::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PySatStats::Check(obj) ) {
    val = PySatStats::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が SatStats タイプか調べる．
bool
PySatStats::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &SatStats_Type);
}

// @brief PyObject から SatStats を取り出す．
SatStats&
PySatStats::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<SatStats_Object*>(obj);
  return my_obj->mVal;
}

// @brief SatStats を表すオブジェクトの型定義を返す．
PyTypeObject*
PySatStats::_typeobject()
{
  return &SatStats_Type;
}

END_NAMESPACE_YM
