
/// @file FraigNode_test.cc
/// @brief FraigNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "FraigNode.h"


BEGIN_NAMESPACE_YM_FRAIG

TEST(FraigNodeTest, input)
{
  SizeType id = 999;
  SizeType input_id = 1234;
  auto init_pat = std::vector<std::uint64_t>{};

  auto node = FraigNode(id, input_id, init_pat);

  EXPECT_EQ( id, node.id() );

  EXPECT_TRUE( node.is_input() );
  EXPECT_EQ( input_id, node.input_id() );

  EXPECT_FALSE( node.is_and() );
}

TEST(FraigNodeTest, and)
{
  auto init_pat = std::vector<std::uint64_t>{};

  auto i0 = FraigNode(0, 0, init_pat);
  auto i1 = FraigNode(1, 1, init_pat);

  SizeType id = 9999;
  auto node = FraigNode(id, &i0, false, &i1, true);

  EXPECT_EQ( id, node.id() );

  EXPECT_FALSE( node.is_input() );

  EXPECT_TRUE( node.is_and() );

  EXPECT_EQ( &i0, node.fanin(0) );
  EXPECT_EQ( &i0, node.fanin0() );

  EXPECT_EQ( &i1, node.fanin(1) );
  EXPECT_EQ( &i1, node.fanin1() );

  EXPECT_FALSE( node.fanin_inv(0) );
  EXPECT_FALSE( node.fanin0_inv() );

  EXPECT_TRUE( node.fanin_inv(1) );
  EXPECT_TRUE( node.fanin1_inv() );
}

END_NAMESPACE_YM_FRAIG
