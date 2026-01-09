
/// @file Scanner_value.cc
/// @brief Scanner の実装ファイル(値の読み込み)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/DotlibScanner.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

// @brief int 型を値を読み込む．
AstValuePtr
DotlibScanner::read_int()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto n = tmp_str.size();
  bool ok = true;
  for ( SizeType i = 0; i < n; ++ i ) {
    if ( !isdigit(tmp_str[i]) ) {
      ok = false;
      break;
    }
  }
  if ( ok ) {
    int val = atoi(tmp_str.c_str());
    return AstValue::new_int(val, token.loc());
  }

  std::ostringstream emsg;
  emsg << "Syntax error: " << tmp_str
       << ": Not an integer value.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  emsg.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief float 型の値を読み込む．
AstValuePtr
DotlibScanner::read_float()
{
  bool minus = false;
  auto token = read_token();
  if ( token.type() == TokenType::MINUS ) {
    minus = true;
    token = read_token();
  }
  auto tmp_str = token.str_value();
  char* end;
  auto value = strtod(tmp_str.c_str(), &end);
  if ( minus ) {
    value = - value;
  }
  if ( end[0] == '\0' ) {
    // 全体が float 文字列だった．
    return AstValue::new_float(value, token.loc());
  }

  std::ostringstream emsg;
  emsg << "Syntax error: " << tmp_str
       << ": Not a number value.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  emsg.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief string 型の値を読み込む．
AstValuePtr
DotlibScanner::read_string()
{
  auto token = read_token();
  if ( token.type() == TokenType::SYMBOL ) {
    return AstValue::new_string(token.value(), token.loc());
  }

  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  "Syntax error. 'string' value is expected.");
  throw std::invalid_argument{"Syntax error"};
}

// @brief bool 型の値を読み込む．
AstValuePtr
DotlibScanner::read_bool()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  if ( tmp_str == "true" ) {
    return AstValue::new_bool(true, token.loc());
  }
  if ( tmp_str == "false" ) {
    return AstValue::new_bool(false, token.loc());
  }

  std::ostringstream buf;
  buf << "Syntax error: "
      << tmp_str << ": Illegal value for boolean, only 'true' or 'false' are allowed.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief delay_model 型の値を読み込む．
AstValuePtr
DotlibScanner::read_delay_model()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto value = ClibDelayModel::none;
  if ( tmp_str == "generic_cmos" ) {
    value = ClibDelayModel::generic_cmos;
  }
  else if ( tmp_str == "table_lookup" ) {
    value = ClibDelayModel::table_lookup;
  }
  else if ( tmp_str == "piecewise_cmos" ) {
    value = ClibDelayModel::piecewise_cmos;
  }
  else if ( tmp_str == "cmos2" ) {
    value = ClibDelayModel::cmos2;
  }
  else if ( tmp_str == "dcm" ) {
    value = ClibDelayModel::dcm;
  }
  else if ( tmp_str == "polynomial" ) {
    value = ClibDelayModel::polynomial;
  }
  if ( value != ClibDelayModel::none ) {
    return AstValue::new_delay_model(value, token.loc());
  }

  std::ostringstream buf;
  buf << "Syntax error: " << tmp_str << ": Illegal value for 'delay_model'."
      << " 'generic_cmos', 'table_lookup', "
      << "'piecewise_cmos', 'cmos2', 'dcm' or 'polynomial' are expected.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
}

// @brief direction 型の値を読み込む．
AstValuePtr
DotlibScanner::read_direction()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto value = ClibDirection::none;
  if ( tmp_str == "input" ) {
    value = ClibDirection::input;
  }
  else if ( tmp_str == "output" ) {
    value = ClibDirection::output;
  }
  else if ( tmp_str == "inout" ) {
    value = ClibDirection::inout;
  }
  else if ( tmp_str == "internal" ) {
    value = ClibDirection::internal;
  }
  if ( value != ClibDirection::none ) {
    return AstValue::new_direction(value, token.loc());
  }

  std::ostringstream buf;
  buf << "Syntax error: " << tmp_str << ": Illegal value for 'direction'."
      << " 'input', 'output', 'inout' or 'internal' are expected.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
  return {};
}

