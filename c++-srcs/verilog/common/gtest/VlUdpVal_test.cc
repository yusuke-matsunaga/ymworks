
/// @file VlUdpVal_test.cc
/// @brief VlUdpVal_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/VlUdpVal.h"


BEGIN_NAMESPACE_YM

TEST(VlUdpValTest, default_construct)
{ // デフォルトコンストラクタ
  VlUdpVal val;

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE( val.is_edge_symbol() );
  EXPECT_TRUE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "-", val.to_string() );

  EXPECT_EQ( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

}

TEST(VlUdpValTest, construct_0)
{
  auto val = VlUdpVal('0');

  EXPECT_TRUE( val.is_level_symbol() );
  EXPECT_FALSE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "0", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_EQ( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE( VlUdpVal('0') <= val );
  EXPECT_FALSE( VlUdpVal('1') <= val );
  EXPECT_FALSE( VlUdpVal('X') <= val );
}

TEST(VlUdpValTest, construct_1)
{
  auto val = VlUdpVal('1');

  EXPECT_TRUE( val.is_level_symbol() );
  EXPECT_FALSE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "1", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_EQ( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0') <= val );
  EXPECT_TRUE( VlUdpVal('1') <= val );
  EXPECT_FALSE( VlUdpVal('X') <= val );
}

TEST(VlUdpValTest, construct_X)
{
  auto val = VlUdpVal('X');

  EXPECT_TRUE( val.is_level_symbol() );
  EXPECT_FALSE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "x", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_EQ( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0') <= val );
  EXPECT_FALSE( VlUdpVal('1') <= val );
  EXPECT_TRUE( VlUdpVal('X') <= val );
}

TEST(VlUdpValTest, construct_B)
{
  auto val = VlUdpVal('B');

  EXPECT_TRUE( val.is_level_symbol() );
  EXPECT_FALSE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_TRUE( val.is_composite_symbol() );
  EXPECT_EQ( "b", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_EQ( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE( VlUdpVal('0') <= val );
  EXPECT_TRUE( VlUdpVal('1') <= val );
  EXPECT_FALSE( VlUdpVal('X') <= val );
}

TEST(VlUdpValTest, construct_Q)
{
  auto val = VlUdpVal('?');

  EXPECT_TRUE( val.is_level_symbol() );
  EXPECT_FALSE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_TRUE( val.is_composite_symbol() );
  EXPECT_EQ( "?", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_EQ( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE( VlUdpVal('0') <= val );
  EXPECT_TRUE( VlUdpVal('1') <= val );
  EXPECT_TRUE( VlUdpVal('X') <= val );
}

TEST(VlUdpValTest, construct_R)
{
  auto val = VlUdpVal('R');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "01", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_EQ( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_EQ( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_F)
{
  auto val = VlUdpVal('F');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "10", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_EQ( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_EQ( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_P)
{
  auto val = VlUdpVal('P');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "p", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_EQ( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_N)
{
  auto val = VlUdpVal('N');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "n", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_EQ( VlUdpVal('n'), val );
  EXPECT_NE( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_STAR)
{
  auto val = VlUdpVal('*');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "??", val.to_string() );

  EXPECT_NE( VlUdpVal(), val );
  EXPECT_NE( VlUdpVal('0'), val );
  EXPECT_NE( VlUdpVal('1'), val );
  EXPECT_NE( VlUdpVal('X'), val );
  EXPECT_NE( VlUdpVal('B'), val );
  EXPECT_NE( VlUdpVal('?'), val );
  EXPECT_NE( VlUdpVal('r'), val );
  EXPECT_NE( VlUdpVal('f'), val );
  EXPECT_NE( VlUdpVal('p'), val );
  EXPECT_NE( VlUdpVal('n'), val );
  EXPECT_EQ( VlUdpVal('*'), val );
  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_EQ( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_00)
{
  auto val = VlUdpVal('0', '0');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "00", val.to_string() );

  EXPECT_EQ( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_01)
{
  auto val = VlUdpVal('0', '1');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "01", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_EQ( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_0X)
{
  auto val = VlUdpVal('0', 'X');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "0x", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_EQ( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_0B)
{
  auto val = VlUdpVal('0', 'B');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "0b", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_EQ( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_0Q)
{
  auto val = VlUdpVal('0', '?');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "0?", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_EQ( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_10)
{
  auto val = VlUdpVal('1', '0');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "10", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_EQ( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_11)
{
  auto val = VlUdpVal('1', '1');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "11", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_EQ( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_1X)
{
  auto val = VlUdpVal('1', 'X');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "1x", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_EQ( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_1B)
{
  auto val = VlUdpVal('1', 'B');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "1b", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_EQ( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_1Q)
{
  auto val = VlUdpVal('1', '?');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "1?", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_EQ( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_X0)
{
  auto val = VlUdpVal('X', '0');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "x0", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_EQ( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_X1)
{
  auto val = VlUdpVal('X', '1');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "x1", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_EQ( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_XX)
{
  auto val = VlUdpVal('X', 'X');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "xx", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_EQ( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_XB)
{
  auto val = VlUdpVal('X', 'B');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "xb", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_EQ( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_XQ)
{
  auto val = VlUdpVal('X', '?');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "x?", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_EQ( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_Q0)
{
  auto val = VlUdpVal('?', '0');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "?0", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_EQ( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_Q1)
{
  auto val = VlUdpVal('?', '1');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "?1", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_EQ( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_QX)
{
  auto val = VlUdpVal('?', 'X');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "?x", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_EQ( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_FALSE( VlUdpVal('0', '0') <= val );
  EXPECT_FALSE( VlUdpVal('0', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('0', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('1', '0') <= val );
  EXPECT_FALSE( VlUdpVal('1', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('1', 'X') <= val );
  EXPECT_FALSE( VlUdpVal('X', '0') <= val );
  EXPECT_FALSE( VlUdpVal('X', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_QB)
{
  auto val = VlUdpVal('?', 'B');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "?b", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_EQ( VlUdpVal('?', 'B'), val );
  EXPECT_NE( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_FALSE( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_FALSE( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_FALSE( VlUdpVal('X', 'X') <= val );
}

TEST(VlUdpValTest, construct_QQ)
{
  auto val = VlUdpVal('?', '?');

  EXPECT_FALSE( val.is_level_symbol() );
  EXPECT_TRUE ( val.is_edge_symbol() );
  EXPECT_FALSE( val.is_nc_symbol() );
  EXPECT_FALSE( val.is_composite_symbol() );
  EXPECT_EQ( "??", val.to_string() );

  EXPECT_NE( VlUdpVal('0', '0'), val );
  EXPECT_NE( VlUdpVal('0', '1'), val );
  EXPECT_NE( VlUdpVal('0', 'X'), val );
  EXPECT_NE( VlUdpVal('0', 'B'), val );
  EXPECT_NE( VlUdpVal('0', '?'), val );
  EXPECT_NE( VlUdpVal('1', '0'), val );
  EXPECT_NE( VlUdpVal('1', '1'), val );
  EXPECT_NE( VlUdpVal('1', 'X'), val );
  EXPECT_NE( VlUdpVal('1', 'B'), val );
  EXPECT_NE( VlUdpVal('1', '?'), val );
  EXPECT_NE( VlUdpVal('X', '0'), val );
  EXPECT_NE( VlUdpVal('X', '1'), val );
  EXPECT_NE( VlUdpVal('X', 'X'), val );
  EXPECT_NE( VlUdpVal('X', 'B'), val );
  EXPECT_NE( VlUdpVal('X', '?'), val );
  EXPECT_NE( VlUdpVal('B', '0'), val );
  EXPECT_NE( VlUdpVal('B', '1'), val );
  EXPECT_NE( VlUdpVal('B', 'X'), val );
  EXPECT_NE( VlUdpVal('B', 'B'), val );
  EXPECT_NE( VlUdpVal('B', '?'), val );
  EXPECT_NE( VlUdpVal('?', '0'), val );
  EXPECT_NE( VlUdpVal('?', '1'), val );
  EXPECT_NE( VlUdpVal('?', 'X'), val );
  EXPECT_NE( VlUdpVal('?', 'B'), val );
  EXPECT_EQ( VlUdpVal('?', '?'), val );

  EXPECT_TRUE ( VlUdpVal('0', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('0', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('0', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('1', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('1', 'X') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '0') <= val );
  EXPECT_TRUE ( VlUdpVal('X', '1') <= val );
  EXPECT_TRUE ( VlUdpVal('X', 'X') <= val );
}

END_NAMESPACE_YM
