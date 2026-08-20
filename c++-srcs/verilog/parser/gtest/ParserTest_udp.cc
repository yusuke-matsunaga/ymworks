
/// @file ParserTest_udp.cc
/// @brief ParserTest_udp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"
#include "ym/vl/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, UdpValue1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  for ( auto sym1: { '0', '1', 'x', 'b', '?', 'r', 'f', 'p', 'n', '-', '*' } ) {
    parser.init_udp_value_list();
    parser.new_UdpValue(fr, sym1);
    auto& udp_value_list = parser._udp_value_list();
    ASSERT_EQ( 1, udp_value_list.size() );
    auto uv = udp_value_list.front();

    ASSERT_TRUE( uv != nullptr );
    EXPECT_EQ( fr, uv->file_region() );
    EXPECT_EQ( VlUdpVal(sym1), uv->symbol() );
  }
}

TEST_F(ParserTest, UdpValue2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  for ( auto sym1: { '0', '1', 'x', 'b', '?' } ) {
    for ( auto sym2: { '0', '1', 'x', 'b', '?' } ) {
      parser.init_udp_value_list();
      parser.new_UdpValue(fr, sym1, sym2);
      auto& udp_value_list = parser._udp_value_list();
      ASSERT_EQ( 1, udp_value_list.size() );
      auto uv = udp_value_list.front();

      ASSERT_TRUE( uv != nullptr );
      EXPECT_EQ( fr, uv->file_region() );
      EXPECT_EQ( VlUdpVal(sym1, sym2), uv->symbol() );
    }
  }
}

TEST_F(ParserTest, UdpEntry1)
{
  parser.init_udp();
  auto fr = make_file_region(1, 2, 3, 4);
  parser.init_udp_value_list();
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto isym1 = '0';
  parser.new_UdpValue(fr1, isym1);
  auto val1 = parser._udp_value_list().front();
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto osym1 = '1';
  parser.new_UdpEntry(fr1, fr2, osym1);
  auto& udp_entry_list = parser._udp_entry_list();
  ASSERT_EQ( 1, udp_entry_list.size() );
  auto ue = udp_entry_list.front();

  ASSERT_TRUE( ue != nullptr );
  EXPECT_EQ( 1, ue->input_num() );
  EXPECT_EQ( std::vector<const AstUdpValue*>{val1},
	     ue->input_list() );

  EXPECT_EQ( VlUdpVal(isym1), val1->symbol() );
  EXPECT_EQ( nullptr, ue->current() );
  auto oval = ue->output();
  EXPECT_EQ( VlUdpVal(osym1), oval->symbol() );
}

TEST_F(ParserTest, UdpEntry2)
{
  parser.init_udp();
  auto fr = make_file_region(1, 2, 3, 4);
  parser.init_udp_value_list();
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto isym1 = '0';
  parser.new_UdpValue(fr1, isym1);
  auto val1 = parser._udp_value_list().front();
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto csym1 = 'x';
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto osym1 = '1';
  parser.new_UdpEntry(fr1, fr2, csym1, fr3, osym1);
  auto& udp_entry_list = parser._udp_entry_list();
  ASSERT_EQ( 1, udp_entry_list.size() );
  auto ue = udp_entry_list.front();

  ASSERT_TRUE( ue != nullptr );
  EXPECT_EQ( 1, ue->input_num() );
  EXPECT_EQ( std::vector<const AstUdpValue*>{val1},
	     ue->input_list() );

  EXPECT_EQ( VlUdpVal(isym1), val1->symbol() );
  auto cval = ue->current();
  EXPECT_EQ( VlUdpVal(csym1), cval->symbol() );
  auto oval = ue->output();
  EXPECT_EQ( VlUdpVal(osym1), oval->symbol() );
}

TEST_F(ParserTest, Udp1995)
{
  parser.init_udp();
  {
    auto fr = make_file_region(1, 2, 3, 4);
    parser.init_udp_value_list();
    auto fr1 = make_file_region(1, 1, 1, 9);
    auto isym1 = '0';
    parser.new_UdpValue(fr1, isym1);
    auto fr2 = make_file_region(1, 10, 1, 19);
    auto osym1 = '1';
    parser.new_UdpEntry(fr1, fr2, osym1);
  }
  auto fr = make_file_region(1, 1, 10, 10);
  auto name = "udp1";
  auto init_name = "var1";
  auto init_loc = make_file_region(2, 2, 2, 2);
  auto fr1 = make_file_region(3, 3, 3, 3);
  auto init_value = parser.new_IntConst(fr1, 1);
  parser.new_Udp1995(fr, name, init_name, init_loc, init_value, nullptr);
}

END_NAMESPACE_YM_VERILOG
