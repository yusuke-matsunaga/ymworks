
/// @file FenceGen_test.cc
/// @brief FenceGen_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "FenceGen.h"


BEGIN_NAMESPACE_YM_EXSYN

TEST(FenceGenTest, test22)
{
  FenceGen fg;

  auto ans_list = fg(2, 2, 2, 1);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 2, ans.max_level() );
  ASSERT_EQ( 2, ans.total_num() );
  EXPECT_EQ( 1, ans.num(0) );
  EXPECT_EQ( 1, ans.num(1) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 1, ans.top(1) );
  EXPECT_EQ( 2, ans.top(2) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 1, ans.level(1) );
}

TEST(FenceGenTest, test32)
{
  FenceGen fg;

  auto ans_list = fg(3, 2, 2, 1);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 2, ans.max_level() );
  ASSERT_EQ( 3, ans.total_num() );
  EXPECT_EQ( 2, ans.num(0) );
  EXPECT_EQ( 1, ans.num(1) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 2, ans.top(1) );
  EXPECT_EQ( 3, ans.top(2) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 0, ans.level(1) );
  EXPECT_EQ( 1, ans.level(2) );
}

TEST(FenceGenTest, test33)
{
  FenceGen fg;

  auto ans_list = fg(3, 3, 2, 1);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 3, ans.max_level() );
  ASSERT_EQ( 3, ans.total_num() );
  EXPECT_EQ( 1, ans.num(0) );
  EXPECT_EQ( 1, ans.num(1) );
  EXPECT_EQ( 1, ans.num(2) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 1, ans.top(1) );
  EXPECT_EQ( 2, ans.top(2) );
  EXPECT_EQ( 3, ans.top(3) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 1, ans.level(1) );
  EXPECT_EQ( 2, ans.level(2) );
}

TEST(FenceGenTest, test43)
{
  FenceGen fg;

  auto ans_list = fg(4, 3, 2, 1);

  ASSERT_EQ( 2, ans_list.size() );

  auto& ans0 = ans_list[0];
  ASSERT_EQ( 3, ans0.max_level() );
  ASSERT_EQ( 4, ans0.total_num() );
  EXPECT_EQ( 1, ans0.num(0) );
  EXPECT_EQ( 2, ans0.num(1) );
  EXPECT_EQ( 1, ans0.num(2) );
  EXPECT_EQ( 0, ans0.top(0) );
  EXPECT_EQ( 1, ans0.top(1) );
  EXPECT_EQ( 3, ans0.top(2) );
  EXPECT_EQ( 4, ans0.top(3) );
  EXPECT_EQ( 0, ans0.level(0) );
  EXPECT_EQ( 1, ans0.level(1) );
  EXPECT_EQ( 1, ans0.level(2) );
  EXPECT_EQ( 2, ans0.level(3) );

  auto& ans1 = ans_list[1];
  ASSERT_EQ( 3, ans1.max_level() );
  ASSERT_EQ( 4, ans1.total_num() );
  EXPECT_EQ( 2, ans1.num(0) );
  EXPECT_EQ( 1, ans1.num(1) );
  EXPECT_EQ( 1, ans1.num(2) );
  EXPECT_EQ( 0, ans1.top(0) );
  EXPECT_EQ( 2, ans1.top(1) );
  EXPECT_EQ( 3, ans1.top(2) );
  EXPECT_EQ( 4, ans1.top(3) );
  EXPECT_EQ( 0, ans1.level(0) );
  EXPECT_EQ( 0, ans1.level(1) );
  EXPECT_EQ( 1, ans1.level(2) );
  EXPECT_EQ( 2, ans1.level(3) );
}

TEST(FenceGenTest, test44)
{
  FenceGen fg;

  auto ans_list = fg(4, 4, 2, 1);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 4, ans.max_level() );
  ASSERT_EQ( 4, ans.total_num() );
  EXPECT_EQ( 1, ans.num(0) );
  EXPECT_EQ( 1, ans.num(1) );
  EXPECT_EQ( 1, ans.num(2) );
  EXPECT_EQ( 1, ans.num(3) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 1, ans.top(1) );
  EXPECT_EQ( 2, ans.top(2) );
  EXPECT_EQ( 3, ans.top(3) );
  EXPECT_EQ( 4, ans.top(4) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 1, ans.level(1) );
  EXPECT_EQ( 2, ans.level(2) );
  EXPECT_EQ( 3, ans.level(3) );
}

