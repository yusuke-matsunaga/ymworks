
/// @file CiSeqInfo_test.cc
/// @brief CiSeqInfo_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "CiSeqInfo.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(cell_CiSeqInfo_test, case0)
{
  CiSeqInfo info{false, false, ClibSeqType::none};
  auto index = info.encode_val();
  EXPECT_EQ( 0, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case1)
{
  CiSeqInfo info{true, false, ClibSeqType::none};
  auto index = info.encode_val();
  EXPECT_EQ( 1, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case2)
{
  CiSeqInfo info{false, true, ClibSeqType::none};
  auto index = info.encode_val();
  EXPECT_EQ( 2, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case3)
{
  CiSeqInfo info{true, true, ClibSeqType::none};
  auto index = info.encode_val();
  EXPECT_EQ( 3, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case4)
{
  CiSeqInfo info{false, false, ClibSeqType::clear};
  auto index = info.encode_val();
  EXPECT_EQ( 4, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case5)
{
  CiSeqInfo info{true, false, ClibSeqType::clear};
  auto index = info.encode_val();
  EXPECT_EQ( 5, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case6)
{
  CiSeqInfo info{false, true, ClibSeqType::clear};
  auto index = info.encode_val();
  EXPECT_EQ( 6, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case7)
{
  CiSeqInfo info{true, true, ClibSeqType::clear};
  auto index = info.encode_val();
  EXPECT_EQ( 7, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_FALSE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case8)
{
  CiSeqInfo info{false, false, ClibSeqType::preset};
  auto index = info.encode_val();
  EXPECT_EQ( 8, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case9)
{
  CiSeqInfo info{true, false, ClibSeqType::preset};
  auto index = info.encode_val();
  EXPECT_EQ( 9, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case10)
{
  CiSeqInfo info{false, true, ClibSeqType::preset};
  auto index = info.encode_val();
  EXPECT_EQ( 10, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case11)
{
  CiSeqInfo info{true, true, ClibSeqType::preset};
  auto index = info.encode_val();
  EXPECT_EQ( 11, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_FALSE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case12_LL)
{
  CiSeqInfo info{false, false, ClibCPV::L, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 12, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case13_LL)
{
  CiSeqInfo info{true, false, ClibCPV::L, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 13, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case14_LL)
{
  CiSeqInfo info{false, true, ClibCPV::L, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 14, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case15_LL)
{
  CiSeqInfo info{true, true, ClibCPV::L, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 15, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case16_HL)
{
  CiSeqInfo info{false, false, ClibCPV::H, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 16, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case17_HL)
{
  CiSeqInfo info{true, false, ClibCPV::H, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 17, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case18_HL)
{
  CiSeqInfo info{false, true, ClibCPV::H, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 18, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case19_HL)
{
  CiSeqInfo info{true, true, ClibCPV::H, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 19, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case20_NL)
{
  CiSeqInfo info{false, false, ClibCPV::N, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 20, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case21_NL)
{
  CiSeqInfo info{true, false, ClibCPV::N, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 21, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case22_NL)
{
  CiSeqInfo info{false, true, ClibCPV::N, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 22, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case23_NL)
{
  CiSeqInfo info{true, true, ClibCPV::N, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 23, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case24_TL)
{
  CiSeqInfo info{false, false, ClibCPV::T, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 24, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case25_TL)
{
  CiSeqInfo info{true, false, ClibCPV::T, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 25, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case26_TL)
{
  CiSeqInfo info{false, true, ClibCPV::T, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 26, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case27_TL)
{
  CiSeqInfo info{true, true, ClibCPV::T, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 27, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case28_XL)
{
  CiSeqInfo info{false, false, ClibCPV::X, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 28, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case29_XL)
{
  CiSeqInfo info{true, false, ClibCPV::X, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 29, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case30_XL)
{
  CiSeqInfo info{false, true, ClibCPV::X, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 30, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case31_XL)
{
  CiSeqInfo info{true, true, ClibCPV::X, ClibCPV::L};
  auto index = info.encode_val();
  EXPECT_EQ( 31, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case32_LH)
{
  CiSeqInfo info{false, false, ClibCPV::L, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 32, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case33_LH)
{
  CiSeqInfo info{true, false, ClibCPV::L, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 33, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case34_LH)
{
  CiSeqInfo info{false, true, ClibCPV::L, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 34, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case35_LH)
{
  CiSeqInfo info{true, true, ClibCPV::L, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 35, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case36_HH)
{
  CiSeqInfo info{false, false, ClibCPV::H, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 36, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case37_HH)
{
  CiSeqInfo info{true, false, ClibCPV::H, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 37, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case38_HH)
{
  CiSeqInfo info{false, true, ClibCPV::H, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 38, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case39_HH)
{
  CiSeqInfo info{true, true, ClibCPV::H, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 39, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case40_NH)
{
  CiSeqInfo info{false, false, ClibCPV::N, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 40, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case41_NH)
{
  CiSeqInfo info{true, false, ClibCPV::N, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 41, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case42_NH)
{
  CiSeqInfo info{false, true, ClibCPV::N, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 42, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case43_NH)
{
  CiSeqInfo info{true, true, ClibCPV::N, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 43, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case44_TH)
{
  CiSeqInfo info{false, false, ClibCPV::T, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 44, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case45_TH)
{
  CiSeqInfo info{true, false, ClibCPV::T, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 45, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case46_TH)
{
  CiSeqInfo info{false, true, ClibCPV::T, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 46, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case47_TH)
{
  CiSeqInfo info{true, true, ClibCPV::T, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 47, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case48_XH)
{
  CiSeqInfo info{false, false, ClibCPV::X, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 48, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case49_XH)
{
  CiSeqInfo info{true, false, ClibCPV::X, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 49, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case50_XH)
{
  CiSeqInfo info{false, true, ClibCPV::X, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 50, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case51_XH)
{
  CiSeqInfo info{true, true, ClibCPV::X, ClibCPV::H};
  auto index = info.encode_val();
  EXPECT_EQ( 51, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case52_LN)
{
  CiSeqInfo info{false, false, ClibCPV::L, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 52, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case53_LN)
{
  CiSeqInfo info{true, false, ClibCPV::L, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 53, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case54_LN)
{
  CiSeqInfo info{false, true, ClibCPV::L, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 54, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case55_LN)
{
  CiSeqInfo info{true, true, ClibCPV::L, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 55, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case56_HN)
{
  CiSeqInfo info{false, false, ClibCPV::H, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 56, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case57_HN)
{
  CiSeqInfo info{true, false, ClibCPV::H, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 57, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case58_HN)
{
  CiSeqInfo info{false, true, ClibCPV::H, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 58, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case59_HN)
{
  CiSeqInfo info{true, true, ClibCPV::H, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 59, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case60_NN)
{
  CiSeqInfo info{false, false, ClibCPV::N, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 60, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case61_NN)
{
  CiSeqInfo info{true, false, ClibCPV::N, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 61, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case62_NN)
{
  CiSeqInfo info{false, true, ClibCPV::N, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 62, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case63_NN)
{
  CiSeqInfo info{true, true, ClibCPV::N, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 63, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case64_TN)
{
  CiSeqInfo info{false, false, ClibCPV::T, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 64, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case65_TN)
{
  CiSeqInfo info{true, false, ClibCPV::T, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 65, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case66_TN)
{
  CiSeqInfo info{false, true, ClibCPV::T, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 66, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case67_TN)
{
  CiSeqInfo info{true, true, ClibCPV::T, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 67, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case68_XN)
{
  CiSeqInfo info{false, false, ClibCPV::X, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 68, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case69_XN)
{
  CiSeqInfo info{true, false, ClibCPV::X, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 69, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case70_XN)
{
  CiSeqInfo info{false, true, ClibCPV::X, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 70, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case71_XN)
{
  CiSeqInfo info{true, true, ClibCPV::X, ClibCPV::N};
  auto index = info.encode_val();
  EXPECT_EQ( 71, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case72_LT)
{
  CiSeqInfo info{false, false, ClibCPV::L, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 72, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case73_LT)
{
  CiSeqInfo info{true, false, ClibCPV::L, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 73, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case74_LT)
{
  CiSeqInfo info{false, true, ClibCPV::L, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 74, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case75_LT)
{
  CiSeqInfo info{true, true, ClibCPV::L, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 75, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case76_HT)
{
  CiSeqInfo info{false, false, ClibCPV::H, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 76, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case77_HT)
{
  CiSeqInfo info{true, false, ClibCPV::H, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 77, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case78_HT)
{
  CiSeqInfo info{false, true, ClibCPV::H, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 78, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case79_HT)
{
  CiSeqInfo info{true, true, ClibCPV::H, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 79, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case80_NT)
{
  CiSeqInfo info{false, false, ClibCPV::N, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 80, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case81_NT)
{
  CiSeqInfo info{true, false, ClibCPV::N, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 81, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case82_NT)
{
  CiSeqInfo info{false, true, ClibCPV::N, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 82, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case83_NT)
{
  CiSeqInfo info{true, true, ClibCPV::N, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 83, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case84_TT)
{
  CiSeqInfo info{false, false, ClibCPV::T, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 84, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case85_TT)
{
  CiSeqInfo info{true, false, ClibCPV::T, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 85, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case86_TT)
{
  CiSeqInfo info{false, true, ClibCPV::T, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 86, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case87_TT)
{
  CiSeqInfo info{true, true, ClibCPV::T, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 87, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case88_XT)
{
  CiSeqInfo info{false, false, ClibCPV::X, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 88, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case89_XT)
{
  CiSeqInfo info{true, false, ClibCPV::X, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 89, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case90_XT)
{
  CiSeqInfo info{false, true, ClibCPV::X, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 90, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case91_XT)
{
  CiSeqInfo info{true, true, ClibCPV::X, ClibCPV::T};
  auto index = info.encode_val();
  EXPECT_EQ( 91, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case92_LX)
{
  CiSeqInfo info{false, false, ClibCPV::L, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 92, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case93_LX)
{
  CiSeqInfo info{true, false, ClibCPV::L, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 93, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case94_LX)
{
  CiSeqInfo info{false, true, ClibCPV::L, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 94, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case95_LX)
{
  CiSeqInfo info{true, true, ClibCPV::L, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 95, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::L, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case96_HX)
{
  CiSeqInfo info{false, false, ClibCPV::H, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 96, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case97_HX)
{
  CiSeqInfo info{true, false, ClibCPV::H, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 97, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case98_HX)
{
  CiSeqInfo info{false, true, ClibCPV::H, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 98, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case99_HX)
{
  CiSeqInfo info{true, true, ClibCPV::H, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 99, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::H, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case100_NX)
{
  CiSeqInfo info{false, false, ClibCPV::N, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 100, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case101_NX)
{
  CiSeqInfo info{true, false, ClibCPV::N, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 101, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case102_NX)
{
  CiSeqInfo info{false, true, ClibCPV::N, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 102, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case103_NX)
{
  CiSeqInfo info{true, true, ClibCPV::N, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 103, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::N, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case104_TX)
{
  CiSeqInfo info{false, false, ClibCPV::T, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 104, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case105_TX)
{
  CiSeqInfo info{true, false, ClibCPV::T, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 105, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case106_TX)
{
  CiSeqInfo info{false, true, ClibCPV::T, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 106, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case107_TX)
{
  CiSeqInfo info{true, true, ClibCPV::T, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 107, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
  EXPECT_EQ( ClibCPV::T, info.clear_preset_var1() );
  EXPECT_EQ( ClibCPV::X, info.clear_preset_var2() );
}

TEST(cell_CiSeqInfo_test, case108_XX)
{
  CiSeqInfo info{false, false, ClibCPV::X, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 108, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case109_XX)
{
  CiSeqInfo info{true, false, ClibCPV::X, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 109, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_FALSE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case110_XX)
{
  CiSeqInfo info{false, true, ClibCPV::X, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 110, index );
  EXPECT_FALSE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, case111_XX)
{
  CiSeqInfo info{true, true, ClibCPV::X, ClibCPV::X};
  auto index = info.encode_val();
  EXPECT_EQ( 111, index );
  EXPECT_TRUE( info.has_slave_clock() );
  EXPECT_TRUE( info.has_xq() );
  EXPECT_TRUE( info.has_clear() );
  EXPECT_TRUE( info.has_preset() );
}

TEST(cell_CiSeqInfo_test, enumerate_all)
{
  for ( SizeType index = 0; index < CiSeqInfo::max_index(); ++ index ) {
    auto info = CiSeqInfo::decode_index(index);
    EXPECT_EQ( index, info.encode_val() );
  }
}

END_NAMESPACE_YM_CLIB
