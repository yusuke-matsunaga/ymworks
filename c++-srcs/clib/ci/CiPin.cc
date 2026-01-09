
/// @file CiPin.cc
/// @brief CiPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "CiPin_sub.h"
#include "ci/Serializer.h"
#include "ci/Deserializer.h"


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラス CiPin
//////////////////////////////////////////////////////////////////////

// @brief 入力ピンを生成するクラスメソッド
std::unique_ptr<CiPin>
CiPin::new_Input(
  SizeType pin_id,
  SizeType input_id,
  const ShString& name,
  ClibCapacitance capacitance,
  ClibCapacitance rise_capacitance,
  ClibCapacitance fall_capacitance
)
{
  auto ptr = new CiInputPin{
    pin_id, input_id, name,
    capacitance,
    rise_capacitance,
    fall_capacitance
  };
  return std::unique_ptr<CiPin>{ptr};
}

// @brief 出力ピンを生成するクラスメソッド
std::unique_ptr<CiPin>
CiPin::new_Output(
  SizeType pin_id,
  SizeType output_id,
  const ShString& name,
  ClibCapacitance max_fanout,
  ClibCapacitance min_fanout,
  ClibCapacitance max_capacitance,
  ClibCapacitance min_capacitance,
  ClibTime max_transition,
  ClibTime min_transition,
  const Expr& function,
  const Expr& tristate
)
{
  auto ptr = new CiOutputPin{
    pin_id, output_id, name,
    max_fanout, min_fanout,
    max_capacitance, min_capacitance,
    max_transition, min_transition,
    function, tristate
  };
  return std::unique_ptr<CiPin>{ptr};
}

// @brief 入出力ピンを生成するクラスメソッド
std::unique_ptr<CiPin>
CiPin::new_Inout(
  SizeType pin_id,
  SizeType input_id,
  SizeType output_id,
  const ShString& name,
  ClibCapacitance capacitance,
  ClibCapacitance rise_capacitance,
  ClibCapacitance fall_capacitance,
  ClibCapacitance max_fanout,
  ClibCapacitance min_fanout,
  ClibCapacitance max_capacitance,
  ClibCapacitance min_capacitance,
  ClibTime max_transition,
  ClibTime min_transition,
  const Expr& function,
  const Expr& tristate
)
{
  auto ptr = new CiInoutPin{
    pin_id, input_id, output_id, name,
    capacitance,
    rise_capacitance,
    fall_capacitance,
    max_fanout, min_fanout,
    max_capacitance, min_capacitance,
    max_transition, min_transition,
    function, tristate
  };
  return std::unique_ptr<CiPin>{ptr};
}

// @brief 内部ピンを生成するクラスメソッド
std::unique_ptr<CiPin>
CiPin::new_Internal(
  SizeType internal_id,
  const ShString& name
)
{
  auto ptr = new CiInternalPin{
    internal_id, name
  };
  return std::unique_ptr<CiPin>{ptr};
}

// @brief 入力ピンの時に true を返す．
bool
CiPin::is_input() const
{
  return false;
}

// @brief 出力ピンの時に true を返す．
bool
CiPin::is_output() const
{
  return false;
}

// @brief 入出力ピンの時に true を返す．
bool
CiPin::is_inout() const
{
  return false;
}

// @brief 内部ピンの時に true を返す．
bool
CiPin::is_internal() const
{
  return false;
}

// @brief 入力ピン番号を返す．
SizeType
CiPin::input_id() const
{
  return 0;
}

// @brief 負荷容量を返す．
ClibCapacitance
CiPin::capacitance() const
{
  return ClibCapacitance(0.0);
}

// @brief 立ち上がり時の負荷容量を返す．
ClibCapacitance
CiPin::rise_capacitance() const
{
  return ClibCapacitance(0.0);
}

// @brief 立ち下がり時の負荷容量を返す．
ClibCapacitance
CiPin::fall_capacitance() const
{
  return ClibCapacitance(0.0);
}

// @brief 出力ピン番号を返す．
SizeType
CiPin::output_id() const
{
  return 0;
}

// @brief 最大ファンアウト容量を返す．
ClibCapacitance
CiPin::max_fanout() const
{
  return ClibCapacitance(0.0);
}

