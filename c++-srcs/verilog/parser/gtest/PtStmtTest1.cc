
/// @file PtStmt1.cc
/// @brief PtStmtTest の実装ファイルその2
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtStmtTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtStmtTest, Disable1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "name1";
  auto stmt = factory.new_Disable(fr, name);

  check_Disable(stmt, fr, name);
}

TEST_F(PtStmtTest, Disable2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "name1";
  auto hname = parser.new_HierName(head, name);
  auto stmt = factory.new_Disable(fr, hname);

  check_Disable(stmt, fr, name, {PtTest::NameBranchSpec{head, 0}});
}

TEST_F(PtStmtTest, Enable1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "name1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = factory.new_Enable(fr, name, expr);

  check_Enable(stmt, fr, name, {expr});
}

TEST_F(PtStmtTest, Enable2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "name1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = factory.new_Enable(fr, hname, expr);

  check_Enable(stmt, fr, name, {PtTest::NameBranchSpec{head, 0}}, {expr});
}

TEST_F(PtStmtTest, SysEnable)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "$name1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = factory.new_SysEnable(fr, name, expr);

  check_SysEnable(stmt, fr, name, {expr});
}

TEST_F(PtStmtTest, DcStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto expr = parser.factory().new_IntConst(fr1, 1U);
  auto delay = factory.new_DelayControl(fr1, expr);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto body = factory.new_NullStmt(fr2);
  auto stmt = factory.new_DcStmt(fr, delay, body);

  check_DelayControl(stmt, fr, delay, body);
}

TEST_F(PtStmtTest, EcStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto event_name = "event1";
  auto event_primary = factory.new_Primary(fr2, event_name);
  auto event = factory.new_EventControl(fr1, event_primary);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto body = factory.new_NullStmt(fr3);
  auto stmt = factory.new_EcStmt(fr, event, body);

  check_EventControl(stmt, fr, event, body);
}

TEST_F(PtStmtTest, Wait)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto cond = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto body = factory.new_NullStmt(fr3);
  auto stmt = factory.new_Wait(fr, cond, body);

  check_Wait(stmt, fr, cond, body);
}

TEST_F(PtStmtTest, Assign1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = factory.new_Assign(fr, lhs, rhs);

  check_Assign(stmt, fr, lhs, rhs);
}

TEST_F(PtStmtTest, Assign2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto event_name = "event1";
  auto event_primary = factory.new_Primary(fr5, event_name);
  auto event_control = factory.new_EventControl(fr4, event_primary);
  auto stmt = factory.new_Assign(fr, lhs, rhs, event_control);

  check_Assign(stmt, fr, lhs, rhs, event_control);
}

TEST_F(PtStmtTest, NbAssign1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = factory.new_NbAssign(fr, lhs, rhs);

  check_NbAssign(stmt, fr, lhs, rhs);
}

TEST_F(PtStmtTest, NbAssign2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto event_name = "event1";
  auto event_primary = factory.new_Primary(fr5, event_name);
  auto event_control = factory.new_EventControl(fr4, event_primary);
  auto stmt = factory.new_NbAssign(fr, lhs, rhs, event_control);

  check_NbAssign(stmt, fr, lhs, rhs, event_control);
}

TEST_F(PtStmtTest, PcAssign)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = factory.new_PcAssign(fr, lhs, rhs);

  check_PcAssign(stmt, fr, lhs, rhs);
}

TEST_F(PtStmtTest, Deassign)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = factory.new_Deassign(fr, lhs);

  check_Deassign(stmt, fr, lhs);
}

TEST_F(PtStmtTest, Force)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto rhs = parser.factory().new_IntConst(fr3, 1U);
  auto stmt = factory.new_Force(fr, lhs, rhs);

  check_Force(stmt, fr, lhs, rhs);
}

TEST_F(PtStmtTest, Release)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto lhs = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = factory.new_Release(fr, lhs);

  check_Release(stmt, fr, lhs);
}

TEST_F(PtStmtTest, EventStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto event = parser.factory().new_IntConst(fr1, 1U);
  auto stmt = factory.new_EventStmt(fr, event);

  check_Event(stmt, fr, event);
}

TEST_F(PtStmtTest, NullStmt)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto stmt = factory.new_NullStmt(fr);

  check_Null(stmt, fr);
}

END_NAMESPACE_YM_VERILOG
