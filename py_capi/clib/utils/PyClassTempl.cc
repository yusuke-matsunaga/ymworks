
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
  const %%IMPL%%* mVal;
};

// Python 用のタイプ定義
PyTypeObject %%CLASS%%_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
};


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

  return Py_BuildValue("s", tmp_str);
}

// 一般的なメソッド
PyObject*
%%CLASS%%_method1(
  PyObject* self,
  PyObject* args
)
{
  char* tmp_str = nullptr;
  if ( !PyArg_ParseTuple(args, "s", &tmp_str) ) {
    return nullptr;
  }

  auto val = Py%%CLASS%%::Get(self);
  Py_RETURNNONE;
}

// 一般的なメソッド
PyObject*
%%CLASS%%_method2(
  PyObject* self,
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "name",
    nullptr
  };

  char* tmp_str = nullptr;
  if ( !PyArg_ParseTuple(args, kwds, "s",
			 const_cast<char**>(kwlist),
			 &tmp_str) ) {
    return nullptr;
  }

  auto val = Py%%CLASS%%::Get(self);
  Py_RETURNNONE;
}

// メソッド定義
PyMethodDef %%CLASS%%_methods[] = {
  {"method1", %%CLASS%%_method1, METH_VARARGS,
   PyDoc_STR("")},
  {"method2",
   reinterpret_cast<PyCFunction>(%%CLASS%%_method2),
   METH_VARARGS | METH_KEYWORDS,
   PyDoc_STR("")},
  {nullptr, nullptr, 0, nullptr}
};

// getter 関数
PyObject*
%%CLASS%%_member1(
  PyObject* self,
  void* Py_UNUSED(closure)
)
{
  auto val = Py%%CLASS%%::Get(self);


}

// getter/setter 関数テーブル
PyGetSetDef %%CLASS%%_getsetters[] = {
  {"member1", %%CLASS%%_member1, nullptr,
   PyDoc_STR("member1"), nullptr},
  {nullptr, nullptr, nullptr, nullptr, nullptr}
};

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
  %%CLASS%%_Type.tp_methods = %%CLASS%%_methods;
  %%CLASS%%_Type.tp_getset = %%CLASS%%_getsetters;
  %%CLASS%%_Type.tp_new = %%CLASS%%_new;
  %%CLASS%%_Type.tp_repr = %%CLASS%%_repr;
  if ( PyType_Ready(&%%CLASS%%_Type) < 0 ) {
    return false;
  }

  return true;
}

// @brief %%CLASS%% を表す PyObject から %%CLASS%% を取り出す．
bool
Py%%CLASS%%::FromPyObject(
  PyObject* obj,
  %%CLASS%%& val,
  const char* msg
)
{
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
  PyObject* obj = %%CLASS%%_Type.tp_alloc(&%%CLASS%%_Type, 0);
  auto val_obj = reinterpret_cast<%%CLASS%%Object*>(obj);

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
