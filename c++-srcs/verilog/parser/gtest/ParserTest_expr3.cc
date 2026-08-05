
/// @file ParserTest_expr.cc
/// @brief ParserTest_expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "ym/pt/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, FuncCall1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "func1";
  auto arg_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = parser.new_IntConst(fr1, 1U);
  arg_list->push_back(arg1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = parser.new_IntConst(fr2, 2U);
  arg_list->push_back(arg2);
  auto expr = parser.new_FuncCall(fr, name, arg_list, nullptr);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::FuncCall, expr->type() );
  EXPECT_THROW( expr->op_type(),
		std::logic_error );
  EXPECT_EQ( 2, expr->operand_num() );
  EXPECT_THROW( expr->operand(2),
		std::out_of_range );
  EXPECT_EQ( arg1, expr->operand0() );
  EXPECT_EQ( arg1, expr->operand(0) );
  EXPECT_EQ( arg2, expr->operand1() );
  EXPECT_EQ( arg2, expr->operand(1) );
  EXPECT_THROW( expr->operand2(),
		std::logic_error );
  EXPECT_FALSE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( nullptr, expr->part() );
  EXPECT_THROW( expr->const_type(),
		std::logic_error );
  EXPECT_THROW( expr->const_size(),
		std::logic_error );
  EXPECT_THROW( expr->const_int(),
		std::logic_error );
  EXPECT_THROW( expr->const_str(),
		std::logic_error );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "func1(1, 2)", expr->decompile() );
}

TEST_F(ParserTest, FuncCall2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "func1";
  auto hname = parser.new_HierName(head, name);
  auto arg_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = parser.new_IntConst(fr1, 1U);
  arg_list->push_back(arg1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = parser.new_IntConst(fr2, 2U);
  arg_list->push_back(arg2);
  auto expr = parser.new_FuncCall(fr, hname, arg_list, nullptr);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  EXPECT_EQ( PtExprType::FuncCall, expr->type() );
  EXPECT_THROW( expr->op_type(),
		std::logic_error );
  EXPECT_EQ( 2, expr->operand_num() );
  EXPECT_THROW( expr->operand(2),
		std::out_of_range );
  EXPECT_EQ( arg1, expr->operand0() );
  EXPECT_EQ( arg1, expr->operand(0) );
  EXPECT_EQ( arg2, expr->operand1() );
  EXPECT_EQ( arg2, expr->operand(1) );
  EXPECT_THROW( expr->operand2(),
		std::logic_error );
  EXPECT_FALSE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( nullptr, expr->part() );
  EXPECT_THROW( expr->const_type(),
		std::logic_error );
  EXPECT_THROW( expr->const_size(),
		std::logic_error );
  EXPECT_THROW( expr->const_int(),
		std::logic_error );
  EXPECT_THROW( expr->const_str(),
		std::logic_error );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "head1.func1(1, 2)", expr->decompile() );
}

TEST_F(ParserTest, SysFuncCall)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "$func1";
  auto arg_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = parser.new_IntConst(fr1, 1U);
  arg_list->push_back(arg1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = parser.new_IntConst(fr2, 2U);
  arg_list->push_back(arg2);
  auto expr = parser.new_SysFuncCall(fr, name, arg_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::SysFuncCall, expr->type() );
  EXPECT_THROW( expr->op_type(),
		std::logic_error );
  EXPECT_EQ( 2, expr->operand_num() );
  EXPECT_THROW( expr->operand(2),
		std::out_of_range );
  EXPECT_EQ( arg1, expr->operand0() );
  EXPECT_EQ( arg1, expr->operand(0) );
  EXPECT_EQ( arg2, expr->operand1() );
  EXPECT_EQ( arg2, expr->operand(1) );
  EXPECT_THROW( expr->operand2(),
		std::logic_error );
  EXPECT_FALSE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( nullptr, expr->part() );
  EXPECT_THROW( expr->const_type(),
		std::logic_error );
  EXPECT_THROW( expr->const_size(),
		std::logic_error );
  EXPECT_THROW( expr->const_int(),
		std::logic_error );
  EXPECT_THROW( expr->const_str(),
		std::logic_error );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "$func1(1, 2)", expr->decompile() );
}

TEST_F(ParserTest, IntConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  std::uint32_t val = 1234;
  auto expr = parser.new_IntConst(fr, val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  EXPECT_EQ( PtExprType::Const, expr->type() );
  EXPECT_THROW( expr->op_type(),
		std::logic_error );
  EXPECT_EQ( 0, expr->operand_num() );
  EXPECT_THROW( expr->operand(0),
		std::out_of_range );
  EXPECT_THROW( expr->operand0(),
		std::logic_error );
  EXPECT_THROW( expr->operand1(),
		std::logic_error );
  EXPECT_THROW( expr->operand2(),
		std::logic_error );
  EXPECT_FALSE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( nullptr, expr->part() );
  EXPECT_EQ( VpiConstType::Int, expr->const_type() );
  EXPECT_EQ( 0, expr->const_size() );
  EXPECT_EQ( val, expr->const_int() );
  EXPECT_STREQ( nullptr, expr->const_str() );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( val, expr->index_value() );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "1234", expr->decompile() );
}

TEST_F(ParserTest, IntConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* val_str = "1234";
  int val = atoi(val_str);
  auto expr = parser.new_IntConst(fr, val_str);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  EXPECT_EQ( PtExprType::Const, expr->type() );
  EXPECT_THROW( expr->op_type(),
		std::logic_error );
  EXPECT_EQ( 0, expr->operand_num() );
  EXPECT_THROW( expr->operand(0),
		std::out_of_range );
  EXPECT_THROW( expr->operand0(),
		std::logic_error );
  EXPECT_THROW( expr->operand1(),
		std::logic_error );
  EXPECT_THROW( expr->operand2(),
		std::logic_error );
  EXPECT_FALSE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( nullptr, expr->part() );
  EXPECT_EQ( VpiConstType::Int, expr->const_type() );
  EXPECT_EQ( 0, expr->const_size() );
  EXPECT_EQ( val, expr->const_int() );
  EXPECT_STREQ( val_str, expr->const_str() );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( val, expr->index_value() );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( val_str, expr->decompile() );
}

TEST_F(ParserTest, IntConst3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* val_str = "1234";
  int val = atoi(val_str);
  auto expr = parser.new_IntConst(fr, val_str);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  EXPECT_EQ( PtExprType::Const, expr->type() );
  EXPECT_THROW( expr->op_type(),
		std::logic_error );
  EXPECT_EQ( 0, expr->operand_num() );
  EXPECT_THROW( expr->operand(0),
		std::out_of_range );
  EXPECT_THROW( expr->operand0(),
		std::logic_error );
  EXPECT_THROW( expr->operand1(),
		std::logic_error );
  EXPECT_THROW( expr->operand2(),
		std::logic_error );
  EXPECT_FALSE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( nullptr, expr->part() );
  EXPECT_EQ( VpiConstType::Int, expr->const_type() );
  EXPECT_EQ( 0, expr->const_size() );
  EXPECT_EQ( val, expr->const_int() );
  EXPECT_STREQ( val_str, expr->const_str() );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( val, expr->index_value() );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( val_str, expr->decompile() );
}

END_NAMESPACE_YM_VERILOG
