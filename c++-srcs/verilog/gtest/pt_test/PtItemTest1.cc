
/// @file PtItemTest1.cc
/// @brief PtItemTest の実装ファイルその1
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtItemTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtItemTest, DefParamH)
{
  auto fr_dp = make_file_region(2, 2, 2, 2);
  const char* name = "name1";
  auto fr_val = make_file_region(3, 3, 3, 3);
  auto val = factory.new_IntConst(fr_val, 1U);
  auto dp = factory.new_DefParam(fr_dp, name, val);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = factory.new_DefParamH(fr, dp);

  check_DefParamH(item, fr,
		  std::vector<const AstDefParam*>{dp});
}

TEST_F(PtItemTest, DefParam1)
{
  auto fr_val = make_file_region(1, 30, 1, 40);
  auto val = factory.new_IntConst(fr_val, 123U);
  auto name = "param1";
  auto fr = make_file_region(1, 14, 1, 20);
  auto defparam = factory.new_DefParam(fr, name, val);

  check_DefParam(defparam, FileRegion(fr, fr_val), name, {}, val);
}

TEST_F(PtItemTest, DefParam2)
{
  auto fr_val = make_file_region(1, 30, 1, 39);
  auto val = factory.new_IntConst(fr_val, 123U);

  auto head = "head";
  auto name = "param1";
  auto hname = parser.new_HierName(head, name);
  auto fr = make_file_region(1, 14, 1, 19);
  auto defparam = factory.new_DefParam(fr, hname, val);

  check_DefParam(defparam, FileRegion(fr, fr_val), name, {PtTest::NameBranchSpec{head, 0}}, val);
}

TEST_F(PtItemTest, ContAssignH)
{
  auto fr_left = make_file_region(2, 2, 2, 9);
  auto name = "name1";
  auto lhs = factory.new_Primary(fr_left, name);
  auto fr_right = make_file_region(2, 10, 2, 20);
  auto rhs = factory.new_IntConst(fr_right, 1U);
  auto fr_ca = make_file_region(2, 2, 2, 40);
  auto ca = factory.new_ContAssign(fr_ca, lhs, rhs);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.factory().new_ContAssignH(fr, ca);

  check_ContAssignH(item, fr,
		    nullptr,
		    nullptr,
		    std::vector<const AstContAssign*>{ca});
}

