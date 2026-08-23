
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
      parser.init_module();
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.factory().new_IOHead(fr, dir, sign);
      parser.add_module_iohead(head);

      check_IOHead(head, fr, dir,
		   VpiAuxType::None,
		   VpiNetType::None,
		   VpiVarType::None,
		   sign, nullptr,
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
      parser.init_module();
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.factory().new_IOHead(fr, dir, sign, range);
      parser.add_module_iohead(head);

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
      parser.init_module();
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.factory().new_RegIOHead(fr, dir, sign);
      parser.add_module_iohead(head);

      check_IOHead(head, fr, dir,
		   VpiAuxType::Reg,
		   VpiNetType::None,
		   VpiVarType::None,
		   sign, nullptr,
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
      parser.init_module();
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.factory().new_RegIOHead(fr, dir, sign, range);
      parser.add_module_iohead(head);

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
	parser.init_module();
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.factory().new_NetIOHead(fr, dir, net, sign);
	parser.add_module_iohead(head);

	check_IOHead(head, fr, dir,
		     VpiAuxType::Net,
		     net,
		     VpiVarType::None,
		     sign, nullptr,
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
	parser.init_module();
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.factory().new_NetIOHead(fr, dir, net, sign, range);
	parser.add_module_iohead(head);

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
      parser.init_module();
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.factory().new_VarIOHead(fr, dir, var);
      parser.add_module_iohead(head);
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
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  auto item = parser.factory().new_IOItem(fr, name);

  check_IOItem(item, fr, name, nullptr);
}

TEST_F(ParserTest_Decl, IOItem2)
{
  parser.init_module();

  auto fr2 = make_file_region(1, 10, 3, 14);
  auto fr3 = make_file_region(2, 20, 2, 22);
  auto name = "port1";
  auto expr = parser.factory().new_IntConst(fr3, 123U);
  auto item = parser.factory().new_IOItem(fr2, name, expr);

  check_IOItem(item, FileRegion(fr2, fr3), name, expr);
}

TEST_F(ParserTest_Decl, ParamH)
{
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_ParamH(fr);
  parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_ParamH(fr, sign, range);
    parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_ParamH(fr, var);
    parser.add_decl_head(head);

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
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_LocalParamH(fr);
  parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_LocalParamH(fr, sign, range);
    parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_LocalParamH(fr, var);
    parser.add_decl_head(head);

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
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_SpecParamH(fr);
  parser.add_decl_head(head);

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
  parser.init_module();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_SpecParamH(fr, range);
  parser.add_decl_head(head);

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
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_EventH(fr);
  parser.add_decl_head(head);

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
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_GenvarH(fr);
  parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_VarH(fr, var);
    parser.add_decl_head(head);

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
  parser.init_module();
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.factory().new_RegH(fr);
  parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_RegH(fr, sign, range);
    parser.add_decl_head(head);

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
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_NetH(fr, net);
    parser.add_decl_head(head);

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
  auto str = parser.factory().new_Strength(fr1,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_NetH(fr, net, str);
    parser.add_decl_head(head);

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
  auto val = parser.factory().new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto delay = parser.factory().new_Delay(fr2, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_NetH(fr, net, delay);
    parser.add_decl_head(head);

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
  auto str = parser.factory().new_Strength(fr1,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto val = parser.factory().new_IntConst(fr2, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto delay = parser.factory().new_Delay(fr3, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    parser.init_module();
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.factory().new_NetH(fr, net, str, delay);
    parser.add_decl_head(head);

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
	parser.init_module();
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.factory().new_NetH(fr, net, vs, sign, range);
	parser.add_decl_head(head);

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
  auto str = parser.factory().new_Strength(fr4,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	parser.init_module();
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.factory().new_NetH(fr, net, str, vs, sign, range);
	parser.add_decl_head(head);

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
  auto val = parser.factory().new_IntConst(fr4, 1U);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto delay = parser.factory().new_Delay(fr5, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	parser.init_module();
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.factory().new_NetH(fr, net, vs, sign, range, delay);
	parser.add_decl_head(head);

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
  auto str = parser.factory().new_Strength(fr4,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto val = parser.factory().new_IntConst(fr5, 1U);
  auto fr6 = make_file_region(6, 6, 6, 6);
  auto delay = parser.factory().new_Delay(fr6, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	parser.init_module();
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.factory().new_NetH(fr, net, str, vs, sign, range, delay);
	parser.add_decl_head(head);

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
  parser.init_module();

  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  auto item = parser.factory().new_DeclItem(fr, name);

  check_DeclItem(item, fr, name, {}, nullptr);
}

TEST_F(ParserTest_Decl, DeclItem2)
{
  parser.init_module();

  auto fr2 = make_file_region(1, 10, 1, 14);
  auto fr3 = make_file_region(1, 20, 1, 28);
  auto name = "port1";
  auto expr = parser.factory().new_IntConst(fr3, 123U);
  auto item = parser.factory().new_DeclItem(fr2, name, expr);

  check_DeclItem(item, FileRegion(fr2, fr3), name, {}, expr);
}

TEST_F(ParserTest_Decl, DeclItem3)
{
  parser.init_module();

  auto name = "port1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto range = new_range(fr1, 31, fr2, 0);
  parser.init_range_list();
  parser.add_range(range);
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.factory().new_DeclItem(fr, name, parser.range_list());

  check_DeclItem(item, fr, name, {range}, nullptr);
}

TEST_F(ParserTest_Decl, Range)
{
  auto left_fr = make_file_region(1, 1, 1, 1);
  SizeType left_val = 31U;
  auto right_fr = make_file_region(2, 2, 2, 2);
  SizeType right_val = 0U;
  auto left = parser.factory().new_IntConst(left_fr, left_val);
  auto right = parser.factory().new_IntConst(right_fr, right_val);
  auto range = parser.factory().new_Range(FileRegion(left_fr, right_fr), left, right);

  check_Base(range, FileRegion(left_fr, right_fr));
  EXPECT_EQ( left, range->left() );
  EXPECT_EQ( right, range->right() );
}

END_NAMESPACE_YM_VERILOG
