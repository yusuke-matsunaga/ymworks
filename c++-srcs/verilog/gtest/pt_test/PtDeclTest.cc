
/// @file PtDeclTest.cc
/// @brief PtDeclTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtDeclTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtDeclTest, IOHead1)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr1 = make_file_region(1, 1, 1, 1);
      const char* name1 = "name1";
      auto item = factory.new_IOItem(fr1, name1);
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_IOHead(fr, dir, item);

      check_IOHead(head, fr, dir,
		   VpiAuxType::None,
		   VpiNetType::None,
		   VpiVarType::None,
		   false, nullptr,
		   std::vector<const PtIOItem*>{item});
    }
  }
}

TEST_F(PtDeclTest, IOHead2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr1 = make_file_region(1, 1, 1, 1);
      const char* name1 = "name1";
      auto item = factory.new_IOItem(fr1, name1);
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_IOHead(fr, dir, sign, range, item);

      check_IOHead(head, fr, dir,
		   VpiAuxType::None,
		   VpiNetType::None,
		   VpiVarType::None,
		   sign, range,
		   std::vector<const PtIOItem*>{item});
    }
  }
}

TEST_F(PtDeclTest, RegIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr1 = make_file_region(1, 1, 1, 1);
      const char* name1 = "name1";
      auto item = factory.new_IOItem(fr1, name1);
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_RegIOHead(fr, dir, item);

      check_IOHead(head, fr, dir,
		   VpiAuxType::Reg,
		   VpiNetType::None,
		   VpiVarType::None,
		   false, nullptr,
		   std::vector<const PtIOItem*>{item});
    }
  }
}

TEST_F(PtDeclTest, RegIOHead2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr1 = make_file_region(1, 1, 1, 1);
      const char* name1 = "name1";
      auto item = factory.new_IOItem(fr1, name1);
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_RegIOHead(fr, dir, sign, range, item);

      check_IOHead(head, fr, dir,
		   VpiAuxType::Reg,
		   VpiNetType::None,
		   VpiVarType::None,
		   sign, range,
		   std::vector<const PtIOItem*>{item});
    }
  }
}

TEST_F(PtDeclTest, NetIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto fr1 = make_file_region(1, 1, 1, 1);
	const char* name1 = "name1";
	auto item = factory.new_IOItem(fr1, name1);
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetIOHead(fr, dir, net, item);

	check_IOHead(head, fr, dir,
		     VpiAuxType::Net,
		     net,
		     VpiVarType::None,
		     false, nullptr,
		     std::vector<const PtIOItem*>{item});
      }
    }
  }
}

TEST_F(PtDeclTest, NetIOHead2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto fr1 = make_file_region(1, 1, 1, 1);
	const char* name1 = "name1";
	auto item = factory.new_IOItem(fr1, name1);
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetIOHead(fr, dir, net, sign, range, item);

	check_IOHead(head, fr, dir,
		     VpiAuxType::Net,
		     net,
		     VpiVarType::None,
		     sign, range,
		     std::vector<const PtIOItem*>{item});
      }
    }
  }
}

TEST_F(PtDeclTest, VarIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		      VpiVarType::Time, VpiVarType::Realtime } ) {
      auto fr1 = make_file_region(1, 1, 1, 1);
      const char* name1 = "name1";
      auto item = factory.new_IOItem(fr1, name1);
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_VarIOHead(fr, dir, var, item);

      check_IOHead(head, fr, dir,
		   VpiAuxType::Var,
		   VpiNetType::None,
		   var,
		   false, nullptr,
		   std::vector<const PtIOItem*>{item});
    }
  }
}

TEST_F(PtDeclTest, IOItem)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  auto item = factory.new_IOItem(fr, name);

  check_IOItem(item, fr, name, nullptr);
}

TEST_F(PtDeclTest, IOItem2)
{
  auto fr2 = make_file_region(1, 10, 3, 14);
  auto fr3 = make_file_region(2, 20, 2, 22);
  auto name = "port1";
  auto expr = factory.new_IntConst(fr3, 123U);
  auto item = factory.new_IOItem(fr2, name, expr);

  check_IOItem(item, FileRegion(fr2, fr3), name, expr);
}