// @brief 最小ファンアウト容量を返す．
ClibCapacitance
CiPin::min_fanout() const
{
  return ClibCapacitance(0.0);
}

// @brief 最大負荷容量を返す．
ClibCapacitance
CiPin::max_capacitance() const
{
  return ClibCapacitance(0.0);
}

// @brief 最小負荷容量を返す．
ClibCapacitance
CiPin::min_capacitance() const
{
  return ClibCapacitance(0.0);
}

// @brief 最大遷移時間を返す．
ClibTime
CiPin::max_transition() const
{
  return ClibTime(0.0);
}

// @brief 最小遷移時間を返す．
ClibTime
CiPin::min_transition() const
{
  return ClibTime(0.0);
}

// @brief 論理式を返す．
Expr
CiPin::function() const
{
  return Expr::invalid();
}

// @brief tristate 条件式を返す．
Expr
CiPin::tristate() const
{
  return Expr::invalid();
}

// @brief 内部ピン番号を返す．
SizeType
CiPin::internal_id() const
{
  return 0;
}

// @brief 内容をシリアライズする．
void
CiPin::serialize(
  Serializer& s
) const
{
  s.reg_obj(this);
}

// @brief dump 用の共通情報を出力する．
void
CiPin::dump_common(
  Serializer& s,
  int sig
) const
{
  s.dump(static_cast<std::uint8_t>(sig));
  s.dump(mName);
  s.dump(mPinId);
}

// @brief ピン情報を復元する．
std::unique_ptr<CiPin>
CiPin::restore(
  Deserializer& s
)
{
  std::uint8_t sig;
  s.restore(sig);
  CiPin* pin;
  switch ( sig ) {
  case 0: pin = new CiInputPin; break;
  case 1: pin = new CiOutputPin; break;
  case 2: pin = new CiInoutPin; break;
  case 3: pin = new CiInternalPin; break;
  default: ASSERT_NOT_REACHED; break;
  }
  auto ptr = std::unique_ptr<CiPin>{pin};
  ptr->_restore(s);
  return ptr;
}

// @brief 内容を読み込む．
void
CiPin::restore_common(
  Deserializer& s
)
{
  s.restore(mName);
  s.restore(mPinId);
}


//////////////////////////////////////////////////////////////////////
// クラス CiInputPin
//////////////////////////////////////////////////////////////////////

// @brief 方向を返す．
ClibDirection
CiInputPin::direction() const
{
  return ClibDirection::input;
}

// @brief 入力ピンの時に true を返す．
bool
CiInputPin::is_input() const
{
  return true;
}

// @brief 入力ピン番号を返す．
SizeType
CiInputPin::input_id() const
{
  return mInputId;
}

// @brief 負荷容量を返す．
ClibCapacitance
CiInputPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
ClibCapacitance
CiInputPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
ClibCapacitance
CiInputPin::fall_capacitance() const
{
  return mFallCapacitance;
}

// @brief 内容をバイナリダンプする．
void
CiInputPin::dump(
  Serializer& s
) const
{
  dump_common(s, 0);
  s.dump(mInputId);
  s.dump(mCapacitance);
  s.dump(mRiseCapacitance);
  s.dump(mFallCapacitance);
}

// @brief 内容を読み込む．
void
CiInputPin::_restore(
  Deserializer& s
)
{
  restore_common(s);
  s.restore(mInputId);
  s.restore(mCapacitance);
  s.restore(mRiseCapacitance);
  s.restore(mFallCapacitance);
}


//////////////////////////////////////////////////////////////////////
// クラス CiOutputPinBase
//////////////////////////////////////////////////////////////////////

// @brief 出力ピン番号を返す．
SizeType
CiOutputPinBase::output_id() const
{
  return mOutputId;
}

// @brief 最大ファンアウト容量を返す．
ClibCapacitance
CiOutputPinBase::max_fanout() const
{
  return mMaxFanout;
}

// @brief 最小ファンアウト容量を返す．
ClibCapacitance
CiOutputPinBase::min_fanout() const
{
  return mMinFanout;
}

// @brief 最大負荷容量を返す．
ClibCapacitance
CiOutputPinBase::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief 最小負荷容量を返す．
ClibCapacitance
CiOutputPinBase::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief 最大遷移時間を返す．
ClibTime
CiOutputPinBase::max_transition() const
{
  return mMaxTransition;;
}

