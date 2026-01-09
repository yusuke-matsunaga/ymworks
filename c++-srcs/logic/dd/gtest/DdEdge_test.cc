
/// @file DdEdge_test.cc
/// @brief DdEdge_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "DdEdge.h"
#include "DdNode.h"


BEGIN_NAMESPACE_YM_DD

TEST(DdEdgeTest, default)
{
  auto e = DdEdge{};

  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_TRUE( e.is_invalid() );
  EXPECT_TRUE( e.is_terminal() );
  EXPECT_FALSE( e.is_node() );

  auto e1 = DdEdge::invalid();
  EXPECT_EQ( e, e1 );

  auto e2 = DdEdge::one();
  EXPECT_FALSE( e == e2 );
}

TEST(DdEdgeTest, zero)
{
  auto e = DdEdge::zero();

  EXPECT_TRUE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
  EXPECT_TRUE( e.is_terminal() );
  EXPECT_FALSE( e.is_node() );

  EXPECT_FALSE( e.inv() );

  auto e_zero = DdEdge::zero();
  EXPECT_EQ( e, e_zero );

  auto e_one = DdEdge::one();
  EXPECT_FALSE( e == e_one );

  auto ie = ~e;
  EXPECT_TRUE( ie.is_one() );

  auto pe = e.positive_edge();
  EXPECT_TRUE( pe.is_zero() );

  auto ne = e.negative_edge();
  EXPECT_TRUE( ne.is_one() );

  auto e2 = DdEdge::zero();
  auto e3 = e2.make_positive();
  EXPECT_TRUE( e2.is_zero() );
  EXPECT_TRUE( e3.is_zero() );

  auto e4 = DdEdge::zero();
  auto e5 = e4.make_negative();
  EXPECT_TRUE( e4.is_one() );
  EXPECT_TRUE( e5.is_one() );

  auto e6 = e * false;
  EXPECT_TRUE( e6.is_zero() );

  auto e7 = e * true;
  EXPECT_TRUE( e7.is_one() );

  auto e8 = DdEdge::zero();
  auto e9 = (e8 *= false);
  EXPECT_TRUE( e8.is_zero() );
  EXPECT_TRUE( e9.is_zero() );

  auto e10 = DdEdge::zero();
  auto e11 = (e10 *= true);
  EXPECT_TRUE( e10.is_one() );
  EXPECT_TRUE( e11.is_one() );

}

TEST(DdEdgeTest, one)
{
  auto e = DdEdge::one();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_TRUE( e.is_one() );
  EXPECT_TRUE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
  EXPECT_TRUE( e.is_terminal() );
  EXPECT_FALSE( e.is_node() );

  EXPECT_TRUE( e.inv() );

  auto e_one = DdEdge::one();
  EXPECT_EQ( e, e_one );

  auto e_zero = DdEdge::zero();
  EXPECT_FALSE( e == e_zero );

  auto ie = ~e;
  EXPECT_TRUE( ie.is_zero() );

  auto pe = e.positive_edge();
  EXPECT_TRUE( pe.is_zero() );

  auto ne = e.negative_edge();
  EXPECT_TRUE( ne.is_one() );

  auto e2 = DdEdge::one();
  auto e3 = e2.make_positive();
  EXPECT_TRUE( e2.is_zero() );
  EXPECT_TRUE( e3.is_zero() );

  auto e4 = DdEdge::one();
  auto e5 = e4.make_negative();
  EXPECT_TRUE( e4.is_one() );
  EXPECT_TRUE( e5.is_one() );

  auto e6 = e * true;
  EXPECT_TRUE( e6.is_zero() );

  auto e7 = e * false;
  EXPECT_TRUE( e7.is_one() );

  auto e8 = DdEdge::one();
  auto e9 = (e8 *= true);
  EXPECT_TRUE( e8.is_zero() );
  EXPECT_TRUE( e9.is_zero() );

  auto e10 = DdEdge::one();
  auto e11 = (e10 *= false);
  EXPECT_TRUE( e10.is_one() );
  EXPECT_TRUE( e11.is_one() );
}

TEST(DdEdgeTest, invalid)
{
  auto e = DdEdge::invalid();

  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_TRUE( e.is_invalid() );
  EXPECT_TRUE( e.is_terminal() );
  EXPECT_FALSE( e.is_node() );

  auto e_invalid = DdEdge::invalid();
  EXPECT_EQ( e_invalid, e );

  auto e_zero = DdEdge::zero();
  EXPECT_FALSE( e_zero == e );

  auto e_one = DdEdge::one();
  EXPECT_FALSE( e_one == e );

  auto ie = ~e;
  EXPECT_TRUE( ie.is_invalid() );

  auto pe = e.positive_edge();
  EXPECT_TRUE( pe.is_invalid() );

  auto ne = e.negative_edge();
  EXPECT_TRUE( ne.is_invalid() );

  auto e2 = DdEdge::invalid();
  auto e3 = e2.make_positive();
  EXPECT_TRUE( e2.is_invalid() );
  EXPECT_TRUE( e3.is_invalid() );

  auto e4 = DdEdge::invalid();
  auto e5 = e4.make_negative();
  EXPECT_TRUE( e4.is_invalid() );
  EXPECT_TRUE( e5.is_invalid() );

  auto e6 = e * false;
  EXPECT_TRUE( e6.is_invalid() );

  auto e7 = e * true;
  EXPECT_TRUE( e7.is_invalid() );

  auto e8 = DdEdge::invalid();
  auto e9 = (e8 *= false);
  EXPECT_TRUE( e8.is_invalid() );
  EXPECT_TRUE( e9.is_invalid() );

  auto e10 = DdEdge::invalid();
  auto e11 = (e10 *= true);
  EXPECT_TRUE( e10.is_invalid() );
  EXPECT_TRUE( e11.is_invalid() );
}

TEST(DdEdgeTest, node)
{
  SizeType level = 10;
  auto e_zero = DdEdge::zero();
  auto e_one = DdEdge::one();
  auto node = DdNode(level, e_zero, e_one);

  auto e = DdEdge(&node, false);

  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.is_const() );
  EXPECT_FALSE( e.is_invalid() );
  EXPECT_FALSE( e.is_terminal() );
  EXPECT_TRUE( e.is_node() );

  EXPECT_EQ( &node, e.node() );
  EXPECT_FALSE( e.inv() );

  auto ie = ~e;
  EXPECT_EQ( &node, ie.node() );
  EXPECT_TRUE( ie.inv() );

  auto pe = e.positive_edge();
  EXPECT_EQ( e, pe );

  auto ne = e.negative_edge();
  EXPECT_EQ( ie, ne );

  auto e2 = DdEdge(&node, false); // = e
  auto e3 = e2.make_positive();
  EXPECT_EQ( e, e2 );
  EXPECT_EQ( e, e3 );

  auto e4 = DdEdge(&node, false); // = e
  auto e5 = e4.make_negative();
  EXPECT_EQ( ie, e4 );
  EXPECT_EQ( ie, e5 );

  auto e6 = e * false;
  EXPECT_EQ( e, e6 );

  auto e7 = e * true;
  EXPECT_EQ( ie, e7 );

  auto e8 = DdEdge(&node, false); // = e
  auto e9 = (e8 *= false);
  EXPECT_EQ( e, e8 );
  EXPECT_EQ( e, e9 );

  auto e10 = DdEdge(&node, false); // = e
  auto e11 = (e10 *= true);
  EXPECT_EQ( ie, e10 );
  EXPECT_EQ( ie, e11 );

}

END_NAMESPACE_YM_DD
