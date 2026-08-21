
/// @file ParserTest_misc.cc
/// @brief ParserTest_misc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, DelayControl)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr = parser.new_IntConst(fr1, 1U);
  auto control = parser.new_DelayControl(fr, expr);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( FileRegion(fr, fr1), control->file_region() );
  EXPECT_EQ( AstControl::Delay, control->type() );
  EXPECT_EQ( expr, control->delay() );
  EXPECT_THROW( control->event_num(),
		std::logic_error );
  EXPECT_THROW( control->event_list(),
		std::logic_error );
  EXPECT_THROW( control->rep_expr(),
		std::logic_error );
}

TEST_F(ParserTest, EventControl1)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto control = parser.new_EventControl(fr);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Event, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 0, control->event_num() );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     control->event_list() );
  EXPECT_THROW( control->rep_expr(),
		std::logic_error );
}

TEST_F(ParserTest, EventControl2)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto name_fr = make_file_region(1, 5, 1, 10);
  auto name = "event1";
  auto control = parser.new_EventControl(fr, name, name_fr);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Event, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 1, control->event_num() );
  auto event0 = control->event_list().front();
  EXPECT_EQ( AstExpr::Primary, event0->type() );
  EXPECT_EQ( name, event0->name() );
  EXPECT_EQ( std::vector<const AstExpr*>{event0},
	     control->event_list() );
  EXPECT_THROW( control->rep_expr(),
		std::logic_error );
}

TEST_F(ParserTest, EventControl3)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto name_fr = make_file_region(1, 5, 1, 10);
  auto head = "head1";
  auto name = "event1";
  auto hname = parser.new_HierName(head, name);
  auto control = parser.new_EventControl(fr, hname, name_fr);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Event, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 1, control->event_num() );
  auto event0 = control->event_list().front();
  EXPECT_EQ( AstExpr::Primary, event0->type() );
  EXPECT_STREQ( name, event0->name() );
  EXPECT_EQ( 1, event0->namebranch_num() );
  auto nb0 = event0->namebranch_list().front();
  EXPECT_STREQ( head, nb0->name() );
  EXPECT_EQ( std::vector<const AstExpr*>{event0},
	     control->event_list() );
  EXPECT_THROW( control->rep_expr(),
		std::logic_error );
}

TEST_F(ParserTest, EventControl4)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto fr1 = make_file_region(1, 5, 1, 10);
  auto name1 = "event1";
  auto expr1 = parser.new_Primary(fr1, name1);
  auto fr2 = make_file_region(1, 20, 1, 30);
  auto head = "head2";
  auto name2 = "event2";
  auto hname = parser.new_HierName(head, name2);
  auto expr2 = parser.new_Primary(fr2, hname);
  auto event_list = parser.new_expr_list();
  event_list->push_back(astmgr.alloc(), expr1);
  event_list->push_back(astmgr.alloc(), expr2);
  auto control = parser.new_EventControl(fr, event_list);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Event, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 2, control->event_num() );
  EXPECT_EQ( expr1, control->event(0) );
  EXPECT_EQ( expr2, control->event(1) );
  std::vector<const AstExpr*> expr_list{expr1, expr2};
  EXPECT_EQ( expr_list, control->event_list() );
  EXPECT_THROW( control->rep_expr(),
		std::logic_error );
}

TEST_F(ParserTest, RepeatControl1)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto rep = parser.new_IntConst(fr1, 1U);
  auto control = parser.new_RepeatControl(fr, rep);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Repeat, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 0, control->event_num() );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     control->event_list() );
  EXPECT_EQ( rep, control->rep_expr() );
}

TEST_F(ParserTest, RepeatControl2)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto rep_fr = make_file_region(1, 10, 1, 19);
  auto rep = parser.new_IntConst(rep_fr, 1U);
  auto name_fr = make_file_region(1, 20, 1, 29);
  auto name = "event1";
  auto control = parser.new_RepeatControl(fr, rep, name, name_fr);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Repeat, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 1, control->event_num() );
  auto event0 = control->event_list().front();
  EXPECT_EQ( AstExpr::Primary, event0->type() );
  EXPECT_EQ( name, event0->name() );
  EXPECT_EQ( std::vector<const AstExpr*>{event0},
	     control->event_list() );
  EXPECT_EQ( rep, control->rep_expr() );
}

