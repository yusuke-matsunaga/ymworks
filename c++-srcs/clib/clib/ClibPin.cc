
/// @file ClibPin.cc
/// @brief ClibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibPin.h"
#include "ci/CiPin.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス ClibPin
//////////////////////////////////////////////////////////////////////

// @brief ピン名を返す．
std::string
ClibPin::name() const
{
  _check_valid();
  return _impl()->name();
}

// @brief 方向を返す．
ClibDirection
ClibPin::direction() const
{
  _check_valid();
  return _impl()->direction();
}

// @brief 入力ピンの時に true を返す．
bool
ClibPin::is_input() const
{
  _check_valid();
  return _impl()->is_input();
}

// @brief 出力ピンの時に true を返す．
bool
ClibPin::is_output() const
{
  _check_valid();
  return _impl()->is_output();
}

// @brief 入出力ピンの時に true を返す．
bool
ClibPin::is_inout() const
{
  _check_valid();
  return _impl()->is_inout();
}

// @brief 内部ピンの時に true を返す．
bool
ClibPin::is_internal() const
{
  _check_valid();
  return _impl()->is_internal();
}

// @brief ピン番号を返す．
SizeType
ClibPin::pin_id() const
{
  _check_valid();
  return _impl()->pin_id();
}

// @brief 入力ピン番号を返す．
SizeType
ClibPin::input_id() const
{
  _check_valid();
  return _impl()->input_id();
}

// @brief 負荷容量を返す．
ClibCapacitance
ClibPin::capacitance() const
{
  _check_valid();
  return _impl()->capacitance();
}

// @brief 立ち上がり時の負荷容量を返す．
ClibCapacitance
ClibPin::rise_capacitance() const
{
  _check_valid();
  return _impl()->rise_capacitance();
}

// @brief 立ち下がり時の負荷容量を返す．
ClibCapacitance
ClibPin::fall_capacitance() const
{
  _check_valid();
  return _impl()->fall_capacitance();
}

// @brief 出力ピン番号を返す．
SizeType
ClibPin::output_id() const
{
  _check_valid();
  return _impl()->output_id();
}

// @brief 最大ファンアウト容量を返す．
ClibCapacitance
ClibPin::max_fanout() const
{
  _check_valid();
  return _impl()->max_fanout();
}

// @brief 最小ファンアウト容量を返す．
ClibCapacitance
ClibPin::min_fanout() const
{
  _check_valid();
  return _impl()->min_fanout();
}

// @brief 最大負荷容量を返す．
ClibCapacitance
ClibPin::max_capacitance() const
{
  _check_valid();
  return _impl()->max_capacitance();
}

// @brief 最小負荷容量を返す．
ClibCapacitance
ClibPin::min_capacitance() const
{
  _check_valid();
  return _impl()->min_capacitance();
}

// @brief 最大遷移時間を返す．
ClibTime
ClibPin::max_transition() const
{
  _check_valid();
  return _impl()->max_transition();
}

// @brief 最小遷移時間を返す．
ClibTime
ClibPin::min_transition() const
{
  _check_valid();
  return _impl()->min_transition();
}

// @brief 論理式を返す．
Expr
ClibPin::function() const
{
  _check_valid();
  return _impl()->function();
}

// @brief tristate 条件式を返す．
Expr
ClibPin::tristate() const
{
  _check_valid();
  return _impl()->tristate();
}

// @brief 内部ピン番号を返す．
SizeType
ClibPin::internal_id() const
{
  _check_valid();
  return _impl()->internal_id();
}

END_NAMESPACE_YM_CLIB
