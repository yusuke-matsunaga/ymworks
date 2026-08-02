
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

END_NAMESPACE_YM_VERILOG