TEST_F(PtItemTest, ContAssignHS)
{
  auto fr_left = make_file_region(2, 2, 2, 9);
  auto name = "name1";
  auto lhs = factory.new_Primary(fr_left, name);
  auto fr_right = make_file_region(2, 10, 2, 20);
  auto rhs = factory.new_IntConst(fr_right, 1U);
  auto fr_ca = make_file_region(2, 2, 2, 40);
  auto ca = factory.new_ContAssign(fr_ca, lhs, rhs);
  auto fr_str = make_file_region(1, 10, 1, 20);
  auto str = parser.factory().new_Strength(fr_str,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  auto fr = make_file_region(1, 2, 1, 4);
  auto item = parser.factory().new_ContAssignH(fr, str, ca);

  check_ContAssignH(item, fr,
		    str,
		    nullptr,
		    std::vector<const AstContAssign*>{ca});
}

TEST_F(PtItemTest, ContAssignHD)
{
  auto fr_left = make_file_region(2, 2, 2, 9);
  auto name = "name1";
  auto lhs = factory.new_Primary(fr_left, name);
  auto fr_right = make_file_region(2, 10, 2, 20);
  auto rhs = factory.new_IntConst(fr_right, 1U);
  auto fr_ca = make_file_region(2, 2, 2, 40);
  auto ca = factory.new_ContAssign(fr_ca, lhs, rhs);
  auto fr_val = make_file_region(1, 1, 1, 4);
  auto val = parser.factory().new_IntConst(fr_val, 1U);
  auto fr_delay = make_file_region(1, 5, 1, 10);
  auto delay = parser.factory().new_Delay(fr_delay, val);
  auto fr = make_file_region(1, 10, 1, 20);
  auto item = parser.factory().new_ContAssignH(fr, delay, ca);

  check_ContAssignH(item, fr,
		    nullptr,
		    delay,
		    std::vector<const AstContAssign*>{ca});
}

TEST_F(PtItemTest, ContAssignHSD)
{
  auto fr_str = make_file_region(1, 10, 1, 19);
  auto str = parser.factory().new_Strength(fr_str,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  auto fr_val = make_file_region(1, 20, 1, 29);
  auto val = parser.factory().new_IntConst(fr_val, 1U);
  auto fr_delay = make_file_region(1, 20, 1, 29);
  auto delay = parser.factory().new_Delay(fr_delay, val);
  auto fr_left = make_file_region(2, 2, 2, 9);
  auto name = "name1";
  auto lhs = factory.new_Primary(fr_left, name);
  auto fr_right = make_file_region(2, 10, 2, 20);
  auto rhs = factory.new_IntConst(fr_right, 1U);
  auto fr_ca = make_file_region(2, 2, 2, 40);
  auto ca = factory.new_ContAssign(fr_ca, lhs, rhs);
  auto fr = make_file_region(1, 2, 1, 4);
  auto item = parser.factory().new_ContAssignH(fr, str, delay, ca);

  check_ContAssignH(item, fr,
		    str,
		    delay,
		    std::vector<const AstContAssign*>{ca});
}

TEST_F(PtItemTest, ContAssign)
{
  auto fr2 = make_file_region(1, 14, 1, 20);
  auto fr3 = make_file_region(1, 30, 1, 40);

  auto name = "net1";
  auto lhs = factory.new_Primary(fr2, name);
  auto rhs = factory.new_IntConst(fr3, 1234U);
  auto fr_ca = FileRegion(fr2, fr3);
  auto ca = factory.new_ContAssign(fr_ca, lhs, rhs);

  check_ContAssign(ca, fr_ca, lhs, rhs);
}

TEST_F(PtItemTest, Initial)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto body = factory.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  auto item = factory.new_Initial(fr, body);

  check_Initial(item, fr, body);
}

TEST_F(PtItemTest, Always)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto body = factory.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  auto item = factory.new_Always(fr, body);

  check_Always(item, fr, body);

}

TEST_F(PtItemTest, Task)
{
  for ( auto automatic: {false, true} ) {
    auto fr_ioitem = make_file_region(2, 10, 2, 19);
    const char* name_ioitem = "io1";
    auto ioitem = factory.new_IOItem(fr_ioitem, name_ioitem);
    auto fr_iohead = make_file_region(2, 1, 2, 9);
    auto iohead = factory.new_IOHead(fr_iohead, VpiDir::Input, ioitem);

    auto fr_declitem = make_file_region(3, 10, 3, 19);
    const char* name_declitem = "reg1";
    auto declitem = factory.new_DeclItem(fr_declitem, name_declitem);
    auto fr_declhead = make_file_region(3, 1, 3, 9);
    auto declhead = factory.new_RegH(fr_declhead, declitem);

    auto fr_stmt = make_file_region(4, 1, 4, 9);
    auto stmt = factory.new_NullStmt(fr_stmt);
    auto fr = make_file_region(1, 1, 10, 10);
    const char* name = "task1";
    auto item = factory.new_Task(fr, name, automatic, iohead, declhead, stmt);

    check_Task(item, fr, name, automatic, {iohead}, {declhead}, stmt);
  }
}

