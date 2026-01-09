
/// @file PyClibCellClass.cc
/// @brief PyClibCellClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyClibCellClass.h"
#include "pym/PyClibCellGroup.h"
#include "pym/PyClibIOMap.h"
#include "pym/PyClibList.h"
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
struct ClibCellClass_Object
{
  PyObject_HEAD
  ClibCellClass mVal;
};

// Python 用のタイプ定義
PyTypeObject ClibCellClass_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyClibCellClass::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<ClibCellClass_Object*>(self);
  obj->mVal.~ClibCellClass();
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
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    if ( PyClibCellClass::Check(other) ) {
      auto& val2 = PyClibCellClass::_get_ref(other);
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

// return idmap
PyObject*
idmap(
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
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyClibIOMap::ToPyObject(val.idmap(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of idmaps
PyObject*
meth_idmap_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyList<ClibIOMap, PyClibIOMap>::ToPyObject(val.idmap_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return CellGroup
PyObject*
cell_group(
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
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyClibCellGroup::ToPyObject(val.cell_group(pos));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return list of CellGroups
PyObject*
meth_cell_group_list(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyClibCellGroupList::ToPyObject(val.cell_group_list());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if valid
PyObject*
is_valid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_valid());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// True if invalid
PyObject*
is_invalid(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyBool_FromLong(val.is_invalid());
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
  {"idmap",
   reinterpret_cast<PyCFunction>(idmap),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return idmap")},
  {"meth_idmap_list",
   meth_idmap_list,
   METH_NOARGS,
   PyDoc_STR("return list of idmaps")},
  {"cell_group",
   reinterpret_cast<PyCFunction>(cell_group),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("return CellGroup")},
  {"meth_cell_group_list",
   meth_cell_group_list,
   METH_NOARGS,
   PyDoc_STR("return list of CellGroups")},
  {"is_valid",
   is_valid,
   METH_NOARGS,
   PyDoc_STR("True if valid")},
  {"is_invalid",
   is_invalid,
   METH_NOARGS,
   PyDoc_STR("True if invalid")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

PyObject*
get_idmap_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.idmap_num());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

PyObject*
get_cell_group_num(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto& val = PyClibCellClass::_get_ref(self);
  try {
    return PyUlong::ToPyObject(val.cell_group_num());
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
  {"idmap_num", get_idmap_num, nullptr, PyDoc_STR("number of idmaps"), nullptr},
  {"cell_group_num", get_cell_group_num, nullptr, PyDoc_STR("number of CellGroups"), nullptr},
  // end-marker
  {nullptr, nullptr, nullptr, nullptr}
};

END_NONAMESPACE


// @brief ClibCellClass オブジェクトを使用可能にする．
bool
PyClibCellClass::init(
  PyObject* m
)
{
  ClibCellClass_Type.tp_name = "ClibCellClass";
  ClibCellClass_Type.tp_basicsize = sizeof(ClibCellClass_Object);
  ClibCellClass_Type.tp_itemsize = 0;
  ClibCellClass_Type.tp_dealloc = dealloc_func;
  ClibCellClass_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  ClibCellClass_Type.tp_doc = PyDoc_STR("Python extended object for ClibCellClass");
  ClibCellClass_Type.tp_richcompare = richcompare_func;
  ClibCellClass_Type.tp_methods = methods;
  ClibCellClass_Type.tp_getset = getsets;
  if ( !PyModule::reg_type(m, "ClibCellClass", &ClibCellClass_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// ClibCellClass を PyObject に変換する．
PyObject*
PyClibCellClass::Conv::operator()(
  const ElemType& val ///< [in] 元の値
)
{
  auto type = PyClibCellClass::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<ClibCellClass_Object*>(obj);
  new (&my_obj->mVal) ClibCellClass(val);
  return obj;
}

// PyObject を ClibCellClass に変換する．
bool
PyClibCellClass::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyClibCellClass::Check(obj) ) {
    val = PyClibCellClass::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が ClibCellClass タイプか調べる．
bool
PyClibCellClass::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &ClibCellClass_Type);
}

// @brief PyObject から ClibCellClass を取り出す．
ClibCellClass&
PyClibCellClass::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<ClibCellClass_Object*>(obj);
  return my_obj->mVal;
}

// @brief ClibCellClass を表すオブジェクトの型定義を返す．
PyTypeObject*
PyClibCellClass::_typeobject()
{
  return &ClibCellClass_Type;
}

END_NAMESPACE_YM
