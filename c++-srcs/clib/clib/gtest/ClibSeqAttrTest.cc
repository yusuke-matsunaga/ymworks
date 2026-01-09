
/// @file ClibSeqAttrTest.cc
/// @brief ClibSeqAttr のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/ClibSeqAttr.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibSeqAttrTest, constr0)
{
  ClibSeqAttr seq_attr{false, false, false};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_FALSE( seq_attr.has_clear() );
  EXPECT_FALSE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 0;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr1)
{
  ClibSeqAttr seq_attr{true, false, false};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_FALSE( seq_attr.has_clear() );
  EXPECT_FALSE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 1;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr2)
{
  ClibSeqAttr seq_attr{false, true, false};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_FALSE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 2;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr3)
{
  ClibSeqAttr seq_attr{true, true, false};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_FALSE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 3;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr4)
{
  ClibSeqAttr seq_attr{false, false, true};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_FALSE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 4;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr5)
{
  ClibSeqAttr seq_attr{true, false, true};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_FALSE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 5;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr6_bad)
{
  ASSERT_THROW(
    ClibSeqAttr seq_attr(false, true, true)
    , std::invalid_argument);
}

TEST(ClibSeqAttrTest, constr6)
{
  ClibSeqAttr seq_attr{false, ClibCPV::L, ClibCPV::L};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 6;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr7_bad)
{
  ASSERT_THROW(
    ClibSeqAttr seq_attr(true, true, true)
    , std::invalid_argument);
}

