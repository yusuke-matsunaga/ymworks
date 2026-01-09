
/// @file Py%%CLASS%%.cc
/// @brief Py%%CLASS%% の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) %%YEAR%% Yusuke Matsunaga
/// All rights reserved.

#include "pym/Py%%CLASS%%.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_%%NAMESPACE%%

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
struct %%CLASS%%Object
{
  PyObject_HEAD
  %%CLASS%% mVal;
};

// Python 用のタイプ定義
PyTypeObject %%CLASS%%_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};

// 各定数を表す文字列
%%CONST_STR%%

// 各定数を表す PyObject
%%CONST_DEF%%

// 生成関数
PyObject*
%%CLASS%%_new(
  PyTypeObject* type,
  PyObject* args,
  PyObject* kwds
)
{
  if ( type != &%%CLASS%%_Type ) {
    PyErr_SetString(PyExc_TypeError, "%%CLASS%% cannot be overloaded");
    return nullptr;
  }

  // キーワード引数
  static const char* kwlist[] = {
    "name",
    nullptr
  };
  const char* name_str = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
				    const_cast<char**>(kwlist),
				    &name_str) ) {
    return nullptr;
  }
  %%CLASS%% val;
%%STRCMP%%
  return Py%%CLASS%%::ToPyObject(val);
}

// 終了関数
void
%%CLASS%%_dealloc(
  PyObject* self
)
{
  // auto val_obj = reinterpret_cast<%%CLASS%%Object*>(self);
  // 必要なら val_obj->mVal の終了処理を行う．
  Py_TYPE(self)->tp_free(self);
}

// repr() 関数
PyObject*
%%CLASS%%_repr(
  PyObject* self
)
{
  auto val = Py%%CLASS%%::Get(self);
  const char* tmp_str = nullptr;
  switch ( val ) {
%%CONST_REPR%%
  }
  return Py_BuildValue("s", tmp_str);
}

// 比較関数
PyObject*
%%CLASS%%_richcmpfunc(
  PyObject* self,
  PyObject* other,
  int op
)
{
  if ( Py%%CLASS%%::Check(self) &&
       Py%%CLASS%%::Check(other) ) {
    auto val1 = Py%%CLASS%%::Get(self);
    auto val2 = Py%%CLASS%%::Get(other);
    if ( op == Py_EQ ) {
      return PyBool_FromLong(val1 == val2);
    }
    if ( op == Py_NE ) {
      return PyBool_FromLong(val1 != val2);
    }
  }
  Py_RETURN_NOTIMPLEMENTED;
}

// 定数オブジェクトの生成
PyObject*
new_obj(
  %%CLASS%% val
)
{
  auto obj = %%CLASS%%_Type.tp_alloc(&%%CLASS%%_Type, 0);
  auto val_obj = reinterpret_cast<%%CLASS%%Object*>(obj);
  val_obj->mVal = val;
  return obj;
}

// 定数オブジェクトの登録
bool
reg_obj(
  const char* name,
  PyObject* obj
)
{
  if ( PyDict_SetItemString(%%CLASS%%_Type.tp_dict, name, obj) < 0 ) {
    return false;
  }
  return true;
}

END_NONAMESPACE


// @brief 初期化する．
bool
Py%%CLASS%%::init(
  PyObject* m
)
{
  %%CLASS%%_Type.tp_name = "%%CLASS%%";
  %%CLASS%%_Type.tp_basicsize = sizeof(%%CLASS%%Object);
  %%CLASS%%_Type.tp_itemsize = 0;
  %%CLASS%%_Type.tp_dealloc = %%CLASS%%_dealloc;
  %%CLASS%%_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  %%CLASS%%_Type.tp_doc = PyDoc_STR("%%CLASS%% objects");
  %%CLASS%%_Type.tp_richcompare = %%CLASS%%_richcmpfunc;
  %%CLASS%%_Type.tp_new = %%CLASS%%_new;
  %%CLASS%%_Type.tp_repr = %%CLASS%%_repr;
  if ( PyType_Ready(&%%CLASS%%_Type) < 0 ) {
    return false;
  }

  // 型オブジェクトの登録
  if ( !PyModule::reg_type(m, "%%CLASS%%", &%%CLASS%%_Type) ) {
    goto error;
  }

  // 定数オブジェクトの生成
%%NEW_OBJ%%

  // 定数の登録
%%REG_OBJ%%

  return true;

 error:

%%XDECREF%%

  return false;
}

// @brief %%CLASS%% を表す PyObject から %%CLASS%% を取り出す．
bool
Py%%CLASS%%::FromPyObject(
  PyObject* obj,
  %%CLASS%%& val,
  const char* msg
)
{
%%NONEOBJ%%
  if ( !Check(obj) ) {
    if ( msg == nullptr ) {
      msg = "object should be a %%CLASS%% type";
    }
    PyErr_SetString(PyExc_TypeError, msg);
    return false;
  }
  val = Get(obj);
  return true;
}

// @brief %%CLASS%% を表す PyObject を作る．
PyObject*
Py%%CLASS%%::ToPyObject(
  %%CLASS%% val
)
{
  PyObject* obj = nullptr;
  switch ( val ) {
%%TOPYOBJECT%%
  }
  Py_INCREF(obj);
  return obj;
}

// @brief PyObject が %%CLASS%% タイプか調べる．
bool
Py%%CLASS%%::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, _typeobject());
}

// @brief %%CLASS%% を表す PyObject から %%CLASS%% を取り出す．
%%CLASS%%
Py%%CLASS%%::Get(
  PyObject* obj
)
{
  auto val_obj = reinterpret_cast<%%CLASS%%Object*>(obj);
  return val_obj->mVal;
}

// @brief %%CLASS%% を表すオブジェクトの型定義を返す．
PyTypeObject*
Py%%CLASS%%::_typeobject()
{
  return &%%CLASS%%_Type;
}

END_NAMESPACE_%%NAMESPACE%%