TEST_F(ParserTest, RepatControl3)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto rep_fr = make_file_region(1, 10, 1, 19);
  auto rep = parser.new_IntConst(rep_fr, 1U);
  auto name_fr = make_file_region(1, 20, 1, 29);
  auto head = "head1";
  auto name = "event1";
  auto hname = parser.new_HierName(head, name);
  auto control = parser.new_RepeatControl(fr, rep, hname, name_fr);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Repeat, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 1, control->event_num() );
  auto event0 = control->event_list().front();
  EXPECT_EQ( AstExpr::Primary, event0->type() );
  EXPECT_STREQ( name, event0->name() );
  EXPECT_EQ( 1, event0->namebranch_num() );
  auto nb0 = event0->namebranch_list().front();
  EXPECT_STREQ( head, nb0->name() );
  EXPECT_EQ( std::vector<const AstExpr*>{event0},
	     control->event_list() );
  EXPECT_EQ( rep, control->rep_expr() );
}

TEST_F(ParserTest, RepeatControl4)
{
  auto fr = make_file_region(1, 2, 1, 4);
  auto rep_fr = make_file_region(1, 10, 1, 19);
  auto rep = parser.new_IntConst(rep_fr, 1U);
  auto fr1 = make_file_region(1, 20, 1, 29);
  auto name1 = "event1";
  auto expr1 = parser.new_Primary(fr1, name1);
  auto fr2 = make_file_region(1, 30, 1, 39);
  auto head = "head2";
  auto name2 = "event2";
  auto hname = parser.new_HierName(head, name2);
  auto expr2 = parser.new_Primary(fr2, hname);
  auto event_list = parser.new_expr_list();
  event_list->push_back(astmgr.alloc(), expr1);
  event_list->push_back(astmgr.alloc(), expr2);
  auto control = parser.new_RepeatControl(fr, rep, event_list);

  ASSERT_TRUE( control != nullptr );
  EXPECT_EQ( fr, control->file_region() );
  EXPECT_EQ( AstControl::Repeat, control->type() );
  EXPECT_THROW( control->delay(),
		std::logic_error);
  EXPECT_EQ( 2, control->event_num() );
  std::vector<const AstExpr*> expr_list{expr1, expr2};
  EXPECT_EQ( expr_list, control->event_list() );
  EXPECT_EQ( rep, control->rep_expr() );
}

TEST_F(ParserTest, OrderedCon1)
{
  auto fr = make_file_region(1, 1, 1, 9);
  auto expr = parser.new_IntConst(fr, 1U);
  auto con = parser.new_OrderedCon(expr);

  ASSERT_TRUE( con != nullptr );
  EXPECT_EQ( nullptr, con->name() );
  EXPECT_EQ( expr, con->expr() );
}

TEST_F(ParserTest, OrderedCon2)
{
  auto fr = make_file_region(1, 1, 1, 9);
  auto expr = parser.new_IntConst(fr, 1U);
  auto attr_name = "attr1";
  auto fr1 = make_file_region(1, 2, 3, 4);
  auto attr_val = parser.new_IntConst(fr1, 2);
  auto as = parser.new_AttrSpec(fr1, attr_name, attr_val);
  parser.init_attrspec_list();
  parser.add_attrspec(as);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto ai = parser.new_AttrInst(fr2);
  parser.init_attrinst_list();
  parser.add_attrinst(ai);
  auto ai_list = parser.new_AttrInstList();
  auto con = parser.new_OrderedCon(fr, expr, ai_list);

  ASSERT_TRUE( con != nullptr );
  EXPECT_EQ( nullptr, con->name() );
  EXPECT_EQ( expr, con->expr() );
}

TEST_F(ParserTest, NamedCon1)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto name = "name1";
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto expr = parser.new_IntConst(fr2, 1U);
  auto con = parser.new_NamedCon(FileRegion(fr1, fr2), name, expr);

  ASSERT_TRUE( con != nullptr );
  EXPECT_STREQ( name, con->name() );
  EXPECT_EQ( expr, con->expr() );
}