TEST(FenceGenTest, test53)
{
  FenceGen fg;

  auto ans_list = fg(5, 3, 2, 1);

  ASSERT_EQ( 2, ans_list.size() );

  auto& ans0 = ans_list[0];
  ASSERT_EQ( 3, ans0.max_level() );
  ASSERT_EQ( 5, ans0.total_num() );
  EXPECT_EQ( 2, ans0.num(0) );
  EXPECT_EQ( 2, ans0.num(1) );
  EXPECT_EQ( 1, ans0.num(2) );
  EXPECT_EQ( 0, ans0.top(0) );
  EXPECT_EQ( 2, ans0.top(1) );
  EXPECT_EQ( 4, ans0.top(2) );
  EXPECT_EQ( 5, ans0.top(3) );
  EXPECT_EQ( 0, ans0.level(0) );
  EXPECT_EQ( 0, ans0.level(1) );
  EXPECT_EQ( 1, ans0.level(2) );
  EXPECT_EQ( 1, ans0.level(3) );
  EXPECT_EQ( 2, ans0.level(4) );

  auto& ans1 = ans_list[1];
  ASSERT_EQ( 3, ans1.max_level() );
  ASSERT_EQ( 5, ans1.total_num() );
  EXPECT_EQ( 3, ans1.num(0) );
  EXPECT_EQ( 1, ans1.num(1) );
  EXPECT_EQ( 1, ans1.num(2) );
  EXPECT_EQ( 0, ans1.top(0) );
  EXPECT_EQ( 3, ans1.top(1) );
  EXPECT_EQ( 4, ans1.top(2) );
  EXPECT_EQ( 5, ans1.top(3) );
  EXPECT_EQ( 0, ans1.level(0) );
  EXPECT_EQ( 0, ans1.level(1) );
  EXPECT_EQ( 0, ans1.level(2) );
  EXPECT_EQ( 1, ans1.level(3) );
  EXPECT_EQ( 2, ans1.level(4) );
}

TEST(FenceGenTest, test54)
{
  FenceGen fg;

  auto ans_list = fg(5, 4, 2, 1);

  ASSERT_EQ( 3, ans_list.size() );

  auto& ans0 = ans_list[0];
  ASSERT_EQ( 4, ans0.max_level() );
  ASSERT_EQ( 5, ans0.total_num() );
  EXPECT_EQ( 1, ans0.num(0) );
  EXPECT_EQ( 1, ans0.num(1) );
  EXPECT_EQ( 2, ans0.num(2) );
  EXPECT_EQ( 1, ans0.num(3) );
  EXPECT_EQ( 0, ans0.top(0) );
  EXPECT_EQ( 1, ans0.top(1) );
  EXPECT_EQ( 2, ans0.top(2) );
  EXPECT_EQ( 4, ans0.top(3) );
  EXPECT_EQ( 5, ans0.top(4) );
  EXPECT_EQ( 0, ans0.level(0) );
  EXPECT_EQ( 1, ans0.level(1) );
  EXPECT_EQ( 2, ans0.level(2) );
  EXPECT_EQ( 2, ans0.level(3) );
  EXPECT_EQ( 3, ans0.level(4) );

  auto& ans1 = ans_list[1];
  ASSERT_EQ( 4, ans1.max_level() );
  ASSERT_EQ( 5, ans1.total_num() );
  EXPECT_EQ( 1, ans1.num(0) );
  EXPECT_EQ( 2, ans1.num(1) );
  EXPECT_EQ( 1, ans1.num(2) );
  EXPECT_EQ( 1, ans1.num(3) );
  EXPECT_EQ( 0, ans1.top(0) );
  EXPECT_EQ( 1, ans1.top(1) );
  EXPECT_EQ( 3, ans1.top(2) );
  EXPECT_EQ( 4, ans1.top(3) );
  EXPECT_EQ( 5, ans1.top(4) );
  EXPECT_EQ( 0, ans1.level(0) );
  EXPECT_EQ( 1, ans1.level(1) );
  EXPECT_EQ( 1, ans1.level(2) );
  EXPECT_EQ( 2, ans1.level(3) );
  EXPECT_EQ( 3, ans1.level(4) );

  auto& ans2 = ans_list[2];
  ASSERT_EQ( 4, ans2.max_level() );
  ASSERT_EQ( 5, ans2.total_num() );
  EXPECT_EQ( 2, ans2.num(0) );
  EXPECT_EQ( 1, ans2.num(1) );
  EXPECT_EQ( 1, ans2.num(2) );
  EXPECT_EQ( 1, ans2.num(3) );
  EXPECT_EQ( 0, ans2.top(0) );
  EXPECT_EQ( 2, ans2.top(1) );
  EXPECT_EQ( 3, ans2.top(2) );
  EXPECT_EQ( 4, ans2.top(3) );
  EXPECT_EQ( 5, ans2.top(4) );
  EXPECT_EQ( 0, ans2.level(0) );
  EXPECT_EQ( 0, ans2.level(1) );
  EXPECT_EQ( 1, ans2.level(2) );
  EXPECT_EQ( 2, ans2.level(3) );
  EXPECT_EQ( 3, ans2.level(4) );
}

