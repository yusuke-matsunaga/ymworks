#ifndef CILUT_H
#define CILUT_H

/// @file　CiLut.h
/// @brief CiLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/clib.h"


BEGIN_NAMESPACE_YM_CLIB

class CiLutTemplate;
class CiStLut;
class Serializer;
class Deserializer;

//////////////////////////////////////////////////////////////////////
/// @class CiLut CiLut.h "CiLut.h"
/// @brief ルックアップテーブルの実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class CiLut
{
protected:

  /// @brief restore() 用のコンストラクタ
  CiLut() = default;

  /// @brief コンストラクタ
  CiLut(
    const CiLutTemplate* lut_template ///< [in] テンプレート番号
  ) : mTemplate{lut_template}
  {
  }


public:

  /// @brief インスタンスを生成するクラスメソッド
  static
  std::unique_ptr<CiLut>
  new_lut(
    const CiLutTemplate* lut_template,      ///< [in] テンプレート
    const std::vector<double>& value_array, ///< [in] 値の配列
    const std::vector<double>& index_array1 ///< [in] インデックス値のリスト1
    = std::vector<double>{},
    const std::vector<double>& index_array2 ///< [in] インデックス値のリスト2
    = std::vector<double>{},
    const std::vector<double>& index_array3 ///< [in] インデックス値のリスト3
    = std::vector<double>{}
  );

  /// @brief インスタンスを生成するクラスメソッド
  static
  std::unique_ptr<CiStLut>
  new_stlut(
    const CiLutTemplate* lut_template,       ///< [in] テンプレート
    const std::vector<double>& value_array,  ///< [in] 値の配列
    const std::vector<double>& index_array1, ///< [in] インデックス値のリスト1
    const std::vector<double>& index_array2  ///< [in] インデックス値のリスト2
  );

  /// @brief デストラクタ
  virtual
  ~CiLut() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  const CiLutTemplate*
  lut_template() const
  {
    return mTemplate;
  }

  /// @brief 次元数の取得
  virtual
  SizeType
  dimension() const = 0;

  /// @brief インデックス数の取得
  virtual
  SizeType
  index_num(
    SizeType var ///< [in] 変数番号 ( 0 <= var < dimension() )
  ) const = 0;

  /// @brief インデックス値の取得
  virtual
  double
  index(
    SizeType var, ///< [in] 変数番号 ( 0 <= var < dimension() )
    SizeType pos  ///< [in] 位置番号 ( 0 <= pos < index_num(var) )
  ) const = 0;

  /// @brief 格子点の値の取得
  virtual
  double
  grid_value(
    const std::vector<SizeType>& pos_array ///< [in] 格子点座標
                                           ///< サイズは dimension() と同じ
  ) const = 0;

  /// @brief 値の取得
  virtual
  double
  value(
    const std::vector<double>& val_array ///< [in] 入力の値の配列
                                         ///< サイズは dimension() と同じ
  ) const = 0;


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
  virtual
  void
  dump(
    Serializer& s ///< [in] シリアライザ
  ) const = 0;

  /// @brief 内容を復元する．
  static
  std::unique_ptr<CiLut>
  restore(
    Deserializer& s   ///< [in] デシリアライザ
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際のダンプを行う関数
  void
  dump_common(
    Serializer& s, ///< [in] シリアライザ
    int d          ///< [in] 次元数
  ) const;

  /// @brief 内容を読み込む．
  void
  restore_common(
    Deserializer& s ///< [in] デシリアライザ
  );

  /// @brief val に対応する区間を求める．
  static
  SizeType
  search(
    double val,
    const std::vector<double>& index_array
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief restore() の下請け関数
  virtual
  void
  _restore(
    Deserializer& s ///< [in] デシリアライザ
  ) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CiLutTemplate* mTemplate{nullptr};

};

END_NAMESPACE_YM_CLIB

#endif // CILUT_H
