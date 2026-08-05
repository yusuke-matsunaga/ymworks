
/// @file ParserTest_decl.cc
/// @brief ParserTest_decl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, empty)
{
}

TEST_F(ParserTest, IOHead)
{
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_IOHead(fr, dir, sign);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_IOHead(fr, dir, sign, range);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_RegIOHead(fr, dir, sign);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_RegIOHead(fr, dir, sign, range);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.new_NetIOHead(fr, dir, net, sign);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( auto dir: { VpiDir::Input, VpiDir::Output, VpiDir::Inout } ) {
    for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		      VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		      VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		      VpiNetType::Supply1, VpiNetType::Supply0 } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.new_NetIOHead(fr, dir, net, sign, range);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( fr, head->file_region() );
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
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_VarIOHead(fr, dir, var);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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

TEST_F(ParserTest, IOItem)
{
  parser.init_module();

  auto dir = VpiDir::Input;
  auto sign = false;
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto head = parser.new_IOHead(fr1, dir, sign);
  parser.add_ioport_head(head, nullptr);

  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  parser.new_IOItem(fr, name);
  parser.flush_io();

  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( name, item->name() );
  EXPECT_EQ( nullptr, item->init_value() );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOItem*>{item},
	     head->item_list() );
}

TEST_F(ParserTest, IOItem2)
{
  parser.init_module();

  auto fr1 = make_file_region(1, 1, 1, 8);
  auto fr2 = make_file_region(1, 10, 3, 14);
  auto fr3 = make_file_region(2, 20, 2, 22);
  auto dir = VpiDir::Input;
  auto sign = false;
  auto head = parser.new_IOHead(fr1, dir, sign);
  parser.add_ioport_head(head, nullptr);

  auto name = "port1";
  auto expr = parser.new_IntConst(fr3, 123U);
  parser.new_IOItem(fr2, name, expr);
  parser.flush_io();

  auto fr = FileRegion(fr2, fr3);
  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( name, item->name() );
  EXPECT_EQ( expr, item->init_value() );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
}

