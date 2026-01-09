
/// @file PyOutStream.cc
/// @brief PyOutStream の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "pym/PyOutStream.h"
#include "pym/PyString.h"
#include "pym/PyModule.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// Python 用のオブジェクト定義
// この構造体は同じサイズのヒープから作られるので
// mVal のコンストラクタは起動されないことに注意．
// そのためあとでコンストラクタを明示的に起動する必要がある．
// またメモリを開放するときにも明示的にデストラクタを起動する必要がある．
struct OutStream_Object
{
  PyObject_HEAD
  OutStream mVal;
};

// Python 用のタイプ定義
PyTypeObject OutStream_Type = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  // 残りは PyOutStream::init() 中で初期化する．
};

// 終了関数
void
dealloc_func(
  PyObject* self
)
{
  auto obj = reinterpret_cast<OutStream_Object*>(self);
  obj->mVal.~OutStream();
  Py_TYPE(self)->tp_free(self);
}

// make OutStream of stdout type
PyObject*
stdout(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyOutStream::ToPyObject(OutStream::stdout());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make OutStream of stderr type
PyObject*
stderr(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyOutStream::ToPyObject(OutStream::stderr());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make OutStream of file type
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
    return PyOutStream::ToPyObject(OutStream::file(filename));
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// make OutStream of string type
PyObject*
string(
  PyObject* Py_UNUSED(self),
  PyObject* Py_UNUSED(args)
)
{
  try {
    return PyOutStream::ToPyObject(OutStream::string());
  }
  catch ( std::exception err ) {
    std::ostringstream buf;
    buf << "exception" << ": " << err.what();
    PyErr_SetString(PyExc_ValueError, buf.str().c_str());
    return nullptr;
  }
}

// return string contents
PyObject*
str(
  PyObject* self,
  PyObject* Py_UNUSED(args)
)
{
  auto& val = PyOutStream::_get_ref(self);
  try {
    return PyString::ToPyObject(val.str());
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
  {"stdout",
   stdout,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make OutStream of stdout type")},
  {"stderr",
   stderr,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make OutStream of stderr type")},
  {"file",
   reinterpret_cast<PyCFunction>(file),
   METH_VARARGS | METH_KEYWORDS | METH_STATIC,
   PyDoc_STR("make OutStream of file type")},
  {"string",
   string,
   METH_NOARGS | METH_STATIC,
   PyDoc_STR("make OutStream of string type")},
  {"str",
   str,
   METH_NOARGS,
   PyDoc_STR("return string contents")},
  // end-marker
  {nullptr, nullptr, 0, nullptr}
};

END_NONAMESPACE


// @brief OutStream オブジェクトを使用可能にする．
bool
PyOutStream::init(
  PyObject* m
)
{
  OutStream_Type.tp_name = "OutStream";
  OutStream_Type.tp_basicsize = sizeof(OutStream_Object);
  OutStream_Type.tp_itemsize = 0;
  OutStream_Type.tp_dealloc = dealloc_func;
  OutStream_Type.tp_flags = Py_TPFLAGS_DEFAULT;
  OutStream_Type.tp_doc = PyDoc_STR("Python extended object for OutStream");
  OutStream_Type.tp_methods = methods;
  if ( !PyModule::reg_type(m, "OutStream", &OutStream_Type) ) {
    goto error;
  }

  return true;

 error:

  return false;
}

// OutStream を PyObject に変換する．
PyObject*
PyOutStream::Conv::operator()(
  ElemType&& val ///< [in] 元の値
)
{
  auto type = PyOutStream::_typeobject();
  auto obj = type->tp_alloc(type, 0);
  auto my_obj = reinterpret_cast<OutStream_Object*>(obj);
  new (&my_obj->mVal) OutStream(std::move(val));
  return obj;
}

// @brief PyObject が OutStream タイプか調べる．
bool
PyOutStream::Check(
  PyObject* obj
)
{
  return Py_IS_TYPE(obj, &OutStream_Type);
}

// @brief PyObject から OutStream を取り出す．
OutStream&
PyOutStream::_get_ref(
  PyObject* obj
)
{
  auto my_obj = reinterpret_cast<OutStream_Object*>(obj);
  return my_obj->mVal;
}

// @brief OutStream を表すオブジェクトの型定義を返す．
PyTypeObject*
PyOutStream::_typeobject()
{
  return &OutStream_Type;
}

END_NAMESPACE_YM