// @brief technology 型の値を読み込む．
// @param[in] 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_technology()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto value = ClibTechnology::none;
  if ( tmp_str == "cmos" ) {
    value = ClibTechnology::cmos;
  }
  else if ( tmp_str == "fpga" ) {
    value = ClibTechnology::fpga;
  }
  if ( value != ClibTechnology::none ) {
    return AstValue::new_technology(value, token.loc());
  }

  std::ostringstream buf;
  buf << "Syntax error: " << tmp_str << ": Illegal value for 'technology'. "
      << "Only 'cmos' or 'fpga' are allowed here.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
  return {};
}

// @brief timing_sense 型の値を読み込む．
// @param[in] 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_timing_sense()
{
  auto token = read_token();
  auto value = ClibTimingSense::none;
  auto tmp_str = token.str_value();
  if ( tmp_str == "positive_unate" ) {
    value = ClibTimingSense::positive_unate;
  }
  else if ( tmp_str == "negative_unate" ) {
    value = ClibTimingSense::negative_unate;
  }
  else if ( tmp_str == "non_unate" ) {
    value = ClibTimingSense::non_unate;
  }
  if ( value != ClibTimingSense::none ) {
    return AstValue::new_timing_sense(value, token.loc());
  }

  std::ostringstream buf;
  buf << "Syntax error: " << tmp_str << ": Illegal value for 'timing_sense'."
      << " Only 'positive_unate', 'negative_unate', or 'non_unate' are allowed here.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
  return {};
}

// @brief timing_type 型の値を読み込む．
// @param[in] 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_timing_type()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto value = ClibTimingType::none;
  if ( tmp_str == "combinational" ) {
    value = ClibTimingType::combinational;
  }
  else if ( tmp_str == "combinational_rise" ) {
    value = ClibTimingType::combinational_rise;
  }
  else if ( tmp_str == "combinational_fall" ) {
    value = ClibTimingType::combinational_fall;
  }
  else if ( tmp_str == "three_state_enable" ) {
    value = ClibTimingType::three_state_enable;
  }
  else if ( tmp_str == "three_state_enable_rise" ) {
    value = ClibTimingType::three_state_enable_rise;
  }
  else if ( tmp_str == "three_state_enable_fall" ) {
    value = ClibTimingType::three_state_enable_fall;
  }
  else if ( tmp_str == "three_state_disable" ) {
    value = ClibTimingType::three_state_disable;
  }
  else if ( tmp_str == "three_state_disable_rise" ) {
    value = ClibTimingType::three_state_disable_rise;
  }
  else if ( tmp_str == "three_state_disable_fall" ) {
    value = ClibTimingType::three_state_disable_fall;
  }
  else if ( tmp_str == "rising_edge" ) {
    value = ClibTimingType::rising_edge;
  }
  else if ( tmp_str == "falling_edge" ) {
    value = ClibTimingType::falling_edge;
  }
  else if ( tmp_str == "preset" ) {
    value = ClibTimingType::preset;
  }
  else if ( tmp_str == "clear" ) {
    value = ClibTimingType::clear;
  }
  else if ( tmp_str == "hold_rising" ) {
    value = ClibTimingType::hold_rising;
  }
  else if ( tmp_str == "hold_falling" ) {
    value = ClibTimingType::hold_falling;
  }
  else if ( tmp_str == "setup_rising" ) {
    value = ClibTimingType::setup_rising;
  }
  else if ( tmp_str == "setup_falling" ) {
    value = ClibTimingType::setup_falling;
  }
  else if ( tmp_str == "recovery_rising" ) {
    value = ClibTimingType::recovery_rising;
  }
  else if ( tmp_str == "recovery_falling" ) {
    value = ClibTimingType::recovery_falling;
  }
  else if ( tmp_str == "skew_rising" ) {
    value = ClibTimingType::skew_rising;
  }
  else if ( tmp_str == "skew_falling" ) {
    value = ClibTimingType::skew_falling;
  }
  else if ( tmp_str == "removal_rising" ) {
    value = ClibTimingType::removal_rising;
  }
  else if ( tmp_str == "removal_falling" ) {
    value = ClibTimingType::removal_falling;
  }
  else if ( tmp_str == "non_seq_setup_rising" ) {
    value = ClibTimingType::non_seq_setup_rising;
  }
  else if ( tmp_str == "non_seq_setup_falling" ) {
    value = ClibTimingType::non_seq_setup_falling;
  }
  else if ( tmp_str == "non_seq_hold_rising" ) {
    value = ClibTimingType::non_seq_hold_rising;
  }
  else if ( tmp_str == "non_seq_hold_falling" ) {
    value = ClibTimingType::non_seq_hold_falling;
  }
  else if ( tmp_str == "nochange_high_high" ) {
    value = ClibTimingType::nochange_high_high;
  }
  else if ( tmp_str == "nochange_high_low" ) {
    value = ClibTimingType::nochange_high_low;
  }
  else if ( tmp_str == "nochange_low_high" ) {
    value = ClibTimingType::nochange_low_high;
  }
  else if ( tmp_str == "nochange_low_low" ) {
    value = ClibTimingType::nochange_low_low;
  }
  if ( value == ClibTimingType::none ) {
    std::ostringstream buf;
    buf << "Syntax error: "
	<< tmp_str << ": Illegal value for 'timing_type'.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_SCANNER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
    return {};
  }

  return AstValue::new_timing_type(value, token.loc());
}