TEST_F(ParserTest, Strength1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto val0 = VpiStrength::SupplyDrive;
  auto val1 = VpiStrength::PullDrive;
  auto str = parser.new_Strength(fr, val0, val1);

  ASSERT_TRUE( str != nullptr );
  EXPECT_EQ( fr, str->file_region() );
  EXPECT_EQ( val0, str->drive0() );
  EXPECT_EQ( val1, str->drive1() );
  EXPECT_EQ( VpiStrength::NoStrength, str->charge() );
}

TEST_F(ParserTest, Strength2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto val0 = VpiStrength::WeakDrive;
  auto str = parser.new_Strength(fr, val0);

  ASSERT_TRUE( str != nullptr );
  EXPECT_EQ( fr, str->file_region() );
  EXPECT_EQ( VpiStrength::NoStrength, str->drive0() );
  EXPECT_EQ( VpiStrength::NoStrength, str->drive1() );
  EXPECT_EQ( val0, str->charge() );
}

TEST_F(ParserTest, Delay1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 1);
  auto delay = parser.new_Delay(fr, expr1);

  ASSERT_TRUE( delay != nullptr );
  EXPECT_EQ( fr, delay->file_region() );
  EXPECT_EQ( expr1, delay->value0() );
  EXPECT_EQ( nullptr, delay->value1() );
  EXPECT_EQ( nullptr, delay->value2() );
}

TEST_F(ParserTest, Delay2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 1);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto expr2 = parser.new_IntConst(fr2, 2);
  auto delay = parser.new_Delay(fr, expr1, expr2);

  ASSERT_TRUE( delay != nullptr );
  EXPECT_EQ( fr, delay->file_region() );
  EXPECT_EQ( expr1, delay->value0() );
  EXPECT_EQ( expr2, delay->value1() );
  EXPECT_EQ( nullptr, delay->value2() );
}

TEST_F(ParserTest, Delay3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 1);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto expr2 = parser.new_IntConst(fr2, 2);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.new_IntConst(fr3, 3);
  auto delay = parser.new_Delay(fr, expr1, expr2, expr3);

  ASSERT_TRUE( delay != nullptr );
  EXPECT_EQ( fr, delay->file_region() );
  EXPECT_EQ( expr1, delay->value0() );
  EXPECT_EQ( expr2, delay->value1() );
  EXPECT_EQ( expr3, delay->value2() );
}

#if 0
TEST_F(ParserTest, HierName1)
{
  auto head = "head1";
  auto name = "name1";
  auto hname = parser.new_HierName(head, name);

  ASSERT_TRUE( hname != nullptr );
  auto nbarray = hname->name_branch_to_array(astmgr.alloc());
  ASSERT_EQ( 1, nbarray.size() );
  EXPECT_THROW( nbarray[1],
		std::out_of_range );
  auto nb = nbarray[0];
  EXPECT_STREQ( head, nb->name() );
  EXPECT_FALSE( nb->has_index() );
  EXPECT_EQ( head, nb->decompile() );
  EXPECT_STREQ( name, hname->tail_name() );
}

TEST_F(ParserTest, HierName2)
{
  auto head = "head1";
  auto name = "name1";
  auto hname = parser.new_HierName(head, 0, name);

  ASSERT_TRUE( hname != nullptr );
  auto nbarray = hname->name_branch_to_array(astmgr.alloc());
  ASSERT_EQ( 1, nbarray.size() );
  EXPECT_THROW( nbarray[1],
		std::out_of_range );
  auto nb = nbarray[0];
  EXPECT_STREQ( head, nb->name() );
  EXPECT_TRUE( nb->has_index() );
  std::ostringstream buf;
  buf << head << "[" << 0 << "]";
  EXPECT_EQ( buf.str(), nb->decompile() );
  EXPECT_STREQ( name, hname->tail_name() );
}

TEST_F(ParserTest, HierName3)
{
  auto head = "head1";
  auto head2 = "head2";
  auto hname = parser.new_HierName(head, head2);
  auto name = "name1";
  parser.add_HierName(hname, name);

  ASSERT_TRUE( hname != nullptr );
  auto nbarray = hname->name_branch_to_array(astmgr.alloc());
  ASSERT_EQ( 2, nbarray.size() );
  EXPECT_THROW( nbarray[2],
		std::out_of_range );
  auto nb = nbarray[0];
  auto nb2 = nbarray[1];
  EXPECT_STREQ( head, nb->name() );
  EXPECT_FALSE( nb->has_index() );
  EXPECT_EQ( head, nb->decompile() );
  EXPECT_STREQ( head2, nb2->name() );
  EXPECT_FALSE( nb2->has_index() );
  EXPECT_EQ( head2, nb2->decompile() );
  EXPECT_STREQ( name, hname->tail_name() );
}

