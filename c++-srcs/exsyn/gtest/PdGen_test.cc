
/// @file PdGen_test.cc
/// @brief PdGen_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "FenceGen.h"
#include "PdGen.h"


BEGIN_NAMESPACE_YM_EXSYN

TEST(PdGenTest, test222)
{
  FenceGen fg;

  auto fence_list = fg(2, 2, 2);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence = fence_list.front();

  PdGen pg;

  auto pd_list = pg(fence, 2);

  ASSERT_EQ( 1, pd_list.size() );

  auto& pd = pd_list.front();

  ASSERT_EQ( 2, pd.op_num() );
  auto& op1 = pd.opr(0);
  ASSERT_EQ( 2, op1.size() );
  EXPECT_EQ( -1, op1[0] );
  EXPECT_EQ( -1, op1[1] );
  auto& op2 = pd.opr(1);
  ASSERT_EQ( 2, op2.size() );
  EXPECT_EQ( -1, op2[0] );
  EXPECT_EQ( 0, op2[1] );
}

TEST(PdGenTest, test223)
{
  FenceGen fg;

  auto fence_list = fg(2, 2, 3);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence = fence_list.front();

  PdGen pg;

  auto pd_list = pg(fence, 3);

  ASSERT_EQ( 4, pd_list.size() );

  auto& pd1 = pd_list[0];
  ASSERT_EQ( 2, pd1.op_num() );
  auto& op1 = pd1.opr(0);
  ASSERT_EQ( 2, op1.size() );
  EXPECT_EQ( -1, op1[0] );
  EXPECT_EQ( -1, op1[1] );
  auto& op2 = pd1.opr(1);
  ASSERT_EQ( 2, op2.size() );
  EXPECT_EQ( -1, op2[0] );
  EXPECT_EQ( 0, op2[1] );

  auto& pd2 = pd_list[1];
  ASSERT_EQ( 2, pd1.op_num() );
  auto& op3 = pd2.opr(0);
  ASSERT_EQ( 2, op3.size() );
  EXPECT_EQ( -1, op3[0] );
  EXPECT_EQ( -1, op3[1] );
  auto& op4 = pd2.opr(1);
  ASSERT_EQ( 3, op4.size() );
  EXPECT_EQ( -1, op4[0] );
  EXPECT_EQ( -1, op4[1] );
  EXPECT_EQ( 0, op4[2] );

  auto& pd3 = pd_list[2];
  ASSERT_EQ( 2, pd3.op_num() );
  auto& op5 = pd3.opr(0);
  ASSERT_EQ( 3, op5.size() );
  EXPECT_EQ( -1, op5[0] );
  EXPECT_EQ( -1, op5[1] );
  EXPECT_EQ( -1, op5[2] );
  auto& op6 = pd3.opr(1);
  ASSERT_EQ( 2, op6.size() );
  EXPECT_EQ( -1, op6[0] );
  EXPECT_EQ( 0, op6[1] );

  auto& pd4 = pd_list[3];
  ASSERT_EQ( 2, pd3.op_num() );
  auto& op7 = pd4.opr(0);
  ASSERT_EQ( 3, op7.size() );
  EXPECT_EQ( -1, op7[0] );
  EXPECT_EQ( -1, op7[1] );
  EXPECT_EQ( -1, op7[2] );
  auto& op8 = pd4.opr(1);
  ASSERT_EQ( 3, op8.size() );
  EXPECT_EQ( -1, op8[0] );
  EXPECT_EQ( -1, op8[1] );
  EXPECT_EQ( 0, op8[2] );
}

TEST(PdGenTest, test32)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(3, 2, 2, 1);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence = fence_list.front();
  EXPECT_EQ( "(2, 1)", fence.to_string() );

  auto pd_list = pg(fence);

  ASSERT_EQ( 1, pd_list.size() );

  auto& pd = pd_list[0];
  EXPECT_EQ( "((-1, -1), (-1, -1), (0, 1))", pd.to_string() );
}

