
/// @file ParserTest.cc
/// @brief ParserTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "parser/Parser.h"


BEGIN_NAMESPACE_YM_VERILOG

class ParserTest :
public ::testing::Test
{
public:

  // コンストラクタ
  ParserTest(
  ) : parser(ptmgr),
      file_info("filename1"),
      fr(file_info, 1, 2, 3, 4)
  {
  }

  PtMgr ptmgr;
  Parser parser;
  FileInfo file_info;
  FileRegion fr;
};


TEST_F(ParserTest, empty)
{
}

TEST_F(ParserTest, IOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_IOHead(fr, dir, sign);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( dir, head->direction() );
      EXPECT_EQ( VpiAuxType::None, head->aux_type() );
      EXPECT_EQ( VpiNetType::None, head->net_type() );
      EXPECT_EQ( VpiVarType::None, head->var_type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtIOItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, IOHead2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_IOHead(fr, dir, sign, range);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( dir, head->direction() );
      EXPECT_EQ( VpiAuxType::None, head->aux_type() );
      EXPECT_EQ( VpiNetType::None, head->net_type() );
      EXPECT_EQ( VpiVarType::None, head->var_type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( range, head->range() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtIOItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, RegIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_RegIOHead(fr, dir, sign);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( dir, head->direction() );
      EXPECT_EQ( VpiAuxType::Reg, head->aux_type() );
      EXPECT_EQ( VpiNetType::None, head->net_type() );
      EXPECT_EQ( VpiVarType::None, head->var_type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtIOItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, RegIOHead2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_RegIOHead(fr, dir, sign, range);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( dir, head->direction() );
      EXPECT_EQ( VpiAuxType::Reg, head->aux_type() );
      EXPECT_EQ( VpiNetType::None, head->net_type() );
      EXPECT_EQ( VpiVarType::None, head->var_type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( range, head->range() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtIOItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, NetIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto head = parser.new_NetIOHead(fr, dir, net, sign);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( dir, head->direction() );
	EXPECT_EQ( VpiAuxType::Net, head->aux_type() );
	EXPECT_EQ( net, head->net_type() );
	EXPECT_EQ( VpiVarType::None, head->var_type() );
	EXPECT_EQ( sign, head->is_signed() );
	EXPECT_EQ( nullptr, head->range() );
	EXPECT_EQ( 0, head->item_num() );
	EXPECT_THROW( head->item(0),
		      std::out_of_range );
	EXPECT_EQ( std::vector<const PtIOItem*>{},
		   head->item_list() );
      }
    }
  }
}

TEST_F(ParserTest, NetIOHead2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto head = parser.new_NetIOHead(fr, dir, net, sign, range);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( dir, head->direction() );
	EXPECT_EQ( VpiAuxType::Net, head->aux_type() );
	EXPECT_EQ( net, head->net_type() );
	EXPECT_EQ( VpiVarType::None, head->var_type() );
	EXPECT_EQ( sign, head->is_signed() );
	EXPECT_EQ( range, head->range() );
	EXPECT_EQ( 0, head->item_num() );
	EXPECT_THROW( head->item(0),
		      std::out_of_range );
	EXPECT_EQ( std::vector<const PtIOItem*>{},
		   head->item_list() );
      }
    }
  }
}

TEST_F(ParserTest, VarIOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		      VpiVarType::Time, VpiVarType::Realtime } ) {
      auto head = parser.new_VarIOHead(fr, dir, var);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( dir, head->direction() );
      EXPECT_EQ( VpiAuxType::Var, head->aux_type() );
      EXPECT_EQ( VpiNetType::None, head->net_type() );
      EXPECT_EQ( var, head->var_type() );
      EXPECT_EQ( false, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtIOItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, ioitem)
{
  parser.init_module();

  auto dir = VpiDir::Input;
  auto sign = false;
  auto head = parser.new_IOHead(fr, dir, sign);
  parser.add_ioport_head(head, nullptr);
  auto name = "port1";
  parser.new_IOItem(fr, name);
  parser.flush_io();

  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  EXPECT_EQ( name, item->name() );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOItem*>{item},
	     head->item_list() );
}

