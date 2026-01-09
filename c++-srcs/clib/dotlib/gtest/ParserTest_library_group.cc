
/// @file ParserTest_library_group.cc
/// @brief library の group statement 用パース関数のテスト
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

// ここでは各グループのヘッダの形式のみチェックを行なう．
// 各グループの具体的な属性についてはそれぞれの別個にチェックする．

TEST_F(ParserTestLibrary, library_cell)
{
  auto buf = std::stringstream{
    "( test ) {\n"
    "  cell ( cell_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "cell", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "cell_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_dc_current_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  dc_current_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "dc_current_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_em_lut_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  em_lut_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "em_lut_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_fall_net_delay)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  fall_net_delay ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "fall_net_delay", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_fall_transition_degradation)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  fall_transition_degradation ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "fall_transition_degradation", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_faults_lut_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  faults_lut_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "faults_lut_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_input_voltage)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  input_voltage ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "input_voltage", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_iv_lut_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  iv_lut_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "iv_lut_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_lu_table_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  lu_table_template ( temp_A ) {\n"
    "  }\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value{dst->value()};

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "lu_table_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_noise_lut_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  noise_lut_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "noise_lut_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_operating_conditions)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  operating_conditions ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "operating_conditions", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_output_voltage)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  output_voltage ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "output_voltage", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_part)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  part ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "part", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_poly_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  poly_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "poly_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_power_lut_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  power_lut_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "power_lut_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_power_poly_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  power_poly_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "power_poly_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_power_supply)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  power_supply ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "power_supply", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_propagation_lut_template)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  propagation_lut_template ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "propagation_lut_template", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_rise_net_delay)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  rise_net_delay ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "rise_net_delay", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_rise_transition_degradation)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  rise_transition_degradation ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "rise_transition_degradation", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_scaled_cell)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  scaled_cell ( temp_A, cond_B ) {\n"
    "  }\n"
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

  EXPECT_EQ( "scaled_cell", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 2, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );
  EXPECT_EQ( "cond_B", header1.complex_elem_value(1).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

#if 0
TEST_F(ParserTestLibrary, library_scaling_factors)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  scaling_factors ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "scaling_factors", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}
#endif

TEST_F(ParserTestLibrary, library_timing)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  timing ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "timing", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_timing_range)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  timing_range ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "timing_range", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_type)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  type ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "type", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_wire_load)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  wire_load ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "wire_load", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_wire_load_selection)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  wire_load_selection ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "wire_load_selection", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

TEST_F(ParserTestLibrary, library_wire_load_table)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  wire_load_table ( temp_A ) {\n"
    "  }\n"
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

  EXPECT_EQ( "wire_load_table", attr1.kwd() );
  auto& value1 = attr1.value();
  auto& header1 = value1.group_header_value();
  ASSERT_EQ( 1, header1.complex_elem_size() );
  EXPECT_EQ( "temp_A", header1.complex_elem_value(0).string_value() );

  EXPECT_EQ( 0, value1.group_elem_size() );
}

END_NAMESPACE_YM_DOTLIB
