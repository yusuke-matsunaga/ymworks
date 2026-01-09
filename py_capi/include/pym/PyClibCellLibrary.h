#ifndef PYCLIBCELLLIBRARY_H
#define PYCLIBCELLLIBRARY_H

/// @file PyClibCellLibrary.h
/// @brief PyClibCellLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/ClibCellLibrary.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyClibCellLibrary PyClibCellLibrary.h "PyClibCellLibrary.h"
/// @brief ClibCellLibrary を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PyClibCellLibrary
{
public:

  using ElemType = ClibCellLibrary;

public:

  /// @brief ClibCellLibrary を PyObject* に変換するファンクタクラス
  struct Conv {
    PyObject*
    operator()(
      const ElemType& val ///< [in] 元の値
    );
  };

  /// @brief PyObject* から ClibCellLibrary を取り出すファンクタクラス
  struct Deconv {
    bool
    operator()(
      PyObject* obj, ///< [in] Python のオブジェクト
      ElemType& val  ///< [out] 結果を格納する変数
    );
  };


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @return 初期化が成功したら true を返す．
  static
  bool
  init(
    PyObject* m ///< [in] 親のモジュールを表す PyObject
  );

  /// @brief ClibCellLibrary を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    const ElemType& val ///< [in] 元の値
  )
  {
    Conv conv;
    return conv(val);
  }

  /// @brief PyObject から ClibCellLibrary を取り出す．
  /// @return 正しく変換できた時に true を返す．
  static
  bool
  FromPyObject(
    PyObject* obj, ///< [in] Python のオブジェクト
    ElemType& val  ///< [out] 結果を格納する変数
  )
  {
    Deconv deconv;
    return deconv(obj, val);
  }

  /// @brief PyObject が ClibCellLibrary タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief PyObject から ClibCellLibrary を取り出す．
  static
  ElemType
  Get(
    PyObject* obj ///< [in] 対象の Python オブジェクト
  )
  {
    ElemType val;
    if ( PyClibCellLibrary::FromPyObject(obj, val) ) {
      return val;
    }
    PyErr_SetString(PyExc_TypeError, "Could not convert to ClibCellLibrary");
    return val;
  }

  /// @brief ClibCellLibrary を表す PyObject から ClibCellLibrary を取り出す．
  /// @return ClibCellLibrary を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  ElemType&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief ClibCellLibrary を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYCLIBCELLLIBRARY_H
