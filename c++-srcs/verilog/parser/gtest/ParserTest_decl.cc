
/// @file ParserTest_decl.cc
/// @brief ParserTest_decl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest_Decl.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest_Decl, IOHead1)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_IOHead(fr, dir);

      check_IOHead(head, fr, dir,
		   VpiAuxType::None,
		   VpiNetType::None,
		   VpiVarType::None,
		   false, nullptr,
		   std::vector<const AstIOItem*>{});
    }
  }
}

TEST_F(ParserTest_Decl, IOHead2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_IOHead(fr, dir, sign, range);

      check_IOHead(head, fr, dir,
		   VpiAuxType::None,
		   VpiNetType::None,
		   VpiVarType::None,
		   sign, range,
		   std::vector<const AstIOItem*>{});
    }
  }
}

TEST_F(ParserTest_Decl, RegIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_RegIOHead(fr, dir);

      check_IOHead(head, fr, dir,
		   VpiAuxType::Reg,
		   VpiNetType::None,
		   VpiVarType::None,
		   false, nullptr,
		   std::vector<const AstIOItem*>{});
    }
  }
}

TEST_F(ParserTest_Decl, RegIOHead2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_RegIOHead(fr, dir, sign, range);

      check_IOHead(head, fr, dir,
		   VpiAuxType::Reg,
		   VpiNetType::None,
		   VpiVarType::None,
		   sign, range,
		   std::vector<const AstIOItem*>{});
    }
  }
}

TEST_F(ParserTest_Decl, NetIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetIOHead(fr, dir, net);

	check_IOHead(head, fr, dir,
		     VpiAuxType::Net,
		     net,
		     VpiVarType::None,
		     false, nullptr,
		     std::vector<const AstIOItem*>{});
      }
    }
  }
}

TEST_F(ParserTest_Decl, NetIOHead2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetIOHead(fr, dir, net, sign, range);

	check_IOHead(head, fr, dir,
		     VpiAuxType::Net,
		     net,
		     VpiVarType::None,
		     sign, range,
		     std::vector<const AstIOItem*>{});
      }
    }
  }
}

TEST_F(ParserTest_Decl, VarIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		      VpiVarType::Time, VpiVarType::Realtime } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = factory.new_VarIOHead(fr, dir, var);
      ASSERT_TRUE( head != nullptr );

      check_IOHead(head, fr, dir,
		   VpiAuxType::Var,
		   VpiNetType::None,
		   var,
		   false, nullptr,
		   std::vector<const AstIOItem*>{});
    }
  }
}

TEST_F(ParserTest_Decl, IOItem)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  auto item = factory.new_IOItem(fr, name);

  check_IOItem(item, fr, name, nullptr);
}

TEST_F(ParserTest_Decl, IOItem2)
{
  auto fr2 = make_file_region(1, 10, 3, 14);
  auto fr3 = make_file_region(2, 20, 2, 22);
  auto name = "port1";
  auto expr = factory.new_IntConst(fr3, 123U);
  auto item = factory.new_IOItem(fr2, name, expr);

  check_IOItem(item, FileRegion(fr2, fr3), name, expr);
}