TEST_F(ParserTest, ioitem2)
{
  parser.init_module();

  auto dir = VpiDir::Input;
  auto sign = false;
  auto head = parser.new_IOHead(fr, dir, sign);
  parser.add_ioport_head(head, nullptr);
  auto name = "port1";
  auto expr = parser.new_IntConst(fr, 123U);
  parser.new_IOItem(fr, name, expr);
  parser.flush_io();

  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  EXPECT_EQ( name, item->name() );
  EXPECT_EQ( expr, item->init_value() );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
}

TEST_F(ParserTest, ParamH)
{
  auto head = parser.new_ParamH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( PtDeclType::Param, head->type() );
  EXPECT_EQ( false, head->is_signed() );
  EXPECT_EQ( nullptr, head->range() );
  EXPECT_EQ( VpiVarType::None, head->data_type() );
  EXPECT_EQ( VpiNetType::None, head->net_type() );
  EXPECT_EQ( VpiVsType::None, head->vs_type() );
  EXPECT_EQ( nullptr, head->strength() );
  EXPECT_EQ( nullptr, head->delay() );
  EXPECT_EQ( 0, head->item_num() );
  EXPECT_THROW( head->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{},
	     head->item_list() );
}

TEST_F(ParserTest, ParamH2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( auto sign: { true, false } ) {
    auto head = parser.new_ParamH(fr, sign, range);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::Param, head->type() );
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( range, head->range() );
    EXPECT_EQ( VpiVarType::None, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, ParamH3)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto head = parser.new_ParamH(fr, var);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::Param, head->type() );
    auto sign = (var == VpiVarType::Time) ? false : true;
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( nullptr, head->range() );
    EXPECT_EQ( var, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, LocalParamH)
{
  auto head = parser.new_LocalParamH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( PtDeclType::LocalParam, head->type() );
  EXPECT_EQ( false, head->is_signed() );
  EXPECT_EQ( nullptr, head->range() );
  EXPECT_EQ( VpiVarType::None, head->data_type() );
  EXPECT_EQ( VpiNetType::None, head->net_type() );
  EXPECT_EQ( VpiVsType::None, head->vs_type() );
  EXPECT_EQ( nullptr, head->strength() );
  EXPECT_EQ( nullptr, head->delay() );
  EXPECT_EQ( 0, head->item_num() );
  EXPECT_THROW( head->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{},
	     head->item_list() );
}

TEST_F(ParserTest, LocalParamH2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( auto sign: { true, false } ) {
    auto head = parser.new_LocalParamH(fr, sign, range);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::LocalParam, head->type() );
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( range, head->range() );
    EXPECT_EQ( VpiVarType::None, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, LocalParamH3)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto head = parser.new_LocalParamH(fr, var);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::LocalParam, head->type() );
    auto sign = (var == VpiVarType::Time) ? false : true;
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( nullptr, head->range() );
    EXPECT_EQ( var, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, SpecParamH)
{
  auto head = parser.new_SpecParamH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( PtDeclType::SpecParam, head->type() );
  EXPECT_EQ( false, head->is_signed() );
  EXPECT_EQ( nullptr, head->range() );
  EXPECT_EQ( VpiVarType::None, head->data_type() );
  EXPECT_EQ( VpiNetType::None, head->net_type() );
  EXPECT_EQ( VpiVsType::None, head->vs_type() );
  EXPECT_EQ( nullptr, head->strength() );
  EXPECT_EQ( nullptr, head->delay() );
  EXPECT_EQ( 0, head->item_num() );
  EXPECT_THROW( head->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{},
	     head->item_list() );
}

TEST_F(ParserTest, SpecParamH2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  auto head = parser.new_SpecParamH(fr, range);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( PtDeclType::SpecParam, head->type() );
  EXPECT_EQ( false, head->is_signed() );
  EXPECT_EQ( range, head->range() );
  EXPECT_EQ( VpiVarType::None, head->data_type() );
  EXPECT_EQ( VpiNetType::None, head->net_type() );
  EXPECT_EQ( VpiVsType::None, head->vs_type() );
  EXPECT_EQ( nullptr, head->strength() );
  EXPECT_EQ( nullptr, head->delay() );
  EXPECT_EQ( 0, head->item_num() );
  EXPECT_THROW( head->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{},
	     head->item_list() );
}