// @brief 最小遷移時間を返す．
ClibTime
CiOutputPinBase::min_transition() const
{
  return mMinTransition;
}

// @brief 論理式を返す．
Expr
CiOutputPinBase::function() const
{
  return mFunction;
}

// @brief tristate 条件式を返す．
Expr
CiOutputPinBase::tristate() const
{
  return mTristate;
}

// @brief 内容をバイナリダンプする．
void
CiOutputPinBase::dump_base(
  Serializer& s,
  int sig
) const
{
  dump_common(s, sig);
  s.dump(mOutputId);
  s.dump(mFanoutLoad);
  s.dump(mMaxFanout);
  s.dump(mMinFanout);
  s.dump(mMaxCapacitance);
  s.dump(mMinCapacitance);
  s.dump(mMaxTransition);
  s.dump(mMinTransition);
  s.dump(mFunction);
  s.dump(mTristate);
}

// @brief 内容を読み込む．
void
CiOutputPinBase::restore_base(
  Deserializer& s
)
{
  restore_common(s);
  s.restore(mOutputId);
  s.restore(mFanoutLoad);
  s.restore(mMaxFanout);
  s.restore(mMinFanout);
  s.restore(mMaxCapacitance);
  s.restore(mMinCapacitance);
  s.restore(mMaxTransition);
  s.restore(mMinTransition);
  s.restore(mFunction);
  s.restore(mTristate);
}


//////////////////////////////////////////////////////////////////////
// クラス CiOutputPin
//////////////////////////////////////////////////////////////////////

// @brief 方向を返す．
ClibDirection
CiOutputPin::direction() const
{
  return ClibDirection::output;
}

// @brief 出力ピンの時に true を返す．
bool
CiOutputPin::is_output() const
{
  return true;
}

// @brief 内容をバイナリダンプする．
void
CiOutputPin::dump(
  Serializer& s
) const
{
  dump_base(s, 1);
}

// @brief 内容を読み込む．
void
CiOutputPin::_restore(
  Deserializer& s
)
{
  restore_base(s);
}


//////////////////////////////////////////////////////////////////////
// クラス CiInoutPin
//////////////////////////////////////////////////////////////////////

// @brief 方向を返す．
ClibDirection
CiInoutPin::direction() const
{
  return ClibDirection::inout;
}

// @brief 入出力ピンの時に true を返す．
bool
CiInoutPin::is_inout() const
{
  return true;
}

// @brief 入力ピン番号を返す．
SizeType
CiInoutPin::input_id() const
{
  return mInputId;
}

// @brief 負荷容量を返す．
ClibCapacitance
CiInoutPin::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
ClibCapacitance
CiInoutPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
ClibCapacitance
CiInoutPin::fall_capacitance() const
{
  return mFallCapacitance;
}

// @brief 内容をバイナリダンプする．
void
CiInoutPin::dump(
  Serializer& s
) const
{
  dump_base(s, 2);
  s.dump(mInputId);
  s.dump(mCapacitance);
  s.dump(mRiseCapacitance);
  s.dump(mFallCapacitance);
}

// @brief 内容を読み込む．
void
CiInoutPin::_restore(
  Deserializer& s
)
{
  restore_base(s);
  s.restore(mInputId);
  s.restore(mCapacitance);
  s.restore(mRiseCapacitance);
  s.restore(mFallCapacitance);
}


//////////////////////////////////////////////////////////////////////
// クラス CiInternalPin
//////////////////////////////////////////////////////////////////////

// @brief 方向を返す．
ClibDirection
CiInternalPin::direction() const
{
  return ClibDirection::internal;
}

// @brief 内部ピンの時に true を返す．
bool
CiInternalPin::is_internal() const
{
  return true;
}

// @brief 内部ピン番号を返す．
SizeType
CiInternalPin::internal_id() const
{
  return mInternalId;
}

// @brief 内容をバイナリダンプする．
void
CiInternalPin::dump(
  Serializer& s
) const
{
  dump_common(s, 3);
  s.dump(internal_id());
}

// @brief 内容を読み込む．
void
CiInternalPin::_restore(
  Deserializer& s
)
{
  restore_common(s);
  s.restore(mInternalId);
}

END_NAMESPACE_YM_CLIB
