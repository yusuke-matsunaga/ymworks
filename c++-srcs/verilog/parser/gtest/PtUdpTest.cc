
/// @file PtUdpTest.cc
/// @brief PtUdpTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtTest.h"
#include "ym/vl/VlUdpVal.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtTest, UdpValue1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  for ( auto sym1: { '0', '1', 'x', 'b', '?', 'r', 'f', 'p', 'n', '-', '*' } ) {
    auto uv = parser.factory().new_UdpValue(fr, sym1);

    ASSERT_TRUE( uv != nullptr );
    EXPECT_EQ( fr, uv->file_region() );
    EXPECT_EQ( VlUdpVal(sym1), uv->symbol() );
  }
}

TEST_F(PtTest, UdpValue2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  for ( auto sym1: { '0', '1', 'x', 'b', '?' } ) {
    for ( auto sym2: { '0', '1', 'x', 'b', '?' } ) {
      auto uv = parser.factory().new_UdpValue(fr, sym1, sym2);

      ASSERT_TRUE( uv != nullptr );
      EXPECT_EQ( fr, uv->file_region() );
      EXPECT_EQ( VlUdpVal(sym1, sym2), uv->symbol() );
    }
  }
}

TEST_F(PtTest, UdpEntry1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto isym1 = '0';
  auto val1 = parser.factory().new_UdpValue(fr1, isym1);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto osym1 = '1';
  auto oval = parser.factory().new_UdpValue(fr2, osym1);
  auto ue = parser.factory().new_UdpEntry(fr1,
					  val1,
					  oval);

  ASSERT_TRUE( ue != nullptr );
  EXPECT_EQ( 1, ue->input_list().size() );
  EXPECT_EQ( std::vector<const AstUdpValue*>{val1},
	     ue->input_list().to_vector() );

  EXPECT_EQ( VlUdpVal(isym1), val1->symbol() );
  EXPECT_EQ( nullptr, ue->current() );
  EXPECT_EQ( oval, ue->output() );
}

TEST_F(PtTest, UdpEntry2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto isym1 = '0';
  auto val1 = parser.factory().new_UdpValue(fr1, isym1);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto csym1 = 'x';
  auto cval = parser.factory().new_UdpValue(fr2, csym1);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto osym1 = '1';
  auto oval = parser.factory().new_UdpValue(fr3, osym1);
  auto ue = parser.factory().new_UdpEntry(fr1,
					  val1,
					  cval, oval);

  ASSERT_TRUE( ue != nullptr );
  EXPECT_EQ( std::vector<const AstUdpValue*>{val1},
	     ue->input_list().to_vector() );

  EXPECT_EQ( VlUdpVal(isym1), val1->symbol() );
  EXPECT_EQ( cval, ue->current() );
  EXPECT_EQ( VlUdpVal(csym1), cval->symbol() );
  EXPECT_EQ( oval, ue->output() );
  EXPECT_EQ( VlUdpVal(osym1), oval->symbol() );
}

TEST_F(PtTest, Udp1995)
{
  PtUdpEntry* entry;
  {
    auto fr = make_file_region(1, 2, 3, 4);
    auto fr1 = make_file_region(1, 1, 1, 9);
    auto isym1 = '0';
    auto val1 = parser.factory().new_UdpValue(fr1, isym1);
    auto fr2 = make_file_region(1, 10, 1, 19);
    auto osym1 = '1';
    auto oval = parser.factory().new_UdpValue(fr2, osym1);
    entry = parser.factory().new_UdpEntry(fr1,
					  val1,
					  oval);
  }
  auto fr = make_file_region(1, 1, 10, 10);
  auto name = "udp1";
  auto init_name = "var1";
  auto init_loc = make_file_region(2, 2, 2, 2);
  auto fr1 = make_file_region(3, 3, 3, 3);
  auto init_value = parser.factory().new_IntConst(fr1, 1);
  parser.new_Udp1995(fr, name, init_name, init_loc, init_value,
		     nullptr,
		     nullptr,
		     nullptr,
		     entry);
}

END_NAMESPACE_YM_VERILOG