TEST(ClibSeqAttrTest, constr7)
{
  ClibSeqAttr seq_attr{true, ClibCPV::L, ClibCPV::L};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 7;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr8)
{
  ClibSeqAttr seq_attr{false, ClibCPV::H, ClibCPV::L};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 8;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr9)
{
  ClibSeqAttr seq_attr{true, ClibCPV::H, ClibCPV::L};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 9;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr10)
{
  ClibSeqAttr seq_attr{false, ClibCPV::N, ClibCPV::L};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 10;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr11)
{
  ClibSeqAttr seq_attr{true, ClibCPV::N, ClibCPV::L};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 11;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr12)
{
  ClibSeqAttr seq_attr{false, ClibCPV::T, ClibCPV::L};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 12;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr13)
{
  ClibSeqAttr seq_attr{true, ClibCPV::T, ClibCPV::L};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 13;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr14)
{
  ClibSeqAttr seq_attr{false, ClibCPV::X, ClibCPV::L};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 14;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr15)
{
  ClibSeqAttr seq_attr{true, ClibCPV::X, ClibCPV::L};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv2() );
  SizeType index = 15;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr16)
{
  ClibSeqAttr seq_attr{false, ClibCPV::L, ClibCPV::H};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 16;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr17)
{
  ClibSeqAttr seq_attr{true, ClibCPV::L, ClibCPV::H};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 17;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr18)
{
  ClibSeqAttr seq_attr{false, ClibCPV::H, ClibCPV::H};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 18;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr19)
{
  ClibSeqAttr seq_attr{true, ClibCPV::H, ClibCPV::H};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 19;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr20)
{
  ClibSeqAttr seq_attr{false, ClibCPV::N, ClibCPV::H};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 20;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr21)
{
  ClibSeqAttr seq_attr{true, ClibCPV::N, ClibCPV::H};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 21;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr22)
{
  ClibSeqAttr seq_attr{false, ClibCPV::T, ClibCPV::H};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 22;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr23)
{
  ClibSeqAttr seq_attr{true, ClibCPV::T, ClibCPV::H};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 23;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr24)
{
  ClibSeqAttr seq_attr{false, ClibCPV::X, ClibCPV::H};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 24;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr25)
{
  ClibSeqAttr seq_attr{true, ClibCPV::X, ClibCPV::H};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv2() );
  SizeType index = 25;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr26)
{
  ClibSeqAttr seq_attr{false, ClibCPV::L, ClibCPV::N};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 26;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr27)
{
  ClibSeqAttr seq_attr{true, ClibCPV::L, ClibCPV::N};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 27;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr28)
{
  ClibSeqAttr seq_attr{false, ClibCPV::H, ClibCPV::N};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 28;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr29)
{
  ClibSeqAttr seq_attr{true, ClibCPV::H, ClibCPV::N};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 29;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr30)
{
  ClibSeqAttr seq_attr{false, ClibCPV::N, ClibCPV::N};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 30;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr31)
{
  ClibSeqAttr seq_attr{true, ClibCPV::N, ClibCPV::N};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 31;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr32)
{
  ClibSeqAttr seq_attr{false, ClibCPV::T, ClibCPV::N};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 32;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr33)
{
  ClibSeqAttr seq_attr{true, ClibCPV::T, ClibCPV::N};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 33;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr34)
{
  ClibSeqAttr seq_attr{false, ClibCPV::X, ClibCPV::N};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 34;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr35)
{
  ClibSeqAttr seq_attr{true, ClibCPV::X, ClibCPV::N};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv2() );
  SizeType index = 35;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr36)
{
  ClibSeqAttr seq_attr{false, ClibCPV::L, ClibCPV::T};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 36;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr37)
{
  ClibSeqAttr seq_attr{true, ClibCPV::L, ClibCPV::T};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 37;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr38)
{
  ClibSeqAttr seq_attr{false, ClibCPV::H, ClibCPV::T};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 38;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr39)
{
  ClibSeqAttr seq_attr{true, ClibCPV::H, ClibCPV::T};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 39;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr40)
{
  ClibSeqAttr seq_attr{false, ClibCPV::N, ClibCPV::T};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 40;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr41)
{
  ClibSeqAttr seq_attr{true, ClibCPV::N, ClibCPV::T};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 41;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr42)
{
  ClibSeqAttr seq_attr{false, ClibCPV::T, ClibCPV::T};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 42;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr43)
{
  ClibSeqAttr seq_attr{true, ClibCPV::T, ClibCPV::T};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 43;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr44)
{
  ClibSeqAttr seq_attr{false, ClibCPV::X, ClibCPV::T};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 44;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr45)
{
  ClibSeqAttr seq_attr{true, ClibCPV::X, ClibCPV::T};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv2() );
  SizeType index = 45;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr46)
{
  ClibSeqAttr seq_attr{false, ClibCPV::L, ClibCPV::X};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 46;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr47)
{
  ClibSeqAttr seq_attr{true, ClibCPV::L, ClibCPV::X};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::L, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 47;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr48)
{
  ClibSeqAttr seq_attr{false, ClibCPV::H, ClibCPV::X};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 48;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr49)
{
  ClibSeqAttr seq_attr{true, ClibCPV::H, ClibCPV::X};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::H, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 49;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr50)
{
  ClibSeqAttr seq_attr{false, ClibCPV::N, ClibCPV::X};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 50;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr51)
{
  ClibSeqAttr seq_attr{true, ClibCPV::N, ClibCPV::X};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::N, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 51;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr52)
{
  ClibSeqAttr seq_attr{false, ClibCPV::T, ClibCPV::X};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 52;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr53)
{
  ClibSeqAttr seq_attr{true, ClibCPV::T, ClibCPV::X};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::T, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 53;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr54)
{
  ClibSeqAttr seq_attr{false, ClibCPV::X, ClibCPV::X};

  EXPECT_FALSE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 54;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

TEST(ClibSeqAttrTest, constr55)
{
  ClibSeqAttr seq_attr{true, ClibCPV::X, ClibCPV::X};

  EXPECT_TRUE( seq_attr.has_slave_clock() );
  EXPECT_TRUE( seq_attr.has_clear() );
  EXPECT_TRUE( seq_attr.has_preset() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv1() );
  EXPECT_EQ( ClibCPV::X, seq_attr.cpv2() );
  SizeType index = 55;
  EXPECT_EQ( index, seq_attr.index() );

  ClibSeqAttr seq_attr1{index};
  EXPECT_EQ( seq_attr1, seq_attr );
}

END_NAMESPACE_YM_CLIB
