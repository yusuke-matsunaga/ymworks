#ifndef CISTLUT_H
#define CISTLUT_H

/// @file　CiStLut.h
/// @brief CiStLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiLut.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
/// @class CiStLut CiLut.h "CiLut.h"
/// @brief 標準的な2次元のルックアップテーブルを表すクラス
///
/// ほぼ CiLut2D と同一だが第1変数が total_output_net_capacitance，
/// 第2変数が input_net_transition と仮定している．
//////////////////////////////////////////////////////////////////////
class CiStLut :
  public CiLut
{
public:

  /// @brief restore() 用のコンストラクタ
  CiStLut() = default;

  /// @brief コンストラクタ
  CiStLut(
    const CiLutTemplate* lut_template,       ///< [in] テンプレート
    const std::vector<double>& value_array,  ///< [in] 値の配列
    const std::vector<double>& index_array1, ///< [in] 変数1のインデックスの配列
    const std::vector<double>& index_array2  ///< [in] 変数2のインデックスの配列
  );

  /// @brief デストラクタ
  ~CiStLut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  SizeType
  dimension() const override;

  /// @brief インデックス数の取得
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const override;

  /// @brief インデックス値の取得
  double
  index(
    SizeType var, ///< [in] 変数番号 ( 0 <= var < dimension() )
    SizeType pos  ///< [in] 位置番号 ( 0 <= pos < index_num(var) )
  ) const override;

  /// @brief 格子点の値の取得
  double
  grid_value(
    const std::vector<SizeType>& pos_array ///< [in] 格子点座標
                                           ///< サイズは dimension() と同じ
  ) const override;

  /// @brief 値の取得
  double
  value(
    const std::vector<double>& val_array ///< [in] 入力の値の配列
                                         ///< サイズは dimension() と同じ
  ) const override;

  /// @brief 値の取得
  double
  value(
    double val1, ///< [in] 入力1の値
    double val2  ///< [in] 入力2の値
  ) const;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をシリアライズする．
  void
  serialize(
    Serializer& s ///< [in] シリアライザ
  ) const;

  /// @brief 内容をバイナリダンプする．
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const override;

  /// @brief 内容を復元する．
  static
  std::unique_ptr<CiStLut>
  restore(
    Deserializer& s   ///< [in] デシリアライザ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mValueArray のインデックスを計算する．
  SizeType
  idx(
    SizeType idx1, ///< [in] 1番めのインデックス
    SizeType idx2  ///< [in] 2番めのインデックス
  ) const
  {
    return idx1 * index_num(1) + idx2;
  }

  /// @brief restore() の下請け関数
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスの配列の配列
  std::vector<double> mIndexArray[2];

  // 格子点の値の配列
  std::vector<double> mValueArray;

};

END_NAMESPACE_YM_CLIB

#endif // CISTLUT_H
