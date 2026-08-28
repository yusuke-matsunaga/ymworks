
/// @file PtItemTest2.cc
/// @brief PtItemTest の実装ファイルその2
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtItemTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtItemTest, GateH)
{
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto fr_inst = make_file_region(2, 2, 2, 9);
    auto fr_expr = make_file_region(2, 10, 2, 19);
    auto expr = factory.new_IntConst(fr_expr, 1);
    auto con = factory.new_OrderedCon(expr);
    auto inst = factory.new_Inst(fr_inst, con);
    auto fr = make_file_region(1, 1, 2, 2);
    auto item = factory.new_GateH(fr, type, inst);

    check_GateH(item, fr, type, {inst});
  }
}

TEST_F(PtItemTest, GateHS)
{
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto fr_inst = make_file_region(2, 2, 2, 9);
    auto fr_expr = make_file_region(2, 10, 2, 19);
    auto expr = factory.new_IntConst(fr_expr, 1);
    auto con = factory.new_OrderedCon(expr);
    auto inst = factory.new_Inst(fr_inst, con);
    auto fr_str = make_file_region(1, 2, 3, 4);
    auto str = factory.new_Strength(fr_str,
				   VpiStrength::SupplyDrive,
				   VpiStrength::StrongDrive);
    auto fr = make_file_region(1, 1, 2, 2);
    auto item = factory.new_GateH(fr, type, str, inst);

    check_GateH(item, fr, type, {inst}, str);
  }
}

TEST_F(PtItemTest, GateHD)
{
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto fr_val = make_file_region(1, 2, 3, 4);
    auto val = parser.factory().new_IntConst(fr_val, 1U);
    auto delay = factory.new_Delay(fr_val, val);
    auto fr_inst = make_file_region(2, 2, 2, 9);
    auto fr_expr = make_file_region(2, 10, 2, 19);
    auto expr = factory.new_IntConst(fr_expr, 1);
    auto con = factory.new_OrderedCon(expr);
    auto inst = factory.new_Inst(fr_inst, con);
    auto fr = make_file_region(1, 1, 2, 2);
    auto item = factory.new_GateH(fr, type, delay, inst);

    check_GateH(item, fr, type, {inst}, nullptr, delay);
  }
}

TEST_F(PtItemTest, GateHSD)
{
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto fr_str = make_file_region(1, 2, 3, 4);
    auto str = factory.new_Strength(fr_str,
				   VpiStrength::SupplyDrive,
				   VpiStrength::StrongDrive);
    auto fr_val = make_file_region(5, 6, 7, 8);
    auto val = parser.factory().new_IntConst(fr_val, 1U);
    auto delay = factory.new_Delay(fr_val, val);
    auto fr_inst = make_file_region(2, 2, 2, 9);
    auto fr_expr = make_file_region(2, 10, 2, 19);
    auto expr = factory.new_IntConst(fr_expr, 1);
    auto con = factory.new_OrderedCon(expr);
    auto inst = factory.new_Inst(fr_inst, con);
    auto fr = make_file_region(1, 1, 2, 2);
    auto item = factory.new_GateH(fr, type, str, delay, inst);

    check_GateH(item, fr, type, {inst}, str, delay);
  }
}

TEST_F(PtItemTest, MuH)
{
  auto fr_inst = make_file_region(2, 2, 2, 9);
  auto fr_expr = make_file_region(2, 10, 2, 19);
  auto expr = factory.new_IntConst(fr_expr, 1);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr_inst, con);
  auto fr = make_file_region(1, 1, 2, 2);
  auto def_name = "module1";
  auto item = factory.new_MuH(fr, def_name, inst);

  check_MuH(item, fr, def_name, {}, {inst});
}

TEST_F(PtItemTest, MuHS)
{
  auto fr_inst = make_file_region(2, 2, 2, 9);
  auto fr_expr = make_file_region(2, 10, 2, 19);
  auto expr = factory.new_IntConst(fr_expr, 1);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr_inst, con);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto def_name = "module1";
  auto str = factory.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr = make_file_region(1, 1, 2, 2);
  auto item = factory.new_MuH(fr, def_name, str, inst);

  check_MuH(item, fr, def_name, {}, {inst}, str);
}

TEST_F(PtItemTest, MuHD)
{
  auto fr_inst = make_file_region(2, 2, 2, 9);
  auto fr_expr = make_file_region(2, 10, 2, 19);
  auto expr = factory.new_IntConst(fr_expr, 1);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr_inst, con);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto def_name = "module1";
  auto val = parser.factory().new_IntConst(fr1, 1U);
  auto delay = factory.new_Delay(fr1, val);
  auto fr = make_file_region(1, 1, 2, 2);
  auto item = factory.new_MuH(fr, def_name, delay, inst);

  check_MuH(item, fr, def_name, {}, {inst}, nullptr, delay);
}

TEST_F(PtItemTest, MuHSD)
{
  auto fr_inst = make_file_region(2, 2, 2, 9);
  auto fr_expr = make_file_region(2, 10, 2, 19);
  auto expr = factory.new_IntConst(fr_expr, 1);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr_inst, con);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto def_name = "module1";
  auto str = factory.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto val = parser.factory().new_IntConst(fr2, 1U);
  auto delay = factory.new_Delay(fr2, val);
  auto fr = make_file_region(1, 1, 2, 2);
  auto item = factory.new_MuH(fr, def_name, str, delay, inst);

  check_MuH(item, fr, def_name, {}, {inst}, str, delay);
}

TEST_F(PtItemTest, MuHP)
{
  auto fr_inst = make_file_region(2, 2, 2, 9);
  auto fr_expr = make_file_region(2, 10, 2, 19);
  auto expr = factory.new_IntConst(fr_expr, 1);
  auto con = factory.new_OrderedCon(expr);
  auto inst = factory.new_Inst(fr_inst, con);
  auto fr = make_file_region(1, 1, 2, 2);
  auto def_name = "module1";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto pa_expr = parser.factory().new_IntConst(fr1, 123U);
  auto param_assign = factory.new_OrderedCon(pa_expr);
  auto item = factory.new_MuH(fr, def_name, param_assign, inst);

  check_MuH(item, fr, def_name, {param_assign}, {inst});
}

END_NAMESPACE_YM_VERILOG
