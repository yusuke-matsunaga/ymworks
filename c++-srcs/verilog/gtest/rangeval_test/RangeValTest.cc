
/// @file RangeValTest.cc
/// @brief RangeValTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "elaborator/RangeVal.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST(RangeValTest, test1)
{
  // [7:0] の範囲
  RangeVal range{7, 0};

  EXPECT_EQ( 7, range.left );
  EXPECT_EQ( 0, range.right );
  EXPECT_EQ( 8, range.calc_size() );
  for ( int i = 0; i < 8; ++ i ) {
    EXPECT_TRUE( range.is_in(i) );
  }
  EXPECT_FALSE( range.is_in(-1) );
  EXPECT_FALSE( range.is_in(8) );

  for ( int i = 0; i < 8; ++ i ) {
    SizeType offset;
    auto r = range.calc_offset(i, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = i;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(-1, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(8, offset);
    EXPECT_FALSE( r );
  }

  for ( int i = 0; i < 8; ++ i ) {
    SizeType offset;
    auto r = range.calc_roffset(i, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = 7 - i;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(-1, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(8, offset);
    EXPECT_FALSE( r );
  }

  for ( SizeType offset = 0; offset < 8; ++ offset ) {
    auto index = range.index(offset);
    int exp_index = offset;
    EXPECT_EQ( exp_index, index );
  }

  for ( SizeType roffset = 0; roffset < 8; ++ roffset ) {
    auto rindex = range.rindex(roffset);
    int exp_rindex = 7 - roffset;
    EXPECT_EQ( exp_rindex, rindex );
  }

  EXPECT_FALSE( range.is_big_endian() );
  EXPECT_TRUE( range.is_little_endian() );
}

TEST(RangeValTest, test2)
{
  // [17:10] の範囲
  RangeVal range{17, 10};

  EXPECT_EQ( 8, range.calc_size() );
  for ( int i = 10; i < 18; ++ i ) {
    EXPECT_TRUE( range.is_in(i) );
  }
  EXPECT_FALSE( range.is_in(9) );
  EXPECT_FALSE( range.is_in(18) );

  for ( int i = 10; i < 18; ++ i ) {
    SizeType offset;
    auto r = range.calc_offset(i, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = i - 10;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(9, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(18, offset);
    EXPECT_FALSE( r );
  }

  for ( int i = 10; i < 18; ++ i ) {
    SizeType offset;
    auto r = range.calc_roffset(i, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = 17 - i;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(9, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(18, offset);
    EXPECT_FALSE( r );
  }

  for ( SizeType offset = 0; offset < 8; ++ offset ) {
    auto index = range.index(offset);
    int exp_index = offset + 10;
    EXPECT_EQ( exp_index, index );
  }

  for ( SizeType roffset = 0; roffset < 8; ++ roffset ) {
    auto rindex = range.rindex(roffset);
    int exp_rindex = 17 - roffset;
    EXPECT_EQ( exp_rindex, rindex );
  }

  EXPECT_FALSE( range.is_big_endian() );
  EXPECT_TRUE( range.is_little_endian() );
}

TEST(RangeValTest, test3)
{
  // [0:7] の範囲
  RangeVal range{0, 7};

  EXPECT_EQ( 8, range.calc_size() );
  for ( int i = 0; i < 8; ++ i ) {
    EXPECT_TRUE( range.is_in(i) );
  }
  EXPECT_FALSE( range.is_in(-1) );
  EXPECT_FALSE( range.is_in(8) );

  for ( int i = 0; i < 8; ++ i ) {
    SizeType offset;
    auto r = range.calc_offset(i, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = 7 - i;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(-1, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(8, offset);
    EXPECT_FALSE( r );
  }

  for ( int i = 0; i < 8; ++ i ) {
    SizeType offset;
    auto r = range.calc_roffset(i, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = i;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(-1, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(8, offset);
    EXPECT_FALSE( r );
  }

  for ( SizeType offset = 0; offset < 8; ++ offset ) {
    auto index = range.index(offset);
    int exp_index = 7 - offset;
    EXPECT_EQ( exp_index, index );
  }

  for ( SizeType roffset = 0; roffset < 8; ++ roffset ) {
    auto rindex = range.rindex(roffset);
    int exp_rindex = roffset;
    EXPECT_EQ( exp_rindex, rindex );
  }

  EXPECT_TRUE( range.is_big_endian() );
  EXPECT_FALSE( range.is_little_endian() );
}

TEST(RangeValTest, test4)
{
  // [0:0] の範囲
  RangeVal range{0, 0};

  EXPECT_EQ( 0, range.left );
  EXPECT_EQ( 0, range.right );
  EXPECT_EQ( 1, range.calc_size() );
  for ( int i = 0; i < 1; ++ i ) {
    EXPECT_TRUE( range.is_in(i) );
  }
  EXPECT_FALSE( range.is_in(-1) );
  EXPECT_FALSE( range.is_in(1) );

  {
    SizeType offset;
    auto r = range.calc_offset(0, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = 0;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(-1, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_offset(1, offset);
    EXPECT_FALSE( r );
  }

  {
    SizeType offset;
    auto r = range.calc_roffset(0, offset);
    ASSERT_TRUE( r );
    SizeType exp_offset = 0;
    EXPECT_EQ( exp_offset, offset );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(-1, offset);
    EXPECT_FALSE( r );
  }
  {
    SizeType offset;
    auto r = range.calc_roffset(1, offset);
    EXPECT_FALSE( r );
  }

  {
    auto index = range.index(0);
    int exp_index = 0;
    EXPECT_EQ( exp_index, index );
  }

  {
    auto rindex = range.rindex(0);
    int exp_rindex = 0;
    EXPECT_EQ( exp_rindex, rindex );
  }

  EXPECT_TRUE( range.is_big_endian() );
  EXPECT_TRUE( range.is_little_endian() );
}

END_NAMESPACE_YM_VERILOG
