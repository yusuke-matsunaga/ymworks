
/// @file FraigLiteral_test.cc
/// @brief FraigLiteral_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/FraigLiteral.h"
#include "ym/FraigMgr.h"


BEGIN_NAMESPACE_YM

TEST(FraigLiteralTest, zero)
{
  auto lit = FraigLiteral::zero();

  EXPECT_TRUE( lit.is_zero() );
  EXPECT_FALSE( lit.is_one() );
  EXPECT_TRUE( lit.is_const() );
  EXPECT_FALSE( lit.is_and() );

  auto ilit = ~lit;
  EXPECT_FALSE( ilit.is_zero() );
  EXPECT_TRUE( ilit.is_one() );
  EXPECT_TRUE( ilit.is_const() );
  EXPECT_FALSE( ilit.is_and() );

  auto lit0 = lit * false;
  EXPECT_EQ( lit, lit0 );

  auto lit1 = lit * true;
  EXPECT_EQ( ilit, lit1 );
}

TEST(FraigLiteralTest, one)
{
  auto lit = FraigLiteral::one();

  EXPECT_FALSE( lit.is_zero() );
  EXPECT_TRUE( lit.is_one() );
  EXPECT_TRUE( lit.is_const() );
  EXPECT_FALSE( lit.is_and() );

  auto ilit = ~lit;
  EXPECT_TRUE( ilit.is_zero() );
  EXPECT_FALSE( ilit.is_one() );
  EXPECT_TRUE( ilit.is_const() );
  EXPECT_FALSE( ilit.is_and() );

  auto lit0 = lit * false;
  EXPECT_EQ( lit, lit0 );

  auto lit1 = lit * true;
  EXPECT_EQ( ilit, lit1 );
}

TEST(FraigLiteralTest, input)
{
  FraigMgr mgr(1024);

  auto lit = mgr.new_input();

  EXPECT_FALSE( lit.is_zero() );
  EXPECT_FALSE( lit.is_one() );
  EXPECT_FALSE( lit.is_const() );
  EXPECT_TRUE( lit.is_and() );

  auto ilit = ~lit;
  EXPECT_FALSE( lit.is_zero() );
  EXPECT_FALSE( lit.is_one() );
  EXPECT_FALSE( lit.is_const() );
  EXPECT_TRUE( lit.is_and() );

  auto lit0 = lit * false;
  EXPECT_EQ( lit, lit0 );

  auto lit1 = lit * true;
  EXPECT_EQ( ilit, lit1 );
}

END_NAMESPACE_YM
