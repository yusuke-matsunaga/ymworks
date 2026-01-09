
/// @file cov2aig_test.cc
/// @brief cov2aig_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

TEST(Cov2AigTest, empty_cube)
{
  auto cube = SopCube();

  AigMgr mgr;

  auto h = mgr.from_cube(cube);

  EXPECT_TRUE( h.is_one() );
}

TEST(Cov2AigTest, cube1)
{
  auto lit0 = Literal{0, false};
  auto lit1 = Literal{1, false};
  auto lit_list = std::vector<Literal>{lit0, ~lit1};
  auto cube = SopCube{2, lit_list};

  AigMgr mgr;

  auto h = mgr.from_cube(cube);

  mgr.new_output(h);

  EXPECT_EQ( 1, mgr.and_num() );

  mgr.print(std::cout);
}

TEST(Cov2AigTest, empty_cover)
{
  auto cover = SopCover{};

  AigMgr mgr;

  auto h = mgr.from_cover(cover);

  EXPECT_TRUE( h.is_zero() );
}

TEST(Cov2AigTest, one)
{
  AigMgr mgr;

  auto cube = SopCube{};
  auto cover = SopCover(0, {cube});

  auto h = mgr.from_cover(cover);

  EXPECT_TRUE( h.is_one() );
}

TEST(Cov2AigTest, cov1)
{
  auto lit0 = Literal{0, false};
  auto lit1 = Literal{1, false};
  auto lit2 = Literal{2, false};
  auto lit3 = Literal{3, false};
  auto cube_list = std::vector<std::vector<Literal>>{
    {lit0, lit1},
    {lit2, lit3}
  };
  SizeType nv = 4;
  auto cover = SopCover{nv, cube_list};

  AigMgr mgr;

  auto h = mgr.from_cover(cover);

  mgr.new_output(h);

  EXPECT_EQ( 3, mgr.and_num() );

  mgr.print(std::cout);
}

TEST(Cov2AigTest, cov2)
{
  auto lit0 = Literal{0, false};
  auto lit1 = Literal{1, false};
  auto lit2 = Literal{2, false};
  auto lit3 = Literal{3, false};
  auto cube_list = std::vector<std::vector<Literal>>{
    {~lit0, ~lit1},
    {~lit2, ~lit3}
  };
  SizeType nv = 4;
  auto cover = SopCover{nv, cube_list};

  AigMgr mgr;

  auto h = mgr.from_cover(cover);

  mgr.new_output(h);

  EXPECT_EQ( 3, mgr.and_num() );

  mgr.print(std::cout);
}

END_NAMESPACE_YM
