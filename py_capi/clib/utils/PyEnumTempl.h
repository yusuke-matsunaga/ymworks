#ifndef PY%%CAP_CLASS%%_H
#define PY%%CAP_CLASS%%_H

/// @file Py%%CLASS%%.h
/// @brief Py%%CLASS%% のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) %%YEAR%% Yusuke Matsunaga
/// All rights reserved.

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "%%HEADER_FILE%%"


BEGIN_NAMESPACE_%%NAMESPACE%%

//////////////////////////////////////////////////////////////////////
/// @class Py%%CLASS%% Py%%CLASS%%.h "Py%%CLASS%%.h"
/// @brief Python 用の %%CLASS%% 拡張
///
/// 複数の関数をひとまとめにしているだけなので実は名前空間として用いている．
//////////////////////////////////////////////////////////////////////
class Py%%CLASS%%
{
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

  /// @brief %%CLASS%% を表す PyObject から %%CLASS%% を取り出す．
  /// @return 変換が成功したら true を返す．
  ///
  /// エラーの場合には Python 例外をセットする．
  static
  bool
  FromPyObject(
    PyObject* obj,            ///< [in] %%CLASS%% を表す PyObject
    %%CLASS%%& val,      ///< [out] 変換された %%CLASS%% を格納する変数
    const char* msg = nullptr ///< [in] エラーメッセージ(省略時にはデフォルト値を使う)
  );

  /// @brief %%CLASS%% を表す PyObject を作る．
  /// @return 生成した PyObject を返す．
  ///
  /// 返り値は新しい参照が返される．
  static
  PyObject*
  ToPyObject(
    %%CLASS%% val ///< [in] 値
  );

  /// @brief PyObject が %%CLASS%% タイプか調べる．
  static
  bool
  Check(
    PyObject* obj ///< [in] 対象の PyObject
  );

  /// @brief %%CLASS%% を表す PyObject から %%CLASS%% を取り出す．
  /// @return %%CLASS%% を返す．
  ///
  /// Check(obj) == true であると仮定している．
  static
  %%CLASS%%
  Get(
    PyObject* obj ///< [in] 変換元の PyObject
  );

  /// @brief %%CLASS%% を表すオブジェクトの型定義を返す．
  static
  PyTypeObject*
  _typeobject();

};

END_NAMESPACE_%%NAMESPACE%%

#endif // PY%%CAP_CLASS%%_H