TEST(PdGenTest, test33)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(3, 3, 2, 1);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence = fence_list.front();
  EXPECT_EQ( "(1, 1, 1)", fence.to_string() );

  auto pd_list = pg(fence);

  ASSERT_EQ( 1, pd_list.size() );

  auto& pd = pd_list[0];
  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1))", pd.to_string() );
}

TEST(PdGenTest, test43)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(4, 3, 2, 1);

  ASSERT_EQ( 2, fence_list.size() );

  auto& fence0 = fence_list[0];
  EXPECT_EQ( "(1, 2, 1)", fence0.to_string() );

  auto pd_list0 = pg(fence0);

  ASSERT_EQ( 1, pd_list0.size() );

  auto& pd0_0 = pd_list0[0];
  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (1, 2))", pd0_0.to_string() );

  auto& fence1 = fence_list[1];
  EXPECT_EQ( "(2, 1, 1)", fence1.to_string() );

  auto pd_list1 = pg(fence1);

  ASSERT_EQ( 2, pd_list1.size() );

  auto& pd1_0 = pd_list1[0];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (1, 2))", pd1_0.to_string() );

  auto& pd1_1 = pd_list1[1];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (0, 2))", pd1_1.to_string() );
}

TEST(PdGenTest, test44)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(4, 4, 2, 1);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence0 = fence_list[0];
  EXPECT_EQ( "(1, 1, 1, 1)", fence0.to_string() );

  auto pd_list = pg(fence0);

  ASSERT_EQ( 2, pd_list.size() );

  auto& pd0 = pd_list[0];
  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (0, 2))", pd0.to_string() );

  auto& pd1 = pd_list[1];
  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 2))", pd1.to_string() );
}

TEST(PdGenTest, test53)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(5, 3, 2, 1);

  ASSERT_EQ( 2, fence_list.size() );

  auto& fence0 = fence_list[0];
  EXPECT_EQ( "(2, 2, 1)", fence0.to_string() );

  auto pd_list0 = pg(fence0);

  ASSERT_EQ( 4, pd_list0.size() );

  auto& pd0_0 = pd_list0[0];
  EXPECT_EQ( "((-1, -1), (-1, -1), (0, 1), (0, 1), (2, 3))", pd0_0.to_string() );

  auto& pd0_1 = pd_list0[1];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (0, 1), (2, 3))", pd0_1.to_string() );

  auto& pd0_2 = pd_list0[2];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (-1, 1), (2, 3))", pd0_2.to_string() );

  auto& pd0_3 = pd_list0[3];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (0, 1), (2, 3))", pd0_3.to_string() );

  auto& fence1 = fence_list[1];
  EXPECT_EQ( "(3, 1, 1)", fence1.to_string() );

  auto pd_list1 = pg(fence1);

  ASSERT_EQ( 2, pd_list1.size() );

  auto& pd1_0 = pd_list1[0];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, -1), (0, 1), (2, 3))", pd1_0.to_string() );

  auto& pd1_1 = pd_list1[1];
  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, -1), (0, 2), (1, 3))", pd1_1.to_string() );

}

TEST(PdGenTest, test54)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(5, 4, 2, 1);

  ASSERT_EQ( 3, fence_list.size() );

  auto& fence0 = fence_list[0];

  ASSERT_EQ( "(1, 1, 2, 1)", fence0.to_string() );

  auto pd_list0 = pg(fence0);

  ASSERT_EQ( 1, pd_list0.size() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 1), (2, 3))",
	     pd_list0[0].to_string() );

  auto& fence1 = fence_list[1];

  ASSERT_EQ( "(1, 2, 1, 1)", fence1.to_string() );

  auto pd_list1 = pg(fence1);

  ASSERT_EQ( 2, pd_list1.size() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 1), (2, 3))",
	     pd_list1[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 2), (1, 3))",
	     pd_list1[1].to_string() );

  auto& fence2 = fence_list[2];

  ASSERT_EQ( "(2, 1, 1, 1)", fence2.to_string() );

  auto pd_list2 = pg(fence2);

  ASSERT_EQ( 3, pd_list2.size() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (-1, 2), (1, 3))",
	     pd_list2[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (0, 2), (1, 3))",
	     pd_list2[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (-1, 2), (0, 3))",
	     pd_list2[2].to_string() );
}