TEST(FenceGenTest, test55)
{
  FenceGen fg;

  auto ans_list = fg(5, 5, 2, 1);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 5, ans.max_level() );
  ASSERT_EQ( 5, ans.total_num() );
  EXPECT_EQ( 1, ans.num(0) );
  EXPECT_EQ( 1, ans.num(1) );
  EXPECT_EQ( 1, ans.num(2) );
  EXPECT_EQ( 1, ans.num(3) );
  EXPECT_EQ( 1, ans.num(4) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 1, ans.top(1) );
  EXPECT_EQ( 2, ans.top(2) );
  EXPECT_EQ( 3, ans.top(3) );
  EXPECT_EQ( 4, ans.top(4) );
  EXPECT_EQ( 5, ans.top(5) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 1, ans.level(1) );
  EXPECT_EQ( 2, ans.level(2) );
  EXPECT_EQ( 3, ans.level(3) );
  EXPECT_EQ( 4, ans.level(4) );
}

TEST(FenceGenTest, test2122)
{
  FenceGen fg;

  auto ans_list = fg(2, 1, 2, 2);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 1, ans.max_level() );
  ASSERT_EQ( 2, ans.total_num() );
  EXPECT_EQ( 2, ans.num(0) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 2, ans.top(1) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 0, ans.level(1) );
}

TEST(FenceGenTest, test4231)
{
  FenceGen fg;

  auto ans_list = fg(4, 2, 3, 1);

  ASSERT_EQ( 1, ans_list.size() );

  auto& ans = ans_list.front();
  ASSERT_EQ( 2, ans.max_level() );
  ASSERT_EQ( 4, ans.total_num() );
  EXPECT_EQ( 3, ans.num(0) );
  EXPECT_EQ( 1, ans.num(1) );
  EXPECT_EQ( 0, ans.top(0) );
  EXPECT_EQ( 3, ans.top(1) );
  EXPECT_EQ( 4, ans.top(2) );
  EXPECT_EQ( 0, ans.level(0) );
  EXPECT_EQ( 0, ans.level(1) );
  EXPECT_EQ( 0, ans.level(2) );
  EXPECT_EQ( 1, ans.level(3) );
}

TEST(FenceGenTest, test6521)
{
  FenceGen fg;

  auto fence_list = fg(6, 5, 2, 1);

  ASSERT_EQ( 4, fence_list.size() );

  auto& fence0 = fence_list[0];
  EXPECT_EQ( "(1, 1, 1, 2, 1)", fence0.to_string() );
  auto& fence1 = fence_list[1];
  EXPECT_EQ( "(1, 1, 2, 1, 1)", fence1.to_string() );
  auto& fence2 = fence_list[2];
  EXPECT_EQ( "(1, 2, 1, 1, 1)", fence2.to_string() );
  auto& fence3 = fence_list[3];
  EXPECT_EQ( "(2, 1, 1, 1, 1)", fence3.to_string() );
}

END_NAMESPACE_YM_EXSYN
