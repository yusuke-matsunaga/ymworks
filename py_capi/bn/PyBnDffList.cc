
/// @file PyBnDffList.cc
/// @brief PyBnDffList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBnDffList.h"
#include "pym/PyBnDff.h"
#include "pym/PyBnDffIter2.h"
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
struct BnDffList_Object
{
  PyObject_HEAD
  BnDffList mVal;
};

// Python 用のタイプ定義
PyTypeObject BnDffList_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBnDffList::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BnDffList_Object*>(self);
  obj->mVal.~BnDffList();
  Py_TYPE(self)->tp_free(self);
}

Py_ssize_t
sq_length(
  PyObject* self
)
{
  auto& val = PyBnDffList::_get_ref(self);
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
  auto& val = PyBnDffList::_get_ref(self);
  try {
    return PyBnDff::ToPyObject(val[index]);
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
  auto& val = PyBnDffList::_get_ref(self);
  try {
    return PyBnDffIter2::ToPyObject(val.iter());
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
    auto my_obj = reinterpret_cast<BnDffList_Object*>(self);
    if ( obj != nullptr ) {
      BnDffList dff_list;
      if ( !PyBnDffList::FromPyObject(obj, dff_list) ) {
        PyErr_SetString(PyExc_TypeError, "argument 1 must be a BnDffList or a list of BnDff");
        return nullptr;
      }
      new (&my_obj->mVal) BnDffList(dff_list);
    }
    else {
      new (&my_obj->mVal) BnDffList();
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


// @brief BnDffList オブジェクトを使用可能にする．
bool
PyBnDffList::init(
  PyObject* m
)
{
  BnDffList_Type.tp_name = "BnDffList";
  BnDffList_Type.tp_basicsize = sizeof(BnDffList_Object);
  BnDffList_Type.tp_itemsize = 0;
  BnDffList_Type.tp_dealloc = dealloc_func;
  BnDffList_Type.tp_as_sequence = &sequence;
  BnDffList_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BnDffList_Type.tp_doc = PyDoc_STR("Python extended object for BnDffList");
  BnDffList_Type.tp_iter = iter_func;
  BnDffList_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BnDffList", &BnDffList_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// BnDffList を PyObject に変換する．
PyObject*
PyBnDffList::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyBnDffList::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<BnDffList_Object*>(obj);
  new (&my_obj->mVal) BnDffList(val);
  return obj;
}

// PyObject を BnDffList に変換する．
bool
PyBnDffList::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBnDffList::Check(obj) ) {
    val = PyBnDffList::_get_ref(obj);
    return true;
  }

  {
    std::vector<BnDff> dff_list;
    if ( PyList<BnDff, PyBnDff>::FromPyObject(obj, dff_list) ) {
      val = BnDffList(dff_list);
      return true;
    }
  }
  return false;
}

// @brief PyObject が BnDffList タイプか調べる．
bool
PyBnDffList::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BnDffList_Type);
}

// @brief PyObject から BnDffList を取り出す．
BnDffList&
PyBnDffList::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BnDffList_Object*>(obj);
  return my_obj->mVal;
}

// @brief BnDffList を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBnDffList::_typeobject()
{
  return &BnDffList_Type;
}

END_NAMESPACE_YM
