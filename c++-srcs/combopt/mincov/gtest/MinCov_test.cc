
/// @file MinCov_test.cc
/// @brief MinCov_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/MinCov.h"
#include "ym/McPerturbator.h"
#include "ym/JsonValue.h"
#include <random>


BEGIN_NAMESPACE_YM_MINCOV

TEST(MinCovTest, greedy)
{
  SizeType nr = 3;
  SizeType nc = 4;
  auto elem_list = std::vector<MinCov::ElemType>{
    {0, 0}, {0, 2},
    {1, 1}, {1, 2},
    {2, 0}, {2, 3}
  };

  MinCov mincov(nr, nc);
  for ( auto& elem: elem_list ) {
    mincov.insert_elem(elem.row_pos, elem.col_pos);
  }

  EXPECT_EQ( nr, mincov.row_size() );
  EXPECT_EQ( nc, mincov.col_size() );

  auto option = JsonValue::object();
  option.add("algorithm", JsonValue("greedy"));

  std::vector<SizeType> solution;
  auto ns = mincov.solve(solution, option);

  EXPECT_EQ( 2, ns );
}

TEST(MinCovTest, exact)
{
  SizeType nr = 3;
  SizeType nc = 4;
  auto elem_list = std::vector<MinCov::ElemType>{
    {0, 0}, {0, 2},
    {1, 1}, {1, 2},
    {2, 0}, {2, 3}
  };

  MinCov mincov(nr, nc);
  for ( auto& elem: elem_list ) {
    mincov.insert_elem(elem.row_pos, elem.col_pos);
  }

  EXPECT_EQ( nr, mincov.row_size() );
  EXPECT_EQ( nc, mincov.col_size() );

  auto option = JsonValue::object();
  option.add("algorithm", JsonValue("exact"));

  std::vector<SizeType> solution;
  auto ns = mincov.solve(solution, option);

  EXPECT_EQ( 2, ns );
}

TEST(MinCovTest, perturb)
{
  SizeType nr = 3;
  SizeType nc = 4;
  auto elem_list = std::vector<MinCov::ElemType>{
    {0, 0}, {0, 2},
    {1, 1}, {1, 2},
    {2, 0}, {2, 3}
  };

  MinCov mincov(nr, nc);
  for ( auto& elem: elem_list ) {
    mincov.insert_elem(elem.row_pos, elem.col_pos);
  }

  EXPECT_EQ( nr, mincov.row_size() );
  EXPECT_EQ( nc, mincov.col_size() );

  auto solution1 = std::vector<SizeType>{0, 1};

  ASSERT_TRUE( mincov.verify_solution(solution1) );

  McPerturbator perturb(mincov);

  auto solution2 = perturb(solution1);
  EXPECT_TRUE( mincov.verify_solution(solution2) );
}

END_NAMESPACE_YM_MINCOV
