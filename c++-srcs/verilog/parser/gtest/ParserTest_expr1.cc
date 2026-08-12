
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

TEST_F(ParserTest, Opr1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  for ( auto type: { VpiOpType::Minus, VpiOpType::Plus,
		     VpiOpType::Not, VpiOpType::BitNeg,
		     VpiOpType::UnaryAnd, VpiOpType::UnaryNand,
		     VpiOpType::UnaryOr, VpiOpType::UnaryNor,
		     VpiOpType::UnaryXor, VpiOpType::UnaryXNor,
		     VpiOpType::Posedge, VpiOpType::Negedge,
		     VpiOpType::Null} ) {
    auto expr = parser.new_Opr(fr, type, expr1, nullptr);

    ASSERT_TRUE( expr != nullptr );
    check_expr_name(expr);
    check_expr_opr(expr, type, {expr1});
    bool exp_is_index_expr = true;
    if ( type == VpiOpType::Posedge || type == VpiOpType::Negedge ) {
      exp_is_index_expr = false;
    }
    EXPECT_EQ( exp_is_index_expr, expr->is_index_expr() );
    if ( exp_is_index_expr ) {
      int exp_index_value = 0;
      switch ( type ) {
      case VpiOpType::Minus:     exp_index_value = -123; break;
      case VpiOpType::Plus:      exp_index_value = 123; break;
      case VpiOpType::Not:       exp_index_value = 0; break;
      case VpiOpType::BitNeg:    exp_index_value = ~123; break;
      case VpiOpType::UnaryAnd:  exp_index_value = 0; break;
      case VpiOpType::UnaryNand: exp_index_value = 1; break;
      case VpiOpType::UnaryOr:   exp_index_value = 1; break;
      case VpiOpType::UnaryNor:  exp_index_value = 0; break;
      case VpiOpType::UnaryXor:  exp_index_value = 0; break;
      case VpiOpType::UnaryXNor: exp_index_value = 1; break;
      case VpiOpType::Null:      exp_index_value = 123; break;
      default: break;
      }
      EXPECT_EQ( exp_index_value, expr->index_value() );
    }
    else {
      EXPECT_THROW( expr->index_value(),
		    std::logic_error );
    }
    EXPECT_THROW( expr->is_simple(),
		  std::logic_error );
    std::string exp_decompile;
    switch ( type ) {
    case VpiOpType::Minus:     exp_decompile = "-123"; break;
    case VpiOpType::Plus:      exp_decompile = "+123"; break;
    case VpiOpType::Not:       exp_decompile = "!123"; break;
    case VpiOpType::BitNeg:    exp_decompile = "~123"; break;
    case VpiOpType::UnaryAnd:  exp_decompile = "&123"; break;
    case VpiOpType::UnaryNand: exp_decompile = "~&123"; break;
    case VpiOpType::UnaryOr:   exp_decompile = "|123"; break;
    case VpiOpType::UnaryNor:  exp_decompile = "~|123"; break;
    case VpiOpType::UnaryXor:  exp_decompile = "^123"; break;
    case VpiOpType::UnaryXNor: exp_decompile = "~^123"; break;
    case VpiOpType::Posedge:   exp_decompile = "posedge 123"; break;
    case VpiOpType::Negedge:   exp_decompile = "negedge 123"; break;
    case VpiOpType::Null:      exp_decompile = "123";
    default: break;
    }
    EXPECT_EQ( exp_decompile, expr->decompile() );
  }
}

