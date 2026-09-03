#ifndef RANGEVAL_H
#define RANGEVAL_H

/// @file RangeVal.h
/// @brief RangeVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/verilog.h"
#include "ym/vl/AstRange.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

/// @brief 範囲を表す型
class RangeVal
{
public:

  int left{0};
  int right{0};

  /// @brief 要素数(ビット幅)を返す．
  SizeType
  calc_size() const
  {
    SizeType ans = 0;
    if ( left >= right ) {
      ans = left - right + 1;
    }
    else {
      ans = right - left + 1;
    }
    return ans;
  }

  /// @brief 範囲のチェック
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  is_in(
    int index ///< [in] インデックス
  ) const
  {
    if ( left >= right ) {
      return right <= index && index <= left;
    }
    else {
      return right >= index && index >= left;
    }
  }

  /// @brief LSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_offset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の LSB からのオフセット
  ) const
  {
    if ( left >= right ) {
      if ( right <= index && index <= left ) {
	offset = index - right;
	return true;
      }
    }
    else {
      if ( right >= index && index >= left ) {
	offset = right - index;
	return true;
      }
    }
    return false;
  }

  /// @brief MSB からのオフセット値の取得
  /// @retval true index が範囲内に入っている．
  /// @retval false index が範囲外
  bool
  calc_roffset(
    int index,       ///< [in] インデックス
    SizeType& offset ///< [out] index の MSB からのオフセット
  ) const
  {
    if ( left >= right ) {
      if ( right <= index && index <= left ) {
	offset = left - index;
	return true;
      }
    }
    else {
      if ( right >= index && index >= left ) {
	offset = index - left;
	return true;
      }
    }
    return false;
  }

  /// @brief offset の逆関数
  /// @return offset に対応したインデックスを返す．
  int
  index(
    SizeType offset ///< [in] LSB からのオフセット値
  ) const
  {
    if ( left >= right ) {
      return offset + right;
    }
    else {
      return right - offset;
    }
  }

  /// @brief roffset の逆関数
  /// @return roffset に対応したインデックスを返す．
  int
  rindex(
    SizeType roffset ///< [in] MSB からのオフセット値
  ) const
  {
    if ( left >= right ) {
      return left - roffset;
    }
    else {
      return roffset + left;
    }
  }

  /// @brief left_range >= right_range の時に true を返す．
  bool
  is_big_endian() const
  {
    return left >= right;
  }

  /// @brief left_range <= right_range の時に true を返す．
  bool
  is_little_endian() const
  {
    return right >= left;
  }

};

/// @brief 範囲の情報
class RangeInfo :
  public RangeVal
{
public:

  /// @brief コンストラクタ
  RangeInfo(
    const AstRange& ast_range, ///< [in] パース木の範囲定義
    const RangeVal& range      ///< [in] 範囲の値
  ) : RangeVal{range},
      mAstRange{ast_range}
  {
  }

  /// @brief デストラクタ
  ~RangeInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 範囲のMSBを表す文字列の取得
  std::string
  left_string() const
  {
    return mAstRange.left().decompile();
  }

  /// @brief 範囲のLSBを表す文字列の取得
  std::string
  right_string() const
  {
    return mAstRange.right().decompile();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の範囲定義
  AstRange mAstRange;

};

END_NAMESPACE_YM_VERILOG

#endif // RANGEVAL_H
