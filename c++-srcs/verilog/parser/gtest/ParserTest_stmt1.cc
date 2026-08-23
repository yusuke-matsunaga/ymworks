
/// @file ParserTest_stmt.cc
/// @brief ParserTest_stmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "parser/PtStmt.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtItem.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, Disable1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "name1";
  auto stmt = parser.new_Disable(fr, name);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( name, stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Disable, stmt->type() );
  EXPECT_STREQ( "disable statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Disable2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "name1";
  auto hname = parser.new_HierName(head, name);
  auto stmt = parser.new_Disable(fr, hname);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 1, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(1),
		std::out_of_range );
  auto nb = stmt->namebranch(0);
  EXPECT_STREQ( head, nb->name() );
  EXPECT_EQ( std::vector<const AstNameBranch*>{nb},
	     stmt->namebranch_list() );
  auto exp_name = std::string(head) + "." + std::string(name);
  EXPECT_EQ( exp_name, stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Disable, stmt->type() );
  EXPECT_STREQ( "disable statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Enable1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "name1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto expr_list = parser.new_expr_list();
  expr_list->push_back(astmgr.alloc(), expr);
  auto stmt = parser.new_Enable(fr, name, expr_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( name, stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Enable, stmt->type() );
  EXPECT_STREQ( "task enable statement", stmt->stmt_name() );
  EXPECT_EQ( 1, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(1),
		std::out_of_range );
  EXPECT_EQ( expr, stmt->arg(0) );
  EXPECT_EQ( std::vector<const AstExpr*>{expr},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Enable2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "name1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto expr_list = parser.new_expr_list();
  expr_list->push_back(astmgr.alloc(), expr);
  auto stmt = parser.new_Enable(fr, hname, expr_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 1, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(1),
		std::out_of_range );
  auto nb = stmt->namebranch(0);
  EXPECT_STREQ( head, nb->name() );
  EXPECT_EQ( std::vector<const AstNameBranch*>{nb},
	     stmt->namebranch_list() );
  auto exp_name = std::string(head) + "." + std::string(name);
  EXPECT_EQ( exp_name, stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Enable, stmt->type() );
  EXPECT_STREQ( "task enable statement", stmt->stmt_name() );
  EXPECT_EQ( 1, stmt->arg_num() );
  EXPECT_EQ( expr, stmt->arg(0) );
  EXPECT_THROW( stmt->arg(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{expr},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, SysEnable)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "$name1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto expr_list = parser.new_expr_list();
  expr_list->push_back(astmgr.alloc(), expr);
  auto stmt = parser.new_SysEnable(fr, name, expr_list);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( name, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( name, stmt->decompile_name() );
  EXPECT_EQ( AstStmt::SysEnable, stmt->type() );
  EXPECT_STREQ( "system task enable statement", stmt->stmt_name() );
  EXPECT_EQ( 1, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(1),
		std::out_of_range );
  EXPECT_EQ( expr, stmt->arg(0) );
  EXPECT_EQ( std::vector<const AstExpr*>{expr},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, DcStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto delay = parser.new_DelayControl(fr1, expr);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = parser.new_NullStmt(fr2);
  auto stmt = parser.new_DcStmt(fr, delay, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::DelayControl, stmt->type() );
  EXPECT_STREQ( "delay control statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( delay, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, EcStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto event_name = "event1";
  auto event = parser.new_EventControl(fr1, event_name, fr2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto body = parser.new_NullStmt(fr3);
  auto stmt = parser.new_EcStmt(fr, event, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::EventControl, stmt->type() );
  EXPECT_STREQ( "event control statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( event, stmt->control() );
  EXPECT_EQ( body, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Wait)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto body = parser.new_NullStmt(fr3);
  auto stmt = parser.new_Wait(fr, cond, body);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Wait, stmt->type() );
  EXPECT_STREQ( "wait statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Assign1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = parser.new_Assign(fr, lhs, rhs);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Assign, stmt->type() );
  EXPECT_STREQ( "assignment", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( rhs, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Assign2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto event_name = "event1";
  auto event = parser.new_EventControl(fr4, event_name, fr5);
  auto stmt = parser.new_Assign(fr, lhs, rhs, event);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Assign, stmt->type() );
  EXPECT_STREQ( "assignment", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( event, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( rhs, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, NbAssign1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = parser.new_NbAssign(fr, lhs, rhs);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::NbAssign, stmt->type() );
  EXPECT_STREQ( "nonblocking assignment", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( rhs, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, NbAssign2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto event_name = "event1";
  auto event = parser.new_EventControl(fr4, event_name, fr5);
  auto stmt = parser.new_NbAssign(fr, lhs, rhs, event);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::NbAssign, stmt->type() );
  EXPECT_STREQ( "nonblocking assignment", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( event, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( rhs, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, PcAssign)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = parser.new_PcAssign(fr, lhs, rhs);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::PcAssign, stmt->type() );
  EXPECT_STREQ( "procedural continuous assignment", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( rhs, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Deassign)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = parser.new_Deassign(fr, lhs);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Deassign, stmt->type() );
  EXPECT_STREQ( "deassign statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Force)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = parser.new_Force(fr, lhs, rhs);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Force, stmt->type() );
  EXPECT_STREQ( "force statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( rhs, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, Release)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = parser.new_Release(fr, lhs);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Release, stmt->type() );
  EXPECT_STREQ( "release statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( lhs, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( nullptr, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, EventStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto event = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = parser.new_EventStmt(fr, event);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Event, stmt->type() );
  EXPECT_STREQ( "event statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     stmt->arg_list() );
  EXPECT_EQ( nullptr, stmt->control() );
  EXPECT_EQ( nullptr, stmt->body() );
  EXPECT_EQ( nullptr, stmt->expr() );
  EXPECT_EQ( nullptr, stmt->lhs() );
  EXPECT_EQ( nullptr, stmt->rhs() );
  EXPECT_EQ( event, stmt->primary() );
  EXPECT_EQ( nullptr, stmt->else_body() );
  EXPECT_EQ( 0, stmt->caseitem_num() );
  EXPECT_THROW( stmt->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

TEST_F(ParserTest, NullStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto stmt = parser.new_NullStmt(fr);

  ASSERT_TRUE( stmt != nullptr );
  EXPECT_STREQ( nullptr, stmt->name() );
  EXPECT_EQ( 0, stmt->namebranch_num() );
  EXPECT_THROW( stmt->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstNameBranch*>{},
	     stmt->namebranch_list() );
  EXPECT_EQ( "", stmt->decompile_name() );
  EXPECT_EQ( AstStmt::Null, stmt->type() );
  EXPECT_STREQ( "null statement", stmt->stmt_name() );
  EXPECT_EQ( 0, stmt->arg_num() );
  EXPECT_THROW( stmt->arg(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
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
  EXPECT_EQ( std::vector<const AstCaseItem*>{},
	     stmt->caseitem_list() );
  EXPECT_EQ( nullptr, stmt->init_stmt() );
  EXPECT_EQ( nullptr, stmt->next_stmt() );
  EXPECT_EQ( 0, stmt->declhead_num() );
  EXPECT_THROW( stmt->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     stmt->declhead_list() );
  EXPECT_EQ( 0, stmt->stmt_num() );
  EXPECT_THROW( stmt->stmt(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstStmt*>{},
	     stmt->stmt_list() );
}

END_NAMESPACE_YM_VERILOG
