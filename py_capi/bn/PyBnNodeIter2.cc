
/// @file PyBnNodeIter2.cc
/// @brief PyBnNodeIter2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnNodeIter2.h"
#include "pym/PyBnNode.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BnNodeIter2_Object
{
  PyObject_HEAD
  BnNodeIter2 mVal;
};

// Python 用のタイプ定義
PyTypeObject BnNodeIter2_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnNodeIter2::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnNodeIter2_Object*>(self);
  obj->mVal.~BnNodeIter2();
  Py_TYPE(self)->tp_free(self);
}

// iter 関数
PyObject*
iter_func(
  PyObject* self
)
{
  auto& val = PyBnNodeIter2::_get_ref(self);
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
  auto& val = PyBnNodeIter2::_get_ref(self);
  try {
    if ( val.has_next() ) {
      return PyBnNode::ToPyObject(val.next());
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


// @brief BnNodeIter2 オブジェクトを使用可能にする．
bool
PyBnNodeIter2::init(
  PyObject* m
)
{
  BnNodeIter2_Type.tp_name = "BnNodeIter2";
  BnNodeIter2_Type.tp_basicsize = sizeof(BnNodeIter2_Object);
  BnNodeIter2_Type.tp_itemsize = 0;
  BnNodeIter2_Type.tp_dealloc = dealloc_func;
  BnNodeIter2_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnNodeIter2_Type.tp_doc = PyDoc_STR("Python extended object for BnNodeIter2");
  BnNodeIter2_Type.tp_iter = iter_func;
  BnNodeIter2_Type.tp_iternext = iternext_func;
  if ( !PyModule::reg_type(m, "BnNodeIter2", &BnNodeIter2_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnNodeIter2 を PyObject に変換する．
PyObject*
PyBnNodeIter2::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnNodeIter2::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnNodeIter2_Object*>(obj);
  new (&my_obj->mVal) BnNodeIter2(val);
  return obj;
}

// @brief PyObject が BnNodeIter2 タイプか調べる．
bool
PyBnNodeIter2::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnNodeIter2_Type);
}

// @brief PyObject から BnNodeIter2 を取り出す．
BnNodeIter2&
PyBnNodeIter2::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnNodeIter2_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnNodeIter2 を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnNodeIter2::_typeobject()
{
  return &BnNodeIter2_Type;
}

END_NAMESPACE_YM
