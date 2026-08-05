
/// @file ParserTest_stmt.cc
/// @brief ParserTest_stmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "ym/pt/PtStmt.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, If)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.new_IntConst(fr1, 0U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto stmt = parser.new_If(fr, cond, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::If, stmt->type() );
  EXPECT_STREQ( "if statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, IfElse)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.new_IntConst(fr1, 0U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto body2 = parser.new_NullStmt(fr3);
  auto stmt = parser.new_If(fr, cond, body, body2);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::If, stmt->type() );
  EXPECT_STREQ( "if statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( body2, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, CaseItem)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto expr_list = parser.new_list<const PtExpr>();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.new_IntConst(fr1, 1U);
  expr_list->push_back(expr);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto caseitem = parser.new_CaseItem(fr, expr_list, body);

  ASSERT_TRUE( caseitem != nullptr );
  EXPECT_EQ( fr, caseitem->file_region() );
  EXPECT_EQ( 1, caseitem->label_num() );
  EXPECT_EQ( expr, caseitem->label(0) );
  EXPECT_THROW( caseitem->label(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{expr},
	     caseitem->label_list() );
  EXPECT_EQ( body, caseitem->body() );
}

TEST_F(ParserTest, Case)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.new_IntConst(fr1, 0U);

  auto caseitem_list = parser.new_list<const PtCaseItem>();

  auto ci1_fr = make_file_region(1, 2, 3, 4);
  auto ci1_expr_list = parser.new_list<const PtExpr>();
  auto ci1_fr1 = make_file_region(1, 1, 1, 1);
  auto ci1_expr = parser.new_IntConst(fr1, 1U);
  ci1_expr_list->push_back(ci1_expr);
  auto ci1_fr2 = make_file_region(2, 2, 2, 2);
  auto ci1_body = parser.new_NullStmt(ci1_fr2);
  auto caseitem1 = parser.new_CaseItem(ci1_fr, ci1_expr_list, ci1_body);
  caseitem_list->push_back(caseitem1);

  auto ci2_fr = make_file_region(11, 2, 13, 4);
  auto ci2_expr_list = parser.new_list<const PtExpr>();
  auto ci2_fr1 = make_file_region(11, 1, 11, 1);
  auto ci2_expr = parser.new_IntConst(fr1, 2U);
  ci2_expr_list->push_back(ci2_expr);
  auto ci2_fr2 = make_file_region(12, 2, 12, 2);
  auto ci2_body = parser.new_NullStmt(ci2_fr2);
  auto caseitem2 = parser.new_CaseItem(ci2_fr, ci2_expr_list, ci2_body);
  caseitem_list->push_back(caseitem2);

  auto stmt = parser.new_Case(fr, cond, caseitem_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::Case, stmt->type() );
  EXPECT_STREQ( "case statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 2, stmt->caseitem_num() );
  EXPECT_EQ( caseitem1, stmt->caseitem(0) );
  EXPECT_EQ( caseitem2, stmt->caseitem(1) );
  EXPECT_THROW( stmt->caseitem(2),
		std::out_of_range );
  auto exp_caseitem_list = std::vector<const PtCaseItem*>{caseitem1, caseitem2};
  EXPECT_EQ( exp_caseitem_list, stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, CaseX)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.new_IntConst(fr1, 0U);

  auto caseitem_list = parser.new_list<const PtCaseItem>();

  auto ci1_fr = make_file_region(1, 2, 3, 4);
  auto ci1_expr_list = parser.new_list<const PtExpr>();
  auto ci1_fr1 = make_file_region(1, 1, 1, 1);
  auto ci1_expr = parser.new_IntConst(fr1, 1U);
  ci1_expr_list->push_back(ci1_expr);
  auto ci1_fr2 = make_file_region(2, 2, 2, 2);
  auto ci1_body = parser.new_NullStmt(ci1_fr2);
  auto caseitem1 = parser.new_CaseItem(ci1_fr, ci1_expr_list, ci1_body);
  caseitem_list->push_back(caseitem1);

  auto ci2_fr = make_file_region(11, 2, 13, 4);
  auto ci2_expr_list = parser.new_list<const PtExpr>();
  auto ci2_fr1 = make_file_region(11, 1, 11, 1);
  auto ci2_expr = parser.new_IntConst(fr1, 2U);
  ci2_expr_list->push_back(ci2_expr);
  auto ci2_fr2 = make_file_region(12, 2, 12, 2);
  auto ci2_body = parser.new_NullStmt(ci2_fr2);
  auto caseitem2 = parser.new_CaseItem(ci2_fr, ci2_expr_list, ci2_body);
  caseitem_list->push_back(caseitem2);

  auto stmt = parser.new_CaseX(fr, cond, caseitem_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::CaseX, stmt->type() );
  EXPECT_STREQ( "casex statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 2, stmt->caseitem_num() );
  EXPECT_EQ( caseitem1, stmt->caseitem(0) );
  EXPECT_EQ( caseitem2, stmt->caseitem(1) );
  EXPECT_THROW( stmt->caseitem(2),
		std::out_of_range );
  auto exp_caseitem_list = std::vector<const PtCaseItem*>{caseitem1, caseitem2};
  EXPECT_EQ( exp_caseitem_list, stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, CaseZ)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.new_IntConst(fr1, 0U);

  auto caseitem_list = parser.new_list<const PtCaseItem>();

  auto ci1_fr = make_file_region(1, 2, 3, 4);
  auto ci1_expr_list = parser.new_list<const PtExpr>();
  auto ci1_fr1 = make_file_region(1, 1, 1, 1);
  auto ci1_expr = parser.new_IntConst(fr1, 1U);
  ci1_expr_list->push_back(ci1_expr);
  auto ci1_fr2 = make_file_region(2, 2, 2, 2);
  auto ci1_body = parser.new_NullStmt(ci1_fr2);
  auto caseitem1 = parser.new_CaseItem(ci1_fr, ci1_expr_list, ci1_body);
  caseitem_list->push_back(caseitem1);

  auto ci2_fr = make_file_region(11, 2, 13, 4);
  auto ci2_expr_list = parser.new_list<const PtExpr>();
  auto ci2_fr1 = make_file_region(11, 1, 11, 1);
  auto ci2_expr = parser.new_IntConst(fr1, 2U);
  ci2_expr_list->push_back(ci2_expr);
  auto ci2_fr2 = make_file_region(12, 2, 12, 2);
  auto ci2_body = parser.new_NullStmt(ci2_fr2);
  auto caseitem2 = parser.new_CaseItem(ci2_fr, ci2_expr_list, ci2_body);
  caseitem_list->push_back(caseitem2);

  auto stmt = parser.new_CaseZ(fr, cond, caseitem_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::CaseZ, stmt->type() );
  EXPECT_STREQ( "casez statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 2, stmt->caseitem_num() );
  EXPECT_EQ( caseitem1, stmt->caseitem(0) );
  EXPECT_EQ( caseitem2, stmt->caseitem(1) );
  EXPECT_THROW( stmt->caseitem(2),
		std::out_of_range );
  auto exp_caseitem_list = std::vector<const PtCaseItem*>{caseitem1, caseitem2};
  EXPECT_EQ( exp_caseitem_list, stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Forever)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto stmt = parser.new_Forever(fr, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::Forever, stmt->type() );
  EXPECT_STREQ( "forever statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Repeat)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.new_IntConst(fr1, 0U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto stmt = parser.new_Repeat(fr, expr, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::Repeat, stmt->type() );
  EXPECT_STREQ( "repeat statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( expr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, While)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.new_IntConst(fr1, 0U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto stmt = parser.new_While(fr, cond, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::While, stmt->type() );
  EXPECT_STREQ( "while statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, For)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto init = parser.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto cond = parser.new_IntConst(fr1, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto next = parser.new_NullStmt(fr3);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto body = parser.new_NullStmt(fr4);
  auto stmt = parser.new_For(fr, init, cond, next, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::For, stmt->type() );
  EXPECT_STREQ( "for-loop statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( cond, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( init, stmt->init_stmt() );
  EXPECT_EQ( next, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, ParBlock)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = parser.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = parser.new_NullStmt(fr2);
  auto stmt_list = parser.new_list<const PtStmt>();
  stmt_list->push_back(stmt1);
  stmt_list->push_back(stmt2);
  auto stmt = parser.new_ParBlock(fr, stmt_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::ParBlock, stmt->type() );
  EXPECT_STREQ( "parallel block", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 2, stmt->stmt_num() );
  EXPECT_EQ( stmt1, stmt->stmt(0) );
  EXPECT_EQ( stmt2, stmt->stmt(1) );
  EXPECT_THROW( stmt->stmt(2),
		std::out_of_range );
  auto exp_list = std::vector<const PtStmt*>{stmt1, stmt2};
  EXPECT_EQ( exp_list, stmt->stmt_list() );
}

TEST_F(ParserTest, NamedParBlock)
{
  parser.init_block();
  auto net_fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(net_fr1, net_name);
  auto net_fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(net_fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = parser.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = parser.new_NullStmt(fr2);
  auto stmt_list = parser.new_list<const PtStmt>();
  stmt_list->push_back(stmt1);
  stmt_list->push_back(stmt2);
  parser.end_block();
  auto name = "block1";
  auto stmt = parser.new_NamedParBlock(fr, name, stmt_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( name, stmt->fullname() );
  EXPECT_EQ( PtStmtType::NamedParBlock, stmt->type() );
  EXPECT_STREQ( "parallel block", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 1, stmt->declhead_num() );
  EXPECT_EQ( net_decl, stmt->declhead(0) );
  EXPECT_THROW( stmt->declhead(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{net_decl},
	     stmt->declhead_list() );
  EXPECT_EQ( 2, stmt->stmt_num() );
  EXPECT_EQ( stmt1, stmt->stmt(0) );
  EXPECT_EQ( stmt2, stmt->stmt(1) );
  EXPECT_THROW( stmt->stmt(2),
		std::out_of_range );
  auto exp_list = std::vector<const PtStmt*>{stmt1, stmt2};
  EXPECT_EQ( exp_list, stmt->stmt_list() );
}

TEST_F(ParserTest, SeqBlock)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = parser.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = parser.new_NullStmt(fr2);
  auto stmt_list = parser.new_list<const PtStmt>();
  stmt_list->push_back(stmt1);
  stmt_list->push_back(stmt2);
  auto stmt = parser.new_SeqBlock(fr, stmt_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->fullname() );
  EXPECT_EQ( PtStmtType::SeqBlock, stmt->type() );
  EXPECT_STREQ( "sequential block", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 2, stmt->stmt_num() );
  EXPECT_EQ( stmt1, stmt->stmt(0) );
  EXPECT_EQ( stmt2, stmt->stmt(1) );
  EXPECT_THROW( stmt->stmt(2),
		std::out_of_range );
  auto exp_list = std::vector<const PtStmt*>{stmt1, stmt2};
  EXPECT_EQ( exp_list, stmt->stmt_list() );
}

TEST_F(ParserTest, NamedSeqBlock)
{
  parser.init_block();
  auto net_fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(net_fr1, net_name);
  auto net_fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(net_fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = parser.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = parser.new_NullStmt(fr2);
  auto stmt_list = parser.new_list<const PtStmt>();
  stmt_list->push_back(stmt1);
  stmt_list->push_back(stmt2);
  parser.end_block();
  auto name = "block1";
  auto stmt = parser.new_NamedSeqBlock(fr, name, stmt_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( name, stmt->fullname() );
  EXPECT_EQ( PtStmtType::NamedSeqBlock, stmt->type() );
  EXPECT_STREQ( "sequential block", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 1, stmt->declhead_num() );
  EXPECT_EQ( net_decl, stmt->declhead(0) );
  EXPECT_THROW( stmt->declhead(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{net_decl},
	     stmt->declhead_list() );
  EXPECT_EQ( 2, stmt->stmt_num() );
  EXPECT_EQ( stmt1, stmt->stmt(0) );
  EXPECT_EQ( stmt2, stmt->stmt(1) );
  EXPECT_THROW( stmt->stmt(2),
		std::out_of_range );
  auto exp_list = std::vector<const PtStmt*>{stmt1, stmt2};
  EXPECT_EQ( exp_list, stmt->stmt_list() );
}

END_NAMESPACE_YM_VERILOG
