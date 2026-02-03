
/// @file PyClibCellGroup.cc
/// @brief PyClibCellGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCellGroup.h"
#include "pym/PyClibCellClass.h"
#include "pym/PyClibCell.h"
#include "pym/PyClibCellType.h"
#include "pym/PyClibIOMap.h"
#include "pym/PyClibSeqAttr.h"
#include "pym/PyClibList.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct ClibCellGroup_Object
{
  PyObject_HEAD
  ClibCellGroup mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCellGroup_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCellGroup::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibCellGroup_Object*>(self);
  obj->mVal.~ClibCellGroup();
  Py_TYPE(self)->tp_free(self);
}

// richcompare 関数
PyObject*
richcompare_func(
  PyObject* self,
  PyObject* other,
  int op
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    if ( PyClibCellGroup::Check(other) ) {
      auto& val2 = PyClibCellGroup::_get_ref(other);
      if ( op == Py_EQ ) {
        return PyBool_FromLong(val == val2);
      }
      if ( op == Py_NE ) {
        return PyBool_FromLong(val != val2);
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

PyObject*
cell(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "pos",
    nullptr
  };
  unsigned long pos;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "k",
                                    const_cast<char**>(kwlist),
                                    &pos) ) {
    return nullptr;
  }
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyClibCell::ToPyObject(val.cell(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
meth_cell_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyClibCellList::ToPyObject(val.cell_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// メソッド定義
PyMethodDef methods[] = {
  {"cell",
   reinterpret_cast<PyCFunction>(cell),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return cell")},
  {"meth_cell_list",
   meth_cell_list,
   METH_NOARGS,
   PyDoc_STR("return list of cells")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_rep_class(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyClibCellClass::ToPyObject(val.rep_class());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_iomap(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyClibIOMap::ToPyObject(val.iomap());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_type(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyClibCellType::ToPyObject(val.cell_type());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_seq_attr(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyClibSeqAttr::ToPyObject(val.seq_attr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellGroup::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.cell_num());
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
  {"rep_class", get_rep_class, nullptr, PyDoc_STR("representative class"), nullptr},
  {"iomap", get_iomap, nullptr, PyDoc_STR("IO map for representative class"), nullptr},
  {"cell_type", get_cell_type, nullptr, PyDoc_STR("cell type(Logic, FF, Latch, FSM)"), nullptr},
  {"seq_attr", get_seq_attr, nullptr, PyDoc_STR("ClibSeqAttr"), nullptr},
  {"cell_num", get_cell_num, nullptr, PyDoc_STR("number of cells"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibCellGroup オブジェクトを使用可能にする．
bool
PyClibCellGroup::init(
  PyObject* m
)
{
  ClibCellGroup_Type.tp_name = "ClibCellGroup";
  ClibCellGroup_Type.tp_basicsize = sizeof(ClibCellGroup_Object);
  ClibCellGroup_Type.tp_itemsize = 0;
  ClibCellGroup_Type.tp_dealloc = dealloc_func;
  ClibCellGroup_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCellGroup_Type.tp_doc = PyDoc_STR("Python extended object for ClibCellGroup");
  ClibCellGroup_Type.tp_richcompare = richcompare_func;
  ClibCellGroup_Type.tp_methods = methods;
  ClibCellGroup_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibCellGroup", &ClibCellGroup_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCellGroup を PyObject に変換する．
PyObject*
PyClibCellGroup::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibCellGroup::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCellGroup_Object*>(obj);
  new (&my_obj->mVal) ClibCellGroup(val);
  return obj;
}

// PyObject を ClibCellGroup に変換する．
bool
PyClibCellGroup::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibCellGroup::Check(obj) ) {
    val = PyClibCellGroup::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCellGroup タイプか調べる．
bool
PyClibCellGroup::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCellGroup_Type);
}

// @brief PyObject から ClibCellGroup を取り出す．
ClibCellGroup&
PyClibCellGroup::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCellGroup_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCellGroup を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCellGroup::_typeobject()
{
  return &ClibCellGroup_Type;
}

END_NAMESPACE_YM
