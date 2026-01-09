
/// @file PyBnNodeList.cc
/// @brief PyBnNodeList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnNodeList.h"
#include "pym/PyBnNode.h"
#include "pym/PyBnNodeIter2.h"
#include "pym/PyList.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BnNodeList_Object
{
  PyObject_HEAD
  BnNodeList mVal;
};

// Python 用のタイプ定義
PyTypeObject BnNodeList_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnNodeList::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnNodeList_Object*>(self);
  obj->mVal.~BnNodeList();
  Py_TYPE(self)->tp_free(self);
}

Py_ssize_t
sq_length(
  PyObject* self
)
{
  auto& val = PyBnNodeList::_get_ref(self);
  try {
    auto len_val = val.size();
    return len_val;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return -1;
  }
}

PyObject*
sq_item(
  PyObject* self,
  Py_ssize_t index
)
{
  auto& val = PyBnNodeList::_get_ref(self);
  try {
    return PyBnNode::ToPyObject(val[index]);
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

// Sequence オブジェクト構造体
PySequenceMethods sequence = {
  .sq_length = sq_length,
  .sq_item = sq_item
};

// iter 関数
PyObject*
iter_func(
  PyObject* self
)
{
  auto& val = PyBnNodeList::_get_ref(self);
  try {
    return PyBnNodeIter2::ToPyObject(val.iter());
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
  PyObject* obj = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "|O",
                                    const_cast<char**>(kwlist),
                                    &obj) ) {
    return nullptr;
  }
  try {
    auto self = type->tp_alloc(type, 0);
    auto my_obj = reinterpret_cast<BnNodeList_Object*>(self);
    if ( obj != nullptr ) {
      BnNodeList node_list;
      if ( !PyBnNodeList::FromPyObject(obj, node_list) ) {
        PyErr_SetString(PyExc_TypeError, "argument 1 must be a BnNodeList or a list of BnNode");
        return nullptr;
      }
      new (&my_obj->mVal) BnNodeList(node_list);
    }
    else {
      new (&my_obj->mVal) BnNodeList();
    }
    return self;
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


// @brief BnNodeList オブジェクトを使用可能にする．
bool
PyBnNodeList::init(
  PyObject* m
)
{
  BnNodeList_Type.tp_name = "BnNodeList";
  BnNodeList_Type.tp_basicsize = sizeof(BnNodeList_Object);
  BnNodeList_Type.tp_itemsize = 0;
  BnNodeList_Type.tp_dealloc = dealloc_func;
  BnNodeList_Type.tp_as_sequence = &sequence;
  BnNodeList_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnNodeList_Type.tp_doc = PyDoc_STR("Python extended object for BnNodeList");
  BnNodeList_Type.tp_iter = iter_func;
  BnNodeList_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BnNodeList", &BnNodeList_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnNodeList を PyObject に変換する．
PyObject*
PyBnNodeList::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnNodeList::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnNodeList_Object*>(obj);
  new (&my_obj->mVal) BnNodeList(val);
  return obj;
}

// PyObject を BnNodeList に変換する．
bool
PyBnNodeList::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBnNodeList::Check(obj) ) {
    val = PyBnNodeList::_get_ref(obj);
    return true;
  }

  {
    std::vector<BnNode> node_list;
    if ( PyList<BnNode, PyBnNode>::FromPyObject(obj, node_list) ) {
      val = BnNodeList(node_list);
      return true;
    }
  }
  return false;
}

// @brief PyObject が BnNodeList タイプか調べる．
bool
PyBnNodeList::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnNodeList_Type);
}

// @brief PyObject から BnNodeList を取り出す．
BnNodeList&
PyBnNodeList::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnNodeList_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnNodeList を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnNodeList::_typeobject()
{
  return &BnNodeList_Type;
}

END_NAMESPACE_YM
