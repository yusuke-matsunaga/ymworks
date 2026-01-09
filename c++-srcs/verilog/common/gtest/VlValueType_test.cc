
/// @file VlValueType_test.cc
/// @brief VlValueType_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/VlValueType.h"


BEGIN_NAMESPACE_YM

TEST(VlValueTypeTest, construct0)
{
  VlValueType vtype;

  EXPECT_TRUE( vtype.is_no_type() );
  EXPECT_FALSE( vtype.is_int_type() );
  EXPECT_FALSE( vtype.is_time_type() );
  EXPECT_FALSE( vtype.is_bitvector_type() );
  EXPECT_FALSE( vtype.is_real_type() );
  EXPECT_FALSE( vtype.is_signed() );
  EXPECT_FALSE( vtype.is_sized() );

  std::ostringstream buf;
  buf << vtype;
  EXPECT_EQ( "NO TYPE", buf.str() );
}

TEST(VlValueTypeTest, construct1)
{
  for ( auto has_sign: {false, true} ) {
    for ( auto has_size: {false, true} ) {
      SizeType size = 10;
      VlValueType vtype(has_sign, has_size, size);
      EXPECT_FALSE( vtype.is_no_type() );
      EXPECT_FALSE( vtype.is_int_type() );
      EXPECT_FALSE( vtype.is_time_type() );
      EXPECT_TRUE( vtype.is_bitvector_type() );
      EXPECT_FALSE( vtype.is_real_type() );
      EXPECT_EQ( has_sign, vtype.is_signed() );
      EXPECT_EQ( has_size, vtype.is_sized() );
      if ( has_size ) {
	EXPECT_EQ( size, vtype.size() );
      }
    }
  }
}

TEST(VlValueTypeTest, int_type)
{
  auto vtype = VlValueType::int_type();

  EXPECT_FALSE( vtype.is_no_type() );
  EXPECT_TRUE( vtype.is_int_type() );
  EXPECT_FALSE( vtype.is_time_type() );
  EXPECT_TRUE( vtype.is_bitvector_type() );
  EXPECT_FALSE( vtype.is_real_type() );
  EXPECT_TRUE( vtype.is_signed() );
  EXPECT_TRUE( vtype.is_sized() );
  EXPECT_EQ( 32, vtype.size() );
}

TEST(VlValueTypeTest, uint_type)
{
  auto vtype = VlValueType::uint_type();

  EXPECT_FALSE( vtype.is_no_type() );
  EXPECT_FALSE( vtype.is_int_type() );
  EXPECT_FALSE( vtype.is_time_type() );
  EXPECT_TRUE( vtype.is_bitvector_type() );
  EXPECT_FALSE( vtype.is_real_type() );
  EXPECT_FALSE( vtype.is_signed() );
  EXPECT_TRUE( vtype.is_sized() );
  EXPECT_EQ( 32, vtype.size() );
}

TEST(VlValueTypeTest, real_type)
{
  auto vtype = VlValueType::real_type();

  EXPECT_FALSE( vtype.is_no_type() );
  EXPECT_FALSE( vtype.is_int_type() );
  EXPECT_FALSE( vtype.is_time_type() );
  EXPECT_FALSE( vtype.is_bitvector_type() );
  EXPECT_TRUE( vtype.is_real_type() );
  EXPECT_TRUE( vtype.is_signed() );
  EXPECT_TRUE( vtype.is_sized() );
  EXPECT_EQ( 64, vtype.size() );
}

TEST(VlValueTypeTest, time_type)
{
  auto vtype = VlValueType::time_type();

  EXPECT_FALSE( vtype.is_no_type() );
  EXPECT_FALSE( vtype.is_int_type() );
  EXPECT_TRUE( vtype.is_time_type() );
  EXPECT_TRUE( vtype.is_bitvector_type() );
  EXPECT_FALSE( vtype.is_real_type() );
  EXPECT_FALSE( vtype.is_signed() );
  EXPECT_TRUE( vtype.is_sized() );
  EXPECT_EQ( 64, vtype.size() );
}

END_NAMESPACE_YM