TEST(PdGenTest, test55)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(5, 5, 2, 1);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence0 = fence_list[0];

  ASSERT_EQ( "(1, 1, 1, 1, 1)", fence0.to_string() );

  auto pd_list = pg(fence0);

  ASSERT_EQ( 4, pd_list.size() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (0, 2), (1, 3))",
	     pd_list[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 2), (0, 3))",
	     pd_list[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 2), (1, 3))",
	     pd_list[2].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 2), (-1, 3))",
	     pd_list[3].to_string() );
}

TEST(PdGenTest, test4231)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(4, 2, 3, 1);

  ASSERT_EQ( 1, fence_list.size() );

  auto& fence0 = fence_list[0];

  ASSERT_EQ( "(3, 1)", fence0.to_string() );

  auto pd_list = pg(fence0, 3);

  ASSERT_EQ( 4, pd_list.size() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, -1), (0, 1, 2))",
	     pd_list[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, -1, -1), (0, 1, 2))",
	     pd_list[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1, -1), (-1, -1, -1), (0, 1, 2))",
	     pd_list[2].to_string() );

  EXPECT_EQ( "((-1, -1, -1), (-1, -1, -1), (-1, -1, -1), (0, 1, 2))",
	     pd_list[3].to_string() );
}

TEST(PdGenTest, test65)
{
  FenceGen fg;
  PdGen pg;

  auto fence_list = fg(6, 5, 2, 1);

  ASSERT_EQ( 4, fence_list.size() );

  auto& fence0 = fence_list[0];

  ASSERT_EQ( "(1, 1, 1, 2, 1)", fence0.to_string() );

  auto pd_list0 = pg(fence0);

  ASSERT_EQ( 3, pd_list0.size() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (0, 2), (0, 2), (3, 4))",
	     pd_list0[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 2), (0, 2), (3, 4))",
	     pd_list0[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 2), (-1, 2), (3, 4))",
	     pd_list0[2].to_string() );

  auto& fence1 = fence_list[1];

  ASSERT_EQ( "(1, 1, 2, 1, 1)", fence1.to_string() );

  auto pd_list1 = pg(fence1);

  ASSERT_EQ( 4, pd_list1.size() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 1), (0, 2), (3, 4))",
	     pd_list1[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 1), (0, 3), (2, 4))",
	     pd_list1[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 1), (-1, 2), (3, 4))",
	     pd_list1[2].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 1), (-1, 1), (-1, 3), (2, 4))",
	     pd_list1[3].to_string() );

  auto &fence2 = fence_list[2];

  ASSERT_EQ( "(1, 2, 1, 1, 1)", fence2.to_string() );

  auto pd_list2 = pg(fence2);

  ASSERT_EQ( 5, pd_list2.size() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 1), (0, 3), (2, 4))",
	     pd_list2[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 1), (-1, 3), (2, 4))",
	     pd_list2[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 2), (0, 3), (1, 4))",
	     pd_list2[2].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 2), (1, 3), (2, 4))",
	     pd_list2[3].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, 0), (-1, 0), (-1, 2), (-1, 3), (1, 4))",
	     pd_list2[4].to_string() );

  auto& fence3 = fence_list[3];

  ASSERT_EQ( "(2, 1, 1, 1, 1)", fence3.to_string() );

  auto pd_list3 = pg(fence3);

  ASSERT_EQ( 7, pd_list3.size() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (-1, 2), (0, 3), (1, 4))",
	     pd_list3[0].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (-1, 2), (1, 3), (2, 4))",
	     pd_list3[1].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 0), (-1, 2), (-1, 3), (1, 4))",
	     pd_list3[2].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (0, 2), (1, 3), (2, 4))",
	     pd_list3[3].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (-1, 2), (0, 3), (1, 4))",
	     pd_list3[4].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (-1, 2), (0, 3), (2, 4))",
	     pd_list3[5].to_string() );

  EXPECT_EQ( "((-1, -1), (-1, -1), (-1, 1), (-1, 2), (-1, 3), (0, 4))",
	     pd_list3[6].to_string() );
}

END_NAMESPACE_YM_EXSYN