TEST_F(ParserTest, EventH)
{
  auto head = parser.new_EventH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( PtDeclType::Event, head->type() );
  EXPECT_EQ( false, head->is_signed() );
  EXPECT_EQ( nullptr, head->range() );
  EXPECT_EQ( VpiVarType::None, head->data_type() );
  EXPECT_EQ( VpiNetType::None, head->net_type() );
  EXPECT_EQ( VpiVsType::None, head->vs_type() );
  EXPECT_EQ( nullptr, head->strength() );
  EXPECT_EQ( nullptr, head->delay() );
  EXPECT_EQ( 0, head->item_num() );
  EXPECT_THROW( head->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{},
	     head->item_list() );
}

TEST_F(ParserTest, GenvarH)
{
  auto head = parser.new_GenvarH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( PtDeclType::Genvar, head->type() );
  EXPECT_EQ( false, head->is_signed() );
  EXPECT_EQ( nullptr, head->range() );
  EXPECT_EQ( VpiVarType::None, head->data_type() );
  EXPECT_EQ( VpiNetType::None, head->net_type() );
  EXPECT_EQ( VpiVsType::None, head->vs_type() );
  EXPECT_EQ( nullptr, head->strength() );
  EXPECT_EQ( nullptr, head->delay() );
  EXPECT_EQ( 0, head->item_num() );
  EXPECT_THROW( head->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{},
	     head->item_list() );
}

