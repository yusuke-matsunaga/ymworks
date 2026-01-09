
/// @file Writer.cc
/// @brief Writer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "Writer.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"
#include "ym/ClibIOMap.h"
#include "ym/ClibLut.h"
#include "ym/ClibPin.h"
#include "ym/ClibTiming.h"
#include "ym/ClibCellClass.h"
#include "ym/ClibCellGroup.h"
#include "ym/ClibPatGraph.h"
#include "ym/ClibSeqAttr.h"
#include "ym/ClibCapacitance.h"
#include "ym/ClibTime.h"

#include "ym/Expr.h"
#include "ym/ExprWriter.h"
#include "ym/NpnMapM.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM

// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  ClibDelayModel delay_model
)
{
  switch ( delay_model ) {
  case ClibDelayModel::generic_cmos:   s << "generic_cmos"; break;
  case ClibDelayModel::table_lookup:   s << "table_lookup"; break;
  case ClibDelayModel::piecewise_cmos: s << "piecewise_cmos"; break;
  case ClibDelayModel::cmos2:          s << "cmos2"; break;
  case ClibDelayModel::dcm:            s << "dcm"; break;
  case ClibDelayModel::polynomial:     s << "polynomial"; break;
  default: ASSERT_NOT_REACHED;
  }
  return s;
}

// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  ClibTimingSense timing_sense
)
{
  switch ( timing_sense ) {
  case ClibTimingSense::positive_unate: s << "posi_unate"; break;
  case ClibTimingSense::negative_unate: s << "nega_unate"; break;
  case ClibTimingSense::non_unate:      s << "non_unate"; break;
  case ClibTimingSense::none:           s << "none"; break;
  default: ASSERT_NOT_REACHED;
  }
  return s;
}

// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  ClibTimingType timing_type
)
{
  switch ( timing_type ) {
  case ClibTimingType::combinational:            s << "combinational"; break;
  case ClibTimingType::combinational_rise:       s << "combinational_rise"; break;
  case ClibTimingType::combinational_fall:       s << "combinational_fall"; break;

  case ClibTimingType::three_state_enable:       s << "three_state_enable"; break;
  case ClibTimingType::three_state_disable:      s << "three_state_disable"; break;
  case ClibTimingType::three_state_enable_rise:  s << "three_state_enable_rise"; break;
  case ClibTimingType::three_state_enable_fall:  s << "three_state_enable_fall"; break;
  case ClibTimingType::three_state_disable_rise: s << "three_state_disable_rise"; break;
  case ClibTimingType::three_state_disable_fall: s << "three_state_disable_fall"; break;

  case ClibTimingType::rising_edge:              s << "rising_edge"; break;
  case ClibTimingType::falling_edge:             s << "falling_edge"; break;

  case ClibTimingType::preset:                   s << "preset"; break;
  case ClibTimingType::clear:                    s << "clear"; break;

  case ClibTimingType::hold_rising:              s << "hold_rising"; break;
  case ClibTimingType::hold_falling:             s << "hold_falling"; break;

  case ClibTimingType::setup_rising:             s << "setup_rising"; break;
  case ClibTimingType::setup_falling:            s << "setup_falling"; break;

  case ClibTimingType::recovery_rising:          s << "recover_rising"; break;
  case ClibTimingType::recovery_falling:         s << "recover_falling"; break;

  case ClibTimingType::skew_rising:              s << "skew_rising"; break;
  case ClibTimingType::skew_falling:             s << "skew_falling"; break;

  case ClibTimingType::removal_rising:           s << "removal_rising"; break;
  case ClibTimingType::removal_falling:          s << "removal_falling"; break;

  case ClibTimingType::non_seq_setup_rising:     s << "non_seq_setup_rising"; break;
  case ClibTimingType::non_seq_setup_falling:    s << "non_seq_setup_falling"; break;
  case ClibTimingType::non_seq_hold_rising:      s << "non_seq_hold_rising"; break;
  case ClibTimingType::non_seq_hold_falling:     s << "non_seq_hold_falling"; break;

  case ClibTimingType::nochange_high_high:       s << "nochange_high_high"; break;
  case ClibTimingType::nochange_high_low:        s << "nochange_high_low"; break;
  case ClibTimingType::nochange_low_high:        s << "nochange_low_high"; break;
  case ClibTimingType::nochange_low_low:         s << "nochange_low_low"; break;

  default: ASSERT_NOT_REACHED;
  }
  return s;
}