TEST_F(PtDeclTest, ParamH)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_ParamH(fr, item);

  check_DeclHead(head, fr,
		 AstDeclHead::Param,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, ParamH2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( auto sign: { true, false } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_ParamH(fr, sign, range, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Param,
		   sign,
		   range,
		   VpiVarType::None,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, ParamH3)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_ParamH(fr, var, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Param,
		   (var == VpiVarType::Time) ? false : true,
		   nullptr,
		   var,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, LocalParamH)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_LocalParamH(fr, item);

  check_DeclHead(head, fr,
		 AstDeclHead::LocalParam,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, LocalParamH2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( auto sign: { true, false } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_LocalParamH(fr, sign, range, item);

    check_DeclHead(head, fr,
		   AstDeclHead::LocalParam,
		   sign,
		   range,
		   VpiVarType::None,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, LocalParamH3)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_LocalParamH(fr, var, item);

    auto sign = (var == VpiVarType::Time) ? false : true;
    check_DeclHead(head, fr,
		   AstDeclHead::LocalParam,
		   sign,
		   nullptr,
		   var,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, SpecParamH)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_SpecParamH(fr, item);

  check_DeclHead(head, fr,
		 AstDeclHead::SpecParam,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, SpecParamH2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_SpecParamH(fr, range, item);

  check_DeclHead(head, fr,
		 AstDeclHead::SpecParam,
		 false,
		 range,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, EventH)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_EventH(fr, item);

  check_DeclHead(head, fr,
		 AstDeclHead::Event,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, GenvarH)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_GenvarH(fr, item);

  check_DeclHead(head, fr,
		 AstDeclHead::Genvar,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, VarH)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_VarH(fr, var, item);

    auto sign = (var == VpiVarType::Time) ? false : true;
    check_DeclHead(head, fr,
		   AstDeclHead::Var,
		   sign,
		   nullptr,
		   var,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, RegH)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_DeclItem(fr1, name1);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_RegH(fr, item);

  check_DeclHead(head, fr,
		 AstDeclHead::Reg,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const PtDeclItem*>{item});
}

TEST_F(PtDeclTest, RegH2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( bool sign: { true, false } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_RegH(fr, sign, range, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Reg,
		   sign,
		   range,
		   VpiVarType::None,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, NetH)
{
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, NetHS)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto str = factory.new_Strength(fr1,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, str, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   str,
		   nullptr,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, NetHD)
{
  auto fr_val = make_file_region(1, 1, 1, 1);
  auto val = factory.new_IntConst(fr_val, 1U);
  auto fr_delay = make_file_region(2, 2, 2, 2);
  auto delay = factory.new_Delay(fr_delay, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, delay, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   nullptr,
		   delay,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, NetHSD)
{
  auto fr_str = make_file_region(1, 1, 1, 1);
  auto str = factory.new_Strength(fr_str,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  auto fr_val = make_file_region(2, 2, 2, 2);
  auto val = factory.new_IntConst(fr_val, 1U);
  auto fr_delay = make_file_region(3, 3, 3, 3);
  auto delay = factory.new_Delay(fr_val, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr1 = make_file_region(1, 1, 1, 1);
    const char* name1 = "name1";
    auto item = factory.new_DeclItem(fr1, name1);
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, str, delay, item);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   str,
		   delay,
		   std::vector<const PtDeclItem*>{item});
  }
}

TEST_F(PtDeclTest, NetH2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr1 = make_file_region(1, 1, 1, 1);
	const char* name1 = "name1";
	auto item = factory.new_DeclItem(fr1, name1);
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, vs, sign, range, item);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       nullptr,
		       nullptr,
		       std::vector<const PtDeclItem*>{item});
      }
    }
  }
}

TEST_F(PtDeclTest, NetHS2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto str = factory.new_Strength(fr4,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr1 = make_file_region(1, 1, 1, 1);
	const char* name1 = "name1";
	auto item = factory.new_DeclItem(fr1, name1);
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, str, vs, sign, range, item);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       str,
		       nullptr,
		       std::vector<const PtDeclItem*>{item});
      }
    }
  }
}

