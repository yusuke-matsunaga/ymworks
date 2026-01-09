
/// @file FraigMgrImpl_test.cc
/// @brief FraigMgrImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "FraigMgrImpl.h"


BEGIN_NAMESPACE_YM_FRAIG

TEST(FraigMgrImplTest, constr)
{
  FraigMgrImpl mgr(64);

  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.node_num() );
}

TEST(FraigMgrImplTest, new_input)
{
  FraigMgrImpl mgr(64);

  auto e = mgr.new_input();
  ASSERT_EQ( 1, mgr.input_num() );

  auto node = mgr.input_node(0);
  EXPECT_EQ( node, mgr.literal2node(e) );
}

TEST(FraigMgrImplTest, literal2node)
{
  FraigMgrImpl mgr(64);

  auto i1 = mgr.new_input();
  auto inode = mgr.literal2node(i1);
  auto i2 = FraigMgrImpl::node2literal(inode);
  EXPECT_EQ( i1, i2 );
}

TEST(FraigMgrImplTest, literal2node_bad)
{
  FraigMgrImpl mgr(64);

  auto zero = FraigLiteral::zero();
  EXPECT_THROW( mgr.literal2node(zero),
		std::invalid_argument );

  auto one = FraigLiteral::one();
  EXPECT_THROW( mgr.literal2node(one),
		std::invalid_argument );
}

TEST(FraigMgrImplTest, new_and)
{
  FraigMgrImpl mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();
  auto e = mgr.new_and(i1, i2);
  EXPECT_EQ( 2, mgr.input_num() );
  EXPECT_EQ( 3, mgr.node_num() );

  auto node = mgr.literal2node(e);
  EXPECT_EQ( mgr.literal2node(i1), node->fanin0() );
  EXPECT_EQ( mgr.literal2node(i2), node->fanin1() );
  EXPECT_FALSE( node->fanin0_inv() );
  EXPECT_FALSE( node->fanin1_inv() );
}

TEST(FraigMgrImplTest, check_equiv1)
{
  FraigMgrImpl mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();

  auto s1 = mgr.new_and(i1, ~i2);
  auto s2 = mgr.new_and(~i1, i2);
  auto e1 = ~mgr.new_and(~s1, ~s2);

  auto t1 = ~mgr.new_and(i1, i2);
  auto t2 = ~mgr.new_and(i1, t1);
  auto t3 = ~mgr.new_and(i2, t1);
  auto e2 = ~mgr.new_and(t2, t3);

  auto res = mgr.check_equiv(e1, e2);
  EXPECT_EQ( SatBool3::True, res );

  auto e3 = mgr.new_and(i1, i2);
  auto res2 = mgr.check_equiv(e1, e3);
  EXPECT_EQ( SatBool3::False, res2 );
}

TEST(FraigMgrImplTest, cofactor1)
{
  FraigMgrImpl mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();
  auto i3 = mgr.new_input();

  auto e1 = mgr.new_and(i1, i2);
  auto e2 = mgr.new_and(~i1, i3);
  auto e3 = ~mgr.new_and(~e1, ~e2);

  auto inode1 = mgr.literal2node(i1);
  auto e4 = mgr.cofactor(e3, inode1->input_id(), false);
  auto res1 = mgr.check_equiv(i3, e4);
  EXPECT_EQ( SatBool3::True, res1 );

  auto e5 = mgr.cofactor(e3, inode1->input_id(), true);
  auto res2 = mgr.check_equiv(i2, e5);
  EXPECT_EQ( SatBool3::True, res2 );
}

END_NAMESPACE_YM_FRAIG