// @brief ストリーム出力演算子
std::ostream&
operator<<(
  std::ostream& s,
  ClibVarType var_type
)
{
  switch ( var_type ) {
  case ClibVarType::input_net_transition:
    s << "input_net_transition";
    break;

  case ClibVarType::total_output_net_capacitance:
    s << "total_output_net_capacitance";
    break;

  case ClibVarType::output_net_length:
    s << "output_net_length";
    break;

  case ClibVarType::output_net_wire_cap:
    s << "output_net_wire_cap";
    break;

  case ClibVarType::output_net_pin_cap:
    s << "output_net_pin_cap";
    break;

  case ClibVarType::related_out_total_output_net_capacitance:
    s << "related_out_total_output_net_capacitance";
    break;

  case ClibVarType::related_out_output_net_length:
    s << "related_out_output_net_length";
    break;

  case ClibVarType::related_out_output_net_wire_cap:
    s << "related_out_output_net_wire_cap";
    break;

  case ClibVarType::related_out_output_net_pin_cap:
    s << "related_out_output_net_pin_cap";
    break;

  case ClibVarType::constrained_pin_transition:
    s << "constrained_pin_transition";
    break;

  case ClibVarType::related_pin_transition:
    s << "related_pin_transition";
    break;

  case ClibVarType::none:
    s << "none";
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  return s;
}

// @brief ClibCPV のストリーム出力演算子
// @return s を返す．
std::ostream&
operator<<(
  std::ostream& s,  ///< [in] 出力先のストリーム
  ClibCPV cpv  ///< [in] clear_preset_var の値
)
{
  switch ( cpv ) {
  case ClibCPV::L: s << "L"; break;
  case ClibCPV::H: s << "H"; break;
  case ClibCPV::N: s << "N"; break;
  case ClibCPV::T: s << "T"; break;
  case ClibCPV::X: s << "X"; break;
  default:         s << "-"; break;
  }
  return s;
}

END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_CLIB

// @brief コンストラクタ
Writer::Writer(
  const ClibCellLibrary& library
) : mLibrary{library}
{
  { // セルクラスの登録
    SizeType id = 0;
    for ( auto cclass: mLibrary.npn_class_list() ) {
      auto key = cclass.key();
      mClassDict.emplace(key, id);
      ++ id;
    }
  }
  { // セルグループの登録
    SizeType id = 0;
    for ( auto cgroup: mLibrary.cell_group_list() ) {
      auto key = cgroup.key();
      mGroupDict.emplace(key, id);
      ++ id;
    }
  }
  { // セルの登録
    SizeType id = 0;
    for ( auto cell: mLibrary.cell_list() ) {
      auto key = cell.key();
      mCellDict.emplace(key, id);
      ++ id;
    }
  }

  // 既定のセルグループ名の登録
  reg_group_name("Const0",   mLibrary.const0_func());
  reg_group_name("Const1",   mLibrary.const1_func());
  reg_group_name("Buffer",   mLibrary.buf_func());
  reg_group_name("Inverter", mLibrary.inv_func());
  reg_group_name("And2",     mLibrary.and_func(2));
  reg_group_name("And3",     mLibrary.and_func(3));
  reg_group_name("And4",     mLibrary.and_func(4));
  reg_group_name("Nand2",    mLibrary.nand_func(2));
  reg_group_name("Nand3",    mLibrary.nand_func(3));
  reg_group_name("Nand4",    mLibrary.nand_func(4));
  reg_group_name("Or2",      mLibrary.or_func(2));
  reg_group_name("Or3",      mLibrary.or_func(3));
  reg_group_name("Or4",      mLibrary.or_func(4));
  reg_group_name("Nor2",     mLibrary.nor_func(2));
  reg_group_name("Nor3",     mLibrary.nor_func(3));
  reg_group_name("Nor4",     mLibrary.nor_func(4));
  reg_group_name("Xor2",     mLibrary.xor_func(2));
  reg_group_name("Xor3",     mLibrary.xor_func(3));
  reg_group_name("Xor4",     mLibrary.xor_func(4));
  reg_group_name("Xnor2",    mLibrary.xnor_func(2));
  reg_group_name("Xnor3",    mLibrary.xnor_func(3));
  reg_group_name("Xnor4",    mLibrary.xnor_func(4));
  reg_group_name("Mux2",     mLibrary.mux2_func());
  reg_group_name("Mux4",     mLibrary.mux4_func());

  // 既定のセルクラス名の登録
  reg_class_name("Const",    mLibrary.const0_func());
  reg_class_name("Buffer",   mLibrary.buf_func());
  reg_class_name("And2",     mLibrary.and_func(2));
  reg_class_name("And3",     mLibrary.and_func(3));
  reg_class_name("And4",     mLibrary.and_func(4));
  reg_class_name("Xor2",     mLibrary.xor_func(2));
  reg_class_name("Xor3",     mLibrary.xor_func(3));
  reg_class_name("Xor4",     mLibrary.xor_func(4));
  reg_class_name("Mux2",     mLibrary.mux2_func());
  reg_class_name("Mux4",     mLibrary.mux4_func());
  reg_dff_class();
  reg_latch_class();
}

// @brief セルライブラリの内容を出力する．
void
Writer::run(
  std::ostream& s
) const
{
  // ライブラリ名
  s << "Library(" << mLibrary.name() << ")" << std::endl;

  // テクノロジ
  s << "  technology: ";
  switch ( mLibrary.technology() ) {
  case ClibTechnology::cmos: s << "cmos"; break;
  case ClibTechnology::fpga: s << "fpga"; break;
  default: ASSERT_NOT_REACHED; break;
  }
  s << std::endl;

  // 遅延モデル
  s << "  delay_model: "
    << mLibrary.delay_model() << std::endl;

  // バス命名規則
  s << "  bus_naming_style: "
    << mLibrary.bus_naming_style() << std::endl;

  // 日付
  s << "  date: "
    << mLibrary.date() << std::endl;

  // リビジョン
  s << "  revision: "
    << mLibrary.revision() << std::endl;

  // コメント
  s << "  comment: "
    << mLibrary.comment() << std::endl;

  // 時間単位
  s << "  time_unit: "
    << mLibrary.time_unit() << std::endl;

  // 電圧単位
  s << "  voltage_unit: "
    << mLibrary.voltage_unit() << std::endl;

  // 電流単位
  s << "  current_unit: "
    << mLibrary.current_unit() << std::endl;

  // 抵抗単位
  s << "  pulling_resistance_unit: "
    << mLibrary.pulling_resistance_unit() << std::endl;

  // 容量単位
  s << "  capacitive_load_unit: "
    << mLibrary.capacitive_load_unit()
    << mLibrary.capacitive_load_unit_str() << std::endl;

  // 電力単位
  s << "  leakage_power_unit: "
    << mLibrary.leakage_power_unit() << std::endl;

  s << std::endl;

  // セル
  for ( auto cell: mLibrary.cell_list() ) {
    display_cell(s, cell);
  }

  // セルグループ
  for ( auto cgroup: mLibrary.cell_group_list() ) {
    display_group(s, cgroup);
  }

  // セルクラス
  for ( auto cclass: mLibrary.npn_class_list() ) {
    display_class(s, cclass);
  }

  // パタングラフの情報
  display_patgraph(s);

}

// セルクラスの情報を出力する．
void
Writer::display_class(
  std::ostream& s,
  const ClibCellClass& cclass
) const
{
  bool has_cell = false;
  for ( auto group: cclass.cell_group_list() ) {
    if ( group.cell_list().size() > 0 ) {
      has_cell = true;
      break;
    }
  }
  if ( !has_cell ) {
    // セルを持たないクラスはスキップする．
    return;
  }

  s << "Class#"
    << mClassDict.at(cclass.key())
    << std::endl;
  if ( mClassNameDict.count(cclass.key()) > 0 ) {
    auto name = mClassNameDict.at(cclass.key());
    s << "  Name = " << name
      << std::endl;
  }
  auto n = cclass.idmap_num();
  if ( n > 0 ) {
    s << "  Idmap List:" << std::endl;
    for ( auto& idmap: cclass.idmap_list() ) {
      s << "         " << idmap << std::endl;
    }
    s << std::endl;
  }
  for ( auto group: cclass.cell_group_list() ) {
    if ( group.cell_list().size() == 0 ) {
      continue;
    }
    s << "  Group#" << mGroupDict.at(group.key())
      << ": Map = " << group.iomap() << std::endl;
  }
  s << std::endl;
}

// セルグループの情報を出力する．
void
Writer::display_group(
  std::ostream& s,
  const ClibCellGroup& group
) const
{
  if ( group.cell_list().size() == 0 ) {
    // セルを持たないグループはスキップする．
    return;
  }

  s << "Group#"
    << mGroupDict.at(group.key())
    << "(Class#"
    << mClassDict.at(group.rep_class().key())
    << ")" << std::endl;
  if ( mGroupNameDict.count(group.key()) > 0 ) {
    auto name = mGroupNameDict.at(group.key());
    s << "  Name = " << name
      << std::endl;
  }
  s << "  Cell =";
  for ( auto cell: group.cell_list() ) {
    s << " " << cell.name();
  }
  s << std::endl
    << std::endl;
}

void
Writer::display_cell(
  std::ostream& s,
  const ClibCell& cell
) const
{
  // セル名とセルの種類を出力
  s << cell.name() << ": Cell#"
    << mCellDict.at(cell.key())
    << "(Group#"
    << mGroupDict.at(cell.group().key())
    << ")" << std::endl
    << "  type = ";
  if ( cell.is_logic() ) {
    s << "Combinational Logic";
  }
  else if ( cell.is_ff() ) {
    s << "Flip-Flop";
  }
  else if ( cell.is_latch() ) {
    s << "Latch";
  }
  else if ( cell.type() == ClibCellType::FSM ) {
    s << "FSM";
  }
  else {
    ASSERT_NOT_REACHED;
  }
  s << std::endl;

  // 面積
  s << "  area = " << cell.area() << std::endl;

  // 論理式の出力用にピン名の辞書を作る．
  std::unordered_map<SizeType, std::string> var_names;
  for ( auto pin: cell.pin_list() ) {
    if ( pin.is_input() || pin.is_inout() ) {
      var_names.emplace(pin.input_id(), pin.name());
    }
  }
  ExprWriter ewriter;

  if ( cell.is_ff() ) {
    // FF の情報
    var_names.emplace(cell.input2_num(), cell.qvar1());
    s << "  Next State         = "
      << ewriter.dump_to_string(cell.next_state_expr(), var_names)
      << std::endl
      << "  Clock              = "
      << ewriter.dump_to_string(cell.clock_expr(), var_names)
      << std::endl;
    if ( cell.clock2_expr().is_valid() ) {
      s << "  Clock2             = "
	<< ewriter.dump_to_string(cell.clock2_expr(), var_names)
	<< std::endl;
    }
    if ( cell.has_clear() ) {
      s << "  Clear              = "
	<< ewriter.dump_to_string(cell.clear_expr(), var_names)
	<< std::endl;
    }
    if ( cell.has_preset() ) {
      s << "  Preset             = "
	<< ewriter.dump_to_string(cell.preset_expr(), var_names)
	<< std::endl;
    }
    if ( cell.has_clear() && cell.has_preset() ) {
      s << "  Clear Preset Var1  = " << cell.clear_preset_var1() << std::endl
	<< "  Clear Preset Var2  = " << cell.clear_preset_var2() << std::endl;
    }
  }
  if ( cell.is_latch() ) {
    // ラッチの情報
    var_names.emplace(cell.input2_num(), cell.qvar1());
    s << "  Data In            = "
      << ewriter.dump_to_string(cell.data_in_expr(), var_names)
      << std::endl
      << "  Enable             = "
      << ewriter.dump_to_string(cell.enable_expr(), var_names)
      << std::endl;
    if ( cell.enable2_expr().is_valid() ) {
      s << "  Enable2            = "
	<< ewriter.dump_to_string(cell.enable2_expr(), var_names)
	<< std::endl;
    }
    if ( cell.has_clear() ) {
      s << "  Clear              = "
	<< ewriter.dump_to_string(cell.clear_expr(), var_names)
	<< std::endl;
    }
    if ( cell.has_preset() ) {
      s << "  Preset             = "
	<< ewriter.dump_to_string(cell.preset_expr(), var_names)
	<< std::endl;
    }
    if ( cell.has_clear() && cell.has_preset() ) {
      s << "  Clear Preset Var1  = " << cell.clear_preset_var1() << std::endl
	<< "  Clear Preset Var2  = " << cell.clear_preset_var2() << std::endl;
    }
  }

  // ピンの情報
  for ( auto pin: cell.pin_list() ) {
    s << "  Pin#" << pin.pin_id() << "[ " << pin.name() << " ]: ";
    if ( pin.is_input() ) {
      // 入力ピン
      s << "Input#" << pin.input_id() << std::endl
	<< "    Capacitance      = " << pin.capacitance() << std::endl
	<< "    Rise Capacitance = " << pin.rise_capacitance() << std::endl
	<< "    Fall Capacitance = " << pin.fall_capacitance() << std::endl;
    }
    else if ( pin.is_output() ) {
      // 出力ピン
      auto opos = pin.output_id();
      s << "Output# " << opos << std::endl;
      if ( cell.has_logic(opos) ) {
	s << "    Logic            = "
	  << ewriter.dump_to_string(cell.logic_expr(opos), var_names)
	  << std::endl;
	if ( cell.has_tristate(opos) ) {
	  s << "    Tristate         = "
	    << ewriter.dump_to_string(cell.tristate_expr(opos), var_names)
	    << std::endl;
	}
      }
      s << "    Max Fanout       = " << pin.max_fanout() << std::endl
	<< "    Min Fanout       = " << pin.min_fanout() << std::endl
	<< "    Max Capacitance  = " << pin.max_capacitance() << std::endl
	<< "    Min Capacitance  = " << pin.min_capacitance() << std::endl
	<< "    Max Transition   = " << pin.max_transition() << std::endl
	<< "    Min Transition   = " << pin.min_transition() << std::endl;
    }
    else if ( pin.is_inout() ) {
      // 入出力ピン
      auto opos = pin.output_id();
      s << "Inout#(" << pin.input_id()
	<< ", " << opos << ")" << std::endl;
      if ( cell.has_logic(opos) ) {
	s << "    Logic            = "
	  << ewriter.dump_to_string(cell.logic_expr(opos), var_names)
	  << std::endl;
	if ( cell.has_tristate(opos) ) {
	  s << "    Tristate         = "
	    << ewriter.dump_to_string(cell.tristate_expr(opos), var_names)
	    << std::endl;
	}
      }
      s << "    Capacitance      = " << pin.capacitance() << std::endl
	<< "    Rise Capacitance = " << pin.rise_capacitance() << std::endl
	<< "    Fall Capacitance = " << pin.fall_capacitance() << std::endl
	<< "    Max Fanout       = " << pin.max_fanout() << std::endl
	<< "    Min Fanout       = " << pin.min_fanout() << std::endl
	<< "    Max Capacitance  = " << pin.max_capacitance() << std::endl
	<< "    Min Capacitance  = " << pin.min_capacitance() << std::endl
	<< "    Max Transition   = " << pin.max_transition() << std::endl
	<< "    Min Transition   = " << pin.min_transition() << std::endl;
    }
    else if ( pin.is_internal() ) {
      // 内部ピン
      auto ipos = pin.internal_id();
      s << "Internal#(" << ipos << ")" << std::endl;
    }
  }

  // タイミング情報
  auto ni2 = cell.input2_num();
  auto no2 = cell.output2_num();
  for ( auto ipos: Range(ni2) ) {
    for ( auto opos: Range(no2) ) {
      display_timing(s, cell, ipos, opos, ClibTimingSense::positive_unate);
      display_timing(s, cell, ipos, opos, ClibTimingSense::negative_unate);
    }
  }
  s << std::endl;
}

// タイミング情報を出力する．
void
Writer::display_timing(
  std::ostream& s,
  const ClibCell& cell,
  SizeType ipos,
  SizeType opos,
  ClibTimingSense sense
) const
{
  for ( auto timing: cell.timing_list(ipos, opos, sense) ) {
    s << "  Timing:" << std::endl
      << "    Type             = " << timing.type() << std::endl
      << "    Input Pin        = " << cell.input(ipos).name() << std::endl
      << "    Output Pin       = " << cell.output(opos).name() << std::endl
      << "    Sense            = ";
    if ( sense == ClibTimingSense::positive_unate ) {
      s << "positive unate";
    }
    else if ( sense == ClibTimingSense::negative_unate ) {
      s << "negative unate";
    }
    else {
      ASSERT_NOT_REACHED;
    }
    s << std::endl;
    if ( !timing.timing_cond().is_one() ) {
      s << "    When             = " << timing.timing_cond() << std::endl;
    }

    switch ( mLibrary.delay_model() ) {
    case ClibDelayModel::generic_cmos:
      s << "    Rise Intrinsic   = " << timing.intrinsic_rise() << std::endl
	<< "    Rise Resistance  = " << timing.rise_resistance() << std::endl
	<< "    Fall Intrinsic   = " << timing.intrinsic_fall() << std::endl
	<< "    Fall Resistance  = " << timing.fall_resistance() << std::endl;
      break;

    case ClibDelayModel::table_lookup:
      display_lut(s, "Cell Rise", timing.cell_rise());
      display_lut(s, "Rise Transition", timing.rise_transition());
      display_lut(s, "Rise Propagation", timing.rise_propagation());

      display_lut(s, "Cell Fall", timing.cell_fall());
      display_lut(s, "Fall Transition", timing.fall_transition());
      display_lut(s, "Fall Propagation", timing.fall_propagation());
      break;

    case ClibDelayModel::piecewise_cmos:
      break;

    case ClibDelayModel::cmos2:
      break;

    case ClibDelayModel::dcm:
      break;

    case ClibDelayModel::polynomial:
      break;

    case ClibDelayModel::none:
      break;
    }
  }
}

// LUT の情報を出力する．
void
Writer::display_lut(
  std::ostream& s,
  const std::string& label,
  const ClibLut& lut
) const
{
  if ( lut.is_invalid() ) {
    return;
  }

  SizeType d = lut.dimension();
  s << "    " << label << std::endl;
  for ( auto i: Range(d) ) {
    s << "      Variable_" << (i + 1) << " = " << lut.variable_type(i) << std::endl;
  }
  for ( auto i: Range(d) ) {
    s << "      Index_" << (i + 1) << "    = ";
    auto n = lut.index_num(i);
    const char* comma = "";
    s << "(";
    for ( auto j: Range(n) ) {
      s << comma << lut.index(i, j);
      comma = ", ";
    }
    s << ")" << std::endl;
  }

  if ( d == 1) {
    s << "      Values = (";
    auto n1 = lut.index_num(0);
    std::vector<SizeType> pos_array(1);
    const char* comma = "";
    for ( auto i1: Range(n1) ) {
      pos_array[0] = i1;
      s << comma << lut.grid_value(pos_array);
      comma = ", ";
    }
    s << ")" << std::endl;
  }
  else if ( d == 2 ) {
    s << "      Values = (" << std::endl;
    auto n1 = lut.index_num(0);
    auto n2 = lut.index_num(1);
    std::vector<SizeType> pos_array(2);
    for ( auto i1: Range(n1) ) {
      s << "                (";
      pos_array[0] = i1;
      const char* comma = "";
      for ( auto i2: Range(n2) ) {
	pos_array[1] = i2;
	s << comma << lut.grid_value(pos_array);
	comma = ", ";
      }
      s << ")" << std::endl;
    }
    s << "               )" << std::endl;
  }
  else if ( d == 3 ) {
    s << "      Values = (" << std::endl;
    auto n1 = lut.index_num(0);
    auto n2 = lut.index_num(1);
    auto n3 = lut.index_num(2);
    std::vector<SizeType> pos_array(3);
    for ( auto i1: Range(n1) ) {
      s << "                (";
      pos_array[0] = i1;
      const char* comma2 = "";
      for ( auto i2: Range(n2) ) {
	pos_array[1] = i2;
	s << comma2 << "(";
	const char* comma3 = "";
	for ( auto i3: Range(n3) ) {
	  pos_array[2] = i3;
	  s << comma3 << lut.grid_value(pos_array);
	  comma3 = ", ";
	}
	s << ")";
	comma2 = ", ";
      }
      s << ")" << std::endl;
    }
    s << "                )" << std::endl;
  }
}

// @brief パタングラフの情報を出力する．
void
Writer::display_patgraph(
  std::ostream& s
) const
{
  s << "==== PatMgr dump start ====" << std::endl;

  // ノードの種類の出力
  auto nn = mLibrary.pg_node_num();
  for ( auto i: Range(nn) ) {
    s << "Node#" << i << ": ";
    switch ( mLibrary.pg_node_type(i) ) {
    case ClibPatType::Input:
      s << "INPUT#"
	<< mLibrary.pg_input_id(i);
      break;
    case ClibPatType::And:
      s << "AND";
      break;
    case ClibPatType::Xor:
      s << "XOR";
      break;
    default:
      ASSERT_NOT_REACHED;
      break;
    }
    s << std::endl;
  }
  s << std::endl;

  // 枝の情報の出力
  auto ne = mLibrary.pg_edge_num();
  for ( auto i: Range(ne) ) {
    s << "Edge#" << i << ": " << mLibrary.pg_edge_from(i)
      << " -> " << mLibrary.pg_edge_to(i)
      << "(" << mLibrary.pg_edge_pos(i) << ")";
    if ( mLibrary.pg_edge_inv(i) ) {
      s << " ***";
    }
    s << std::endl;
  }
  s << std::endl;

  // パタングラフの情報の出力
  auto np = mLibrary.pg_pat_num();
  for ( auto i: Range(np) ) {
    const auto& pat = mLibrary.pg_pat(i);
    auto rep = pat.rep_class();
    auto rep_id = mClassDict.at(rep.key());
    s << "Pat#" << i << ": "
      << "Rep#" << rep_id << ": ";
    if ( pat.root_inv() ) {
      s << "[inv]";
    }
    s << "(" << pat.input_num() << "), ";
    auto n = pat.edge_num();
    for ( auto i: Range(n) ) {
      s << " " << pat.edge(i);
    }
    s << std::endl;
  }

  s << "==== PatMgr dump end ====" << std::endl;
}

// @brief DFF のセルクラスに名前をつける．
void
Writer::reg_dff_class()
{
  for ( SizeType index = 0; index < ClibSeqAttr::max_index(); ++ index ) {
    ClibSeqAttr seq_attr{index};
    auto class_list = mLibrary.find_ff_class(seq_attr);
    std::ostringstream buf;
    buf << "DFF";
    if ( seq_attr.has_slave_clock() ) {
      buf << "2";
    }
    if ( seq_attr.has_clear() ) {
      if ( seq_attr.has_preset() ) {
	auto cpv1 = seq_attr.cpv1();
	auto cpv2 = seq_attr.cpv2();
	buf << "_RS" << cpv1 << cpv2;
      }
      else {
	buf << "_R";
      }
    }
    else if ( seq_attr.has_preset() ) {
      buf << "_S";
    }
    auto name = buf.str();
    for ( auto cclass: class_list ) {
      mClassNameDict.emplace(cclass.key(), name);
    }
  }
}

// @brief ラッチのセルクラスに名前をつける．
void
Writer::reg_latch_class()
{
  for ( SizeType index = 0; index < ClibSeqAttr::max_index(); ++ index ) {
    ClibSeqAttr seq_attr{index};
    auto class_list = mLibrary.find_latch_class(seq_attr);
    std::ostringstream buf;
    buf << "Latch";
    if ( seq_attr.has_slave_clock() ) {
      buf << "2";
    }
    if ( seq_attr.has_clear() ) {
      if ( seq_attr.has_preset() ) {
	auto cpv1 = seq_attr.cpv1();
	auto cpv2 = seq_attr.cpv2();
	buf << "_RS" << cpv1 << cpv2;
      }
      else {
	buf << "_R";
      }
    }
    else if ( seq_attr.has_preset() ) {
      buf << "_S";
    }
    auto name = buf.str();
    for ( auto cclass: class_list ) {
      mClassNameDict.emplace(cclass.key(), name);
    }
  }
}

// @brief セルグループ名を登録する．
void
Writer::reg_group_name(
  const std::string& name,
  const ClibCellGroup& cgroup
)
{
  mGroupNameDict.emplace(cgroup.key(), name);
}

// @brief セルクラス名を登録する．
void
Writer::reg_class_name(
  const std::string& name,
  const ClibCellGroup& cgroup
)
{
  auto cclass = cgroup.rep_class();
  mClassNameDict.emplace(cclass.key(), name);
}

END_NAMESPACE_YM_CLIB
