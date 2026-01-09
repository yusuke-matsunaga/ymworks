
/// @file ClibCapacitanceTest.cc
/// @brief ClibCapacitance のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/ClibCapacitance.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibCapacitanceTest, constructor1)
{
  ClibCapacitance cap{};

  // デフォルトコンストラクタの初期値は 0.0
  EXPECT_EQ( 0.0, cap.value() );
}

TEST(ClibCapacitanceTest, constructor2)
{
  const double INIT_VAL = 1.2;

  ClibCapacitance cap{INIT_VAL};

  EXPECT_EQ( INIT_VAL, cap.value() );
}

TEST(ClibCapacitanceTest, constructor3)
{
  const double INIT_VAL = 1.2;

  ClibCapacitance cap1{INIT_VAL};

  // コピーコンストラクタ
  auto cap2{cap1};

  EXPECT_EQ( INIT_VAL, cap2.value() );
}

TEST(ClibCapacitanceTest, assignment)
{
  const double INIT_VAL = 1.2;

  ClibCapacitance cap1{INIT_VAL};

  // 代入演算
  auto cap2 = cap1;

  EXPECT_EQ( INIT_VAL, cap2.value() );
}

TEST(ClibCapacitanceTest, inf)
{
  auto inf_cap{ClibCapacitance::infty()};

  EXPECT_EQ( std::numeric_limits<double>::max(), inf_cap.value() );
}

TEST(ClibCapacitanceTest, add)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};

  auto cap3 = cap1 + cap2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, cap3.value() );
}

TEST(ClibCapacitanceTest, add_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};

  cap1 += cap2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, cap1.value() );
}

TEST(ClibCapacitanceTest, sub)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};

  auto cap3 = cap1 - cap2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, cap3.value() );
}

TEST(ClibCapacitanceTest, sub_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};

  cap1 -= cap2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, cap1.value() );
}

TEST(ClibCapacitanceTest, eq)
{
  const double INIT_VAL1 = 1.2;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL1};

  EXPECT_EQ( cap1, cap2 );
}

TEST(ClibCapacitanceTest, ne)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};

  EXPECT_NE( cap1, cap2 );
}

TEST(ClibCapacitanceTest, lt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};
  ClibCapacitance cap3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 < INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, cap1 < cap2 );

  bool EXP_VAL2 = INIT_VAL2 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, cap2 < cap1 );

  bool EXP_VAL3 = INIT_VAL1 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, cap1 < cap3 );
}

TEST(ClibCapacitanceTest, le)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};
  ClibCapacitance cap3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 <= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, cap1 <= cap2 );

  bool EXP_VAL2 = INIT_VAL2 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, cap2 <= cap1 );

  bool EXP_VAL3 = INIT_VAL1 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, cap1 <= cap3 );
}

TEST(ClibCapacitanceTest, gt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};
  ClibCapacitance cap3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 > INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, cap1 > cap2 );

  bool EXP_VAL2 = INIT_VAL2 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, cap2 > cap1 );

  bool EXP_VAL3 = INIT_VAL1 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, cap1 > cap3 );
}

TEST(ClibCapacitanceTest, ge)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibCapacitance cap1{INIT_VAL1};
  ClibCapacitance cap2{INIT_VAL2};
  ClibCapacitance cap3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 >= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, cap1 >= cap2 );

  bool EXP_VAL2 = INIT_VAL2 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, cap2 >= cap1 );

  bool EXP_VAL3 = INIT_VAL1 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, cap1 >= cap3 );
}

TEST(ClibCapacitanceTest, stream)
{
  const double INIT_VAL = 1.2;
  ClibCapacitance cap1{INIT_VAL};

  std::ostringstream buf1;
  buf1 << cap1;

  std::istringstream buf2{buf1.str()};
  ClibCapacitance cap2;
  buf2 >> cap2;

  EXPECT_EQ( cap1, cap2 );
}
END_NAMESPACE_YM_CLIB