TEST_F(ParserTest, HierName4)
{
  auto head = "head1";
  auto head2 = "head2";
  auto hname = parser.new_HierName(head, head2);
  auto name = "name1";
  parser.add_HierName(hname, 1, name);

  ASSERT_TRUE( hname != nullptr );
  auto nbarray = hname->name_branch_to_array(astmgr.alloc());
  ASSERT_EQ( 2, nbarray.size() );
  EXPECT_THROW( nbarray[2],
		std::out_of_range );
  auto nb = nbarray[0];
  auto nb2 = nbarray[1];
  EXPECT_STREQ( head, nb->name() );
  EXPECT_FALSE( nb->has_index() );
  EXPECT_EQ( head, nb->decompile() );
  EXPECT_STREQ( head2, nb2->name() );
  EXPECT_TRUE( nb2->has_index() );
  std::ostringstream buf;
  buf << head2 << "[" << 1 << "]";
  EXPECT_EQ( buf.str(), nb2->decompile() );
  EXPECT_STREQ( name, hname->tail_name() );
}
#endif

TEST_F(ParserTest, Part1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 3);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto expr2 = parser.new_IntConst(fr2, 2);
  auto part = parser.new_Part(fr, VpiRangeMode::Const, expr1, expr2);

  ASSERT_TRUE( part != nullptr );
  EXPECT_EQ( fr, part->file_region() );
  EXPECT_EQ( VpiRangeMode::Const, part->mode() );
  EXPECT_EQ( expr1, part->left() );
  EXPECT_EQ( expr2, part->right() );
}

TEST_F(ParserTest, Part2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 3);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto expr2 = parser.new_IntConst(fr2, 2);
  auto part = parser.new_Part(fr, VpiRangeMode::Plus, expr1, expr2);

  ASSERT_TRUE( part != nullptr );
  EXPECT_EQ( fr, part->file_region() );
  EXPECT_EQ( VpiRangeMode::Plus, part->mode() );
  EXPECT_EQ( expr1, part->left() );
  EXPECT_EQ( expr2, part->right() );
}

TEST_F(ParserTest, Part3)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 3);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto expr2 = parser.new_IntConst(fr2, 2);
  auto part = parser.new_Part(fr, VpiRangeMode::Minus, expr1, expr2);

  ASSERT_TRUE( part != nullptr );
  EXPECT_EQ( fr, part->file_region() );
  EXPECT_EQ( VpiRangeMode::Minus, part->mode() );
  EXPECT_EQ( expr1, part->left() );
  EXPECT_EQ( expr2, part->right() );
}

TEST_F(ParserTest, AttrSpec)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name1 = "attr1";
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto expr1 = parser.new_IntConst(fr1, 1);
  auto as = parser.new_AttrSpec(fr, name1, expr1);

  ASSERT_TRUE( as != nullptr );
  EXPECT_EQ( fr, as->file_region() );
  EXPECT_STREQ( name1, as->name() );
  EXPECT_EQ( expr1, as->expr() );
}

TEST_F(ParserTest, AttrInst)
{
  auto fr = make_file_region(1, 2, 3, 4);
  parser.init_attrspec_list();
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto name1 = "attr1";
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr1 = parser.new_IntConst(fr2, 1);
  auto as = parser.new_AttrSpec(fr1, name1, expr1);
  parser.add_attrspec(as);
  parser.init_attrinst_list();
  auto ai = parser.new_AttrInst(fr);

  ASSERT_TRUE( ai != nullptr );
  EXPECT_EQ( fr, ai->file_region() );
  EXPECT_EQ( 1, ai->attrspec_num() );
  std::vector<const AstAttrSpec*> as_vec{as};
  EXPECT_EQ( as_vec, ai->attrspec_list() );
}

END_NAMESPACE_YM_VERILOG
