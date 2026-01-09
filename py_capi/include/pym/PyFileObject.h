#ifndef PYFILEOBJECT_H
#define PYFILEOBJECT_H

/// @file PyFileObject.h
/// @brief PyFileObject のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym_config.h"

// ファイルオブジェクトのチェックに io.IOBase を用いる時 1 にする．
#define USE_IOBASE 0


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyFileObject PyFileObject.h "PyFileObject.h"
/// @brief ファイルオブジェクトに関連した関数を提供するクラス
///
/// 静的関数メンバしか持たない．
//////////////////////////////////////////////////////////////////////
class PyFileObject
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 対象がファイルオブジェクトがどうか調べる．
  ///
  /// 純粋なファイルオブジェクトの場合，io.IOBase の派生クラスかどうか
  /// 調べればよいが，tempfile.NamedTemporaryFile() のように一見，
  /// ファイルオブジェクトのようにみえて io.IOBase の派生クラスではない
  /// 場合があるので，ここでは readable() と writable() が実行可能か
  /// どうかで判断する．
  static
  bool
  Check(
    PyObject* obj,  ///< [in] 対象の Python オブジェクト
    bool& readable, ///< [out] 読み込み可能の時 true となるフラグ
    bool& writable  ///< [out] 書き込み可能の時 true となるフラグ
  )
  {
#if USE_IOBASE
    // 1. io モジュールをインポートする
    auto io_module = PyImport_ImportModule("io");
    if ( io_module == nullptr ) {
      throw std::runtime_error{"Could not import io module"};
    }

    // 2. io.IOBase クラスを取得する
    auto io_class = PyObject_GetAttrString(io_module, "IOBase");
    if ( io_class == nullptr ) {
      Py_DECREF(io_module);
      throw std::runtime_error{"Could not find io.IOBase"};
    }

    // 3. インスタンスチェックを行う
    auto result = PyObject_IsInstance(obj, io_class);

    // 4. 参照カウントを減らす
    Py_DECREF(io_module);
    Py_DECREF(io_class);

    switch ( result ) {
    case 0:
      return false;
    case 1:
      return true;
    }
    throw std::runtime_error{"Error in PyObject_IsInstance()"};
#endif
    try {
      readable = is_readable(obj);
    }
    catch ( std::runtime_error ) {
      PyErr_Clear();
      return false;
    }
    try {
      writable = is_writable(obj);
    }
    catch ( std::runtime_error ) {
      PyErr_Clear();
      return false;
    }
    // そもそもどちらも false ならファイルオブジェクトとして使えない．
    return readable || writable;
  }

  /// @brief 読み込み可能かどうか調べる．
  ///
  /// 具体的には obj.readable() を呼んで結果を調べる．
  /// そのそも上記のメソッド呼び出しが失敗する場合は例外を送出する．
  static
  bool
  is_readable(
    PyObject* obj ///< [in] 対象の Python オブジェクト
  )
  {
    auto res_obj = PyObject_CallMethod(obj, "readable", nullptr);
    if ( res_obj == nullptr ) {
      throw std::runtime_error{"Error in PyObject_CallMethod(obj, \"readable\")"};
    }
    bool result = false;
    if ( res_obj == Py_True ) {
      result = true;
    }
    else if ( res_obj == Py_False ) {
      result = false;
    }
    else {
      Py_DECREF(res_obj);
      throw std::runtime_error{"obj.readable() returned non Boolean value"};
    }
    Py_DECREF(res_obj);
    return result;
  }

  /// @brief 書き込み可能かどうか調べる．
  ///
  /// 具体的には obj.writable() を呼んで結果を調べる．
  /// そのそも上記のメソッド呼び出しが失敗する場合は例外を送出する．
  static
  bool
  is_writable(
    PyObject* obj ///< [in] 対象の Python オブジェクト
  )
  {
    auto res_obj = PyObject_CallMethod(obj, "writable", nullptr);
    if ( res_obj == nullptr ) {
      throw std::runtime_error{"Error in PyObject_CallMethod(obj, \"writable\")"};
    }
    bool result = false;
    if ( res_obj == Py_True ) {
      result = true;
    }
    else if ( res_obj == Py_False ) {
      result = false;
    }
    else {
      Py_DECREF(res_obj);
      throw std::runtime_error{"obj.wriable() returned non Boolean value"};
    }
    Py_DECREF(res_obj);
    return result;
  }

};
END_NAMESPACE_YM

#endif // PYFILEOBJECT_H
