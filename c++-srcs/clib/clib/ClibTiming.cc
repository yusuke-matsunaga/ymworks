
/// @file ClibTiming.cc
/// @brief ClibTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibTiming.h"
#include "ym/ClibLut.h"
#include "ci/CiTiming.h"
#include "ci/CiCellLibrary.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibTiming
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
ClibTimingType
ClibTiming::type() const
{
  _check_valid();
  return _impl()->type();
}

// @brief タイミング条件式の取得
Expr
ClibTiming::timing_cond() const
{
  _check_valid();
  return _impl()->timing_cond();
}

// @brief 立ち上がり遅延時間を計算する．
ClibTime
ClibTiming::calc_rise_delay(
  ClibTime input_transition,
  ClibCapacitance output_capacitance
) const
{
  _check_valid();
  return _impl()->calc_rise_delay(input_transition, output_capacitance);
}

// @brief 立ち下がり遅延時間を計算する．
ClibTime
ClibTiming::calc_fall_delay(
  ClibTime input_transition,
  ClibCapacitance output_capacitance
) const
{
  _check_valid();
  return _impl()->calc_fall_delay(input_transition, output_capacitance);
}

// @brief 立ち上がり遷移時間を計算する．
ClibTime
ClibTiming::calc_rise_transition(
  ClibTime input_transition,
  ClibCapacitance output_capacitance
) const
{
  _check_valid();
  return _impl()->calc_rise_transition(input_transition, output_capacitance);
}

// @brief 立ち下がり遷移時間を計算する．
ClibTime
ClibTiming::calc_fall_transition(
  ClibTime input_transition,
  ClibCapacitance output_capacitance
) const
{
  _check_valid();
  return _impl()->calc_fall_transition(input_transition, output_capacitance);
}

// @brief 立ち上がり固有遅延の取得
ClibTime
ClibTiming::intrinsic_rise() const
{
  _check_valid();
  return _impl()->intrinsic_rise();
}

// @brief 立ち下がり固有遅延の取得
ClibTime
ClibTiming::intrinsic_fall() const
{
  _check_valid();
  return _impl()->intrinsic_fall();
}

// @brief 立ち上がりスロープ遅延の取得
ClibTime
ClibTiming::slope_rise() const
{
  _check_valid();
  return _impl()->slope_rise();
}

// @brief 立ち下がりスロープ遅延の取得
ClibTime
ClibTiming::slope_fall() const
{
  _check_valid();
  return _impl()->slope_fall();
}

// @brief 立ち上がり遷移遅延の取得
ClibResistance
ClibTiming::rise_resistance() const
{
  _check_valid();
  return _impl()->rise_resistance();
}

// @brief 立ち下がり遷移遅延の取得
ClibResistance
ClibTiming::fall_resistance() const
{
  _check_valid();
  return _impl()->fall_resistance();
}

// @brief 立ち上がりピン抵抗の取得
ClibResistance
ClibTiming::rise_pin_resistance(
  SizeType piece_id
) const
{
  _check_valid();
  return _impl()->rise_pin_resistance(piece_id);
}

// @brief 立ち下がりピン抵抗の取得
ClibResistance
ClibTiming::fall_pin_resistance(
  SizeType piece_id
) const
{
  _check_valid();
  return _impl()->fall_pin_resistance(piece_id);
}

// @brief 立ち上がりY切片の取得
ClibTime
ClibTiming::rise_delay_intercept(
  SizeType piece_id
) const
{
  _check_valid();
  return _impl()->rise_delay_intercept(piece_id);
}

// @brief 立ち下がりY切片の取得
ClibTime
ClibTiming::fall_delay_intercept(
  SizeType piece_id
) const
{
  _check_valid();
  return _impl()->fall_delay_intercept(piece_id);
}

// @brief 立ち上がり遷移遅延テーブルの取得
ClibLut
ClibTiming::rise_transition() const
{
  _check_valid();
  auto lut = _impl()->rise_transition();
  return ClibLut{_cell(), lut};
}

// @brief 立ち下がり遷移遅延テーブルの取得
ClibLut
ClibTiming::fall_transition() const
{
  _check_valid();
  auto lut = _impl()->fall_transition();
  return ClibLut{_cell(), lut};
}

// @brief 立ち上がり伝搬遅延テーブルの取得
ClibLut
ClibTiming::rise_propagation() const
{
  _check_valid();
  auto lut = _impl()->rise_propagation();
  return ClibLut{_cell(), lut};
}

// @brief 立ち下がり伝搬遅延テーブルの取得
ClibLut
ClibTiming::fall_propagation() const
{
  _check_valid();
  auto lut = _impl()->fall_propagation();
  return ClibLut{_cell(), lut};
}

// @brief 立ち上がりセル遅延テーブルの取得
ClibLut
ClibTiming::cell_rise() const
{
  _check_valid();
  auto lut = _impl()->cell_rise();
  return ClibLut{_cell(), lut};
}

// @brief 立ち下がりセル遅延テーブルの取得
ClibLut
ClibTiming::cell_fall() const
{
  _check_valid();
  auto lut = _impl()->cell_fall();
  return ClibLut{_cell(), lut};
}

END_NAMESPACE_YM_CLIB
