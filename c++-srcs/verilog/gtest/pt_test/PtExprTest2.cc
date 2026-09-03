
/// @file PtExprTest2.cc
/// @brief PtExprTest の実装ファイルその2
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtExprTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtExprTest, Primary1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto expr = factory.new_Primary(fr, name);

  check_Primary(expr, fr, name, false);
}

TEST_F(PtExprTest, Primary2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = factory.new_IntConst(fr2, 2U);
  auto index_list = PtExprList::new_obj(index1);
  index_list.add(index2);
  auto expr = factory.new_Primary(fr, name, index_list.top);

  check_Primary(expr, fr, name, false, {index1, index2});
  //EXPECT_EQ( "net1[1][2]", expr->decompile() );
}

TEST_F(PtExprTest, Primary3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType left_val = 3;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType right_val = 0;
  auto part = new_part(fr1, left_val, fr2, right_val, VpiRangeMode::Const);
  auto expr = factory.new_Primary(fr, name, part);

  check_Primary(expr, fr, name, false, part);
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << name << "[" << left_val << ":" << right_val << "]";
  //EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(PtExprTest, Primary4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "net1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = factory.new_IntConst(fr2, 2U);
  auto index_list = PtExprList();
  index_list.add(index1);
  index_list.add(index2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  SizeType left_val = 3;
  auto fr4 = make_file_region(4, 4, 4, 4);
  SizeType right_val = 0;
  auto part = new_part(fr3, left_val, fr4, right_val, VpiRangeMode::Const);
  auto expr = factory.new_Primary(fr, name, index_list.top, part);

  check_Primary(expr, fr, name, false, {index1, index2}, part);
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << name << "[1][2][" << left_val << ":" << right_val << "]";
  //EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(PtExprTest, PrimaryH1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto expr = factory.new_Primary(fr, hname);

  check_Primary(expr, fr, name, {PtTest::NameBranchSpec{head, 0}}, false);
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  //EXPECT_EQ( "head1.net1", expr->decompile() );
}

TEST_F(PtExprTest, PrimaryH2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = factory.new_IntConst(fr2, 2U);
  auto index_list = PtExprList::new_obj(index1);
  index_list.add(index2);
  auto expr = factory.new_Primary(fr, hname, index_list.top);


  check_Primary(expr, fr, name, {PtTest::NameBranchSpec{head, 0}}, false, {index1, index2});
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  //EXPECT_EQ( "head1.net1[1][2]", expr->decompile() );
}

TEST_F(PtExprTest, PrimaryH3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  SizeType left_val = 3;
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType right_val = 0;
  auto part = new_part(fr1, left_val, fr2, right_val, VpiRangeMode::Const);
  auto expr = factory.new_Primary(fr, hname, part);

  check_Primary(expr, fr, name, {NameBranchSpec{head, 0}}, false, part);
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << head << "." << name << "[" << left_val << ":" << right_val << "]";
  //EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(PtExprTest, PrimaryH4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "net1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = factory.new_IntConst(fr2, 2U);
  auto index_list = PtExprList::new_obj(index1);
  index_list.add(index2);
  auto fr3 = make_file_region(3, 3, 3, 3);
  SizeType left_val = 3;
  auto fr4 = make_file_region(4, 4, 4, 4);
  SizeType right_val = 0;
  auto part = new_part(fr3, left_val, fr4, right_val, VpiRangeMode::Const);
  auto expr = factory.new_Primary(fr, hname, index_list.top, part);

  check_Primary(expr, fr, name, {NameBranchSpec{head, 0}}, false, {index1, index2}, part);
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << head << "." << name << "[1][2]"
      << "[" << left_val << ":" << right_val << "]";
  //EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(PtExprTest, CPrimary1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto expr = factory.new_CPrimary(fr, name, index1);

  check_Primary(expr, fr, name, true, {index1});
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  //EXPECT_EQ( "param1[1]", expr->decompile() );
}

TEST_F(PtExprTest, CPrimary2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = factory.new_IntConst(fr2, 2U);
  auto index_list = PtExprList();
  index_list.add(index1);
  index_list.add(index2);
  auto expr = factory.new_CPrimary(fr, name, index_list.top);

  check_Primary(expr, fr, name, true, {index1, index2});
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  //EXPECT_EQ( "param1[1][2]", expr->decompile() );
}

TEST_F(PtExprTest, CPrimary3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "param1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto fr2 = make_file_region(2, 2, 2, 2);
  SizeType left_val = 3;
  SizeType right_val = 0;
  auto part = new_part(fr1, left_val, fr2, right_val, VpiRangeMode::Const);
  auto expr = factory.new_CPrimary(fr, name, part);

  check_Primary(expr, fr, name, true, part);
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  std::ostringstream buf;
  buf << name << "[" << left_val << ":" << right_val << "]";
  //EXPECT_EQ( buf.str(), expr->decompile() );
}

TEST_F(PtExprTest, CPrimary4)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "param1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto index1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto index2 = factory.new_IntConst(fr2, 2U);
  auto index_list = PtExprList();
  index_list.add(index1);
  index_list.add(index2);
  auto expr = factory.new_CPrimary(fr, hname, index_list.top);

  check_Primary(expr, fr, name, {NameBranchSpec{head, 0}}, true, {index1, index2});
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  //EXPECT_EQ( "head1.param1[1][2]", expr->decompile() );
}

END_NAMESPACE_YM_VERILOG
