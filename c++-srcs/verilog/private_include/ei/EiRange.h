#ifndef EIRANGE_H
#define EIRANGE_H

/// @file EiRange.h
/// @brief EiRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbRange.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiRange EiRange.h "EiRange.h"
/// @brief ElbRange の実装クラス
//////////////////////////////////////////////////////////////////////
class EiRange :
  public ElbRange
{
public:

  /// @brief コンストラクタ
  EiRange();

  /// @brief デストラクタ
  ~EiRange();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlRange の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数(ビット幅)を返す．
  SizeType
  size() const override;

  /// @brief MSB の値を返す．
  /// @retval MSB の値 値が確定しているとき
  /// @retval -1 値が確定していない
  int
  left_range_val() const override;

  /// @brief LSB の値を返す．
  /// @retval LSB の値 値が確定しているとき
  /// @retval -1 値が確定していない
  int
  right_range_val() const override;

  /// @brief MSB を表す文字列を返す．
  std::string
  left_range_string() const override;

  /// @brief LSB を表す文字列を返す．
  std::string
  right_range_string() const override;

  /// @brief 範囲のチェック
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(
    int index ///< [in] インデックス
  ) const override;

  /// @brief LSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の LSB からのオフセット
  ) const override;

  /// @brief MSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_roffset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の MSB からのオフセット
  ) const override;

  /// @brief offset の逆関数
  /// @return offset に対応したインデックスを返す．
  int
  index(
    SizeType offset ///< [in] LSB からのオフセット値
  ) const override;

  /// @brief roffset の逆関数
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(
    SizeType roffset ///< [in] MSB からのオフセット値
  ) const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  void
  set(
    const ElbRangeSrc& src ///< [in] 元となる情報
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の範囲定義
  AstRange mAstRange;

  // 範囲の MSB
  AstExpr mLeftRange;

  // 範囲の LSB
  AstExpr mRightRange;

  // 範囲の値
  RangeVal mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class EiRangeArray EiRange.h "EiRange.h"
/// @brief EiRange の配列 + α
//////////////////////////////////////////////////////////////////////
class EiRangeArray
{
public:

  /// @brief コンストラクタ
  EiRangeArray(
    const std::vector<EiRange>& array ///< [in] 範囲のリスト
  );

  // デストラクタ
  ~EiRangeArray();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容にアクセスする関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数を得る．
  SizeType
  size() const
  {
    return mArray.size();
  }

  /// @brief 要素数を計算する
  /// @return サイズを返す．
  SizeType
  elem_size() const
  {
    return mElemSize;
  }

  /// @brief pos 番めの範囲を返す．
  const EiRange*
  range(
    SizeType pos ///< [in] 位置番号
  ) const
  {
    if ( pos >= elem_size() ) {
      throw std::out_of_range{"pos is out of range"};
    }
    return &mArray[pos];
  }

  /// @brief アドレス(オフセット)からインデックスのリストを作る．
  void
  index(
    SizeType offset,             ///< [in] オフセット
    std::vector<int>& index_list ///< [out] インデックスのリスト
  ) const;

  /// @brief インデックスのリストからオフセットを得る．
  /// @retval true オフセットの計算が正しく行えた．
  /// @retval false index_list のいずれかの値が範囲外だった．
  bool
  calc_offset(
    const std::vector<int>& index_list, ///< [in] インデックスのリスト
    SizeType& offset                    ///< [out] index_list の値に対応したオフセット値
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲の配列
  std::vector<EiRange> mArray;

  // 要素数
  SizeType mElemSize;

};

END_NAMESPACE_YM_VERILOG

#endif // EIRANGE_H
