
/// @file PtStmtTest2.cc
/// @brief PtStmtTest の実装ファイルその2
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtStmtTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtStmtTest, If)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto stmt = factory.new_If(fr, cond, body);

  check_If(stmt, fr, cond, body);
}

TEST_F(PtStmtTest, IfElse)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto body2 = factory.new_NullStmt(fr3);
  auto stmt = factory.new_IfElse(fr, cond, body, body2);

  check_If(stmt, fr, cond, body, body2);
}

TEST_F(PtStmtTest, CaseItem)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto caseitem = factory.new_CaseItem(fr, expr, body);

  check_CaseItem(caseitem, fr, {expr}, body);
}

TEST_F(PtStmtTest, Case)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);

  auto caseitem_list = PtCaseItemList::new_obj();

  auto ci1_fr = make_file_region(1, 2, 3, 4);
  auto ci1_fr1 = make_file_region(1, 1, 1, 1);
  auto ci1_expr = parser.factory().new_IntConst(fr1, 1U);
  auto ci1_fr2 = make_file_region(2, 2, 2, 2);
  auto ci1_body = factory.new_NullStmt(ci1_fr2);
  auto caseitem1 = factory.new_CaseItem(ci1_fr, ci1_expr, ci1_body);
  caseitem_list.add(caseitem1);

  auto ci2_fr = make_file_region(11, 2, 13, 4);
  auto ci2_fr1 = make_file_region(11, 1, 11, 1);
  auto ci2_expr = parser.factory().new_IntConst(fr1, 2U);
  auto ci2_fr2 = make_file_region(12, 2, 12, 2);
  auto ci2_body = factory.new_NullStmt(ci2_fr2);
  auto caseitem2 = factory.new_CaseItem(ci2_fr, ci2_expr, ci2_body);
  caseitem_list.add(caseitem2);

  auto stmt = factory.new_Case(fr, cond, caseitem_list.top);

  check_Case(stmt, fr, cond, {caseitem1, caseitem2});
}

TEST_F(PtStmtTest, CaseX)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);

  auto caseitem_list = PtCaseItemList::new_obj();

  auto ci1_fr = make_file_region(1, 2, 3, 4);
  auto ci1_fr1 = make_file_region(1, 1, 1, 1);
  auto ci1_expr = parser.factory().new_IntConst(fr1, 1U);
  auto ci1_fr2 = make_file_region(2, 2, 2, 2);
  auto ci1_body = factory.new_NullStmt(ci1_fr2);
  auto caseitem1 = factory.new_CaseItem(ci1_fr, ci1_expr, ci1_body);
  caseitem_list.add(caseitem1);

  auto ci2_fr = make_file_region(11, 2, 13, 4);
  auto ci2_fr1 = make_file_region(11, 1, 11, 1);
  auto ci2_expr = parser.factory().new_IntConst(fr1, 2U);
  auto ci2_fr2 = make_file_region(12, 2, 12, 2);
  auto ci2_body = factory.new_NullStmt(ci2_fr2);
  auto caseitem2 = factory.new_CaseItem(ci2_fr, ci2_expr, ci2_body);
  caseitem_list.add(caseitem2);

  auto stmt = factory.new_CaseX(fr, cond, caseitem_list.top);

  check_CaseX(stmt, fr, cond, {caseitem1, caseitem2});
}

TEST_F(PtStmtTest, CaseZ)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);

  auto ci1_fr = make_file_region(1, 2, 3, 4);
  auto ci1_fr1 = make_file_region(1, 1, 1, 1);
  auto ci1_expr = parser.factory().new_IntConst(fr1, 1U);
  auto ci1_fr2 = make_file_region(2, 2, 2, 2);
  auto ci1_body = factory.new_NullStmt(ci1_fr2);
  auto caseitem1 = factory.new_CaseItem(ci1_fr, ci1_expr, ci1_body);

  auto caseitem_list = PtCaseItemList::new_obj();
  caseitem_list.add(caseitem1);

  auto ci2_fr = make_file_region(11, 2, 13, 4);
  auto ci2_fr1 = make_file_region(11, 1, 11, 1);
  auto ci2_expr = parser.factory().new_IntConst(fr1, 2U);
  auto ci2_fr2 = make_file_region(12, 2, 12, 2);
  auto ci2_body = factory.new_NullStmt(ci2_fr2);
  auto caseitem2 = factory.new_CaseItem(ci2_fr, ci2_expr, ci2_body);
  caseitem_list.add(caseitem2);

  auto stmt = factory.new_CaseZ(fr, cond, caseitem_list.top);

  check_CaseZ(stmt, fr, cond, {caseitem1, caseitem2});
}

TEST_F(PtStmtTest, Forever)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto stmt = factory.new_Forever(fr, body);

  check_Forever(stmt, fr, body);
}

TEST_F(PtStmtTest, Repeat)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto stmt = factory.new_Repeat(fr, expr, body);

  check_Repeat(stmt, fr, expr, body);
}

TEST_F(PtStmtTest, While)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto stmt = factory.new_While(fr, cond, body);

  check_While(stmt, fr, cond, body);
}

TEST_F(PtStmtTest, For)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto init = factory.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto cond = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto next = factory.new_NullStmt(fr3);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto body = factory.new_NullStmt(fr4);
  auto stmt = factory.new_For(fr, init, cond, next, body);

  check_For(stmt, fr, init, cond, next, body);
}

TEST_F(PtStmtTest, ParBlock)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = factory.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = factory.new_NullStmt(fr2);
  auto stmt_list = PtStmtList::new_obj(stmt1);
  stmt_list.add(stmt2);
  auto stmt = factory.new_ParBlock(fr, stmt_list.top);

  check_ParBlock(stmt, fr, {stmt1, stmt2});
}

TEST_F(PtStmtTest, NamedParBlock)
{
  auto net_fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  auto net1 = factory.new_DeclItem(net_fr1, net_name);
  auto net_fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = factory.new_NetH(net_fr2, VpiNetType::Wire, net1);

  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = factory.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = factory.new_NullStmt(fr2);
  auto stmt_list = PtStmtList::new_obj(stmt1);
  stmt_list.add(stmt2);
  auto name = "block1";
  auto stmt = factory.new_NamedParBlock(fr, name, net_decl, stmt_list.top);

  check_NamedParBlock(stmt, fr, name, {net_decl}, {stmt1, stmt2});
}

TEST_F(PtStmtTest, SeqBlock)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = factory.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = factory.new_NullStmt(fr2);
  auto stmt_list = PtStmtList::new_obj(stmt1);
  stmt_list.add(stmt2);
  auto stmt = factory.new_SeqBlock(fr, stmt_list.top);

  check_SeqBlock(stmt, fr, {stmt1, stmt2});
}

TEST_F(PtStmtTest, NamedSeqBlock)
{
  auto net_fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  auto net1 = factory.new_DeclItem(net_fr1, net_name);
  auto net_fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = factory.new_NetH(net_fr2, VpiNetType::Wire, net1);

  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto stmt1 = factory.new_NullStmt(fr1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto stmt2 = factory.new_NullStmt(fr2);
  auto stmt_list = PtStmtList::new_obj(stmt1);
  stmt_list.add(stmt2);
  auto name = "block1";
  auto stmt = factory.new_NamedSeqBlock(fr, name, net_decl, stmt_list.top);

  check_NamedSeqBlock(stmt, fr, name, {net_decl}, {stmt1, stmt2});
}

END_NAMESPACE_YM_VERILOG
