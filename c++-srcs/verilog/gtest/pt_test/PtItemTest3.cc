
/// @file PtItemTest3.cc
/// @brief PtItemTest の実装ファイルその3
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtItemTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtItemTest, Inst)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr = parser.factory().new_IntConst(fr1, 123U);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr0, con);

  check_Inst(inst, fr0, nullptr, nullptr, {con});
}

TEST_F(PtItemTest, Inst1)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto inst = factory.new_Inst(fr0, expr1);

  check_Inst(inst, fr0, nullptr, nullptr, {expr1});
}

TEST_F(PtItemTest, Inst2)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto inst = factory.new_Inst(fr0, expr1, expr2);

  check_Inst(inst, fr0, nullptr, nullptr, {expr1, expr2});
}

TEST_F(PtItemTest, Inst3)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.factory().new_IntConst(fr3, 456U);
  auto inst = factory.new_Inst(fr0, expr1, expr2, expr3);

  check_Inst(inst, fr0, nullptr, nullptr, {expr1, expr2, expr3});
}

TEST_F(PtItemTest, Inst4)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 30, 1, 39);
  auto expr3 = parser.factory().new_IntConst(fr3, 456U);
  auto fr4 = make_file_region(1, 40, 1, 49);
  auto expr4 = parser.factory().new_IntConst(fr3, 456U);
  auto inst = factory.new_Inst(fr0, expr1, expr2, expr3, expr4);

  check_Inst(inst, fr0, nullptr, nullptr, {expr1, expr2, expr3, expr4});
}

TEST_F(PtItemTest, InstN)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr = parser.factory().new_IntConst(fr1, 123U);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr0, name, con);

  check_Inst(inst,fr0, name, nullptr, {con});
}

TEST_F(PtItemTest, InstN1)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto inst = factory.new_Inst(fr0, name, expr1);

  check_Inst(inst, fr0, name, nullptr, {expr1});
}

TEST_F(PtItemTest, InstN2)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto inst = factory.new_Inst(fr0, name, expr1, expr2);

  check_Inst(inst, fr0, name, nullptr, {expr1, expr2});
}

TEST_F(PtItemTest, InstN3)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto name = "inst_name";
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.factory().new_IntConst(fr3, 456U);
  auto inst = factory.new_Inst(fr0, name, expr1, expr2, expr3);

  check_Inst(inst, fr0, name, nullptr, {expr1, expr2, expr3});
}

TEST_F(PtItemTest, InstN4)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 30, 1, 39);
  auto expr3 = parser.factory().new_IntConst(fr3, 456U);
  auto fr4 = make_file_region(1, 40, 1, 49);
  auto expr4 = parser.factory().new_IntConst(fr3, 456U);
  auto inst = factory.new_Inst(fr0, name, expr1, expr2, expr3, expr4);

  check_Inst(inst, fr0, name, nullptr, {expr1, expr2, expr3, expr4});
}

TEST_F(PtItemTest, InstV)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto frl = make_file_region(1, 10, 1, 19);
  auto frr = make_file_region(1, 20, 1, 29);
  auto range = new_range(frl, 0, frr, 15);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 30, 1, 39);
  auto expr = parser.factory().new_IntConst(fr1, 123U);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr0, name, range, con);

  check_Inst(inst, fr0, name, range, {con});
}

TEST_F(PtItemTest, InstV1)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto frr = make_file_region(1, 20, 1, 29);
  auto range = new_range(frl, 0, frr, 15);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto inst = factory.new_Inst(fr0, name, range, expr1);

  check_Inst(inst, fr0, name, range, {expr1});
}

TEST_F(PtItemTest, InstV2)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto frr = make_file_region(1, 20, 1, 29);
  auto range = new_range(frl, 0, frr, 15);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto inst = factory.new_Inst(fr0, name, range, expr1, expr2);

  check_Inst(inst, fr0, name, range, {expr1, expr2});
}

TEST_F(PtItemTest, InstV3)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto frr = make_file_region(1, 20, 1, 29);
  auto range = new_range(frl, 0, frr, 15);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.factory().new_IntConst(fr3, 456U);
  auto inst = factory.new_Inst(fr0, name, range, expr1, expr2, expr3);

  check_Inst(inst, fr0, name, range, {expr1, expr2, expr3});
}

TEST_F(PtItemTest, InstV4)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto frr = make_file_region(1, 20, 1, 29);
  auto range = new_range(frl, 0, frr, 15);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.factory().new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.factory().new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 30, 1, 39);
  auto expr3 = parser.factory().new_IntConst(fr3, 456U);
  auto fr4 = make_file_region(1, 40, 1, 49);
  auto expr4 = parser.factory().new_IntConst(fr3, 456U);
  auto inst = factory.new_Inst(fr0, name, range, expr1, expr2, expr3, expr4);

  check_Inst(inst, fr0, name, range, {expr1, expr2, expr3, expr4});
}

END_NAMESPACE_YM_VERILOG
