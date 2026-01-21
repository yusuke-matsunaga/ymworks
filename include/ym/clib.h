#ifndef YM_CLIB_H
#define YM_CLIB_H

/// @file ym/clib.h
/// @brief ym-clib 用の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga (松永 裕介)
/// All rights reserved.

/// @defgroup ClibGroup セルライブラリ
///
/// セルライブラリを扱うためのクラスライブラリ

#include "ym_config.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief cell の名前空間の開始
#define BEGIN_NAMESPACE_YM_CLIB \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsClib)

/// @brief cell の名前空間の終了
#define END_NAMESPACE_YM_CLIB \
END_NAMESPACE(nsClib) \
END_NAMESPACE_YM

/// @namespace nsYm::nsClib
/// @brief セルライブラリ関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_CLIB

//////////////////////////////////////////////////////////////////////
// クラスの前方参照
//////////////////////////////////////////////////////////////////////

class ClibArea;
class ClibTime;
class ClibCapacitance;
class ClibResistance;

class ClibTimeUnit;
class ClibVoltageUnit;
class ClibCurrentUnit;
class ClibResistanceUnit;
class ClibCapacitanceUnit;
class ClibPowerUnit;

class ClibCellLibrary;
class ClibCell;
class ClibPin;
class ClibBusType;
class ClibBus;
class ClibBundle;
class ClibTiming;
class ClibLut;

class ClibCellGroup;
class ClibCellClass;
class ClibIOMap;
class ClibPinMap;
class ClibPatGraph;
class ClibSeqAttr;

END_NAMESPACE_YM_CLIB


BEGIN_NAMESPACE_YM

using nsClib::ClibArea;
using nsClib::ClibTime;
using nsClib::ClibCapacitance;
using nsClib::ClibResistance;

using nsClib::ClibTimeUnit;
using nsClib::ClibVoltageUnit;
using nsClib::ClibCurrentUnit;
using nsClib::ClibResistanceUnit;
using nsClib::ClibCapacitanceUnit;
using nsClib::ClibPowerUnit;

using nsClib::ClibCellLibrary;
using nsClib::ClibCell;
using nsClib::ClibPin;
using nsClib::ClibBusType;
using nsClib::ClibBus;
using nsClib::ClibBundle;
using nsClib::ClibTiming;
using nsClib::ClibLut;

using nsClib::ClibCellGroup;
using nsClib::ClibCellClass;
using nsClib::ClibIOMap;
using nsClib::ClibPinMap;
using nsClib::ClibPatGraph;
using nsClib::ClibSeqAttr;


//////////////////////////////////////////////////////////////////////
/// @brief テクノロジを表す列挙型
//////////////////////////////////////////////////////////////////////
enum class ClibTechnology : std::uint8_t
{
  cmos,
  fpga,
  none
};


//////////////////////////////////////////////////////////////////////
/// @brief 遅延モデルを表す列挙型
//////////////////////////////////////////////////////////////////////
enum class ClibDelayModel : std::uint8_t
{
  generic_cmos,
  table_lookup,
  piecewise_cmos,
  cmos2,
  dcm,
  polynomial,
  none
};


//////////////////////////////////////////////////////////////////////
/// @brief ピンの方向を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class ClibDirection : std::uint8_t
{
  input,
  output,
  inout,
  internal,
  none
};


//////////////////////////////////////////////////////////////////////
/// @brief タイミングの条件を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class ClibTimingSense : std::uint8_t
{
  positive_unate = 1,
  negative_unate = 2,
  non_unate  = 3,
  none
};