TEST_F(ParserTest, VarH)
{
  for ( auto var: { VpiVarType::Integer, VpiVarType::Real,
		    VpiVarType::Time, VpiVarType::Realtime } ) {
    auto head = parser.new_VarH(fr, var);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::Var, head->type() );
    auto sign = (var == VpiVarType::Time) ? false : true;
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( nullptr, head->range() );
    EXPECT_EQ( var, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, RegH)
{
  for ( bool sign: { true, false } ) {
    auto head = parser.new_RegH(fr, sign);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::Reg, head->type() );
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( nullptr, head->range() );
    EXPECT_EQ( VpiVarType::None, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, RegH2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( bool sign: { true, false } ) {
    auto head = parser.new_RegH(fr, sign, range);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( PtDeclType::Reg, head->type() );
    EXPECT_EQ( sign, head->is_signed() );
    EXPECT_EQ( range, head->range() );
    EXPECT_EQ( VpiVarType::None, head->data_type() );
    EXPECT_EQ( VpiNetType::None, head->net_type() );
    EXPECT_EQ( VpiVsType::None, head->vs_type() );
    EXPECT_EQ( nullptr, head->strength() );
    EXPECT_EQ( nullptr, head->delay() );
    EXPECT_EQ( 0, head->item_num() );
    EXPECT_THROW( head->item(0),
		  std::out_of_range );
    EXPECT_EQ( std::vector<const PtDeclItem*>{},
	       head->item_list() );
  }
}

TEST_F(ParserTest, NetH)
{
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_NetH(fr, net, sign);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( PtDeclType::Net, head->type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( VpiVarType::None, head->data_type() );
      EXPECT_EQ( net, head->net_type() );
      EXPECT_EQ( VpiVsType::None, head->vs_type() );
      EXPECT_EQ( nullptr, head->strength() );
      EXPECT_EQ( nullptr, head->delay() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtDeclItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, NetHS)
{
  auto str = parser.new_Strength(fr,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_NetH(fr, net, sign, str);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( PtDeclType::Net, head->type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( VpiVarType::None, head->data_type() );
      EXPECT_EQ( net, head->net_type() );
      EXPECT_EQ( VpiVsType::None, head->vs_type() );
      EXPECT_EQ( str, head->strength() );
      EXPECT_EQ( nullptr, head->delay() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtDeclItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, NetHD)
{
  auto val = parser.new_IntConst(fr, 1U);
  auto delay = parser.new_Delay(fr, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_NetH(fr, net, sign, delay);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( PtDeclType::Net, head->type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( VpiVarType::None, head->data_type() );
      EXPECT_EQ( net, head->net_type() );
      EXPECT_EQ( VpiVsType::None, head->vs_type() );
      EXPECT_EQ( nullptr, head->strength() );
      EXPECT_EQ( delay, head->delay() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtDeclItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, NetHSD)
{
  auto str = parser.new_Strength(fr,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto val = parser.new_IntConst(fr, 1U);
  auto delay = parser.new_Delay(fr, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto head = parser.new_NetH(fr, net, sign, str, delay);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( PtDeclType::Net, head->type() );
      EXPECT_EQ( sign, head->is_signed() );
      EXPECT_EQ( nullptr, head->range() );
      EXPECT_EQ( VpiVarType::None, head->data_type() );
      EXPECT_EQ( net, head->net_type() );
      EXPECT_EQ( VpiVsType::None, head->vs_type() );
      EXPECT_EQ( str, head->strength() );
      EXPECT_EQ( delay, head->delay() );
      EXPECT_EQ( 0, head->item_num() );
      EXPECT_THROW( head->item(0),
		    std::out_of_range );
      EXPECT_EQ( std::vector<const PtDeclItem*>{},
		 head->item_list() );
    }
  }
}

TEST_F(ParserTest, NetH2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto head = parser.new_NetH(fr, net, vs, sign, range);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( PtDeclType::Net, head->type() );
	EXPECT_EQ( sign, head->is_signed() );
	EXPECT_EQ( range, head->range() );
	EXPECT_EQ( VpiVarType::None, head->data_type() );
	EXPECT_EQ( net, head->net_type() );
	EXPECT_EQ( vs, head->vs_type() );
	EXPECT_EQ( nullptr, head->strength() );
	EXPECT_EQ( nullptr, head->delay() );
	EXPECT_EQ( 0, head->item_num() );
	EXPECT_THROW( head->item(0),
		      std::out_of_range );
	EXPECT_EQ( std::vector<const PtDeclItem*>{},
		   head->item_list() );
      }
    }
  }
}

TEST_F(ParserTest, NetHS2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  auto str = parser.new_Strength(fr,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto head = parser.new_NetH(fr, net, vs, sign, range, str);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( PtDeclType::Net, head->type() );
	EXPECT_EQ( sign, head->is_signed() );
	EXPECT_EQ( range, head->range() );
	EXPECT_EQ( VpiVarType::None, head->data_type() );
	EXPECT_EQ( net, head->net_type() );
	EXPECT_EQ( vs, head->vs_type() );
	EXPECT_EQ( str, head->strength() );
	EXPECT_EQ( nullptr, head->delay() );
	EXPECT_EQ( 0, head->item_num() );
	EXPECT_THROW( head->item(0),
		      std::out_of_range );
	EXPECT_EQ( std::vector<const PtDeclItem*>{},
		   head->item_list() );
      }
    }
  }
}

TEST_F(ParserTest, NetHD2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  auto val = parser.new_IntConst(fr, 1U);
  auto delay = parser.new_Delay(fr, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto head = parser.new_NetH(fr, net, vs, sign, range, delay);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( PtDeclType::Net, head->type() );
	EXPECT_EQ( sign, head->is_signed() );
	EXPECT_EQ( range, head->range() );
	EXPECT_EQ( VpiVarType::None, head->data_type() );
	EXPECT_EQ( net, head->net_type() );
	EXPECT_EQ( vs, head->vs_type() );
	EXPECT_EQ( nullptr, head->strength() );
	EXPECT_EQ( delay, head->delay() );
	EXPECT_EQ( 0, head->item_num() );
	EXPECT_THROW( head->item(0),
		      std::out_of_range );
	EXPECT_EQ( std::vector<const PtDeclItem*>{},
		   head->item_list() );
      }
    }
  }
}

TEST_F(ParserTest, NetHSD2)
{
  auto left = parser.new_IntConst(fr, 31U);
  auto right = parser.new_IntConst(fr, 0U);
  auto range = parser.new_Range(fr, left, right);
  auto str = parser.new_Strength(fr,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto val = parser.new_IntConst(fr, 1U);
  auto delay = parser.new_Delay(fr, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto head = parser.new_NetH(fr, net, vs, sign, range, str, delay);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( PtDeclType::Net, head->type() );
	EXPECT_EQ( sign, head->is_signed() );
	EXPECT_EQ( range, head->range() );
	EXPECT_EQ( VpiVarType::None, head->data_type() );
	EXPECT_EQ( net, head->net_type() );
	EXPECT_EQ( vs, head->vs_type() );
	EXPECT_EQ( str, head->strength() );
	EXPECT_EQ( delay, head->delay() );
	EXPECT_EQ( 0, head->item_num() );
	EXPECT_THROW( head->item(0),
		      std::out_of_range );
	EXPECT_EQ( std::vector<const PtDeclItem*>{},
		   head->item_list() );
      }
    }
  }
}

END_NAMESPACE_YM_VERILOG
