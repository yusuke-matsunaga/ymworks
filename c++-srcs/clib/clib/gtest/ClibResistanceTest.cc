
/// @file ClibResistanceTest.cc
/// @brief ClibResistance のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/ClibResistance.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibResistanceTest, constructor1)
{
  ClibResistance res{};

  // デフォルトコンストラクタの初期値は 0.0
  EXPECT_EQ( 0.0, res.value() );
}

TEST(ClibResistanceTest, constructor2)
{
  const double INIT_VAL = 1.2;

  ClibResistance res{INIT_VAL};

  EXPECT_EQ( INIT_VAL, res.value() );
}

TEST(ClibResistanceTest, constructor3)
{
  const double INIT_VAL = 1.2;

  ClibResistance res1{INIT_VAL};

  // コピーコンストラクタ
  auto res2{res1};

  EXPECT_EQ( INIT_VAL, res2.value() );
}

TEST(ClibResistanceTest, assignment)
{
  const double INIT_VAL = 1.2;

  ClibResistance res1{INIT_VAL};

  // 代入演算
  auto res2 = res1;

  EXPECT_EQ( INIT_VAL, res2.value() );
}

TEST(ClibResistanceTest, inf)
{
  auto inf_res = ClibResistance::infinity();

  EXPECT_EQ( std::numeric_limits<double>::infinity(), inf_res.value() );
}

TEST(ClibResistanceTest, add)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};

  auto res3 = res1 + res2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, res3.value() );
}

TEST(ClibResistanceTest, add_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};

  res1 += res2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, res1.value() );
}

TEST(ClibResistanceTest, sub)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};

  auto res3 = res1 - res2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, res3.value() );
}

TEST(ClibResistanceTest, sub_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};

  res1 -= res2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, res1.value() );
}

TEST(ClibResistanceTest, eq)
{
  const double INIT_VAL1 = 1.2;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL1};

  EXPECT_EQ( res1, res2 );
}

TEST(ClibResistanceTest, ne)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};

  EXPECT_NE( res1, res2 );
}

TEST(ClibResistanceTest, lt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};
  ClibResistance res3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 < INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, res1 < res2 );

  bool EXP_VAL2 = INIT_VAL2 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, res2 < res1 );

  bool EXP_VAL3 = INIT_VAL1 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, res1 < res3 );
}

TEST(ClibResistanceTest, le)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};
  ClibResistance res3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 <= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, res1 <= res2 );

  bool EXP_VAL2 = INIT_VAL2 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, res2 <= res1 );

  bool EXP_VAL3 = INIT_VAL1 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, res1 <= res3 );
}

TEST(ClibResistanceTest, gt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};
  ClibResistance res3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 > INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, res1 > res2 );

  bool EXP_VAL2 = INIT_VAL2 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, res2 > res1 );

  bool EXP_VAL3 = INIT_VAL1 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, res1 > res3 );
}

TEST(ClibResistanceTest, ge)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibResistance res1{INIT_VAL1};
  ClibResistance res2{INIT_VAL2};
  ClibResistance res3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 >= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, res1 >= res2 );

  bool EXP_VAL2 = INIT_VAL2 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, res2 >= res1 );

  bool EXP_VAL3 = INIT_VAL1 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, res1 >= res3 );
}

TEST(ClibResistanceTest, stream)
{
  const double INIT_VAL = 1.2;
  ClibResistance res1{INIT_VAL};

  std::ostringstream buf1;
  buf1 << res1;

  std::istringstream buf2{buf1.str()};
  ClibResistance res2;
  buf2 >> res2;

  EXPECT_EQ( res1, res2 );
}
END_NAMESPACE_YM_CLIB
