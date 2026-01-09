#ifndef PYINSTREAM_H
#define PYINSTREAM_H

/// @file PyInStream.h
/// @brief PyInStream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/InStream.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyInStream PyInStream.h "PyInStream.h"
/// @brief InStream を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PyInStream
{
public:

  using ElemType = InStream;

public:

  /// @brief InStream を PyObject* に変換するファンクタクラス
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

  /// @brief InStream を表す PyObject を作る．
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

  /// @brief PyObject が InStream タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief InStream を表す PyObject から InStream を取り出す．
  /// @return InStream を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  ElemType&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief InStream を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYINSTREAM_H