TEST_F(ParserTest, ParamH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.new_ParamH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( auto sign: { true, false } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_ParamH(fr, sign, range);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_ParamH(fr, var);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.new_LocalParamH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( auto sign: { true, false } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_LocalParamH(fr, sign, range);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_LocalParamH(fr, var);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.new_SpecParamH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);

  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.new_SpecParamH(fr, range);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( fr, head->file_region() );
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
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.new_EventH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( fr, head->file_region() );
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
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = parser.new_GenvarH(fr);
  ASSERT_TRUE( head != nullptr );
  EXPECT_EQ( fr, head->file_region() );
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
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_VarH(fr, var);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_RegH(fr, sign);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( bool sign: { true, false } ) {
    auto fr = make_file_region(1, 2, 3, 4);
    auto head = parser.new_RegH(fr, sign, range);
    ASSERT_TRUE( head != nullptr );
    EXPECT_EQ( fr, head->file_region() );
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
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_NetH(fr, net, sign);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto str = parser.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_NetH(fr, net, sign, str);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto val = parser.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto delay = parser.new_Delay(fr2, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_NetH(fr, net, sign, delay);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto str = parser.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto val = parser.new_IntConst(fr2, 1U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto delay = parser.new_Delay(fr3, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( bool sign: { true, false } ) {
      auto fr = make_file_region(1, 2, 3, 4);
      auto head = parser.new_NetH(fr, net, sign, str, delay);
      ASSERT_TRUE( head != nullptr );
      EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.new_NetH(fr, net, vs, sign, range);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto str = parser.new_Strength(fr4,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.new_NetH(fr, net, vs, sign, range, str);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto val = parser.new_IntConst(fr4, 1U);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto delay = parser.new_Delay(fr5, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.new_NetH(fr, net, vs, sign, range, delay);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( fr, head->file_region() );
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
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 31U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto str = parser.new_Strength(fr4,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto val = parser.new_IntConst(fr5, 1U);
  auto fr6 = make_file_region(6, 6, 6, 6);
  auto delay = parser.new_Delay(fr6, val);
  for ( auto net: { VpiNetType::Wire, VpiNetType::Wand, VpiNetType::Wor,
		    VpiNetType::Tri, VpiNetType::Tri0, VpiNetType::Tri1,
		    VpiNetType::TriReg, VpiNetType::TriAnd, VpiNetType::TriOr,
		    VpiNetType::Supply1, VpiNetType::Supply0 } ) {
    for ( auto vs: { VpiVsType::Vectored, VpiVsType::Scalared } ) {
      for ( bool sign: { true, false } ) {
	auto fr = make_file_region(1, 2, 3, 4);
	auto head = parser.new_NetH(fr, net, vs, sign, range, str, delay);
	ASSERT_TRUE( head != nullptr );
	EXPECT_EQ( fr, head->file_region() );
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

TEST_F(ParserTest, DeclItem)
{
  parser.init_module();

  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "port1";
  parser.new_DeclItem(fr, name);

  auto fr1 = make_file_region(1, 1, 1, 1);
  auto sign = false;
  auto type = VpiNetType::Wire;
  auto head = parser.new_NetH(fr1, type, sign);
  parser.add_decl_head(head, nullptr);

  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( name, item->name() );
  EXPECT_EQ( nullptr, item->init_value() );
  EXPECT_EQ( 0, item->range_num() );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{item},
	     head->item_list() );
  EXPECT_THROW( item->range(0),
		std::out_of_range );
}

TEST_F(ParserTest, DeclItem2)
{
  parser.init_module();

  auto fr1 = make_file_region(1, 2, 1, 4);
  auto fr2 = make_file_region(1, 10, 1, 14);
  auto fr3 = make_file_region(1, 20, 1, 28);
  auto name = "port1";
  auto expr = parser.new_IntConst(fr3, 123U);
  parser.new_DeclItem(fr2, name, expr);
  auto fr = FileRegion(fr2, fr3);

  auto sign = false;
  auto type = VpiNetType::Wire;
  auto head = parser.new_NetH(fr1, type, sign);
  parser.add_decl_head(head, nullptr);

  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( name, item->name() );
  EXPECT_EQ( expr, item->init_value() );
  EXPECT_EQ( 0, item->range_num() );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{item},
	     head->item_list() );
  EXPECT_THROW( item->range(0),
		std::out_of_range );
}

TEST_F(ParserTest, DeclItem3)
{
  parser.init_module();

  auto name = "port1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 16U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto range = parser.new_Range(fr3, left, right);
  auto range_list = parser.new_list<const PtRange>();
  range_list->push_back(range);
  auto fr = make_file_region(1, 2, 3, 4);
  parser.new_DeclItem(fr, name, range_list);

  auto sign = false;
  auto type = VpiNetType::Wire;
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto head = parser.new_NetH(fr5, type, sign);
  parser.add_decl_head(head, nullptr);

  EXPECT_EQ( 1, head->item_num() );
  auto item = head->item(0);
  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( name, item->name() );
  EXPECT_EQ( nullptr, item->init_value() );
  EXPECT_EQ( 1, item->range_num() );
  EXPECT_EQ( range, item->range(0) );
  EXPECT_THROW( head->item(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclItem*>{item},
	     head->item_list() );
}

TEST_F(ParserTest, Range)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto left = parser.new_IntConst(fr1, 16U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto right = parser.new_IntConst(fr2, 0U);
  auto fr = make_file_region(1, 2, 3, 4);
  auto range = parser.new_Range(fr, left, right);
  ASSERT_TRUE( range != nullptr );
  EXPECT_EQ( fr, range->file_region() );
  EXPECT_EQ( left, range->left() );
  EXPECT_EQ( right, range->right() );
}

END_NAMESPACE_YM_VERILOG