TEST_F(PtDeclTest, NetHD2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  auto fr_val = make_file_region(4, 4, 4, 4);
  auto val = factory.new_IntConst(fr_val, 1U);
  auto fr_delay = make_file_region(5, 5, 5, 5);
  auto delay = factory.new_Delay(fr_delay, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr1 = make_file_region(1, 1, 1, 1);
	const char* name1 = "name1";
	auto item = factory.new_DeclItem(fr1, name1);
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, vs, sign, range, delay, item);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       nullptr,
		       delay,
		       std::vector<const PtDeclItem*>{item});
      }
    }
  }
}

TEST_F(PtDeclTest, NetHSD2)
{
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  auto fr_str = make_file_region(4, 4, 4, 4);
  auto str = factory.new_Strength(fr_str,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  auto fr_val = make_file_region(5, 5, 5, 5);
  auto val = factory.new_IntConst(fr_val, 1U);
  auto fr_delay = make_file_region(6, 6, 6, 6);
  auto delay = factory.new_Delay(fr_delay, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr1 = make_file_region(1, 1, 1, 1);
	const char* name1 = "name1";
	auto item = factory.new_DeclItem(fr1, name1);
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, str, vs, sign, range, delay, item);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       str,
		       delay,
		       std::vector<const PtDeclItem*>{item});
      }
    }
  }
}

TEST_F(PtDeclTest, DeclItem)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  auto item = factory.new_DeclItem(fr, name);

  check_DeclItem(item, fr, name, {}, nullptr);
}

TEST_F(PtDeclTest, DeclItem2)
{
  auto fr2 = make_file_region(1, 10, 1, 14);
  auto fr3 = make_file_region(1, 20, 1, 28);
  auto name = "port1";
  auto expr = factory.new_IntConst(fr3, 123U);
  auto item = factory.new_DeclItem(fr2, name, expr);

  check_DeclItem(item, FileRegion(fr2, fr3), name, {}, expr);
}

TEST_F(PtDeclTest, DeclItem3)
{
  auto name = "port1";
  auto fr_left = make_file_region(1, 1, 1, 1);
  auto fr_right = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr_left, 31, fr_right, 0);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = factory.new_DeclItem(fr, name, range);

  check_DeclItem(item, fr, name, {range}, nullptr);
}

TEST_F(PtDeclTest, DeclItem4)
{
  auto name = "port1";
  auto fr_left1 = make_file_region(1, 1, 1, 9);
  auto fr_right1 = make_file_region(2, 10, 1, 19);
  auto range1 = new_range(fr_left1, 31, fr_right1, 0);
  auto fr_left2 = make_file_region(1, 20, 1, 29);
  auto fr_right2 = make_file_region(2, 30, 1, 39);
  auto range2 = new_range(fr_left2, 63, fr_right2, 0);
  range1->set_link(range2);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = factory.new_DeclItem(fr, name, range1);

  check_DeclItem(item, fr, name, {range1, range2}, nullptr);
}

TEST_F(PtDeclTest, Range)
{
  auto left_fr = make_file_region(1, 1, 1, 1);
  SizeType left_val = 31U;
  auto right_fr = make_file_region(2, 2, 2, 2);
  SizeType right_val = 0U;
  auto left = factory.new_IntConst(left_fr, left_val);
  auto right = factory.new_IntConst(right_fr, right_val);
  auto range = factory.new_Range(FileRegion(left_fr, right_fr), left, right);

  check_Base(range, FileRegion(left_fr, right_fr));
  EXPECT_EQ( left, range->left() );
  EXPECT_EQ( right, range->right() );
}

END_NAMESPACE_YM_VERILOG
