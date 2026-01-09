
/// @file ParserTest_simple.cc
/// @brief simple attribute 用のパース関数のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"
#include "dotlib/Parser.h"
#include "dotlib/AstAttr.h"
#include "dotlib/AstValue.h"

#include "parse_simple.h"


BEGIN_NAMESPACE_YM_DOTLIB

TEST_F(ParserTest, simple_bool1)
{
  auto buf = std::istringstream{": true;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_bool(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( true, value.bool_value() );
  EXPECT_EQ( "true", value.decompile() );
}

TEST_F(ParserTest, simple_bool2)
{
  auto buf = std::istringstream{": false;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_bool(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( false, value.bool_value() );
  EXPECT_EQ( "false", value.decompile() );
}

TEST_F(ParserTest, simple_bool3)
{
  // スペルミス
  auto buf = std::istringstream{": fals;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_bool(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 6: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: fals: Illegal value for boolean, only 'true' or 'false' are allowed.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_bool4)
{
  // セミコロンの後に改行がない
  auto buf = std::istringstream{": true; another_attr"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_bool(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 9 - 20: (ERROR  ) [DOTLIB_PARSER]: Syntax error. New-line is expected.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_bool5)
{
  // セミコロンがない．
  auto buf = std::istringstream{": true\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_bool(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error. Semicolon is expected.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_bool6)
{
  // セミコロンがないが，
  // allow_no_semi が true なので成功する．
  auto buf = std::istringstream{": true\n"};
  Parser parser{buf, info, false, true};

  auto dst = simple_bool(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( true, value.bool_value() );
  EXPECT_EQ( "true", value.decompile() );
}

TEST_F(ParserTest, simple_int1)
{
  auto buf = std::istringstream{": 123;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_int(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 123, value.int_value() );
  EXPECT_EQ( "123", value.decompile() );
}

TEST_F(ParserTest, simple_int2)
{
  // 非数字
  auto buf = std::istringstream{": a123;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW ( {
      auto _ = simple_int(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 6: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: a123: Not an integer value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_float1)
{
  auto buf = std::istringstream{": 123.456;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_float(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 123.456, value.float_value() );
  EXPECT_EQ( "123.456", value.decompile() );
}

TEST_F(ParserTest, simple_float2)
{
  // 整数でもOK
  auto buf = std::istringstream{": 123;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_float(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 123, value.float_value() );
  EXPECT_EQ( "123", value.decompile() );
}

TEST_F(ParserTest, simple_float3)
{
  // 先頭が非数字
  auto buf = std::istringstream{": f123.456;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 10: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: f123.456: Not a number value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_string1)
{
  auto buf = std::istringstream{": abcdefg;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( "abcdefg", value.string_value() );
  EXPECT_EQ( "abcdefg", value.decompile() );
}

TEST_F(ParserTest, simple_string2)
{
  // " で囲まれていてもOK
  auto buf = std::istringstream{": \"abcdefg\";\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( "abcdefg", value.string_value() );
  EXPECT_EQ( "abcdefg", value.decompile() );
}

TEST_F(ParserTest, simple_string2b)
{
  // " で囲まれていれば空白を含んでもOK
  auto buf = std::istringstream{": \"abc def\";\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( "abc def", value.string_value() );
  EXPECT_EQ( "\"abc def\"", value.decompile() );
}

TEST_F(ParserTest, simple_string3)
{
  // 先頭が数字でもOK
  auto buf = std::istringstream{": 1.3a;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( "1.3a", value.string_value() );
  EXPECT_EQ( "\"1.3a\"", value.decompile() );
}

TEST_F(ParserTest, simple_delay_model1)
{
  auto buf = std::istringstream{": generic_cmos;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_delay_model(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDelayModel::generic_cmos, value.delay_model_value() );
  EXPECT_EQ( "generic_cmos", value.decompile() );
}

TEST_F(ParserTest, simple_delay_model2)
{
  auto buf = std::istringstream{": table_lookup;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_delay_model(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDelayModel::table_lookup, value.delay_model_value() );
  EXPECT_EQ( "table_lookup", value.decompile() );
}

TEST_F(ParserTest, simple_delay_model3)
{
  auto buf = std::istringstream{": piecewise_cmos;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_delay_model(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDelayModel::piecewise_cmos, value.delay_model_value() );
  EXPECT_EQ( "piecewise_cmos", value.decompile() );
}

TEST_F(ParserTest, simple_delay_model4)
{
  auto buf = std::istringstream{": cmos2;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_delay_model(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDelayModel::cmos2, value.delay_model_value() );
  EXPECT_EQ( "cmos2", value.decompile() );
}

TEST_F(ParserTest, simple_delay_model5)
{
  auto buf = std::istringstream{": dcm;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_delay_model(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDelayModel::dcm, value.delay_model_value() );
  EXPECT_EQ( "dcm", value.decompile() );
}

TEST_F(ParserTest, simple_delay_model6)
{
  // 不適切な文字列．
  auto buf = std::istringstream{": abcdef;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_delay_model(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 8: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: abcdef: Illegal value for 'delay_model'. 'generic_cmos', 'table_lookup', 'piecewise_cmos', 'cmos2', 'dcm' or 'polynomial' are expected.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_direction1)
{
  auto buf = std::istringstream{": input;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_direction(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDirection::input, value.direction_value() );
  EXPECT_EQ( "input", value.decompile() );
}

TEST_F(ParserTest, simple_direction2)
{
  auto buf = std::istringstream{": output;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_direction(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDirection::output, value.direction_value() );
  EXPECT_EQ( "output", value.decompile() );
}

TEST_F(ParserTest, simple_direction3)
{
  auto buf = std::istringstream{": inout;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_direction(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDirection::inout, value.direction_value() );
  EXPECT_EQ( "inout", value.decompile() );
}

TEST_F(ParserTest, simple_direction4)
{
  auto buf = std::istringstream{": internal;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_direction(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibDirection::internal, value.direction_value() );
  EXPECT_EQ( "internal", value.decompile() );
}

TEST_F(ParserTest, simple_direction5)
{
  // 不適切な文字列
  auto buf = std::istringstream{": 0.1;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_direction(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 5: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: 0.1: Illegal value for 'direction'. 'input', 'output', 'inout' or 'internal' are expected.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_timing_sense1)
{
  auto buf = std::istringstream{": positive_unate;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_sense(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingSense::positive_unate, value.timing_sense_value() );
  EXPECT_EQ( "positive_unate", value.decompile() );
}

TEST_F(ParserTest, simple_timing_sense2)
{
  auto buf = std::istringstream{": negative_unate;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_sense(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingSense::negative_unate, value.timing_sense_value() );
  EXPECT_EQ( "negative_unate", value.decompile() );
}

TEST_F(ParserTest, simple_timing_sense3)
{
  auto buf = std::istringstream{": non_unate;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_sense(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingSense::non_unate, value.timing_sense_value() );
  EXPECT_EQ( "non_unate", value.decompile() );
}

TEST_F(ParserTest, simple_timing_sense4)
{
  // 不適切な文字列
  auto buf = std::istringstream{": unate;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_timing_sense(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 7: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: unate: Illegal value for 'timing_sense'. Only 'positive_unate', 'negative_unate', or 'non_unate' are allowed here.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_timing_type1)
{
  auto buf = std::istringstream{": combinational;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::combinational, value.timing_type_value() );
  EXPECT_EQ( "combinational", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type2)
{
  auto buf = std::istringstream{": combinational_rise;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::combinational_rise, value.timing_type_value() );
  EXPECT_EQ( "combinational_rise", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type3)
{
  auto buf = std::istringstream{": combinational_fall;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::combinational_fall, value.timing_type_value() );
  EXPECT_EQ( "combinational_fall", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type4)
{
  auto buf = std::istringstream{": three_state_enable;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::three_state_enable, value.timing_type_value() );
  EXPECT_EQ( "three_state_enable", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type5)
{
  auto buf = std::istringstream{": three_state_disable;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::three_state_disable, value.timing_type_value() );
  EXPECT_EQ( "three_state_disable", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type6)
{
  auto buf = std::istringstream{": three_state_enable_rise;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::three_state_enable_rise, value.timing_type_value() );
  EXPECT_EQ( "three_state_enable_rise", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type7)
{
  auto buf = std::istringstream{": three_state_enable_fall;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::three_state_enable_fall, value.timing_type_value() );
  EXPECT_EQ( "three_state_enable_fall", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type8)
{
  auto buf = std::istringstream{": three_state_disable_rise;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::three_state_disable_rise, value.timing_type_value() );
  EXPECT_EQ( "three_state_disable_rise", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type9)
{
  auto buf = std::istringstream{": three_state_disable_fall;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::three_state_disable_fall, value.timing_type_value() );
  EXPECT_EQ( "three_state_disable_fall", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type10)
{
  auto buf = std::istringstream{": rising_edge;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::rising_edge, value.timing_type_value() );
  EXPECT_EQ( "rising_edge", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type11)
{
  auto buf = std::istringstream{": falling_edge;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::falling_edge, value.timing_type_value() );
  EXPECT_EQ( "falling_edge", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type12)
{
  auto buf = std::istringstream{": preset;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::preset, value.timing_type_value() );
  EXPECT_EQ( "preset", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type13)
{
  auto buf = std::istringstream{": clear;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::clear, value.timing_type_value() );
  EXPECT_EQ( "clear", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type14)
{
  auto buf = std::istringstream{": hold_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::hold_rising, value.timing_type_value() );
  EXPECT_EQ( "hold_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type15)
{
  auto buf = std::istringstream{": hold_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::hold_falling, value.timing_type_value() );
  EXPECT_EQ( "hold_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type16)
{
  auto buf = std::istringstream{": setup_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::setup_rising, value.timing_type_value() );
  EXPECT_EQ( "setup_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type17)
{
  auto buf = std::istringstream{": setup_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::setup_falling, value.timing_type_value() );
  EXPECT_EQ( "setup_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type18)
{
  auto buf = std::istringstream{": recovery_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::recovery_rising, value.timing_type_value() );
  EXPECT_EQ( "recovery_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type19)
{
  auto buf = std::istringstream{": recovery_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::recovery_falling, value.timing_type_value() );
  EXPECT_EQ( "recovery_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type20)
{
  auto buf = std::istringstream{": skew_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::skew_rising, value.timing_type_value() );
  EXPECT_EQ( "skew_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type21)
{
  auto buf = std::istringstream{": skew_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::skew_falling, value.timing_type_value() );
  EXPECT_EQ( "skew_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type22)
{
  auto buf = std::istringstream{": removal_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::removal_rising, value.timing_type_value() );
  EXPECT_EQ( "removal_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type23)
{
  auto buf = std::istringstream{": removal_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::removal_falling, value.timing_type_value() );
  EXPECT_EQ( "removal_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type24)
{
  auto buf = std::istringstream{": non_seq_setup_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::non_seq_setup_rising, value.timing_type_value() );
  EXPECT_EQ( "non_seq_setup_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type25)
{
  auto buf = std::istringstream{": non_seq_setup_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::non_seq_setup_falling, value.timing_type_value() );
  EXPECT_EQ( "non_seq_setup_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type26)
{
  auto buf = std::istringstream{": non_seq_hold_rising;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::non_seq_hold_rising, value.timing_type_value() );
  EXPECT_EQ( "non_seq_hold_rising", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type27)
{
  auto buf = std::istringstream{": non_seq_hold_falling;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::non_seq_hold_falling, value.timing_type_value() );
  EXPECT_EQ( "non_seq_hold_falling", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type28)
{
  auto buf = std::istringstream{": nochange_high_high;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::nochange_high_high, value.timing_type_value() );
  EXPECT_EQ( "nochange_high_high", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type29)
{
  auto buf = std::istringstream{": nochange_high_low;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::nochange_high_low, value.timing_type_value() );
  EXPECT_EQ( "nochange_high_low", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type30)
{
  auto buf = std::istringstream{": nochange_low_high;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::nochange_low_high, value.timing_type_value() );
  EXPECT_EQ( "nochange_low_high", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type31)
{
  auto buf = std::istringstream{": nochange_low_low;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_timing_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibTimingType::nochange_low_low, value.timing_type_value() );
  EXPECT_EQ( "nochange_low_low", value.decompile() );
}

TEST_F(ParserTest, simple_timing_type32)
{
  // 不適切な文字列
  auto buf = std::istringstream{": abc;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_timing_type(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 5: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: abc: Illegal value for 'timing_type'.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_variable_type1)
{
  auto buf = std::istringstream{": input_net_transition;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::input_net_transition, value.variable_type_value() );
  EXPECT_EQ( "input_net_transition", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type2)
{
  auto buf = std::istringstream{": total_output_net_capacitance;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::total_output_net_capacitance, value.variable_type_value() );
  EXPECT_EQ( "total_output_net_capacitance", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type3)
{
  auto buf = std::istringstream{": output_net_length;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::output_net_length, value.variable_type_value() );
  EXPECT_EQ( "output_net_length", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type4)
{
  auto buf = std::istringstream{": output_net_wire_cap;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::output_net_wire_cap, value.variable_type_value() );
  EXPECT_EQ( "output_net_wire_cap", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type5)
{
  auto buf = std::istringstream{": output_net_pin_cap;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::output_net_pin_cap, value.variable_type_value() );
  EXPECT_EQ( "output_net_pin_cap", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type6)
{
  auto buf = std::istringstream{": equal_or_opposite_output_net_capacitance;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::equal_or_opposite_output_net_capacitance, value.variable_type_value() );
  EXPECT_EQ( "equal_or_opposite_output_net_capacitance", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type7)
{
  auto buf = std::istringstream{": input_transition_time;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::input_transition_time, value.variable_type_value() );
}

TEST_F(ParserTest, simple_variable_type8)
{
  auto buf = std::istringstream{": related_out_total_output_net_capacitance;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::related_out_total_output_net_capacitance, value.variable_type_value() );
  EXPECT_EQ( "related_out_total_output_net_capacitance", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type9)
{
  auto buf = std::istringstream{": related_out_output_net_length;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::related_out_output_net_length, value.variable_type_value() );
  EXPECT_EQ( "related_out_output_net_length", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type10)
{
  auto buf = std::istringstream{": related_out_output_net_wire_cap;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::related_out_output_net_wire_cap, value.variable_type_value() );
  EXPECT_EQ( "related_out_output_net_wire_cap", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type11)
{
  auto buf = std::istringstream{": related_out_output_net_pin_cap;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::related_out_output_net_pin_cap, value.variable_type_value() );
  EXPECT_EQ( "related_out_output_net_pin_cap", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type12)
{
  auto buf = std::istringstream{": constrained_pin_transition;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::constrained_pin_transition, value.variable_type_value() );
  EXPECT_EQ( "constrained_pin_transition", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type13)
{
  auto buf = std::istringstream{": related_pin_transition;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_variable_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::related_pin_transition, value.variable_type_value() );
  EXPECT_EQ( "related_pin_transition", value.decompile() );
}

TEST_F(ParserTest, simple_variable_type15)
{
  // 不適切な文字列
  auto buf = std::istringstream{": abc;\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto _ = simple_variable_type(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 5: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: abc: Illegal value for 'variable_type'.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, simple_piece_type1)
{
  auto buf = std::istringstream{": piece_length;\n"};
  Parser parser{buf, info, false, false};

  auto dst = simple_piece_type(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( ClibVarType::output_net_length, value.piece_type_value() );
  EXPECT_EQ( "piece_length", value.decompile() );
}

END_NAMESPACE_YM_DOTLIB
