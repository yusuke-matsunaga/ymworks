
/// @file ParserTest_complex.cc
/// @brief commplex attribute 用のパース関数のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"
#include "dotlib/Parser.h"
#include "dotlib/AstAttr.h"
#include "dotlib/AstValue.h"

#include "parse_complex.h"


BEGIN_NAMESPACE_YM_DOTLIB

TEST_F(ParserTest, complex_float_float1)
{
  auto buf = std::istringstream{"( 1.0, 2 );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_float_float(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( 1.0, value.complex_elem_value(0).float_value() );
  EXPECT_EQ( 2.0, value.complex_elem_value(1).float_value() );

  EXPECT_EQ( "( 1, 2)", value.decompile() );
}

TEST_F(ParserTest, complex_float_float2)
{
  // 要素数が少ない
  auto buf = std::istringstream{"( 1.0 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_float3)
{
  // 要素数が多い
  auto buf = std::istringstream{"( 1.0, 2.0, 3.0 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 11: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_float4)
{
  // 1番目の要素の型が合わない．
  auto buf = std::istringstream{"( 1.0f, 2.0 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 6: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: 1.0f: Not a number value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_float5)
{
  // 2番目の要素の型が合わない．
  auto buf = std::istringstream{"( 1.0, false );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 8 - 12: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: false: Not a number value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_string1)
{
  auto buf = std::istringstream{"( 1.0, ff );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_float_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( 1.0, value.complex_elem_value(0).float_value() );
  EXPECT_EQ( "ff", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "( 1, ff)", value.decompile() );
}

TEST_F(ParserTest, complex_float_string2)
{
  // " で囲まれた文字列
  auto buf = std::istringstream{"( 1.0, \"ff\" );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_float_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( 1.0, value.complex_elem_value(0).float_value() );
  EXPECT_EQ( "ff", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "( 1, ff)", value.decompile() );
}

TEST_F(ParserTest, complex_float_string3)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( 1.0 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_string4)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( 1.0, ff, 2.0 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 10: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_string5)
{
  // 1番目の要素の型が不適
  auto buf = std::istringstream{"( a1.0, \"ff\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 6: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: a1.0: Not a number value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_vector1)
{
  auto buf = std::istringstream{"( \"1.0, 2.0, 3.0\" );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_float_vector(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( "( \"1, 2, 3\")", value.decompile() );

  EXPECT_EQ( 1, value.complex_elem_size() );
  auto fv = value.complex_elem_value(0).float_vector_value();
  EXPECT_EQ( 3, fv.size() );
  EXPECT_EQ( 1.0, fv[0] );
  EXPECT_EQ( 2.0, fv[1] );
  EXPECT_EQ( 3.0, fv[2] );
}

TEST_F(ParserTest, complex_float_vector2)
{
  // 文字列の内容が不適切
  auto buf = std::istringstream{"( \"1.0, 2.0a, 3.0\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 18: (ERROR  ) [DOTLIB_PARSER]: Syntax error: 2.0a: Could not convert to a number.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_vector3)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"(  );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 4: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_float_vector4)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( \"1.0, 2.0, 3.0\", \"4.0, 5.0\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_float_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 18: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float1)
{
  auto buf = std::istringstream{"( 1, 2.3 );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_int_float(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( 1, value.complex_elem_value(0).int_value() );
  EXPECT_EQ( 2.3, value.complex_elem_value(1).float_value() );
  EXPECT_EQ( "( 1, 2.3)", value.decompile() );
}

TEST_F(ParserTest, complex_int_float2)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( 1 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 5: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float3)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( 1, 2.3, 3.0 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  EXPECT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 9: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float4)
{
  // 1番目の要素の型が不適切
  auto buf = std::istringstream{"( 1x, 2.3 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 4: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: 1x: Not an integer value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float5)
{
  // 2番目の要素の型が不適切
  auto buf = std::istringstream{"( 1, _2.3 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 6 - 9: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: _2.3: Not a number value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float_vector1)
{
  auto buf = std::istringstream{"( 1, \"2.3, 4.5\" );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_int_float_vector(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( 1, value.complex_elem_value(0).int_value() );
  auto fv = value.complex_elem_value(1).float_vector_value();
  EXPECT_EQ( 2, fv.size() );
  EXPECT_EQ( 2.3, fv[0] );
  EXPECT_EQ( 4.5, fv[1] );
  EXPECT_EQ( "( 1, \"2.3, 4.5\")", value.decompile() );
}

TEST_F(ParserTest, complex_int_float_vector2)
{
  // 要素の数が少ない．
  auto buf = std::istringstream{"( 1 );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 5: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float_vector3)
{
  // 要素の数が多い．
  auto buf = std::istringstream{"( 1, \"2.3, 4.5\", abc );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 16: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_float_vector4)
{
  // 1番目の要素の型が不適切
  auto buf = std::istringstream{"( 1a, \"2.3, 4.5\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_float_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 4: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: 1a: Not an integer value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_vector1)
{
  auto buf = std::istringstream{"( \"1, 2, 3\" );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_int_vector(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( "( \"1, 2, 3\")", value.decompile() );

  EXPECT_EQ( 1, value.complex_elem_size() );
  auto iv = value.complex_elem_value(0).int_vector_value();
  EXPECT_EQ( 3, iv.size() );
  EXPECT_EQ( 1, iv[0] );
  EXPECT_EQ( 2, iv[1] );
  EXPECT_EQ( 3, iv[2] );
}

TEST_F(ParserTest, complex_int_vector2)
{
  // 要素の数が少ない．
  auto buf = std::istringstream{"(  );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 4: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_int_vector3)
{
  // 要素の数が多い．
  auto buf = std::istringstream{"( \"1, 2, 3\", \"4, 5\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_int_vector(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 12: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string1)
{
  auto buf = std::istringstream{"( abc );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( "( abc)", value.decompile() );

  EXPECT_EQ( 1, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );

}

TEST_F(ParserTest, complex_string2)
{
  auto buf = std::istringstream{"( \"abc\" );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( "( abc)", value.decompile() );

  EXPECT_EQ( 1, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
}

TEST_F(ParserTest, complex_string3)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 3: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string4)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( abc, def );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 6: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_float1)
{
  auto buf = std::istringstream{"( abc, 1.2 );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_float(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( 1.2, value.complex_elem_value(1).float_value() );
  EXPECT_EQ( "( abc, 1.2)", value.decompile() );
}

TEST_F(ParserTest, complex_string_float2)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( abc );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_float3)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( abc, 1.2, xyz );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 11: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_float4)
{
  // 2番目の要素の型が不適
  auto buf = std::istringstream{"( abc, \"xyz\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_float(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 8 - 12: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: xyz: Not a number value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_int1)
{
  auto buf = std::istringstream{"( abc, 1 );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_int(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( 1, value.complex_elem_value(1).int_value() );
  EXPECT_EQ( "( abc, 1)", value.decompile() );
}

TEST_F(ParserTest, complex_string_int2)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( abc );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_int(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_int3)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( abc, 1, xyz );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_int(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 9: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_int4)
{
  // 2番目の要素の型が不適
  auto buf = std::istringstream{"( abc, \"cd\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_int(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 8 - 11: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: cd: Not an integer value.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_list1)
{
  auto buf = std::istringstream{"( abc, def, ghi );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_list(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 3, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( "def", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "ghi", value.complex_elem_value(2).string_value() );
  EXPECT_EQ( "( abc, def, ghi)", value.decompile() );
}

TEST_F(ParserTest, complex_string_list2)
{
  auto buf = std::istringstream{"( \"abc\", def, ghi );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_list(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 3, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( "def", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "ghi", value.complex_elem_value(2).string_value() );
  EXPECT_EQ( "( abc, def, ghi)", value.decompile() );
}

TEST_F(ParserTest, complex_string_list3)
{
  // 定義上は空もあり．
  auto buf = std::istringstream{"( );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_list(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 0, value.complex_elem_size() );
  EXPECT_EQ( "( )", value.decompile() );
}

TEST_F(ParserTest, complex_string_string1)
{
  auto buf = std::istringstream{"( abc, def );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( "def", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "( abc, def)", value.decompile() );
}

TEST_F(ParserTest, complex_string_string2)
{
  auto buf = std::istringstream{"( abc, \"def\" );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 2, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( "def", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "( abc, def)", value.decompile() );
}

TEST_F(ParserTest, complex_string_string3)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( abc );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_string4)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( abc, def, xyz );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 11: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 2.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_string_string1)
{
  auto buf = std::istringstream{"( abc, \"def\", xyz );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_string_string_string(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();

  EXPECT_EQ( 3, value.complex_elem_size() );
  EXPECT_EQ( "abc", value.complex_elem_value(0).string_value() );
  EXPECT_EQ( "def", value.complex_elem_value(1).string_value() );
  EXPECT_EQ( "xyz", value.complex_elem_value(2).string_value() );
  EXPECT_EQ( "( abc, def, xyz)", value.decompile() );
}

TEST_F(ParserTest, complex_string_string_string2)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( abc, \"def\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_string_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 14: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 3.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_string_string_string3)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( abc, \"def\", xyz, \"012\" );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_string_string_string(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 18: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 3.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_technology1)
{
  auto buf = std::istringstream{"( cmos );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_technology(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( "( cmos)", value.decompile() );

  EXPECT_EQ( 1, value.complex_elem_size() );
  EXPECT_EQ( ClibTechnology::cmos, value.complex_elem_value(0).technology_value() );
}

TEST_F(ParserTest, complex_technology2)
{
  auto buf = std::istringstream{"( fpga );\n"};
  Parser parser{buf, info, false, false};

  auto dst = complex_technology(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( "( fpga)", value.decompile() );

  EXPECT_EQ( 1, value.complex_elem_size() );
  EXPECT_EQ( ClibTechnology::fpga, value.complex_elem_value(0).technology_value() );
}

TEST_F(ParserTest, complex_technology3)
{
  // 要素数が少ない．
  auto buf = std::istringstream{"( );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_technology(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 3: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too few values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_technology4)
{
  // 要素数が多い．
  auto buf = std::istringstream{"( cmos, fpga );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_technology(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 1 - 7: (ERROR  ) [DOTLIB_PARSER]: Syntax error: Too many values, expected 1.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_technology5)
{
  // 内容が不適切
  auto buf = std::istringstream{"( abc );\n"};
  Parser parser{buf, info, false, false};

  EXPECT_THROW( {
      auto dst = complex_technology(parser, kwd, kwd_loc);
    }, std::invalid_argument );
  auto msg_list = mh.message_list();
  ASSERT_EQ( 1, msg_list.size() );
  EXPECT_EQ( "parser_test.lib: line 1, column 3 - 5: (ERROR  ) [DOTLIB_SCANNER]: Syntax error: abc: Illegal value for 'technology'. Only 'cmos' or 'fpga' are allowed here.\n",
	     msg_list[0]);
}

TEST_F(ParserTest, complex_values1)
{
  auto buf = std::istringstream{
    "( \"1.1, 2.2, 3.3\", \"4.4, 5.5, 6.6\" );\n"
  };
  Parser parser{buf, info, false, false};

  auto dst = complex_values(parser, kwd, kwd_loc);

  ASSERT_TRUE( dst != nullptr );
  auto& value = dst->value();
  EXPECT_EQ( "( \"1.1, 2.2, 3.3\", \"4.4, 5.5, 6.6\")", value.decompile() );

  EXPECT_EQ( 2, value.complex_elem_size() );

  auto fv1 = value.complex_elem_value(0).float_vector_value();
  EXPECT_EQ( 3, fv1.size() );
  EXPECT_EQ( 1.1, fv1[0] );
  EXPECT_EQ( 2.2, fv1[1] );
  EXPECT_EQ( 3.3, fv1[2] );

  auto fv2 = value.complex_elem_value(1).float_vector_value();
  EXPECT_EQ( 3, fv2.size() );
  EXPECT_EQ( 4.4, fv2[0] );
  EXPECT_EQ( 5.5, fv2[1] );
  EXPECT_EQ( 6.6, fv2[2] );
}

END_NAMESPACE_YM_DOTLIB
