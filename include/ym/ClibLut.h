#ifndef YM_CLIBLUT_H
#define YM_CLIBLUT_H

/// @file ym/ClibLut.h
/// @brief ClibLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellElem.h"


BEGIN_NAMESPACE_YM_CLIB

class CiLut;

//////////////////////////////////////////////////////////////////////
/// @ingroup ClibGroup
/// @class ClibLut ClibLut.h "ym/ClibLut.h"
/// @brief ルックアップテーブル(LUT)を表すクラス
//////////////////////////////////////////////////////////////////////
class ClibLut :
  public ClibCellElem<CiLut>
{
public:

  /// @brief 空のコンストラクタ
  ///
  /// - 不正値となる．
  ClibLut() = default;

  /// @brief 内容を指定したコンストラクタ
  ClibLut(
    const CiCell* cell, ///< [in] 親のセル
    const CiLut* impl   ///< [in] 本体
  );

  /// @brief デストラクタ
  ~ClibLut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 属性の取得
  /// @{
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  /// @exception std::logic_error is_valid() = false の場合
  SizeType
  dimension() const;

  /// @brief 変数型の取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  /// @sa ClibVarType
  ClibVarType
  variable_type(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const;

  /// @brief インデックス数の取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const;

  /// @brief インデックス値の取得
  /// @exception std::logic_error is_valid() = false の場合
  /// @exception std::out_of_range 範囲外のアクセス
  double
  index(
    SizeType var, ///< [in] 変数番号 ( 0 <= var < dimension() )
    SizeType pos  ///< [in] 位置番号 ( 0 <= pos < index_num(var) )
  ) const;

  /// @brief 格子点の値の取得
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - pos_array のサイズは dimension() と同じ
  double
  grid_value(
    const std::vector<SizeType>& pos_array ///< [in] pos_array 格子点座標
  ) const;

  /// @brief 値の取得
  /// @exception std::logic_error is_valid() = false の場合
  ///
  /// - val_array のサイズは dimension() と同じ
  double
  value(
    const std::vector<double>& val_array ///< [in] 入力の値の配列
  ) const;

  //////////////////////////////////////////////////////////////////////
  /// @}
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_CLIB

#endif // YM_CLIBLUT_H
