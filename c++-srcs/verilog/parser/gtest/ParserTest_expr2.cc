
/// @file ParserTest_expr.cc
/// @brief ParserTest_expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtMisc.h"
#include "ym/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, Primary1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto expr = parser.new_Primary(fr, name);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_TRUE( expr->is_simple() );
  EXPECT_EQ( "net1", expr->decompile() );
}

TEST_F(ParserTest, Primary2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto index_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(index2);
  auto expr = parser.new_Primary(fr, name, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_EQ( 2, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_EQ( index2, expr->index(1) );
  EXPECT_THROW( expr->index(2),
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "net1[1][2]", expr->decompile() );
}

TEST_F(ParserTest, Primary3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 3U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto part = parser.new_Part(FileRegion(fr1, fr2), VpiRangeMode::Const, left, right);
  auto expr = parser.new_Primary(fr, name, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_EQ( part, expr->part() );
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "net1[3:0]", expr->decompile() );
}

TEST_F(ParserTest, Primary4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto index_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(index2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto left = parser.new_IntConst(fr3, 3U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto right = parser.new_IntConst(fr4, 0U);
  auto part = parser.new_Part(FileRegion(fr3, fr4), VpiRangeMode::Const, left, right);
  auto expr = parser.new_Primary(fr, name, index_list, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_EQ( 2, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_EQ( index2, expr->index(1) );
  EXPECT_THROW( expr->index(2),
		std::out_of_range );
  EXPECT_EQ( part, expr->part() );
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "net1[1][2][3:0]", expr->decompile() );
}

TEST_F(ParserTest, PrimaryH1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto expr = parser.new_Primary(fr, hname);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_TRUE( expr->is_simple() );
  EXPECT_EQ( "head1.net1", expr->decompile() );
}

TEST_F(ParserTest, PrimaryH2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto index_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(index2);
  auto expr = parser.new_Primary(fr, hname, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_EQ( 2, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_EQ( index2, expr->index(1) );
  EXPECT_THROW( expr->index(2),
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "head1.net1[1][2]", expr->decompile() );
}

TEST_F(ParserTest, PrimaryH3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 3U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto part = parser.new_Part(FileRegion(fr1, fr2), VpiRangeMode::Const, left, right);
  auto expr = parser.new_Primary(fr, hname, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_EQ( part, expr->part() );
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "head1.net1[3:0]", expr->decompile() );
}

TEST_F(ParserTest, PrimaryH4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto index_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(index2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto left = parser.new_IntConst(fr3, 3U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto right = parser.new_IntConst(fr4, 0U);
  auto part = parser.new_Part(FileRegion(fr3, fr4), VpiRangeMode::Const, left, right);
  auto expr = parser.new_Primary(fr, hname, index_list, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_EQ( 2, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_EQ( index2, expr->index(1) );
  EXPECT_THROW( expr->index(2),
		std::out_of_range );
  EXPECT_EQ( part, expr->part() );
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "head1.net1[1][2][3:0]", expr->decompile() );
}

TEST_F(ParserTest, CPrimary1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  auto expr = parser.new_CPrimary(fr, name, index1);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_TRUE( expr->is_const_index() );
  EXPECT_EQ( 1, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_THROW( expr->index(1),
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "param1[1]", expr->decompile() );
}

TEST_F(ParserTest, CPrimary2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto index_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(index2);
  auto expr = parser.new_CPrimary(fr, name, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_TRUE( expr->is_const_index() );
  EXPECT_EQ( 2, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_EQ( index2, expr->index(1) );
  EXPECT_THROW( expr->index(2),
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "param1[1][2]", expr->decompile() );
}

TEST_F(ParserTest, CPrimary3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 3U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto part = parser.new_Part(FileRegion(fr1, fr2), VpiRangeMode::Const, left, right);
  auto expr = parser.new_CPrimary(fr, name, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_TRUE( expr->is_const_index() );
  EXPECT_EQ( 0, expr->index_num() );
  EXPECT_THROW( expr->index(0),
		std::out_of_range );
  EXPECT_EQ( part, expr->part() );
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "param1[3:0]", expr->decompile() );
}

TEST_F(ParserTest, CPrimary4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "param1";
  auto hname = parser.new_HierName(head, name);
  auto index_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(index2);
  auto expr = parser.new_CPrimary(fr, hname, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  EXPECT_EQ( PtExprType::Primary, expr->type() );
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
  EXPECT_TRUE( expr->is_const_index() );
  EXPECT_EQ( 2, expr->index_num() );
  EXPECT_EQ( index1, expr->index(0) );
  EXPECT_EQ( index2, expr->index(1) );
  EXPECT_THROW( expr->index(2),
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
  EXPECT_FALSE( expr->is_simple() );
  EXPECT_EQ( "head1.param1[1][2]", expr->decompile() );
}

END_NAMESPACE_YM_VERILOG
