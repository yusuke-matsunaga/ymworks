
/// @file EiRange.cc
/// @brief EiRange の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiRange.h"

#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiRange
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRange::EiRange()
{
}

// @brief デストラクタ
EiRange::~EiRange()
{
}

// @brief 値を設定する．
void
EiRange::set(
  const ElbRangeSrc& src
)
{
  mPtRange = src.pt_range();
  mLeftRange = src.left_range();
  mRightRange = src.right_range();
  mVal.left = src.left_range_val();
  mVal.right = src.right_range_val();
}

// @brief 型の取得
VpiObjType
EiRange::type() const
{
  return VpiObjType::Range;
}

// @brief ファイル位置を返す．
FileRegion
EiRange::file_region() const
{
  return mPtRange->file_region();
}

// @brief 要素数(ビット幅)を返す．
SizeType
EiRange::size() const
{
  return mVal.calc_size();
}

// @brief MSB の値を返す．
int
EiRange::left_range_val() const
{
  return mVal.left;
}

// @brief LSB の値を返す．
int
EiRange::right_range_val() const
{
  return mVal.right;
}

// @brief MSB を表す文字列を返す．
std::string
EiRange::left_range_string() const
{
  return mLeftRange->decompile();
}

// @brief LSB を表す文字列を返す．
std::string
EiRange::right_range_string() const
{
  return mRightRange->decompile();
}

// @brief 範囲のチェック
bool
EiRange::is_in(
  int index
) const
{
  return mVal.is_in(index);
}

// @brief LSB からのオフセット値の取得
bool
EiRange::calc_offset(
  int index,
  SizeType& offset
) const
{
  return mVal.calc_offset(index, offset);
}

// @brief MSB からのオフセット値の取得
bool
EiRange::calc_roffset(
  int index,
  SizeType& offset
) const
{
  return mVal.calc_roffset(index, offset);
}

// @brief offset の逆関数
int
EiRange::index(
  SizeType offset
) const
{
  return mVal.index(offset);
}

// @brief roffset の逆関数
int
EiRange::rindex(
  SizeType roffset
) const
{
  return mVal.rindex(roffset);
}


//////////////////////////////////////////////////////////////////////
// EiRange の配列
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRangeArray::EiRangeArray(
  const std::vector<EiRange>& array
) : mArray{array}
{
  // 要素数を計算する．
  mElemSize = 1;
  for ( const auto& range: mArray ) {
    mElemSize *= range.size();
  }
}

// デストラクタ
EiRangeArray::~EiRangeArray()
{
}

// @brief アドレス(オフセット)からインデックスの配列を作る．
void
EiRangeArray::index(
  SizeType offset,
  std::vector<int>& index_list
) const
{
  SizeType n = size();
  index_list.resize(n);
  for ( SizeType i = n; i -- > 0; ) {
    auto r = range(i);
    SizeType k = r->size();
    SizeType offset1 = offset % k;
    offset /= k;
    index_list[i] = r->rindex(offset1);
  }
}

// @brief インデックスのリストからオフセットを得る．
bool
EiRangeArray::calc_offset(
  const std::vector<int>& index_list,
  SizeType& offset
) const
{
  SizeType n = size();
  if ( index_list.size() != n ) {
    // そもそもインデックス配列のサイズが違う．
    return false;
  }

  offset = 0;
  for ( SizeType i = 0; i < n; ++ i ) {
    auto r = range(i);
    SizeType k = r->size();
    offset *= k;
    SizeType offset1;
    if ( r->calc_roffset(index_list[i], offset1) ) {
      offset += offset1;
    }
    else {
      // インデックスが範囲外だった．
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_VERILOG
