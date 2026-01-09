#ifndef PARSE_GROUP_H
#define PARSE_GROUP_H

/// @file parse_group.h
/// @brief group statement 用のパーサー関数定義
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Parser.h"
#include "dotlib/AstAttr.h"

BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @{
/// @name group statement を読み込む関数
//////////////////////////////////////////////////////////////////////

/// @brief library group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_library(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "library", Parser::sStrHeader);
}

/// @brief base_curves group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_base_curves(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "base_curves", Parser::sStrHeader);
}

/// @brief clut_template group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_clut_template(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "clut_template", Parser::sStrHeader);
}

/// @brief domain group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_domain(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "domain", Parser::sStrHeader);
}

/// @brief fpga_isd group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_fpga_isd(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "fpga_isd", Parser::sStrHeader);
}

/// @brief input_voltage group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_input_voltage(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "input_voltage", Parser::sStrHeader);
}

/// @brief output_voltage group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_output_voltage(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "output_voltage", Parser::sStrHeader);
}

/// @brief operating_conditions group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_operating_conditions(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "operating_conditions", Parser::sStrHeader);
}

/// @brief part group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_part(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "part", Parser::sStrHeader);
}

/// @brief speed_grade group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_speed_grade(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "speed_grade", Parser::sStrHeader);
}

/// @brief poly_template group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_poly_template(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "poly_template", Parser::sStrHeader);
}

/// @brief power_supply group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_power_supply(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "power_supply", Parser::sStrHeader);
}

/// @brief sensitization group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_sensitization(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "sensitization", Parser::sStrHeader);
}

/// @brief table group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_table(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "table", Parser::sStrHeader);
}

/// @brief template group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_template(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "template", Parser::sStrHeader);
}

/// @brief timing_range group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_timing_range(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "timing_range", Parser::sStrHeader);
}

/// @brief type group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_type(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "type", Parser::sStrHeader);
}

/// @brief wire_load group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_wire_load(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "wire_load", Parser::sStrHeader);
}

/// @brief wire_load_selection group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_wire_load_selection(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "wire_load_selection", Parser::sOptStrHeader);
}

/// @brief wire_load_table group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_wire_load_table(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "wire_load_table", Parser::sStrHeader);
}

/// @brief cell group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_cell(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "cell", Parser::sStrHeader);
}

/// @brief scaled_cell group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_scaled_cell(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "scaled_cell", Parser::sStrStrHeader);
}

/// @brief model group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_model(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "model", Parser::sStrHeader);
}

/// @brief dynamic_current group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_dynamic_current(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "dynamic_current", Parser::sEmptyHeader);
}

/// @brief switching_group group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_switching_group(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "switching_group", Parser::sEmptyHeader);
}

/// @brief sg_pg_current group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_sg_pg_current(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "sg_pg_current", Parser::sEmptyHeader);
}

/// @brief sg_vector group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_sg_vector(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "sg_vector", Parser::sEmptyHeader);
}

/// @brief ff group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_ff(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "ff", Parser::sStrStrHeader);
}

/// @brief ff_bank group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_ff_bank(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "ff_bank", Parser::sStrStrIntHeader);
}

/// @brief fpga_condition group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_fpga_condition(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "fpga_condition", Parser::sStrHeader);
}

/// @brief fpga_condition_value group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_fpga_condition_value(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "fpga_condition_value", Parser::sStrHeader);
}

/// @brief functional_yield_metric group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_functional_yield_metric(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "functional_yield_metric", Parser::sEmptyHeader);
}

/// @brief average_number_of_faults group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_average_number_of_faults(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "average_number_of_faults", Parser::sStrHeader);
}

/// @brief leakage_current group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_leakage_current(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "leakage_current", Parser::sEmptyHeader);
}

/// @brief lc_pg_current group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_lc_pg_current(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "lc_pg_current", Parser::sEmptyHeader);
}

/// @brief gate_leakage group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_gate_leakage(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "gate_leakage", Parser::sStrHeader);
}

/// @brief leakage_power group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_leakage_power(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "leakage_power", Parser::sEmptyHeader);
}

/// @brief generated_clock group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_generated_clock(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "generated_clock", Parser::sStrHeader);
}

/// @brief intrinsic_parasitic group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_intrinsic_parasitic(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "intrinsic_parasitic", Parser::sStrHeader);
}

/// @brief intrinsic_capacitance group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_intrinsic_capacitance(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "intrinsic_capacitance", Parser::sStrHeader);
}

/// @brief intrinsic_resistance group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_intrinsic_resistance(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "intrinsic_resistance", Parser::sStrHeader);
}

/// @brief total_capacitance group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_total_capacitance(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "total_capacitance", Parser::sStrHeader);
}

/// @brief latch group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_latch(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "latch", Parser::sStrStrHeader);
}

