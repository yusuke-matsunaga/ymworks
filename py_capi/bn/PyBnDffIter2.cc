
/// @file PyBnDffIter2.cc
/// @brief PyBnDffIter2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnDffIter2.h"
#include "pym/PyBnDff.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BnDffIter2_Object
{
  PyObject_HEAD
  BnDffIter2 mVal;
};

// Python 用のタイプ定義
PyTypeObject BnDffIter2_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnDffIter2::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnDffIter2_Object*>(self);
  obj->mVal.~BnDffIter2();
  Py_TYPE(self)->tp_free(self);
}

// iter 関数
PyObject*
iter_func(
  PyObject* self
)
{
  auto& val = PyBnDffIter2::_get_ref(self);
  try {
    return self;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// iternext 関数
PyObject*
iternext_func(
  PyObject* self
)
{
  auto& val = PyBnDffIter2::_get_ref(self);
  try {
    if ( val.has_next() ) {
      return PyBnDff::ToPyObject(val.next());
    }
    return nullptr;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief BnDffIter2 オブジェクトを使用可能にする．
bool
PyBnDffIter2::init(
  PyObject* m
)
{
  BnDffIter2_Type.tp_name = "BnDffIter2";
  BnDffIter2_Type.tp_basicsize = sizeof(BnDffIter2_Object);
  BnDffIter2_Type.tp_itemsize = 0;
  BnDffIter2_Type.tp_dealloc = dealloc_func;
  BnDffIter2_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnDffIter2_Type.tp_doc = PyDoc_STR("Python extended object for BnDffIter2");
  BnDffIter2_Type.tp_iter = iter_func;
  BnDffIter2_Type.tp_iternext = iternext_func;
  if ( !PyModule::reg_type(m, "BnDffIter2", &BnDffIter2_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnDffIter2 を PyObject に変換する．
PyObject*
PyBnDffIter2::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnDffIter2::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnDffIter2_Object*>(obj);
  new (&my_obj->mVal) BnDffIter2(val);
  return obj;
}

// @brief PyObject が BnDffIter2 タイプか調べる．
bool
PyBnDffIter2::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnDffIter2_Type);
}

// @brief PyObject から BnDffIter2 を取り出す．
BnDffIter2&
PyBnDffIter2::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnDffIter2_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnDffIter2 を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnDffIter2::_typeobject()
{
  return &BnDffIter2_Type;
}

END_NAMESPACE_YM
