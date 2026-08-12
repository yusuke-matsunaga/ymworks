
/// @file ParserTest_expr.cc
/// @brief ParserTest_expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, FuncCall1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "func1";
  auto arg_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = parser.new_IntConst(fr1, 1U);
  arg_list->push_back(arg1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = parser.new_IntConst(fr2, 2U);
  arg_list->push_back(arg2);
  auto expr = parser.new_FuncCall(fr, name, arg_list, nullptr);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_funccall(expr, AstExpr::FuncCall, {arg1, arg2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "func1(1, 2)", expr->decompile() );
}

TEST_F(ParserTest, FuncCall2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "func1";
  auto hname = parser.new_HierName(head, name);
  auto arg_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = parser.new_IntConst(fr1, 1U);
  arg_list->push_back(arg1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = parser.new_IntConst(fr2, 2U);
  arg_list->push_back(arg2);
  auto expr = parser.new_FuncCall(fr, hname, arg_list, nullptr);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  check_expr_funccall(expr, AstExpr::FuncCall, {arg1, arg2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "head1.func1(1, 2)", expr->decompile() );
}

TEST_F(ParserTest, SysFuncCall)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "$func1";
  auto arg_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = parser.new_IntConst(fr1, 1U);
  arg_list->push_back(arg1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = parser.new_IntConst(fr2, 2U);
  arg_list->push_back(arg2);
  auto expr = parser.new_SysFuncCall(fr, name, arg_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_funccall(expr, AstExpr::SysFuncCall, {arg1, arg2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "$func1(1, 2)", expr->decompile() );
}

TEST_F(ParserTest, IntConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  SizeType uint_val = 1234;
  auto bv_val = BitVector(uint_val);
  auto expr = parser.new_IntConst(fr, uint_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Int, bv_val, nullptr);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( uint_val, expr->index_value() );
  EXPECT_EQ( "1234", expr->decompile() );
}

TEST_F(ParserTest, IntConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* str_val = "1234";
  auto bv_val = BitVector(str_val);
  {
    std::cout << "BitVector(" << str_val << ") = "
	      << bv_val.verilog_string() << std::endl;
  }
  auto int_val = bv_val.to_int();
  auto expr = parser.new_IntConst(fr, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Int, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( str_val, expr->decompile() );
}

TEST_F(ParserTest, DecConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* str_val = "1234";
  auto bv_val = BitVector(str_val);
  auto int_val = bv_val.to_int();
  auto expr = parser.new_IntConst(fr, VpiConstType::Dec, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Dec, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "d1234", expr->decompile() );
}

TEST_F(ParserTest, DecConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  SizeType size = 16;
  const char* str_val = "1234";
  auto bv_val = BitVector(str_val);
  int int_val = bv_val.to_int();
  auto expr = parser.new_IntConst(fr, size, VpiConstType::Dec, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, size, VpiConstType::Dec, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "16'd1234", expr->decompile() );
}

TEST_F(ParserTest, BinConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* str_val = "1001";
  auto bv_val = BitVector(0, false, 2, str_val);
  int int_val = bv_val.to_int();
  auto expr = parser.new_IntConst(fr, VpiConstType::Binary, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Binary, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "b1001", expr->decompile() );
}

TEST_F(ParserTest, BinConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  SizeType size = 4;
  const char* str_val = "1001";
  auto bv_val = BitVector(size, false, 2, str_val);
  int int_val = bv_val.to_int();
  auto expr = parser.new_IntConst(fr, size, VpiConstType::Binary, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, size, VpiConstType::Binary, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "b1001", expr->decompile() );
}

END_NAMESPACE_YM_VERILOG
