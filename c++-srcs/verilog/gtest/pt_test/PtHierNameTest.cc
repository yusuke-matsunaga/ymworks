
/// @file PtHierNameTest.cc
/// @brief PtHierNameTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtTest.h"
#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtTest, new1)
{
  const char* head = "head1";
  const char* tail = "tail1";
  auto hname = parser.new_HierName(head, tail);

  EXPECT_STREQ( tail, hname.tail_name );
  auto nb_top = hname.nb_list.top;
  EXPECT_STREQ( head, nb_top->name() );
  EXPECT_FALSE( nb_top->has_index() );
  EXPECT_THROW( nb_top->index(),
		std::logic_error );
  EXPECT_EQ( nullptr, nb_top->link() );
}

TEST_F(PtTest, new2)
{
  const char* head = "head1";
  int index = 2;
  const char* tail = "tail1";
  auto hname = parser.new_HierName(head, index, tail);

  EXPECT_STREQ( tail, hname.tail_name );
  auto nb_top = hname.nb_list.top;
  EXPECT_STREQ( head, nb_top->name() );
  EXPECT_TRUE( nb_top->has_index() );
  EXPECT_EQ( index, nb_top->index() );
  EXPECT_EQ( nullptr, nb_top->link() );
}

TEST_F(PtTest, add1)
{
  const char* head = "head1";
  const char* tail = "tail1";
  auto hname = parser.new_HierName(head, tail);

  const char* tail2 = "tail2";
  parser.add_HierName(hname, tail2);

  EXPECT_STREQ( tail2, hname.tail_name );
  auto nb_top = hname.nb_list.top;
  EXPECT_STREQ( head, nb_top->name() );
  EXPECT_FALSE( nb_top->has_index() );
  auto nb2 = nb_top->link();
  EXPECT_STREQ( tail, nb2->name() );
  EXPECT_FALSE( nb2->has_index() );
  EXPECT_EQ( nullptr, nb2->link() );
}

TEST_F(PtTest, add2)
{
  const char* head = "head1";
  const char* tail = "tail1";
  auto hname = parser.new_HierName(head, tail);

  const char* tail2 = "tail2";
  int index2 = 3;
  parser.add_HierName(hname, index2, tail2);

  EXPECT_STREQ( tail2, hname.tail_name );
  auto nb_top = hname.nb_list.top;
  EXPECT_STREQ( head, nb_top->name() );
  EXPECT_FALSE( nb_top->has_index() );
  auto nb2 = nb_top->link();
  EXPECT_STREQ( tail, nb2->name() );
  EXPECT_TRUE( nb2->has_index() );
  EXPECT_EQ( index2, nb2->index() );
  EXPECT_EQ( nullptr, nb2->link() );
}

END_NAMESPACE_YM_VERILOG
