
/// @file PtIODListTest.cc
/// @brief PtIODListTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"
#include "parser/PtDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, PtIODList_init1)
{
  PtIODList port_list;

  port_list.init();

  EXPECT_EQ( nullptr, port_list.head_list.top );
  EXPECT_EQ( nullptr, port_list.head_list.tail );
  EXPECT_EQ( nullptr, port_list.item_tail );
}

TEST_F(ParserTest, PtIODList_init2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_IOItem(fr1, name1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto head = factory.new_IOHead(fr2, VpiDir::Input, item);

  EXPECT_EQ( item, head->_item_top() );

  PtIODList port_list;

  port_list.init(head);

  EXPECT_EQ( head, port_list.head_list.top );
  EXPECT_EQ( head, port_list.head_list.tail );
  EXPECT_EQ( nullptr, head->_link() );
  EXPECT_EQ( item, port_list.item_tail );
  EXPECT_EQ( nullptr, item->_link() );
}

TEST_F(ParserTest, PtIODList_add_head1)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_IOItem(fr1, name1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto head = factory.new_IOHead(fr2, VpiDir::Input, item);

  EXPECT_EQ( item, head->_item_top() );

  PtIODList port_list;

  port_list.init(head);

  auto fr3 = make_file_region(3, 3, 3, 3);
  const char* name2 = "name2";
  auto item2 = factory.new_IOItem(fr3, name2);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto head2 = factory.new_IOHead(fr4, VpiDir::Output, item2);

  port_list.add_head(head2);

  EXPECT_EQ( head, port_list.head_list.top );
  EXPECT_EQ( head2, head->_link() );
  EXPECT_EQ( head2, port_list.head_list.tail );
  EXPECT_EQ( nullptr, head2->_link() );
  EXPECT_EQ( item2, port_list.item_tail );
}

TEST_F(ParserTest, PtIODList_add_head2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_IOItem(fr1, name1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto head = factory.new_IOHead(fr2, VpiDir::Input, item);

  EXPECT_EQ( item, head->_item_top() );

  PtIODList port_list;

  port_list.init(head);

  auto fr3 = make_file_region(3, 3, 3, 3);
  const char* name2 = "name2";
  auto item2 = factory.new_IOItem(fr3, name2);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto head2 = factory.new_IOHead(fr4, VpiDir::Output, item2);

  port_list.add_head(head2);

  auto fr5 = make_file_region(5, 5, 5, 5);
  const char* name3 = "name3";
  auto item3 = factory.new_IOItem(fr5, name3);
  auto fr6 = make_file_region(6, 6, 6, 6);
  auto head3 = factory.new_IOHead(fr6, VpiDir::Output, item3);

  port_list.add_head(head3);

  EXPECT_EQ( head, port_list.head_list.top );
  EXPECT_EQ( head2, head->_link() );
  EXPECT_EQ( head3, head2->_link() );
  EXPECT_EQ( nullptr, head3->_link() );
  EXPECT_EQ( head3, port_list.head_list.tail );
  EXPECT_EQ( item3, port_list.item_tail );
}

TEST_F(ParserTest, PtIODList_add_item1)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_IOItem(fr1, name1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto head = factory.new_IOHead(fr2, VpiDir::Input, item);

  EXPECT_EQ( item, head->_item_top() );

  PtIODList port_list;

  port_list.init(head);

  auto fr3 = make_file_region(3, 3, 3, 3);
  const char* name2 = "name2";
  auto item2 = factory.new_IOItem(fr3, name2);

  port_list.add_item(item2);

  EXPECT_EQ( head, port_list.head_list.top );
  EXPECT_EQ( nullptr, head->_link() );
  EXPECT_EQ( head, port_list.head_list.tail );
  EXPECT_EQ( item2, item->_link() );
  EXPECT_EQ( item2, port_list.item_tail );
}

TEST_F(ParserTest, PtIODList_add_item2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  const char* name1 = "name1";
  auto item = factory.new_IOItem(fr1, name1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto head = factory.new_IOHead(fr2, VpiDir::Input, item);

  EXPECT_EQ( item, head->_item_top() );

  PtIODList port_list;

  port_list.init(head);

  auto fr3 = make_file_region(3, 3, 3, 3);
  const char* name2 = "name2";
  auto item2 = factory.new_IOItem(fr3, name2);

  port_list.add_item(item2);

  auto fr4 = make_file_region(4, 4, 4, 4);
  const char* name3 = "name3";
  auto item3 = factory.new_IOItem(fr4, name3);

  port_list.add_item(item3);

  EXPECT_EQ( head, port_list.head_list.top );
  EXPECT_EQ( nullptr, head->_link() );
  EXPECT_EQ( head, port_list.head_list.tail );
  EXPECT_EQ( item2, item->_link() );
  EXPECT_EQ( item3, item2->_link() );
  EXPECT_EQ( nullptr, item3->_link() );
  EXPECT_EQ( item3, port_list.item_tail );
}

END_NAMESPACE_YM_VERILOG
