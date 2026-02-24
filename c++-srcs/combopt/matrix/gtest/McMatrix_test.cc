
/// @file McMatrix_test.cc
/// @brief McMatrix_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "mincov/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

TEST(McMatrixTest, constr1)
{
  MinCov mincov;
  auto mtx = McMatrix(mincov);

  EXPECT_EQ( 0, mtx.row_size() );
  EXPECT_EQ( 0, mtx.active_row_num() );
  EXPECT_EQ( 0, mtx.col_size() );
  EXPECT_EQ( 0, mtx.active_col_num() );
  auto& cc_array = mtx.col_cost_array();
  EXPECT_EQ( 0, cc_array.size() );
}

TEST(McMatrixTest, constr3)
{
  SizeType nr = 3;
  SizeType nc = 4;
  auto elem_list = std::vector<MinCov::ElemType>{
    {0, 0}, {0, 2},
    {1, 1}, {1, 2},
    {2, 0}, {2, 3}
  };
  MinCov mincov;
  for ( auto& elem: elem_list) {
    mincov.insert_elem(elem.row_pos, elem.col_pos);
  }
  auto mtx = McMatrix(mincov);

  EXPECT_EQ( nr, mtx.row_size() );
  EXPECT_EQ( nc, mtx.col_size() );

  EXPECT_EQ( nr, mtx.active_row_num() );
  EXPECT_EQ( nc, mtx.active_col_num() );

  auto row0_list = mtx.row_list(0);
  {
    auto it = row0_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 2, *it );
  }
  auto row1_list = mtx.row_list(1);
  {
    auto it = row1_list.begin();
    EXPECT_EQ( 1, *it );
    ++ it;
    EXPECT_EQ( 2, *it );
  }
  auto row2_list = mtx.row_list(2);
  {
    auto it = row2_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 3, *it );
  }

  auto col0_list = mtx.col_list(0);
  {
    auto it = col0_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 2, *it );
  }
  auto col1_list = mtx.col_list(1);
  {
    auto it = col1_list.begin();
    EXPECT_EQ( 1, *it );
  }
  auto col2_list = mtx.col_list(2);
  {
    auto it = col2_list.begin();
    EXPECT_EQ( 0, *it );
    ++ it;
    EXPECT_EQ( 1, *it );
  }
  auto col3_list = mtx.col_list(3);
  {
    auto it = col3_list.begin();
    EXPECT_EQ( 2, *it );
  }
}

END_NAMESPACE_YM_MINCOV