TEST_F(ParserTest, Opr2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto expr2 = parser.new_IntConst(fr2, 4U);
  for ( auto type: { VpiOpType::Sub,
		     VpiOpType::Div, VpiOpType::Mod,
		     VpiOpType::Eq, VpiOpType::Neq,
		     VpiOpType::CaseEq, VpiOpType::CaseNeq,
		     VpiOpType::Gt, VpiOpType::Ge,
		     VpiOpType::Lt, VpiOpType::Le,
		     VpiOpType::LShift, VpiOpType::RShift,
		     VpiOpType::Add, VpiOpType::Mult,
		     VpiOpType::LogAnd, VpiOpType::LogOr,
		     VpiOpType::BitAnd, VpiOpType::BitOr,
		     VpiOpType::BitXor, VpiOpType::BitXNor,
		     VpiOpType::ArithLShift, VpiOpType::ArithRShift} ) {
    auto expr = parser.new_Opr(fr, type, expr1, expr2, nullptr);

    ASSERT_TRUE( expr != nullptr );
    check_expr_name(expr);
    check_expr_opr(expr, type, {expr1, expr2});
    EXPECT_TRUE( expr->is_index_expr() );
    int exp_index_value = 0;
    switch ( type ) {
    case VpiOpType::Sub:         exp_index_value = 123 - 4; break;
    case VpiOpType::Div:         exp_index_value = 123 / 4; break;
    case VpiOpType::Mod:         exp_index_value = 123 % 4; break;
    case VpiOpType::Eq:          exp_index_value = 123 == 4; break;
    case VpiOpType::Neq:         exp_index_value = 123 != 4; break;
    case VpiOpType::CaseEq:      exp_index_value = 123 == 4; break;
    case VpiOpType::CaseNeq:     exp_index_value = 123 != 4; break;
    case VpiOpType::Gt:          exp_index_value = 123 > 4; break;
    case VpiOpType::Ge:          exp_index_value = 123 >= 4; break;
    case VpiOpType::Lt:          exp_index_value = 123 < 4; break;
    case VpiOpType::Le:          exp_index_value = 123 <= 4; break;
    case VpiOpType::LShift:      exp_index_value = 123 << 4; break;
    case VpiOpType::RShift:      exp_index_value = 123 >> 4; break;
    case VpiOpType::Add:         exp_index_value = 123 + 4; break;
    case VpiOpType::Mult:        exp_index_value = 123 * 4; break;
    case VpiOpType::LogAnd:      exp_index_value = 1; break;
    case VpiOpType::LogOr:       exp_index_value = 1; break;
    case VpiOpType::BitAnd:      exp_index_value = 123 & 4; break;
    case VpiOpType::BitOr:       exp_index_value = 123 | 4; break;
    case VpiOpType::BitXor:      exp_index_value = 123 ^ 4; break;
    case VpiOpType::BitXNor:     exp_index_value = ~(123 ^ 4); break;
    case VpiOpType::ArithLShift: exp_index_value = 123 << 4; break;
    case VpiOpType::ArithRShift: exp_index_value = 123 >> 4; break;
    default: break;
    }
    EXPECT_EQ( exp_index_value, expr->index_value() );
    EXPECT_THROW( expr->is_simple(),
		  std::logic_error );
    std::string exp_decompile;
    switch ( type ) {
    case VpiOpType::Sub:         exp_decompile = "123-4"; break;
    case VpiOpType::Div:         exp_decompile = "123/4"; break;
    case VpiOpType::Mod:         exp_decompile = "123%4"; break;
    case VpiOpType::Eq:          exp_decompile = "123==4"; break;
    case VpiOpType::Neq:         exp_decompile = "123!=4"; break;
    case VpiOpType::CaseEq:      exp_decompile = "123===4"; break;
    case VpiOpType::CaseNeq:     exp_decompile = "123!==4"; break;
    case VpiOpType::Gt:          exp_decompile = "123>4"; break;
    case VpiOpType::Ge:          exp_decompile = "123>=4"; break;
    case VpiOpType::Lt:          exp_decompile = "123<4"; break;
    case VpiOpType::Le:          exp_decompile = "123<=4"; break;
    case VpiOpType::LShift:      exp_decompile = "123<<4"; break;
    case VpiOpType::RShift:      exp_decompile = "123>>4"; break;
    case VpiOpType::Add:         exp_decompile = "123+4"; break;
    case VpiOpType::Mult:        exp_decompile = "123*4"; break;
    case VpiOpType::LogAnd:      exp_decompile = "123&&4"; break;
    case VpiOpType::LogOr:       exp_decompile = "123||4"; break;
    case VpiOpType::BitAnd:      exp_decompile = "123&4"; break;
    case VpiOpType::BitOr:       exp_decompile = "123|4"; break;
    case VpiOpType::BitXor:      exp_decompile = "123^4"; break;
    case VpiOpType::BitXNor:     exp_decompile = "123~^4"; break;
    case VpiOpType::ArithLShift: exp_decompile = "123<<<4"; break;
    case VpiOpType::ArithRShift: exp_decompile = "123>>>4"; break;
    default: break;
    }
    EXPECT_EQ( exp_decompile, expr->decompile() );
  }
}

TEST_F(ParserTest, Opr3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr1 = parser.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto expr2 = parser.new_IntConst(fr2, 2U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto expr3 = parser.new_IntConst(fr3, 3U);
  auto type = VpiOpType::Condition;
  auto expr = parser.new_Opr(fr, type, expr1, expr2, expr3, nullptr);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_opr(expr, type, {expr1, expr2, expr3});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "1?2:3", expr->decompile() );
}

TEST_F(ParserTest, Concat)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto expr_list = parser.new_expr_list();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr1 = parser.new_IntConst(fr1, 1U);
  expr_list->push_back(expr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto expr2 = parser.new_IntConst(fr2, 2U);
  expr_list->push_back(expr2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto expr3 = parser.new_IntConst(fr3, 3U);
  expr_list->push_back(expr3);
  auto expr = parser.new_Concat(fr, expr_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_opr(expr, VpiOpType::Concat, {expr1, expr2, expr3});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "{1,2,3}", expr->decompile() );
}

TEST_F(ParserTest, MultiConcat)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto rep = parser.new_IntConst(fr1, 4U);
  auto expr_list = parser.new_expr_list();
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto expr1 = parser.new_IntConst(fr2, 1U);
  expr_list->push_back(expr1);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto expr2 = parser.new_IntConst(fr3, 2U);
  expr_list->push_back(expr2);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto expr3 = parser.new_IntConst(fr4, 3U);
  expr_list->push_back(expr3);
  auto expr = parser.new_MultiConcat(fr, rep, expr_list);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_multiconcat(expr, VpiOpType::MultiConcat, rep, {expr1, expr2, expr3});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "{4{1,2,3}}", expr->decompile() );
}

TEST_F(ParserTest, MinTypMax)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr1 = parser.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto expr2 = parser.new_IntConst(fr2, 2U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto expr3 = parser.new_IntConst(fr3, 3U);
  auto expr = parser.new_MinTypMax(fr, expr1, expr2, expr3);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_opr(expr, VpiOpType::MinTypMax, {expr1, expr2, expr3});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_THROW( expr->is_simple(),
		std::logic_error );
  EXPECT_EQ( "1:2:3", expr->decompile() );
}

END_NAMESPACE_YM_VERILOG
