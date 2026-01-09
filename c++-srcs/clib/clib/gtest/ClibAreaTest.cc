
/// @file ClibAreaTest.cc
/// @brief ClibArea のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/ClibArea.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibAreaTest, constructor1)
{
  ClibArea area{};

  // デフォルトコンストラクタの初期値は 0.0
  EXPECT_EQ( 0.0, area.value() );
}

TEST(ClibAreaTest, constructor2)
{
  const double INIT_VAL = 1.2;

  ClibArea area{INIT_VAL};

  EXPECT_EQ( INIT_VAL, area.value() );
}

TEST(ClibAreaTest, constructor3)
{
  const double INIT_VAL = 1.2;

  ClibArea area1{INIT_VAL};

  // コピーコンストラクタ
  auto area2{area1};

  EXPECT_EQ( INIT_VAL, area2.value() );
}

TEST(ClibAreaTest, assignment)
{
  const double INIT_VAL = 1.2;

  ClibArea area1{INIT_VAL};

  // 代入演算
  auto area2 = area1;

  EXPECT_EQ( INIT_VAL, area2.value() );
}

TEST(ClibAreaTest, inf)
{
  auto inf_area{ClibArea::infty()};

  EXPECT_EQ( std::numeric_limits<double>::max(), inf_area.value() );
}

TEST(ClibAreaTest, add)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};

  auto area3 = area1 + area2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, area3.value() );
}

TEST(ClibAreaTest, add_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};

  area1 += area2;

  const double ANS_VAL = INIT_VAL1 + INIT_VAL2;
  EXPECT_EQ( ANS_VAL, area1.value() );
}

TEST(ClibAreaTest, sub)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};

  auto area3 = area1 - area2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, area3.value() );
}

TEST(ClibAreaTest, sub_intern)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};

  area1 -= area2;

  const double ANS_VAL = INIT_VAL1 - INIT_VAL2;
  EXPECT_EQ( ANS_VAL, area1.value() );
}

TEST(ClibAreaTest, eq)
{
  const double INIT_VAL1 = 1.2;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL1};

  EXPECT_EQ( area1, area2 );
}

TEST(ClibAreaTest, ne)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};

  EXPECT_NE( area1, area2 );
}

TEST(ClibAreaTest, lt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};
  ClibArea area3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 < INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, area1 < area2 );

  bool EXP_VAL2 = INIT_VAL2 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, area2 < area1 );

  bool EXP_VAL3 = INIT_VAL1 < INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, area1 < area3 );
}

TEST(ClibAreaTest, le)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};
  ClibArea area3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 <= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, area1 <= area2 );

  bool EXP_VAL2 = INIT_VAL2 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, area2 <= area1 );

  bool EXP_VAL3 = INIT_VAL1 <= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, area1 <= area3 );
}

TEST(ClibAreaTest, gt)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};
  ClibArea area3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 > INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, area1 > area2 );

  bool EXP_VAL2 = INIT_VAL2 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, area2 > area1 );

  bool EXP_VAL3 = INIT_VAL1 > INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, area1 > area3 );
}

TEST(ClibAreaTest, ge)
{
  const double INIT_VAL1 = 1.2;
  const double INIT_VAL2 = 3.4;
  ClibArea area1{INIT_VAL1};
  ClibArea area2{INIT_VAL2};
  ClibArea area3{INIT_VAL1};

  bool EXP_VAL1 = INIT_VAL1 >= INIT_VAL2;
  EXPECT_EQ( EXP_VAL1, area1 >= area2 );

  bool EXP_VAL2 = INIT_VAL2 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL2, area2 >= area1 );

  bool EXP_VAL3 = INIT_VAL1 >= INIT_VAL1;
  EXPECT_EQ( EXP_VAL3, area1 >= area3 );
}

TEST(ClibAreaTest, stream)
{
  const double INIT_VAL = 1.2;
  ClibArea area1{INIT_VAL};

  std::ostringstream buf1;
  buf1 << area1;

  std::istringstream buf2{buf1.str()};
  ClibArea area2;
  buf2 >> area2;

  EXPECT_EQ( area1, area2 );
}
END_NAMESPACE_YM_CLIB