/// @brief latch_bank group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_latch_bank(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "latch_bank", Parser::sStrStrIntHeader);
}

/// @brief lut group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_lut(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "lut", Parser::sStrHeader);
}

/// @brief mode_definition group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_mode_definition(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "mode_definition", Parser::sStrHeader);
}

/// @brief mode_value group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_mode_value(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "mode_value", Parser::sStrHeader);
}

/// @brief pg_pin group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_pg_pin(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "pg_pin", Parser::sStrHeader);
}

/// @brief routing_track group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_routing_track(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "routing_track", Parser::sStrHeader);
}

/// @brief statetable group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_statetable(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "statetable", Parser::sStrStrHeader);
}

/// @brief test_cell group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_test_cell(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "test_cell", Parser::sEmptyHeader);
}

/// @brief pin group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_pin(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "pin", Parser::sStrHeader);
}

/// @brief bundle group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_bundle(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "bundle", Parser::sStrHeader);
}

/// @brief bus group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_bus(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "bus", Parser::sStrHeader);
}

/// @brief ccsn group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_ccsn(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "ccsn", Parser::sEmptyHeader);
}

/// @brief ccsn_dc_current group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_ccsn_dc_current(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "ccsn_dc_current", Parser::sStrHeader);
}

/// @brief ccsn_output_voltage group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_ccsn_output_voltage(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "ccsn_output_voltage", Parser::sStrHeader);
}

/// @brief ccsn_propagated_noise group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_ccsn_propagated_noise(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "ccsn_propagated_noise", Parser::sStrHeader);
}

/// @brief electromigration group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_electromigration(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "electromigration", Parser::sEmptyHeader);
}

/// @brief em_max_toggle_rate group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_em_max_toggle_rate(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "em_max_toggle_rate", Parser::sStrHeader);
}

/// @brief hyperbolic_noise group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_hyperbolic_noise(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "hyperbolic_noise", Parser::sEmptyHeader);
}

/// @brief internal_power group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_internal_power(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "internal_power", Parser::sEmptyHeader);
}

/// @brief power group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_power(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "power", Parser::sStrHeader);
}

/// @brief max_cap group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_max_cap(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "max_cap", Parser::sStrHeader);
}

/// @brief max_trans group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_max_trans(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "max_trans", Parser::sStrHeader);
}

/// @brief min_pulse_width group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_min_pulse_width(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "min_pulse_width", Parser::sEmptyHeader);
}

/// @brief minimum_period group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_minimum_period(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "minimum_period", Parser::sEmptyHeader);
}

/// @brief pin_capacitance group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_pin_capacitance(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "pin_capacitance", Parser::sEmptyHeader);
}

/// @brief pc_capacitance group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_pc_capacitance(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "pc_capacitance", Parser::sStrHeader);
}

/// @brief pc_capacitance_range group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_pc_capacitance_range(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "pc_capacitance_range", Parser::sStrHeader);
}

/// @brief pc_capacitance_range_lu group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_pc_capacitance_range_lu(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "pc_capacitance_range_lu", Parser::sStrHeader);
}

/// @brief tlatch group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_tlatch(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "tlatch", Parser::sStrHeader);
}

/// @brief timing group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_timing(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "timing", Parser::sOptStrHeader);
}

/// @brief cell_degradation group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_cell_degradation(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "cell_degradation", Parser::sStrHeader);
}

/// @brief cell_rf group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_cell_rf(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "cell_rf", Parser::sStrHeader);
}

/// @brief compact_ccs group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_compact_ccs(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "compact_ccs", Parser::sStrHeader);
}

/// @brief noise_immunity group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_noise_immunity(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "noise_immunity", Parser::sStrHeader);
}

/// @brief output_current_rf group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_output_current_rf(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "output_current_rf", Parser::sStrHeader);
}

/// @brief oc_vector group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_oc_vector(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "oc_vector", Parser::sStrHeader);
}

/// @brief propagated_noise_height group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_propagated_noise_height(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "propagated_noise_height", Parser::sStrHeader);
}

/// @brief receiver_capacitance group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_receiver_capacitance(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "receiver_capacitance", Parser::sEmptyHeader);
}

/// @brief receiver_capacitance_value group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_receiver_capacitance_value(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "receiver_capacitance_value", Parser::sStrHeader);
}

/// @brief retaining_rf group statment のパースを行なう．
/// @return パース結果を返す．
///
/// エラーの時は nullptr を返す．
inline
AstAttrPtr
group_retaining_rf(
  Parser& parser,           ///< [in] パーサー
  const std::string& kwd,   ///< [in] 属性名
  const FileRegion& kwd_loc ///< [in] 属性名の位置
)
{
  return parser.parse_group_statement(kwd, kwd_loc, "retaining_rf", Parser::sStrHeader);
}

//////////////////////////////////////////////////////////////////////
/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_DOTLIB

#endif // PARSE_GROUP_H
