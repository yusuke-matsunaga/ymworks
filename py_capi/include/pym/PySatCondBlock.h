#ifndef PYSATCONDBLOCK_H
#define PYSATCONDBLOCK_H

/// @file PySatCondBlock.h
/// @brief PySatCondBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ym/SatCondBlock.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PySatCondBlock PySatCondBlock.h "PySatCondBlock.h"
/// @brief SatCondBlock を Python から使用するための拡張
///
/// 実際には static メンバ関数しか持たないのでクラスではない．
//////////////////////////////////////////////////////////////////////
class PySatCondBlock
{
public:

  using ElemType = SatCondBlock;

public:

  /// @brief SatCondBlock を PyObject* に変換するファンクタクラス
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

  /// @brief SatCondBlock を表す PyObject を作る．
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

  /// @brief PyObject が SatCondBlock タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief SatCondBlock を表す PyObject から SatCondBlock を取り出す．
  /// @return SatCondBlock を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  ElemType&
  _get_ref(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief SatCondBlock を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_YM

#endif // PYSATCONDBLOCK_H
