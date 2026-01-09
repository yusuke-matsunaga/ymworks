
/// @file ParserTest_library_complex.cc
/// @brief library グループの complex attribute のパーステスト
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

TEST_F(ParserTestLibrary, library_capacitive_load_unit1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  capacitive_load_unit ( 1, pf);\n"
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

  EXPECT_EQ( "capacitive_load_unit", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 2, value1.complex_elem_size() );
  EXPECT_EQ( 1, value1.complex_elem_value(0).float_value() );
  EXPECT_EQ( "pf", value1.complex_elem_value(1).string_value() );
}

TEST_F(ParserTestLibrary, library_default_part1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  default_part ( AUTO, \"-5\");\n"
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

  EXPECT_EQ( "default_part", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 2, value1.complex_elem_size() );
  EXPECT_EQ( "AUTO", value1.complex_elem_value(0).string_value() );
  EXPECT_EQ( "-5", value1.complex_elem_value(1).string_value() );
}

TEST_F(ParserTestLibrary, library_define1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  define ( bork, pin, string);\n"
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

  EXPECT_EQ( "define", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 3, value1.complex_elem_size() );
  EXPECT_EQ( "bork", value1.complex_elem_value(0).string_value() );
  EXPECT_EQ( "pin", value1.complex_elem_value(1).string_value() );
  EXPECT_EQ( "string", value1.complex_elem_value(2).string_value() );
}

TEST_F(ParserTestLibrary, library_define_cell_area1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  define_cell_area ( bond_pads, pad_slots);\n"
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

  EXPECT_EQ( "define_cell_area", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 2, value1.complex_elem_size() );
  EXPECT_EQ( "bond_pads", value1.complex_elem_value(0).string_value() );
  EXPECT_EQ( "pad_slots", value1.complex_elem_value(1).string_value() );
}

TEST_F(ParserTestLibrary, library_define_group1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  define_group ( myGroup, timing);\n"
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

  EXPECT_EQ( "define_group", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 2, value1.complex_elem_size() );
  EXPECT_EQ( "myGroup", value1.complex_elem_value(0).string_value() );
  EXPECT_EQ( "timing", value1.complex_elem_value(1).string_value() );
}

TEST_F(ParserTestLibrary, library_library_features1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  library_features ( report_delay_calculation );\n"
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

  EXPECT_EQ( "library_features", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 1, value1.complex_elem_size() );
  EXPECT_EQ( "report_delay_calculation",
	     value1.complex_elem_value(0).string_value() );
}

TEST_F(ParserTestLibrary, library_piece_define1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  piece_define ( \"0 10 20\");\n"
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

  EXPECT_EQ( "piece_define", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 1, value1.complex_elem_size() );
  EXPECT_EQ( "0 10 20", value1.complex_elem_value(0).string_value() );
}

TEST_F(ParserTestLibrary, library_routing_layers1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  routing_layers ( metal1, metal2, metal3, metal4 );\n"
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

  EXPECT_EQ( "routing_layers", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 4, value1.complex_elem_size() );
  EXPECT_EQ( "metal1", value1.complex_elem_value(0).string_value() );
  EXPECT_EQ( "metal2", value1.complex_elem_value(1).string_value() );
  EXPECT_EQ( "metal3", value1.complex_elem_value(2).string_value() );
  EXPECT_EQ( "metal4", value1.complex_elem_value(3).string_value() );
}

TEST_F(ParserTestLibrary, library_technology1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  technology ( cmos );\n"
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

  EXPECT_EQ( "technology", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 1, value1.complex_elem_size() );
  EXPECT_EQ( ClibTechnology::cmos, value1.complex_elem_value(0).technology_value() );
}

TEST_F(ParserTestLibrary, library_voltage_map1)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  voltage_map ( VDD1, 3.0);\n"
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

  EXPECT_EQ( "voltage_map", attr1.kwd() );
  auto& value1 = attr1.value();
  ASSERT_EQ( 2, value1.complex_elem_size() );
  EXPECT_EQ( "VDD1", value1.complex_elem_value(0).string_value() );
  EXPECT_EQ( 3.0, value1.complex_elem_value(1).float_value() );
}

#if 0
TEST_F(ParserTestLibrary, library_delay_model6)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  delay_model: xyz;\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst == nullptr );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 2, column 16 - 18: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: xyz: Illegal value for 'delay_model'. 'generic_cmos', 'table_lookup', 'piecewise_cmos', 'cmos2', 'dcm' or 'polynomial' are expected.\n",
	     msg_list[0]);
}
#endif

END_NAMESPACE_YM_DOTLIB
