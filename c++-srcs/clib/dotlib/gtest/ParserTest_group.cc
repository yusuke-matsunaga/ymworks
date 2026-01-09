
/// @file ParserTest3.cc
/// @brief ParserTest の実装ファイル(3)
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

#if 0
TEST_F(ParserTest, parse_input_voltage1)
{
  auto buf = std::istringstream{
    "( 1.0, 2 );\n"
    ""
  };
  Parser parser{buf, info, false, false};

  auto dst = parse_float_float(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( 1.0, value.complex_elem_value(0).float_value() );
  EXPECT_EQ( 2.0, value.complex_elem_value(1).float_value() );
}
#endif

TEST_F(ParserTest, group_cell0)
{
  // ヘッダの検査
  auto buf = std::istringstream{
    "( test ) {\n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_cell(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  auto& header = value.group_header_value();
  ASSERT_TRUE( header.is_valid() );

  EXPECT_EQ( 1, header.complex_elem_size() );
  EXPECT_EQ( "test", header.complex_elem_value(0).string_value() );
}

TEST_F(ParserTest, parse_cell_area)
{
  auto buf = std::istringstream{
    "( test ) {\n"
    "  area: 1.2; \n"
    "}\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = group_cell(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 1, value.group_elem_size() );
  auto& attr1 = value.group_elem_attr(0);

  EXPECT_EQ( "area", attr1.kwd() );
  EXPECT_EQ( 1.2, attr1.value().float_value() );
}

END_NAMESPACE_YM_DOTLIB
