
/// @file PyUdEdge.cc
/// @brief PyUdEdge の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyUdEdge.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct UdEdge_Object
{
  PyObject_HEAD
  UdEdge mVal;
};

// Python 用のタイプ定義
PyTypeObject UdEdge_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyUdEdge::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<UdEdge_Object*>(self);
  obj->mVal.~UdEdge();
  Py_TYPE(self)->tp_free(self);
}

PyObject*
get_id1(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyUdEdge::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.id1());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_id2(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyUdEdge::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.id2());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// getter/setter定義
PyGetSetDef getsets[] = {
  {"id1", get_id1, nullptr, PyDoc_STR(""), nullptr},
  {"id2", get_id2, nullptr, PyDoc_STR(""), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief UdEdge オブジェクトを使用可能にする．
bool
PyUdEdge::init(
  PyObject* m
)
{
  UdEdge_Type.tp_name = "UdEdge";
  UdEdge_Type.tp_basicsize = sizeof(UdEdge_Object);
  UdEdge_Type.tp_itemsize = 0;
  UdEdge_Type.tp_dealloc = dealloc_func;
  UdEdge_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  UdEdge_Type.tp_doc = PyDoc_STR("Python extended object for UdEdge");
  UdEdge_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "UdEdge", &UdEdge_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// UdEdge を PyObject に変換する．
PyObject*
PyUdEdge::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyUdEdge::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<UdEdge_Object*>(obj);
  new (&my_obj->mVal) UdEdge(val);
  return obj;
}

// PyObject を UdEdge に変換する．
bool
PyUdEdge::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyUdEdge::Check(obj) ) {
    val = PyUdEdge::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が UdEdge タイプか調べる．
bool
PyUdEdge::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &UdEdge_Type);
}

// @brief PyObject から UdEdge を取り出す．
UdEdge&
PyUdEdge::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<UdEdge_Object*>(obj);
  return my_obj->mVal;
}

// @brief UdEdge を表すオブジェクトの型定義を返す．
PyTypeObject*
PyUdEdge::_typeobject()
{
  return &UdEdge_Type;
}

END_NAMESPACE_YM