// @brief variable type 型の値を読み込む．
// @param[in] 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_variable_type()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto value = ClibVarType::none;
  if ( tmp_str == "input_net_transition" ) {
    value = ClibVarType::input_net_transition;
  }
  else if ( tmp_str == "total_output_net_capacitance" ) {
    value = ClibVarType::total_output_net_capacitance;
  }
  else if ( tmp_str == "equal_or_opposite_output_net_capacitance" ) {
    value = ClibVarType::equal_or_opposite_output_net_capacitance;
  }
  else if ( tmp_str == "input_transition_time" ) {
    value = ClibVarType::input_transition_time;
  }
  else if ( tmp_str == "output_net_length" ) {
    value = ClibVarType::output_net_length;
  }
  else if ( tmp_str == "output_net_wire_cap" ) {
    value = ClibVarType::output_net_wire_cap;
  }
  else if ( tmp_str == "output_net_pin_cap" ) {
    value = ClibVarType::output_net_pin_cap;
  }
  else if ( tmp_str == "related_out_total_output_net_capacitance" ) {
    value = ClibVarType::related_out_total_output_net_capacitance;
  }
  else if ( tmp_str == "related_out_output_net_length" ) {
    value = ClibVarType::related_out_output_net_length;
  }
  else if ( tmp_str == "related_out_output_net_wire_cap" ) {
    value = ClibVarType::related_out_output_net_wire_cap;
  }
  else if ( tmp_str == "related_out_output_net_pin_cap" ) {
    value = ClibVarType::related_out_output_net_pin_cap;
  }
  else if ( tmp_str == "constrained_pin_transition" ) {
    value = ClibVarType::constrained_pin_transition;
  }
  else if ( tmp_str == "related_pin_transition" ) {
    value = ClibVarType::related_pin_transition;
  }
  if ( value != ClibVarType::none ) {
    return AstValue::new_variable_type(value, token.loc());
  }

  std::ostringstream buf;
  buf << "Syntax error: "
      << tmp_str << ": Illegal value for 'variable_type'.";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  token.loc(),
		  MsgType::Error,
		  "DOTLIB_SCANNER",
		  buf.str());
  throw std::invalid_argument{"Syntax error"};
  return {};
}

