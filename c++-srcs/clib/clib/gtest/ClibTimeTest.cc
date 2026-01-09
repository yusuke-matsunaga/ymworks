
/// @file ClibTimeTest.cc
/// @brief ClibTime のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/ClibTime.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibTimeTest, constructor1)
{
  ClibTime time{};

  // デフォルトコンストラクタの初期値は 0.0
  EXPECT_EQ( 0.0, time.value() );
}

TEST(ClibTimeTest, constructor2)
{
  const double INIT_VAL = 1.2;

  ClibTime time{INIT_VAL};

  EXPECT_EQ( INIT_VAL, time.value() );
}

TEST(ClibTimeTest, constructor3)
{
  const double INIT_VAL = 1.2;

  ClibTime time1{INIT_VAL};

  // コピーコンストラクタ
  auto time2{time1};

  EXPECT_EQ( INIT_VAL, time2.value() );
}

TEST(ClibTimeTest, assignment)
{
  const double INIT_VAL = 1.2;

  ClibTime time1{INIT_VAL};

  // 代入演算
  auto time2 = time1;

  EXPECT_EQ( INIT_VAL, time2.value() );
}

TEST(ClibTimeTest, inf)
{
  auto inf_time{ClibTime::infty()};

  EXPECT_EQ( std::numeric_limits<double>::max(), inf_time.value() );
}

TEST(ClibTimeTest, add)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};

  auto time3 = time1 + time2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, time3.value() );
}

TEST(ClibTimeTest, add_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};

  time1 += time2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, time1.value() );
}

TEST(ClibTimeTest, sub)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};

  auto time3 = time1 - time2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, time3.value() );
}

TEST(ClibTimeTest, sub_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};

  time1 -= time2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, time1.value() );
}

TEST(ClibTimeTest, eq)
{
  const double INIT_VAL1 = 1.2;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL1};

  EXPECT_EQ( time1, time2 );
}

TEST(ClibTimeTest, ne)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};

  EXPECT_NE( time1, time2 );
}

TEST(ClibTimeTest, lt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};
  ClibTime time3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 < INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, time1 < time2 );

  bool EXP_VAL2 = INIT_VAL2 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, time2 < time1 );

  bool EXP_VAL3 = INIT_VAL1 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, time1 < time3 );
}

TEST(ClibTimeTest, le)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};
  ClibTime time3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 <= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, time1 <= time2 );

  bool EXP_VAL2 = INIT_VAL2 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, time2 <= time1 );

  bool EXP_VAL3 = INIT_VAL1 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, time1 <= time3 );
}

TEST(ClibTimeTest, gt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};
  ClibTime time3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 > INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, time1 > time2 );

  bool EXP_VAL2 = INIT_VAL2 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, time2 > time1 );

  bool EXP_VAL3 = INIT_VAL1 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, time1 > time3 );
}

TEST(ClibTimeTest, ge)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibTime time1{INIT_VAL1};
  ClibTime time2{INIT_VAL2};
  ClibTime time3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 >= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, time1 >= time2 );

  bool EXP_VAL2 = INIT_VAL2 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, time2 >= time1 );

  bool EXP_VAL3 = INIT_VAL1 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, time1 >= time3 );
}

TEST(ClibTimeTest, stream)
{
  const double INIT_VAL = 1.2;
  ClibTime time1{INIT_VAL};

  std::ostringstream buf1;
  buf1 << time1;

  std::istringstream buf2{buf1.str()};
  ClibTime time2;
  buf2 >> time2;

  EXPECT_EQ( time1, time2 );
}
END_NAMESPACE_YM_CLIB
