#ifndef PYOUTSTREAM_H
#define PYOUTSTREAM_H

/// @file PyOutStream.h
/// @brief PyOutStream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/OutStream.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyOutStream PyOutStream.h "PyOutStream.h"
/// @brief OutStream を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PyOutStream
{
public:

  using ElemType = OutStream;

public:

  /// @brief OutStream を PyObject* に変換するファンクタクラス
  struct Conv {
    PyObject*
    operator()(
      ElemType&& val ///< [in] 元の値
    );
  };

  // このクラスは Deconv を持たない．


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

  /// @brief OutStream を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    ElemType&& val ///< [in] 元の値
  )
  {
    Conv conv;
    return conv(std::move(val));
  }

  /// @brief PyObject が OutStream タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief OutStream を表す PyObject から OutStream を取り出す．
  /// @return OutStream を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  ElemType&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief OutStream を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYOUTSTREAM_H
