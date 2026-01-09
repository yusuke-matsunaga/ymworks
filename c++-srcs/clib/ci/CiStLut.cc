
/// @file CiStLut.cc
/// @brief CiStLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ci/CiStLut.h"
#include "ci/CiLutTemplate.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiStLut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiStLut::CiStLut(
  const CiLutTemplate* lut_template,
  const std::vector<double>& value_array,
  const std::vector<double>& index_array1,
  const std::vector<double>& index_array2
) : CiLut{lut_template},
    mValueArray{value_array},
    mIndexArray{index_array1, index_array2}
{
  SizeType n = mIndexArray[0].size() * mIndexArray[1].size();
  if ( value_array.size() != n ) {
    throw std::invalid_argument{"value_array.size() is not consistent with index_array1.size() and index_arary2.size()"};
  }
}

// @brief 次元数の取得
SizeType
CiStLut::dimension() const
{
  return 2;
}

// @brief インデックス数の取得
SizeType
CiStLut::index_num(
  SizeType var
) const
{
  if ( var < 0 || 2 <= var ) {
    throw std::invalid_argument{"var should 0 or 1"};
  }
  return mIndexArray[var].size();
}

// @brief インデックス値の取得
double
CiStLut::index(
  SizeType var,
  SizeType pos
) const
{
  if ( var < 0 || 2 <= var ) {
    throw std::invalid_argument{"var should 0 or 1"};
  }
  if ( pos < 0 || index_num(var) <= pos ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mIndexArray[var][pos];
}

// @brief 格子点の値の取得
double
CiStLut::grid_value(
  const std::vector<SizeType>& pos_array
) const
{
  if ( pos_array.size() != 2 ) {
    throw std::invalid_argument{"pos_array.size() should be 2"};
  }
  auto pos1 = pos_array[0];
  auto pos2 = pos_array[1];
  if ( pos1 < 0 || index_num(0) <= pos1 ) {
    throw std::out_of_range{"pos_array[0] is out of range"};
  }
  if ( pos2 < 0 || index_num(1) <= pos2 ) {
    throw std::out_of_range{"pos_array[1] is out of range"};
  }
  return mValueArray[idx(pos1, pos2)];
}

// @brief 値の取得
double
CiStLut::value(
  const std::vector<double>& val_array
) const
{
  if ( val_array.size() != 2 ) {
    throw std::invalid_argument{"val_array.size() should be 2"};
  }
  double val1 = val_array[0];
  double val2 = val_array[1];
  return value(val1, val2);
}

// @brief 値の取得
double
CiStLut::value(
  double val1,
  double val2
) const
{
  auto idx1_a = search(val1, mIndexArray[0]);
  auto idx1_b = idx1_a + 1;
  double x0 = mIndexArray[0][idx1_a];
  double x1 = mIndexArray[0][idx1_b];

  auto idx2_a = search(val2, mIndexArray[1]);
  auto idx2_b = idx2_a + 1;
  double y0 = mIndexArray[1][idx2_a];
  double y1 = mIndexArray[1][idx2_b];

  // 単純な線形補間
  double wx  = x1 - x0;
  double dx0 = (val1 - x0) / wx;
  double dx1 = (x1 - val1) / wx;
  double wy  = y1 - y0;
  double dy0 = (val2 - y0) / wy;
  double dy1 = (y1 - val2) / wy;
  double val_00 = mValueArray[idx(idx1_a, idx2_a)];
  double val_01 = mValueArray[idx(idx1_a, idx2_b)];
  double val_10 = mValueArray[idx(idx1_b, idx2_a)];
  double val_11 = mValueArray[idx(idx1_b, idx2_b)];

  return dx1 * (dy1 * val_00 + dy0 * val_01) +
         dx0 * (dy1 * val_10 + dy0 * val_11);
}

// @brief 内容をシリアライズする．
void
CiStLut::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief 内容をバイナリダンプする．
void
CiStLut::dump(
  Serializer& s
) const
{
  dump_common(s, 2);
  s.dump(mIndexArray[0]);
  s.dump(mIndexArray[1]);
  s.dump(mValueArray);
}

// @brief 内容を復元する．
std::unique_ptr<CiStLut>
CiStLut::restore(
  Deserializer& s
)
{
  std::uint8_t d;
  s.restore(d);
  ASSERT_COND( d == 2 );
  auto lut = std::unique_ptr<CiStLut>{new CiStLut};
  lut->_restore(s);
  return lut;
}

// @brief 内容を読み込む．
void
CiStLut::_restore(
  Deserializer& s
)
{
  restore_common(s);
  s.restore(mIndexArray[0]);
  s.restore(mIndexArray[1]);
  s.restore(mValueArray);
}

END_NAMESPACE_YM_CLIB