// @brief piece type 型の値を読み込む．
// @param[in] 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_piece_type()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  auto value = ClibVarType::none;
  if ( tmp_str == "piece_total_net_cap" ) {
    value = ClibVarType::total_output_net_capacitance;
  }
  else if ( tmp_str == "piece_length" ) {
    value = ClibVarType::output_net_length;
  }
  else if ( tmp_str == "piece_net_wire_cap" ) {
    value = ClibVarType::output_net_wire_cap;
  }
  else if ( tmp_str == "piece_net_pin_cap" ) {
    value = ClibVarType::output_net_pin_cap;
  }
  if ( value == ClibVarType::none ) {
    std::ostringstream buf;
    buf << "Syntax error: "
	<< tmp_str << ": Illegal value for 'piece_type'.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_SCANNER",
		    buf.str());
    throw std::invalid_argument{"Syntax error"};
    return {};
  }
  return AstValue::new_piece_type(value, token.loc());
}

// @brief int vector 型の値を読み込む．
// @return 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_int_vector()
{
  auto token = read_token();
  auto tmp_str = token.str_value();
  if ( tmp_str.empty() ) {
    throw std::invalid_argument{"Syntax error"};
    return {};
  }

  std::vector<int> dst_list;
  std::string buf;
  for ( auto c: tmp_str ) {
    if ( isspace(c) ) {
      continue;
    }
    else if ( c == ',' ) {
      if ( buf.size() == 0 ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			token.loc(),
			MsgType::Error,
			"DOTLIB_SCANNER",
			"Syntax error: Null element.");
	throw std::invalid_argument{"Syntax error"};
	return {};
      }
      dst_list.push_back(atoi(buf.c_str()));
      buf.clear();
    }
    else if ( isdigit(c) ) {
      buf += c;
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      token.loc(),
		      MsgType::Error,
		      "DOTLIB_SCANNER",
		      "Syntax error: Integer value expected.");
      throw std::invalid_argument{"Syntax error"};
      return {};
    }
  }
  if ( buf.size() > 0 ) {
    dst_list.push_back(atoi(buf.c_str()));
  }

  return AstValue::new_int_vector(dst_list, token.loc());
}

// @brief float vector 型の値を読み込む．
// @return 生成した AstValue を返す．
//
// エラーが起きた場合にはエラーメッセージを出力して nullptr を返す．
AstValuePtr
DotlibScanner::read_float_vector()
{
  auto token = read_token();
  auto value_loc = token.loc();
  auto tmp_str = token.str_value();
  if ( tmp_str.empty() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    token.loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    "Syntax error. 'string' value is expected.");
    throw std::invalid_argument{"Syntax error"};
    return {};
  }

  std::vector<double> dst_list;
  std::string buf;
  for ( auto c: tmp_str ) {
    if ( isspace(c) ) {
      continue;
    }
    else if ( c == ',' ) {
      if ( buf.size() == 0 ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			value_loc,
			MsgType::Error,
			"DOTLIB_PARSER",
			"Syntax error. Null element.");
	throw std::invalid_argument{"Syntax error"};
	return {};
      }
      char* end;
      auto val = strtod(buf.c_str(), &end);
      if ( end[0] != '\0' ) {
	std::ostringstream emsg;
	emsg << "Syntax error: "
	     << buf << ": Could not convert to a number.";
	MsgMgr::put_msg(__FILE__, __LINE__,
			value_loc,
			MsgType::Error,
			"DOTLIB_PARSER",
			emsg.str());
	throw std::invalid_argument{"Syntax error"};
	return {};
      }
      dst_list.push_back(val);
      buf.clear();
    }
    else {
      buf += c;
    }
  }
  if ( buf.size() > 0 ) {
    dst_list.push_back(strtod(buf.c_str(), nullptr));
  }

  return AstValue::new_float_vector(dst_list, token.loc());
}

END_NAMESPACE_YM_DOTLIB