TEST_F(ParserTest_Decl, ParamH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_ParamH(fr);

  check_DeclHead(head, fr,
		 AstDeclHead::Param,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, ParamH2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( auto sign: { true, false } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_ParamH(fr, sign, range);

    check_DeclHead(head, fr,
		   AstDeclHead::Param,
		   sign,
		   range,
		   VpiVarType::None,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, ParamH3)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_ParamH(fr, var);

    check_DeclHead(head, fr,
		   AstDeclHead::Param,
		   (var == VpiVarType::Time) ? false : true,
		   nullptr,
		   var,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, LocalParamH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_LocalParamH(fr, nullptr);

  check_DeclHead(head, fr,
		 AstDeclHead::LocalParam,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, LocalParamH2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( auto sign: { true, false } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_LocalParamH(fr, sign, range, nullptr);

    check_DeclHead(head, fr,
		   AstDeclHead::LocalParam,
		   sign,
		   range,
		   VpiVarType::None,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, LocalParamH3)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_LocalParamH(fr, var, nullptr);

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
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, SpecParamH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_SpecParamH(fr, nullptr);

  check_DeclHead(head, fr,
		 AstDeclHead::SpecParam,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, SpecParamH2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_SpecParamH(fr, range, nullptr);

  check_DeclHead(head, fr,
		 AstDeclHead::SpecParam,
		 false,
		 range,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, EventH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_EventH(fr, nullptr);

  check_DeclHead(head, fr,
		 AstDeclHead::Event,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, GenvarH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_GenvarH(fr, nullptr);

  check_DeclHead(head, fr,
		 AstDeclHead::Genvar,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, VarH)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_VarH(fr, var, nullptr);

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
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, RegH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = factory.new_RegH(fr, nullptr);

  check_DeclHead(head, fr,
		 AstDeclHead::Reg,
		 false,
		 nullptr,
		 VpiVarType::None,
		 VpiNetType::None,
		 VpiVsType::None,
		 nullptr,
		 nullptr,
		 std::vector<const AstDeclItem*>{});
}

TEST_F(ParserTest_Decl, RegH2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( bool sign: { true, false } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_RegH(fr, sign, range, nullptr);

    check_DeclHead(head, fr,
		   AstDeclHead::Reg,
		   sign,
		   range,
		   VpiVarType::None,
		   VpiNetType::None,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, NetH)
{
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, nullptr);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   nullptr,
		   nullptr,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, NetHS)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto str = factory.new_Strength(fr1,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, str, nullptr);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   str,
		   nullptr,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, NetHD)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto val = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto delay = factory.new_Delay(fr2, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, delay, nullptr);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   nullptr,
		   delay,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, NetHSD)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto str = factory.new_Strength(fr1,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto val = factory.new_IntConst(fr2, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto delay = factory.new_Delay(fr3, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = factory.new_NetH(fr, net, str, delay, nullptr);

    check_DeclHead(head, fr,
		   AstDeclHead::Net,
		   false,
		   nullptr,
		   VpiVarType::None,
		   net,
		   VpiVsType::None,
		   str,
		   delay,
		   std::vector<const AstDeclItem*>{});
  }
}

TEST_F(ParserTest_Decl, NetH2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, vs, sign, range, nullptr);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       nullptr,
		       nullptr,
		       std::vector<const AstDeclItem*>{});
      }
    }
  }
}

TEST_F(ParserTest_Decl, NetHS2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
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
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, str, vs, sign, range, nullptr);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       str,
		       nullptr,
		       std::vector<const AstDeclItem*>{});
      }
    }
  }
}

TEST_F(ParserTest_Decl, NetHD2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto val = factory.new_IntConst(fr4, 1U);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto delay = factory.new_Delay(fr5, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, vs, sign, range, delay, nullptr);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       nullptr,
		       delay,
		       std::vector<const AstDeclItem*>{});
      }
    }
  }
}

TEST_F(ParserTest_Decl, NetHSD2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto str = factory.new_Strength(fr4,
				  VpiStrength::SupplyDrive,
				  VpiStrength::StrongDrive);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto val = factory.new_IntConst(fr5, 1U);
  auto fr6 = make_file_region(6, 6, 6, 6);
  auto delay = factory.new_Delay(fr6, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = factory.new_NetH(fr, net, str, vs, sign, range, delay, nullptr);

	check_DeclHead(head, fr,
		       AstDeclHead::Net,
		       sign,
		       range,
		       VpiVarType::None,
		       net,
		       vs,
		       str,
		       delay,
		       std::vector<const AstDeclItem*>{});
      }
    }
  }
}

TEST_F(ParserTest_Decl, DeclItem)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  auto item = factory.new_DeclItem(fr, name);

  check_DeclItem(item, fr, name, {}, nullptr);
}

TEST_F(ParserTest_Decl, DeclItem2)
{
  auto fr2 = make_file_region(1, 10, 1, 14);
  auto fr3 = make_file_region(1, 20, 1, 28);
  auto name = "port1";
  auto expr = factory.new_IntConst(fr3, 123U);
  auto item = factory.new_DeclItem(fr2, name, expr);

  check_DeclItem(item, FileRegion(fr2, fr3), name, {}, expr);
}

TEST_F(ParserTest_Decl, DeclItem3)
{
  auto name = "port1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = factory.new_DeclItem(fr, name, range);

  check_DeclItem(item, fr, name, {range}, nullptr);
}

TEST_F(ParserTest_Decl, DeclItem4)
{
  auto name = "port1";
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 1, 19);
  auto range1 = new_range(fr1, 31, fr2, 0);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto fr4 = make_file_region(2, 30, 1, 39);
  auto range2 = new_range(fr2, 63, fr2, 0);
  range1->set_link(range2);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = factory.new_DeclItem(fr, name, range1);

  check_DeclItem(item, fr, name, {range1, range2}, nullptr);
}

TEST_F(ParserTest_Decl, Range)
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
