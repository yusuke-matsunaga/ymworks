
/// @file PyInStream.cc
/// @brief PyInStream の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyInStream.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct InStream_Object
{
  PyObject_HEAD
  InStream mVal;
};

// Python 用のタイプ定義
PyTypeObject InStream_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyInStream::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<InStream_Object*>(self);
  obj->mVal.~InStream();
  Py_TYPE(self)->tp_free(self);
}

// make InStream of stdin type
PyObject*
stdin(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyInStream::ToPyObject(InStream::stdin());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make InStream of file type
PyObject*
file(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "filename",
    nullptr
  };
  const char* filename_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &filename_tmp) ) {
    return nullptr;
  }
  std::string filename;
  if ( filename_tmp != nullptr ) {
    filename = std::string(filename_tmp);
  }
  try {
    return PyInStream::ToPyObject(InStream::file(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make InStrean of string type
PyObject*
string(
  PyObject* Py_UNUSED(self),
  PyObject* args,
  PyObject* kwds
)
{
  static const char* kwlist[] = {
    "str",
    nullptr
  };
  const char* str_tmp = nullptr;
  if ( !PyArg_ParseTupleAndKeywords(args, kwds, "s",
                                    const_cast<char**>(kwlist),
                                    &str_tmp) ) {
    return nullptr;
  }
  std::string str;
  if ( str_tmp != nullptr ) {
    str = std::string(str_tmp);
  }
  try {
    return PyInStream::ToPyObject(InStream::string(str));
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
  {"stdin",
   stdin,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make InStream of stdin type")},
  {"file",
   reinterpret_cast<PyCFunction>(file),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make InStream of file type")},
  {"string",
   reinterpret_cast<PyCFunction>(string),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make InStrean of string type")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

END_NONAMESPACE


// @brief InStream オブジェクトを使用可能にする．
bool
PyInStream::init(
  PyObject* m
)
{
  InStream_Type.tp_name = "InStream";
  InStream_Type.tp_basicsize = sizeof(InStream_Object);
  InStream_Type.tp_itemsize = 0;
  InStream_Type.tp_dealloc = dealloc_func;
  InStream_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  InStream_Type.tp_doc = PyDoc_STR("Python extended object for InStream");
  InStream_Type.tp_methods = methods;
  if ( !PyModule::reg_type(m, "InStream", &InStream_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// InStream を PyObject に変換する．
PyObject*
PyInStream::Conv::operator()(
  ElemType&& val ///< [in] 元の値
)
{
  auto type = PyInStream::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<InStream_Object*>(obj);
  new (&my_obj->mVal) InStream(std::move(val));
  return obj;
}

// @brief PyObject が InStream タイプか調べる．
bool
PyInStream::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &InStream_Type);
}

// @brief PyObject から InStream を取り出す．
InStream&
PyInStream::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<InStream_Object*>(obj);
  return my_obj->mVal;
}

// @brief InStream を表すオブジェクトの型定義を返す．
PyTypeObject*
PyInStream::_typeobject()
{
  return &InStream_Type;
}

END_NAMESPACE_YM
