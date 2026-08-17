
/// @file ParserTest_expr.cc
/// @brief ParserTest_expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "parser/PtDecl.h"
#include "parser/PtMisc.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, Primary1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto expr = parser.new_Primary(fr, name);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_primary(expr, false);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "net1", expr->decompile() );
}

TEST_F(ParserTest, Primary2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto index_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(astmgr.alloc(), index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(astmgr.alloc(), index2);
  auto expr = parser.new_Primary(fr, name, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_primary(expr, false, {index1, index2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "net1[1][2]", expr->decompile() );
}

TEST_F(ParserTest, Primary3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType left_val = 3;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType right_val = 0;
  auto part = new_part(fr1, left_val, fr2, right_val, VpiRangeMode::Const);
  auto expr = parser.new_Primary(fr, name, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_primary(expr, false, {}, part);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << name << "[" << left_val << ":" << right_val << "]";
  EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(ParserTest, Primary4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto index_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(astmgr.alloc(), index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(astmgr.alloc(), index2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  SizeType left_val = 3;
  auto fr4 = make_file_region(4, 4, 4, 4);
  SizeType right_val = 0;
  auto part = new_part(fr3, left_val, fr4, right_val, VpiRangeMode::Const);
  auto expr = parser.new_Primary(fr, name, index_list, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_primary(expr, false, {index1, index2}, part);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << name << "[1][2][" << left_val << ":" << right_val << "]";
  EXPECT_EQ( buf.str(), expr->decompile() );
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
  check_expr_primary(expr, false);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "head1.net1", expr->decompile() );
}

TEST_F(ParserTest, PrimaryH2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto index_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(astmgr.alloc(), index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(astmgr.alloc(), index2);
  auto expr = parser.new_Primary(fr, hname, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  check_expr_primary(expr, false, {index1, index2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "head1.net1[1][2]", expr->decompile() );
}

TEST_F(ParserTest, PrimaryH3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType left_val = 3;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType right_val = 0;
  auto part = new_part(fr1, left_val, fr2, right_val, VpiRangeMode::Const);
  auto expr = parser.new_Primary(fr, hname, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  check_expr_primary(expr, false, {}, part);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << head << "." << name << "[" << left_val << ":" << right_val << "]";
  EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(ParserTest, PrimaryH4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto index_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(astmgr.alloc(), index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(astmgr.alloc(), index2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  SizeType left_val = 3;
  auto fr4 = make_file_region(4, 4, 4, 4);
  SizeType right_val = 0;
  auto part = new_part(fr3, left_val, fr4, right_val, VpiRangeMode::Const);
  auto expr = parser.new_Primary(fr, hname, index_list, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  check_expr_primary(expr, false, {index1, index2}, part);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << head << "." << name << "[1][2]"
      << "[" << left_val << ":" << right_val << "]";
  EXPECT_EQ( buf.str(), expr->decompile() );
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
  check_expr_primary(expr, true, {index1});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "param1[1]", expr->decompile() );
}

TEST_F(ParserTest, CPrimary2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto index_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(astmgr.alloc(), index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(astmgr.alloc(), index2);
  auto expr = parser.new_CPrimary(fr, name, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_primary(expr, true, {index1, index2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "param1[1][2]", expr->decompile() );
}

TEST_F(ParserTest, CPrimary3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType left_val = 3;
  SizeType right_val = 0;
  auto part = new_part(fr1, left_val, fr2, right_val, VpiRangeMode::Const);
  auto expr = parser.new_CPrimary(fr, name, part);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name);
  check_expr_primary(expr, true, {}, part);
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << name << "[" << left_val << ":" << right_val << "]";
  EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(ParserTest, CPrimary4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "param1";
  auto hname = parser.new_HierName(head, name);
  auto index_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = parser.new_IntConst(fr1, 1U);
  index_list->push_back(astmgr.alloc(), index1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = parser.new_IntConst(fr2, 2U);
  index_list->push_back(astmgr.alloc(), index2);
  auto expr = parser.new_CPrimary(fr, hname, index_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr, name, {head});
  check_expr_primary(expr, true, {index1, index2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "head1.param1[1][2]", expr->decompile() );
}

END_NAMESPACE_YM_VERILOG
