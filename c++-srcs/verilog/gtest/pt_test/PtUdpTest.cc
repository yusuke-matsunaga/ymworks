
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
    auto uv = factory.new_UdpValue(fr, sym1);

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
      auto uv = factory.new_UdpValue(fr, sym1, sym2);

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
  auto val1 = factory.new_UdpValue(fr1, isym1);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto osym1 = '1';
  auto oval = factory.new_UdpValue(fr2, osym1);
  auto ue = factory.new_UdpEntry(fr1, val1, oval);

  ASSERT_TRUE( ue != nullptr );
  auto exp_input_list = std::vector<const PtUdpValue*>{val1};
  EXPECT_EQ( exp_input_list, to_vector(ue->input_top()) );

  EXPECT_EQ( VlUdpVal(isym1), val1->symbol() );
  EXPECT_EQ( nullptr, ue->current() );
  EXPECT_EQ( oval, ue->output() );
}

TEST_F(PtTest, UdpEntry2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr_val1 = make_file_region(1, 1, 1, 9);
  auto isym1 = '0';
  auto val1 = factory.new_UdpValue(fr_val1, isym1);
  auto fr_val2 = make_file_region(1, 10, 1, 19);
  auto isym2 = '1';
  auto val2 = factory.new_UdpValue(fr_val2, isym2);
  auto val_list = PtUdpValueList::new_obj();
  val_list.add(val1);
  val_list.add(val2);
  auto fr_cval = make_file_region(1, 20, 1, 29);
  auto csym1 = 'x';
  auto cval = factory.new_UdpValue(fr_cval, csym1);
  auto fr_oval = make_file_region(1, 30, 1, 39);
  auto osym1 = '1';
  auto oval = factory.new_UdpValue(fr_oval, osym1);
  auto ue = factory.new_UdpEntry(fr, val_list.top, cval, oval);

  ASSERT_TRUE( ue != nullptr );
  auto exp_val_list = std::vector<const PtUdpValue*>{val1, val2};
  EXPECT_EQ( exp_val_list, to_vector(ue->input_top()) );
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
    auto val1 = factory.new_UdpValue(fr1, isym1);
    auto fr2 = make_file_region(1, 10, 1, 19);
    auto osym1 = '1';
    auto oval = factory.new_UdpValue(fr2, osym1);
    entry = factory.new_UdpEntry(fr1, val1, oval);
  }
  auto fr = make_file_region(1, 1, 10, 10);
  auto name = "udp1";
  auto init_name = "var1";
  auto init_loc = make_file_region(2, 2, 2, 2);
  auto fr1 = make_file_region(3, 3, 3, 3);
  auto init_value = factory.new_IntConst(fr1, 1);
  parser.new_Udp1995(fr, name,
		     init_name, init_loc, init_value,
		     nullptr,
		     nullptr,
		     nullptr,
		     entry);
}

END_NAMESPACE_YM_VERILOG
