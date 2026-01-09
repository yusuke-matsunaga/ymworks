
/// @file PySatLiteralMap.cc
/// @brief PySatLiteralMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PySatLiteralMap.h"
#include "pym/PySatLiteral.h"
#include "pym/PyUlong.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct SatLiteralMap_Object
{
  PyObject_HEAD
  SatLiteralMap mVal;
};

// Python 用のタイプ定義
PyTypeObject SatLiteralMap_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PySatLiteralMap::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<SatLiteralMap_Object*>(self);
  obj->mVal.~SatLiteralMap();
  Py_TYPE(self)->tp_free(self);
}

Py_ssize_t
mp_length(
  PyObject* self
)
{
  auto& val = PySatLiteralMap::_get_ref(self);
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
  auto& val = PySatLiteralMap::_get_ref(self);
  try {
    if ( !PyUlong::Check(key) ) {
      PyErr_SetString(PyExc_TypeError, "1st argument should be integer");
      return nullptr;
    }
    auto var = PyUlong::Get(key);
    if ( val.count(var) == 0 ) {
      PyErr_SetString(PyExc_ValueError, "key not found");
      return nullptr;
    }
    return PySatLiteral::ToPyObject(val.at(var));
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

int
mp_ass_subscript(
  PyObject* self,
  PyObject* key,
  PyObject* obj
)
{
  auto& val = PySatLiteralMap::_get_ref(self);
  try {
    if ( !PyUlong::Check(key) ) {
      PyErr_SetString(PyExc_TypeError, "1st argument should be integer type");
      return -1;
    }
    if ( !PySatLiteral::Check(obj) ) {
      PyErr_SetString(PyExc_TypeError, "2nd argument should be SatLiteral type");
      return -1;
    }
    auto var = PyUlong::Get(key);
    auto& lit = PySatLiteral::_get_ref(obj);
    val.emplace(var, lit);
    return 0;
  }
  catch ( std::invalid_argument err ) {
    std::ostringstream buf;
    buf << "invalid argument" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return -1;
  }
  catch ( std::out_of_range err ) {
    std::ostringstream buf;
    buf << "out of range" << ": " << err.what();
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
    auto my_obj = reinterpret_cast<SatLiteralMap_Object*>(self);
    new (&my_obj->mVal) SatLiteralMap();
    if ( obj != nullptr ) {
      auto emsg = "1st argument should be a sequence of (integer, SatLiteral)";
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
        if ( !PyUlong::Check(var_obj) ) {
          Py_DecRef(obj1);
          PyErr_SetString(PyExc_TypeError, emsg);
          return nullptr;
        }
        if ( !PySatLiteral::Check(lit_obj) ) {
          Py_DecRef(obj1);
          PyErr_SetString(PyExc_TypeError, emsg);
          return nullptr;
        }
        auto var = PyUlong::Get(var_obj);
        auto& lit = PySatLiteral::_get_ref(lit_obj);
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
  catch ( std::out_of_range err ) {
    std::ostringstream buf;
    buf << "out of range" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

END_NONAMESPACE


// @brief SatLiteralMap オブジェクトを使用可能にする．
bool
PySatLiteralMap::init(
  PyObject* m
)
{
  SatLiteralMap_Type.tp_name = "SatLiteralMap";
  SatLiteralMap_Type.tp_basicsize = sizeof(SatLiteralMap_Object);
  SatLiteralMap_Type.tp_itemsize = 0;
  SatLiteralMap_Type.tp_dealloc = dealloc_func;
  SatLiteralMap_Type.tp_as_mapping = &mapping;
  SatLiteralMap_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  SatLiteralMap_Type.tp_doc = PyDoc_STR("Python extended object for SatLiteralMap");
  SatLiteralMap_Type.tp_new = new_func;
  if ( !PyModule::reg_type(m, "SatLiteralMap", &SatLiteralMap_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// PyObject を SatLiteralMap に変換する．
bool
PySatLiteralMap::Deconv::operator()(
  PyObject* obj, ///< [in] Python のオブジェクト
  ElemType& val  ///< [out] 結果を格納する変数
)
{
  if ( PySatLiteralMap::Check(obj) ) {
    val = PySatLiteralMap::_get_ref(obj);
    return true;
  }
  return false;
}

// @brief PyObject が SatLiteralMap タイプか調べる．
bool
PySatLiteralMap::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &SatLiteralMap_Type);
}

// @brief PyObject から SatLiteralMap を取り出す．
SatLiteralMap&
PySatLiteralMap::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<SatLiteralMap_Object*>(obj);
  return my_obj->mVal;
}

// @brief SatLiteralMap を表すオブジェクトの型定義を返す．
PyTypeObject*
PySatLiteralMap::_typeobject()
{
  return &SatLiteralMap_Type;
}

END_NAMESPACE_YM
