
/// @file Val3_test.cc
/// @brief Val3_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Val3.h"


BEGIN_NAMESPACE_YM

TEST(Val3Test, inv_op)
{
  EXPECT_EQ( Val3::ONE,  ~Val3::ZERO );
  EXPECT_EQ( Val3::ZERO, ~Val3::ONE );
  EXPECT_EQ( Val3::X,    ~Val3::X );
}

TEST(Val3Test, and_op)
{
  EXPECT_EQ( Val3::ZERO, Val3::ZERO & Val3::ZERO );
  EXPECT_EQ( Val3::ZERO, Val3::ZERO & Val3::ONE );
  EXPECT_EQ( Val3::ZERO, Val3::ZERO & Val3::X );
  EXPECT_EQ( Val3::ZERO, Val3::ONE  & Val3::ZERO );
  EXPECT_EQ( Val3::ONE,  Val3::ONE  & Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::ONE  & Val3::X );
  EXPECT_EQ( Val3::ZERO, Val3::X    & Val3::ZERO );
  EXPECT_EQ( Val3::X,    Val3::X    & Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::X    & Val3::X );
}

TEST(Val3Test, or_op)
{
  EXPECT_EQ( Val3::ZERO, Val3::ZERO | Val3::ZERO );
  EXPECT_EQ( Val3::ONE,  Val3::ZERO | Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::ZERO | Val3::X );
  EXPECT_EQ( Val3::ONE,  Val3::ONE  | Val3::ZERO );
  EXPECT_EQ( Val3::ONE,  Val3::ONE  | Val3::ONE );
  EXPECT_EQ( Val3::ONE,  Val3::ONE  | Val3::X );
  EXPECT_EQ( Val3::X,    Val3::X    | Val3::ZERO );
  EXPECT_EQ( Val3::ONE,  Val3::X    | Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::X    | Val3::X );
}

TEST(Val3Test, xor_op)
{
  EXPECT_EQ( Val3::ZERO, Val3::ZERO ^ Val3::ZERO );
  EXPECT_EQ( Val3::ONE,  Val3::ZERO ^ Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::ZERO ^ Val3::X );
  EXPECT_EQ( Val3::ONE,  Val3::ONE  ^ Val3::ZERO );
  EXPECT_EQ( Val3::ZERO, Val3::ONE  ^ Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::ONE  ^ Val3::X );
  EXPECT_EQ( Val3::X,    Val3::X    ^ Val3::ZERO );
  EXPECT_EQ( Val3::X,    Val3::X    ^ Val3::ONE );
  EXPECT_EQ( Val3::X,    Val3::X    ^ Val3::X );
}

TEST(Val3Test, val3_name)
{
  EXPECT_EQ( 'X', val3_name(Val3::X) );
  EXPECT_EQ( '0', val3_name(Val3::ZERO) );
  EXPECT_EQ( '1', val3_name(Val3::ONE) );
}

END_NAMESPACE_YM
