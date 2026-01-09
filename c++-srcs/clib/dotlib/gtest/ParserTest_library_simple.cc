
/// @file ParserTest_library.cc
/// @brief ParserTest_library の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"
#include "dotlib/Parser.h"
#include "dotlib/AstAttr.h"
#include "dotlib/AstValue.h"

#include "parse_group.h"


BEGIN_NAMESPACE_YM_DOTLIB

TEST_F(ParserTestLibrary, library_header)
{
  // ヘッダの検査
  auto buf = std::istringstream{
    "( test ) {\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );
}

TEST_F(ParserTestLibrary, library_bus_naming_style)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  bus_naming_style: %s[%d];\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "bus_naming_style", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "%s[%d]", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_comment)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  comment: \"this is a comment\";\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "comment", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "this is a comment", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_current_unit)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  current_unit: uA;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "current_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "uA", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_date)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  date: \"2021/06/28\";\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "date", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "2021/06/28", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_delay_model1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: generic_cmos;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "delay_model", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibDelayModel::generic_cmos, value1.delay_model_value() );
}

TEST_F(ParserTestLibrary, library_delay_model2)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: table_lookup;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "delay_model", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibDelayModel::table_lookup, value1.delay_model_value() );
}

TEST_F(ParserTestLibrary, library_delay_model3)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: piecewise_cmos;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "delay_model", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibDelayModel::piecewise_cmos, value1.delay_model_value() );
}

TEST_F(ParserTestLibrary, library_delay_model4)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: dcm;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "delay_model", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibDelayModel::dcm, value1.delay_model_value() );
}

TEST_F(ParserTestLibrary, library_delay_model5)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: polynomial;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "delay_model", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibDelayModel::polynomial, value1.delay_model_value() );
}

TEST_F(ParserTestLibrary, library_delay_model6)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: xyz;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = group_library(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 2, column 16 - 18: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: xyz: Illegal value for 'delay_model'. 'generic_cmos', 'table_lookup', 'piecewise_cmos', 'cmos2', 'dcm' or 'polynomial' are expected.\n",
	     msg_list[0]);
}

TEST_F(ParserTestLibrary, library_em_temp_degradation_factor1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  em_temp_degradation_factor: 1.0;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "em_temp_degradation_factor", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 1.0, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_fpga_domain_style)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  fpga_domain_style: abc;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "fpga_domain_style", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "abc", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_fpga_technology1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  fpga_technology: xyz;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "fpga_technology", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "xyz", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_in_place_swap_mode1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  in_place_swap_mode: match_footprint;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "in_place_swap_mode", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "match_footprint", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_input_threshold_pct_fall1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  input_threshold_pct_fall: 123.456;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "input_threshold_pct_fall", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 123.456, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_input_threshold_pct_rise1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  input_threshold_pct_rise: 123.456;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "input_threshold_pct_rise", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 123.456, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_leakage_power_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  leakage_power_unit: 1mW;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "leakage_power_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "1mW", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_nom_calc_mode1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  nom_calc_mode: abc;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "nom_calc_mode", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "abc", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_nom_process1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  nom_process: 1.1;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "nom_process", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 1.1, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_nom_temperature1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  nom_temperature: 1.2;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "nom_temperature", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 1.2, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_nom_voltage1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  nom_voltage: 1.3;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "nom_voltage", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 1.3, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_output_threshold_pct_fall1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  output_threshold_pct_fall: 123.456;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "output_threshold_pct_fall", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 123.456, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_output_threshold_pct_rise1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  output_threshold_pct_rise: 123.456;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "output_threshold_pct_rise", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 123.456, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_piece_type1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  piece_type: piece_length;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "piece_type", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibVarType::output_net_length, value1.piece_type_value() );
}

TEST_F(ParserTestLibrary, library_piece_type2)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  piece_type: piece_net_pin_cap;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "piece_type", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibVarType::output_net_pin_cap, value1.piece_type_value() );
}

TEST_F(ParserTestLibrary, library_piece_type3)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  piece_type: piece_net_wire_cap;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "piece_type", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibVarType::output_net_wire_cap, value1.piece_type_value() );
}

TEST_F(ParserTestLibrary, library_piece_type4)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  piece_type: piece_total_net_cap;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "piece_type", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( ClibVarType::total_output_net_capacitance, value1.piece_type_value() );
}

TEST_F(ParserTestLibrary, library_power_model1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  power_model: table_lookup;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "power_model", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "table_lookup", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_power_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  power_unit: 100uW;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "power_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "100uW", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_preferred_input_pad_voltage1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  preferred_input_pad_voltage: VSS1;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "preferred_input_pad_voltage", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "VSS1", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_preferred_output_pad_slew_rate_control1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  preferred_output_pad_slew_rate_control: high;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "preferred_output_pad_slew_rate_control", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "high", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_preferred_output_pad_voltage1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  preferred_output_pad_voltage: VDD2;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "preferred_output_pad_voltage", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "VDD2", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_pulling_resistance_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  pulling_resistance_unit: 100ohm;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "pulling_resistance_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "100ohm", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_resistance_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  resistance_unit: 1kohm;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "resistance_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "1kohm", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_revision1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  revision: 1.0a;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "revision", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "1.0a", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_simulation1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  simulation: true;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "simulation", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( true, value1.bool_value() );
}

TEST_F(ParserTestLibrary, library_slew_derate_from_library1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  slew_derate_from_library: 0.9;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "slew_derate_from_library", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 0.9, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_slew_lower_threshold_pct_fall1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  slew_lower_threshold_pct_fall: 30.0;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "slew_lower_threshold_pct_fall", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 30.0, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_slew_lower_threshold_pct_rise1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  slew_lower_threshold_pct_rise: 40.0;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "slew_lower_threshold_pct_rise", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 40.0, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_slew_upper_threshold_pct_fall1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  slew_upper_threshold_pct_fall: 50.0;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "slew_upper_threshold_pct_fall", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 50.0, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_slew_upper_threshold_pct_rise1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  slew_upper_threshold_pct_rise: 60.0;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "slew_upper_threshold_pct_rise", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( 60.0, value1.float_value() );
}

TEST_F(ParserTestLibrary, library_default_threshold_voltage_group1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  default_threshold_voltage_group: high_vt_cell;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "default_threshold_voltage_group", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "high_vt_cell", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_time_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  time_unit: 1ps;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "time_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "1ps", value1.string_value() );
}

TEST_F(ParserTestLibrary, library_voltage_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  voltage_unit: 10ms;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "voltage_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  EXPECT_EQ( "10ms", value1.string_value() );
}

END_NAMESPACE_YM_DOTLIB