/////////////////////////////////////////////////////////////////////
/// @brief タイミング情報の型を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class ClibTimingType : std::uint8_t
{
  combinational            = 0,
  combinational_rise       = 1,
  combinational_fall       = 2,

  three_state_enable       = 3,
  three_state_disable      = 4,
  three_state_enable_rise  = 5,
  three_state_enable_fall  = 6,
  three_state_disable_rise = 7,
  three_state_disable_fall = 8,

  rising_edge              = 9,
  falling_edge             = 10,

  preset                   = 11,
  clear                    = 12,

  hold_rising              = 13,
  hold_falling             = 14,

  setup_rising             = 15,
  setup_falling            = 16,

  recovery_rising          = 17,
  recovery_falling         = 18,

  skew_rising              = 19,
  skew_falling             = 20,

  removal_rising           = 21,
  removal_falling          = 22,

  non_seq_setup_rising     = 23,
  non_seq_setup_falling    = 24,
  non_seq_hold_rising      = 25,
  non_seq_hold_falling     = 26,

  nochange_high_high       = 27,
  nochange_high_low        = 28,
  nochange_low_high        = 29,
  nochange_low_low         = 30,

  none
};


//////////////////////////////////////////////////////////////////////
/// @brief ルックアップテーブルの変数の型
//////////////////////////////////////////////////////////////////////
enum class ClibVarType : std::uint8_t
{
  input_net_transition,

  total_output_net_capacitance,
  output_net_length,
  output_net_wire_cap,
  output_net_pin_cap,

  equal_or_opposite_output_net_capacitance,
  input_transition_time,

  related_out_total_output_net_capacitance,
  related_out_output_net_length,
  related_out_output_net_wire_cap,
  related_out_output_net_pin_cap,

  constrained_pin_transition,

  related_pin_transition,

  none
};


//////////////////////////////////////////////////////////////////////
/// @brief セルの種類
//////////////////////////////////////////////////////////////////////
enum class ClibCellType : std::uint8_t
{
  none    = 0, ///< なし(不正値)
  Logic   = 1, ///< 組み合わせ論理
  FF      = 2, ///< master-slave FF
  Latch   = 3, ///< master-slave latch
  FSM     = 4, ///< finite state machine
};


//////////////////////////////////////////////////////////////////////
/// @brief clear_preset_var の値
//////////////////////////////////////////////////////////////////////
enum class ClibCPV : std::uint8_t
{
  L = 0, ///< low(0)
  H = 1, ///< high(1)
  N = 2, ///< not change
  T = 3, ///< toggle
  X = 4, ///< unknown
};

/// @brief ClibCPV の全ての値のリスト
///
/// @code
/// for ( auto cpv: CPV_list ) {
///    ...
/// }
/// のように用いられることを想定している．
const ClibCPV CPV_LIST[] = {
  ClibCPV::L,
  ClibCPV::H,
  ClibCPV::N,
  ClibCPV::T,
  ClibCPV::X
};


//////////////////////////////////////////////////////////////////////
/// @brief パタングラフのノードの型
//////////////////////////////////////////////////////////////////////
enum class ClibPatType : std::uint8_t
{
  Input = 0,
  And   = 2,
  Xor   = 3
};

/// @brief 無効なID番号を表す値
const SizeType CLIB_NULLID = -1;


//////////////////////////////////////////////////////////////////////
// ストリーム出力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ClibDelayModel のストリーム出力演算子
/// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s,           ///< [in] 出力先のストリーム
  ClibDelayModel delay_model ///< [in] 遅延モデル
);

/// @brief ClibTimingSense のストリーム出力演算子
/// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s,             ///< [in] 出力先のストリーム
  ClibTimingSense timing_sense ///< [in] タイミングセンス
);

/// @brief ClibTimingType のストリーム出力演算子
/// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s,           ///< [in] 出力先のストリーム
  ClibTimingType timing_type ///< [in] タイミング条件
);

/// @brief ClibCPV のストリーム出力演算子
/// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s, ///< [in] 出力先のストリーム
  ClibCPV cpv      ///< [in] clear_preset_var の値
);

/// @brief ClibVarType のストリーム出力演算子
/// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s,     ///< [in] 出力先のストリーム
  ClibVarType var_type ///< [in] 変数の型
);

END_NAMESPACE_YM

#endif // YM_CLIB_H