TEST_F(PtItemTest, Function)
{
  for ( auto automatic: {false, true} ) {
    auto fr_ioitem = make_file_region(2, 10, 2, 19);
    const char* name_ioitem = "io1";
    auto ioitem = factory.new_IOItem(fr_ioitem, name_ioitem);
    auto fr_iohead = make_file_region(2, 1, 2, 9);
    auto iohead = factory.new_IOHead(fr_iohead, VpiDir::Input, ioitem);

    auto fr_declitem = make_file_region(3, 10, 3, 19);
    const char* name_declitem = "reg1";
    auto declitem = factory.new_DeclItem(fr_declitem, name_declitem);
    auto fr_declhead = make_file_region(3, 1, 3, 9);
    auto declhead = factory.new_RegH(fr_declhead, declitem);

    auto fr_stmt = make_file_region(4, 1, 4, 9);
    auto stmt = factory.new_NullStmt(fr_stmt);
    auto fr = make_file_region(1, 1, 10, 10);
    const char* name = "func1";
    auto item = factory.new_Function(fr, name, automatic, iohead, declhead, stmt);

    check_Function(item, fr, name, false, nullptr, VpiVarType::None,
			automatic, {iohead}, {declhead}, stmt);
  }
}

TEST_F(PtItemTest, SizedFunc)
{
  for ( auto automatic: {false, true} ) {
    for ( auto sign: {false, true} ) {
      auto fr_left = make_file_region(2, 1, 2, 4);
      auto fr_right = make_file_region(2, 5, 2, 9);
      auto range = new_range(fr_left, 31, fr_right, 0);

      auto fr_ioitem = make_file_region(2, 10, 2, 19);
      const char* name_ioitem = "io1";
      auto ioitem = factory.new_IOItem(fr_ioitem, name_ioitem);
      auto fr_iohead = make_file_region(2, 1, 2, 9);
      auto iohead = factory.new_IOHead(fr_iohead, VpiDir::Input, ioitem);

      auto fr_declitem = make_file_region(3, 10, 3, 19);
      const char* name_declitem = "reg1";
      auto declitem = factory.new_DeclItem(fr_declitem, name_declitem);
      auto fr_declhead = make_file_region(3, 1, 3, 9);
      auto declhead = factory.new_RegH(fr_declhead, declitem);

      auto fr_stmt = make_file_region(4, 1, 4, 9);
      auto stmt = factory.new_NullStmt(fr_stmt);
      auto fr = make_file_region(1, 1, 10, 10);
      const char* name = "func1";
      auto item = factory.new_SizedFunc(fr, name, automatic, sign, range,
					iohead, declhead, stmt);

      check_Function(item, fr, name, sign, range, VpiVarType::None,
			  automatic, {iohead}, {declhead}, stmt);
    }
  }
}

TEST_F(PtItemTest, TypedFunc)
{
  for ( auto automatic: {false, true} ) {
    for ( auto sign: {false, true} ) {
      for ( auto var_type: { VpiVarType::Integer, VpiVarType::Real,
			     VpiVarType::Time, VpiVarType::Realtime } ) {

	auto fr_ioitem = make_file_region(2, 10, 2, 19);
	const char* name_ioitem = "io1";
	auto ioitem = factory.new_IOItem(fr_ioitem, name_ioitem);
	auto fr_iohead = make_file_region(2, 1, 2, 9);
	auto iohead = factory.new_IOHead(fr_iohead, VpiDir::Input, ioitem);

	auto fr_declitem = make_file_region(3, 10, 3, 19);
	const char* name_declitem = "reg1";
	auto declitem = factory.new_DeclItem(fr_declitem, name_declitem);
	auto fr_declhead = make_file_region(3, 1, 3, 9);
	auto declhead = factory.new_RegH(fr_declhead, declitem);

	auto fr_stmt = make_file_region(4, 1, 4, 9);
	auto stmt = factory.new_NullStmt(fr_stmt);
	auto fr = make_file_region(1, 1, 10, 10);
	const char* name = "func1";
	auto item = factory.new_TypedFunc(fr, name, automatic, sign, var_type,
					  iohead, declhead, stmt);

	check_Function(item, fr, name, sign, nullptr, var_type,
		       automatic, {iohead}, {declhead}, stmt);
      }
    }
  }
}

END_NAMESPACE_YM_VERILOG
