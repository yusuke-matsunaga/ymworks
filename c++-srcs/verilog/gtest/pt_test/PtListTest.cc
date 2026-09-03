
/// @file PtListTest.cc
/// @brief PtListTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "PtTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtTest, PtRangeList_init1)
{
  auto range_list = PtRangeList::new_obj();

  EXPECT_EQ( nullptr, range_list.top );
  EXPECT_EQ( nullptr, range_list.tail );
}

TEST_F(PtTest, PtRangeList_init2)
{
  auto range_list = PtRangeList::new_obj();

  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType val1 = 1;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType val2 = 2;
  auto range1 = new_range(fr1, val1, fr2, val2);

  range_list.add(range1);

  EXPECT_EQ( range1, range_list.top );
  EXPECT_EQ( range1, range_list.tail );
  EXPECT_EQ( nullptr, range1->link() );
}

TEST_F(PtTest, PtRangeList_add1)
{

  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType val1 = 1;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType val2 = 2;
  auto range1 = new_range(fr1, val1, fr2, val2);

  auto range_list = PtRangeList::new_obj(range1);

  EXPECT_EQ( range1, range_list.top );
  EXPECT_EQ( range1, range_list.tail );
  EXPECT_EQ( nullptr, range1->link() );
}

TEST_F(PtTest, PtRangeList_add2)
{
  auto range_list = PtRangeList::new_obj();

  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType val1 = 1;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType val2 = 2;
  auto range1 = new_range(fr1, val1, fr2, val2);

  auto fr3 = make_file_region(3, 3, 3, 3);
  SizeType val3 = 3;
  auto fr4 = make_file_region(4, 4, 4, 4);
  SizeType val4 = 4;
  auto range2 = new_range(fr3, val3, fr4, val4);

  auto fr5 = make_file_region(5, 5, 5, 5);
  SizeType val5 = 5;
  auto fr6 = make_file_region(6, 6, 6, 6);
  SizeType val6 = 6;
  auto range3 = new_range(fr5, val5, fr6, val6);

  range_list.add(range1);
  range_list.add(range2);
  range_list.add(range3);

  EXPECT_EQ( range1, range_list.top );
  EXPECT_EQ( range3, range_list.tail );
  EXPECT_EQ( range2, range1->link() );
  EXPECT_EQ( range3, range2->link() );
  EXPECT_EQ( nullptr, range3->link() );
}

END_NAMESPACE_YM_VERILOG
