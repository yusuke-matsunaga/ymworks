
/// @file PyBddVarMap.cc
/// @brief PyBddVarMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyBddVarMap.h"
#include "pym/PyBddVar.h"
#include "pym/PyBddLit.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct BddVarMap_Object
{
  PyObject_HEAD
  Bdd::VarMap mVal;
};

// Python 用のタイプ定義
PyTypeObject BddVarMap_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyBddVarMap::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<BddVarMap_Object*>(self);
  using myclass = Bdd::VarMap;
  obj->mVal.~myclass();
  Py_TYPE(self)->tp_free(self);
}

Py_ssize_t
mp_length(
  PyObject* self
)
{
  auto& val = PyBddVarMap::_get_ref(self);
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
mp_subscript(
  PyObject* self,
  PyObject* key
)
{
  auto& val = PyBddVarMap::_get_ref(self);
  try {
    if ( !PyBddVar::Check(key) ) {
      PyErr_SetString(PyExc_TypeError, "1st argument should be BddVar");
      return nullptr;
    }
    auto& var = PyBddVar::_get_ref(key);
    if ( val.count(var) == 0 ) {
      PyErr_SetString(PyExc_ValueError, "key not found");
      return nullptr;
    }
    return PyBddLit::ToPyObject(val.at(var));
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

int
mp_ass_subscript(
  PyObject* self,
  PyObject* key,
  PyObject* obj
)
{
  auto& val = PyBddVarMap::_get_ref(self);
  try {
    if ( !PyBddVar::Check(key) ) {
      PyErr_SetString(PyExc_TypeError, "1st argument should be BddVar");
      return -1;
    }
    if ( !PyBddLit::Check(obj) ) {
      PyErr_SetString(PyExc_TypeError, "2nd argument should be BddLit");
      return -1;
    }
    auto& var = PyBddVar::_get_ref(key);
    auto& lit = PyBddLit::_get_ref(obj);
    val.emplace(var, lit);
    return 0;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return -1;
  }
}

// Mapping オブジェクト構造体
PyMappingMethods mapping = {
  .mp_length = mp_length,
  .mp_subscript = mp_subscript,
  .mp_ass_subscript = mp_ass_subscript
};

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
    auto my_obj = reinterpret_cast<BddVarMap_Object*>(self);
    new (&my_obj->mVal) Bdd::VarMap();
    if ( obj != nullptr ) {
      auto emsg = "1st argument should be a sequence of (BddVar, BddLit)";
      if ( !PySequence_Check(obj) ) {
        PyErr_SetString(PyExc_TypeError, emsg);
        return nullptr;
      }
      auto n = PySequence_Size(obj);
      for ( SizeType i = 0; i < n; ++ i ) {
        auto obj1 = PySequence_GetItem(obj, i);
        PyObject* var_obj;
        PyObject* lit_obj;
        if ( !PyArg_ParseTuple(obj1, "(OO)", &var_obj, &lit_obj) ) {
          Py_DecRef(obj1);
          PyErr_SetString(PyExc_TypeError, emsg);
          return nullptr;
        }
        if ( !PyBddVar::Check(var_obj) ) {
          Py_DecRef(obj1);
          PyErr_SetString(PyExc_TypeError, emsg);
          return nullptr;
        }
        if ( !PyBddLit::Check(lit_obj) ) {
          Py_DecRef(obj1);
          PyErr_SetString(PyExc_TypeError, emsg);
          return nullptr;
        }
        auto& var = PyBddVar::_get_ref(var_obj);
        auto& lit = PyBddLit::_get_ref(lit_obj);
        my_obj->mVal.emplace(var, lit);
        Py_DecRef(obj1);
      }
    }
    return self;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief BddVarMap オブジェクトを使用可能にする．
bool
PyBddVarMap::init(
  PyObject* m
)
{
  BddVarMap_Type.tp_name = "BddVarMap";
  BddVarMap_Type.tp_basicsize = sizeof(BddVarMap_Object);
  BddVarMap_Type.tp_itemsize = 0;
  BddVarMap_Type.tp_dealloc = dealloc_func;
  BddVarMap_Type.tp_as_mapping = &mapping;
  BddVarMap_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  BddVarMap_Type.tp_doc = PyDoc_STR("Python extended object for Bdd::VarMap");
  BddVarMap_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "BddVarMap", &BddVarMap_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// PyObject を Bdd::VarMap に変換する．
bool
PyBddVarMap::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PyBddVarMap::Check(obj) ) {
    val = PyBddVarMap::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が Bdd::VarMap タイプか調べる．
bool
PyBddVarMap::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &BddVarMap_Type);
}

// @brief PyObject から Bdd::VarMap を取り出す．
Bdd::VarMap&
PyBddVarMap::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<BddVarMap_Object*>(obj);
  return my_obj->mVal;
}

// @brief Bdd::VarMap を表すオブジェクトの型定義を返す．
PyTypeObject*
PyBddVarMap::_typeobject()
{
  return &BddVarMap_Type;
}

END_NAMESPACE_YM
