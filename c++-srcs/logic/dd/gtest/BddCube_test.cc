
/// @file BddCube_test.cc
/// @brief BddCube_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BddCube.h"
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM

TEST(BddCubeTest, empty_constr)
{
  auto cube = BddCube{}; // 空キューブ

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_TRUE( cube.is_universal_cube() );
  EXPECT_EQ( 0, cube.size() );
  auto lit_list = cube.lit_list();
  EXPECT_EQ( 0, lit_list.size() );
}

TEST(BddCubeTest, constr1)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto cube = BddCube({lit0, lit1});

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 2, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 2, lit_list.size() );
  EXPECT_EQ( lit0, lit_list[0] );
  EXPECT_EQ( lit1, lit_list[1] );
}

TEST(BddCubeTest, constr2)
{ // BddLit からの変換
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto cube = BddCube(lit0);

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 1, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 1, lit_list.size() );
  EXPECT_EQ( lit0, lit_list[0] );
}

TEST(BddCubeTest, universal_cube)
{
  auto cube = BddCube::universal_cube();

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_TRUE( cube.is_universal_cube() );
  EXPECT_EQ( 0, cube.size() );
  auto lit_list = cube.lit_list();
  EXPECT_EQ( 0, lit_list.size() );
}

TEST(BddCubeTest, invalid)
{
  auto cube = BddCube::invalid();

  EXPECT_TRUE( cube.is_invalid() );
}

TEST(BddCubeTest, and_op1)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto cube1 = BddCube({lit0, lit1});
  auto cube = cube1 & lit2;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 3, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 3, lit_list.size() );
  EXPECT_EQ( lit0, lit_list[0] );
  EXPECT_EQ( lit1, lit_list[1] );
  EXPECT_EQ( lit2, lit_list[2] );
}

TEST(BddCubeTest, and_op2)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto cube1 = BddCube({lit0, lit1});
  auto cube = lit2 & cube1;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 3, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 3, lit_list.size() );
  EXPECT_EQ( lit0, lit_list[0] );
  EXPECT_EQ( lit1, lit_list[1] );
  EXPECT_EQ( lit2, lit_list[2] );
}

TEST(BddCubeTest, and_op3)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube({lit0, lit1});
  auto cube2 = BddCube({lit2, lit3});
  auto cube = cube1 & cube2;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 4, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 4, lit_list.size() );
  EXPECT_EQ( lit0, lit_list[0] );
  EXPECT_EQ( lit1, lit_list[1] );
  EXPECT_EQ( lit2, lit_list[2] );
  EXPECT_EQ( lit3, lit_list[3] );
}

TEST(BddCubeTest, and_op4)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto cube1 = BddCube::universal_cube();
  auto cube = cube1 & lit2;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 1, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 1, lit_list.size() );
  EXPECT_EQ( lit2, lit_list[0] );
}

TEST(BddCubeTest, and_op5)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto cube1 = BddCube::universal_cube();
  auto cube = lit2 & cube1;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 1, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 1, lit_list.size() );
  EXPECT_EQ( lit2, lit_list[0] );
}

TEST(BddCubeTest, and_op6)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube::universal_cube();
  auto cube2 = BddCube({lit2, lit3});
  auto cube = cube1 & cube2;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 2, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 2, lit_list.size() );
  EXPECT_EQ( lit2, lit_list[0] );
  EXPECT_EQ( lit3, lit_list[1] );
}

TEST(BddCubeTest, and_op7)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube({lit0, lit1});
  auto cube2 = BddCube::universal_cube();
  auto cube = cube1 & cube2;

  EXPECT_FALSE( cube.is_invalid() );
  EXPECT_FALSE( cube.is_universal_cube() );
  EXPECT_EQ( 2, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 2, lit_list.size() );
  EXPECT_EQ( lit0, lit_list[0] );
  EXPECT_EQ( lit1, lit_list[1] );
}

TEST(BddCubeTest, and_op_null1)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(0, true);
  auto cube1 = BddCube({lit0, lit1});
  auto cube = cube1 & lit2;

  EXPECT_TRUE( cube.is_invalid() );
}

TEST(BddCubeTest, and_op_null2)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(0, true);
  auto cube1 = BddCube({lit0, lit1});
  auto cube = lit2 & cube1;

  EXPECT_TRUE( cube.is_invalid() );
}

TEST(BddCubeTest, and_op_null3)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(0, true);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube({lit0, lit1});
  auto cube2 = BddCube({lit2, lit3});
  auto cube = cube1 & cube2;

  EXPECT_TRUE( cube.is_invalid() );
}

TEST(BddCubeTest, and_op_invalid1)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto cube1 = BddCube::invalid();
  EXPECT_THROW( cube1 & lit2,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid2)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = BddCube::invalid();
  auto cube1 = BddCube({lit0, lit1});
  EXPECT_THROW( cube1 & lit2,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid3)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto cube1 = BddCube::invalid();
  EXPECT_THROW( lit2 & cube1,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid4)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = BddCube::invalid();
  auto cube1 = BddCube({lit0, lit1});
  EXPECT_THROW( lit2 & cube1,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid5)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube::invalid();
  auto cube2 = BddCube({lit2, lit3});
  EXPECT_THROW( cube1 & cube2,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid6)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube({lit0, lit1});
  auto cube2 = BddCube::invalid();
  EXPECT_THROW( cube1 & cube2,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid7)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = BddLit::invalid();
  auto cube1 = BddCube::universal_cube();
  EXPECT_THROW( cube1 & lit2,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid8)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = BddLit::invalid();
  auto cube1 = BddCube::universal_cube();
  EXPECT_THROW( lit2 & cube1,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid9)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube::universal_cube();
  auto cube2 = BddCube::invalid();
  EXPECT_THROW( cube1 & cube2,
		std::invalid_argument );
}

TEST(BddCubeTest, and_op_invalid10)
{
  BddMgr mgr;
  auto lit0 = mgr.literal(0, false);
  auto lit1 = mgr.literal(1, true);
  auto lit2 = mgr.literal(2, false);
  auto lit3 = mgr.literal(3, true);
  auto cube1 = BddCube::invalid();
  auto cube2 = BddCube::universal_cube();
  EXPECT_THROW( cube1 & cube2,
		std::invalid_argument );
}

END_NAMESPACE_YM
