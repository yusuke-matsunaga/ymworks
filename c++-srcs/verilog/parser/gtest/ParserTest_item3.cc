
/// @file ParserTest.cc
/// @brief ParserTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "ym/pt/PtDecl.h"
#include "ym/pt/PtItem.h"
#include "ym/pt/PtMisc.h"
#include "parser/Parser.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, MuH)
{
  auto fr = make_file_region(1, 1, 2, 2);
  auto def_name = "module1";
  auto item = parser.new_MuH(fr, def_name);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( def_name, item->name() );
  EXPECT_EQ( PtItemType::MuInst, item->type() );
  EXPECT_THROW( item->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item->strength() );
  EXPECT_EQ( nullptr, item->delay() );
  EXPECT_EQ( 0, item->paramassign_num() );
  EXPECT_THROW( item->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{},
	     item->paramassign_list() );
  EXPECT_EQ( 0, item->defparam_num() );
  EXPECT_THROW( item->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDefParam*>{},
	     item->defparam_list() );
  EXPECT_EQ( 0, item->contassign_num() );
  EXPECT_THROW( item->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtContAssign*>{},
	     item->contassign_list() );
  EXPECT_EQ( 0, item->inst_num() );
  EXPECT_THROW( item->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtInst*>{},
	     item->inst_list() );
  EXPECT_FALSE( item->automatic() );
  EXPECT_EQ( 0, item->ioitem_num() );
  EXPECT_EQ( 0, item->iohead_num() );
  EXPECT_THROW( item->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOHead*>{},
	     item->iohead_list() );
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->item_list() );
  EXPECT_EQ( nullptr, item->body() );
  EXPECT_FALSE( item->is_signed() );
  EXPECT_EQ( nullptr, item->range() );
  EXPECT_EQ( VpiVarType::None, item->data_type() );
  EXPECT_THROW( item->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item->terminal_num() );
  EXPECT_THROW( item->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     item->terminal_list() );
  EXPECT_EQ( nullptr, item->path_decl() );
  EXPECT_EQ( nullptr, item->expr() );
  EXPECT_EQ( 0, item->then_declhead_num() );
  EXPECT_THROW( item->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->then_declhead_list() );
  EXPECT_EQ( 0, item->then_item_num() );
  EXPECT_THROW( item->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->then_item_list() );
  EXPECT_EQ( 0, item->else_declhead_num() );
  EXPECT_THROW( item->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->else_declhead_list() );
  EXPECT_EQ( 0, item->else_item_num() );
  EXPECT_THROW( item->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->else_item_list() );
  EXPECT_EQ( 0, item->caseitem_num() );
  EXPECT_THROW( item->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtGenCaseItem*>{},
	     item->caseitem_list() );
  EXPECT_EQ( nullptr, item->loop_var() );
  EXPECT_EQ( nullptr, item->init_expr() );
  EXPECT_EQ( nullptr, item->next_expr() );
}

TEST_F(ParserTest, MuHS)
{
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto def_name = "module1";
  auto str = parser.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr = make_file_region(1, 1, 2, 2);
  auto item = parser.new_MuH(fr, def_name, str);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( PtItemType::MuInst, item->type() );
  EXPECT_THROW( item->prim_type(),
		std::logic_error );
  EXPECT_EQ( str, item->strength() );
  EXPECT_EQ( nullptr, item->delay() );
  EXPECT_EQ( 0, item->paramassign_num() );
  EXPECT_THROW( item->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{},
	     item->paramassign_list() );
  EXPECT_EQ( 0, item->defparam_num() );
  EXPECT_THROW( item->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDefParam*>{},
	     item->defparam_list() );
  EXPECT_EQ( 0, item->contassign_num() );
  EXPECT_THROW( item->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtContAssign*>{},
	     item->contassign_list() );
  EXPECT_EQ( 0, item->inst_num() );
  EXPECT_THROW( item->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtInst*>{},
	     item->inst_list() );
  EXPECT_FALSE( item->automatic() );
  EXPECT_EQ( 0, item->ioitem_num() );
  EXPECT_EQ( 0, item->iohead_num() );
  EXPECT_THROW( item->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOHead*>{},
	     item->iohead_list() );
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->item_list() );
  EXPECT_EQ( nullptr, item->body() );
  EXPECT_FALSE( item->is_signed() );
  EXPECT_EQ( nullptr, item->range() );
  EXPECT_EQ( VpiVarType::None, item->data_type() );
  EXPECT_THROW( item->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item->terminal_num() );
  EXPECT_THROW( item->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     item->terminal_list() );
  EXPECT_EQ( nullptr, item->path_decl() );
  EXPECT_EQ( nullptr, item->expr() );
  EXPECT_EQ( 0, item->then_declhead_num() );
  EXPECT_THROW( item->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->then_declhead_list() );
  EXPECT_EQ( 0, item->then_item_num() );
  EXPECT_THROW( item->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->then_item_list() );
  EXPECT_EQ( 0, item->else_declhead_num() );
  EXPECT_THROW( item->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->else_declhead_list() );
  EXPECT_EQ( 0, item->else_item_num() );
  EXPECT_THROW( item->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->else_item_list() );
  EXPECT_EQ( 0, item->caseitem_num() );
  EXPECT_THROW( item->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtGenCaseItem*>{},
	     item->caseitem_list() );
  EXPECT_EQ( nullptr, item->loop_var() );
  EXPECT_EQ( nullptr, item->init_expr() );
  EXPECT_EQ( nullptr, item->next_expr() );
}

TEST_F(ParserTest, MuHD)
{
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto def_name = "module1";
  auto val = parser.new_IntConst(fr1, 1U);
  auto delay = parser.new_Delay(fr1, val);
  auto fr = make_file_region(1, 1, 2, 2);
  auto item = parser.new_MuH(fr, def_name, delay);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( PtItemType::MuInst, item->type() );
  EXPECT_THROW( item->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item->strength() );
  EXPECT_EQ( delay, item->delay() );
  EXPECT_EQ( 0, item->paramassign_num() );
  EXPECT_THROW( item->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{},
	     item->paramassign_list() );
  EXPECT_EQ( 0, item->defparam_num() );
  EXPECT_THROW( item->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDefParam*>{},
	     item->defparam_list() );
  EXPECT_EQ( 0, item->contassign_num() );
  EXPECT_THROW( item->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtContAssign*>{},
	     item->contassign_list() );
  EXPECT_EQ( 0, item->inst_num() );
  EXPECT_THROW( item->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtInst*>{},
	     item->inst_list() );
  EXPECT_FALSE( item->automatic() );
  EXPECT_EQ( 0, item->ioitem_num() );
  EXPECT_EQ( 0, item->iohead_num() );
  EXPECT_THROW( item->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOHead*>{},
	     item->iohead_list() );
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->item_list() );
  EXPECT_EQ( nullptr, item->body() );
  EXPECT_FALSE( item->is_signed() );
  EXPECT_EQ( nullptr, item->range() );
  EXPECT_EQ( VpiVarType::None, item->data_type() );
  EXPECT_THROW( item->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item->terminal_num() );
  EXPECT_THROW( item->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     item->terminal_list() );
  EXPECT_EQ( nullptr, item->path_decl() );
  EXPECT_EQ( nullptr, item->expr() );
  EXPECT_EQ( 0, item->then_declhead_num() );
  EXPECT_THROW( item->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->then_declhead_list() );
  EXPECT_EQ( 0, item->then_item_num() );
  EXPECT_THROW( item->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->then_item_list() );
  EXPECT_EQ( 0, item->else_declhead_num() );
  EXPECT_THROW( item->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->else_declhead_list() );
  EXPECT_EQ( 0, item->else_item_num() );
  EXPECT_THROW( item->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->else_item_list() );
  EXPECT_EQ( 0, item->caseitem_num() );
  EXPECT_THROW( item->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtGenCaseItem*>{},
	     item->caseitem_list() );
  EXPECT_EQ( nullptr, item->loop_var() );
  EXPECT_EQ( nullptr, item->init_expr() );
  EXPECT_EQ( nullptr, item->next_expr() );
}

TEST_F(ParserTest, MuHSD)
{
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto def_name = "module1";
  auto str = parser.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto val = parser.new_IntConst(fr2, 1U);
  auto delay = parser.new_Delay(fr2, val);
  auto fr = make_file_region(1, 1, 2, 2);
  auto item = parser.new_MuH(fr, def_name, str, delay);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( PtItemType::MuInst, item->type() );
  EXPECT_THROW( item->prim_type(),
		std::logic_error );
  EXPECT_EQ( str, item->strength() );
  EXPECT_EQ( delay, item->delay() );
  EXPECT_EQ( 0, item->paramassign_num() );
  EXPECT_THROW( item->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{},
	     item->paramassign_list() );
  EXPECT_EQ( 0, item->defparam_num() );
  EXPECT_THROW( item->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDefParam*>{},
	     item->defparam_list() );
  EXPECT_EQ( 0, item->contassign_num() );
  EXPECT_THROW( item->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtContAssign*>{},
	     item->contassign_list() );
  EXPECT_EQ( 0, item->inst_num() );
  EXPECT_THROW( item->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtInst*>{},
	     item->inst_list() );
  EXPECT_FALSE( item->automatic() );
  EXPECT_EQ( 0, item->ioitem_num() );
  EXPECT_EQ( 0, item->iohead_num() );
  EXPECT_THROW( item->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOHead*>{},
	     item->iohead_list() );
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->item_list() );
  EXPECT_EQ( nullptr, item->body() );
  EXPECT_FALSE( item->is_signed() );
  EXPECT_EQ( nullptr, item->range() );
  EXPECT_EQ( VpiVarType::None, item->data_type() );
  EXPECT_THROW( item->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item->terminal_num() );
  EXPECT_THROW( item->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     item->terminal_list() );
  EXPECT_EQ( nullptr, item->path_decl() );
  EXPECT_EQ( nullptr, item->expr() );
  EXPECT_EQ( 0, item->then_declhead_num() );
  EXPECT_THROW( item->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->then_declhead_list() );
  EXPECT_EQ( 0, item->then_item_num() );
  EXPECT_THROW( item->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->then_item_list() );
  EXPECT_EQ( 0, item->else_declhead_num() );
  EXPECT_THROW( item->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->else_declhead_list() );
  EXPECT_EQ( 0, item->else_item_num() );
  EXPECT_THROW( item->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->else_item_list() );
  EXPECT_EQ( 0, item->caseitem_num() );
  EXPECT_THROW( item->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtGenCaseItem*>{},
	     item->caseitem_list() );
  EXPECT_EQ( nullptr, item->loop_var() );
  EXPECT_EQ( nullptr, item->init_expr() );
  EXPECT_EQ( nullptr, item->next_expr() );
}

TEST_F(ParserTest, MuHP)
{
  auto fr = make_file_region(1, 1, 2, 2);
  auto def_name = "module1";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto con_list = parser.new_list<const PtConnection>();
  auto expr = parser.new_IntConst(fr1, 123U);
  auto con = parser.new_OrderedCon(expr);
  con_list->push_back(con);
  auto item = parser.new_MuH(fr, def_name, con_list);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( def_name, item->name() );
  EXPECT_EQ( PtItemType::MuInst, item->type() );
  EXPECT_THROW( item->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item->strength() );
  EXPECT_EQ( nullptr, item->delay() );
  EXPECT_EQ( 1, item->paramassign_num() );
  EXPECT_EQ( con, item->paramassign(0) );
  EXPECT_THROW( item->paramassign(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{con},
	     item->paramassign_list() );
  EXPECT_EQ( 0, item->defparam_num() );
  EXPECT_THROW( item->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDefParam*>{},
	     item->defparam_list() );
  EXPECT_EQ( 0, item->contassign_num() );
  EXPECT_THROW( item->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtContAssign*>{},
	     item->contassign_list() );
  EXPECT_EQ( 0, item->inst_num() );
  EXPECT_THROW( item->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtInst*>{},
	     item->inst_list() );
  EXPECT_FALSE( item->automatic() );
  EXPECT_EQ( 0, item->ioitem_num() );
  EXPECT_EQ( 0, item->iohead_num() );
  EXPECT_THROW( item->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtIOHead*>{},
	     item->iohead_list() );
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->item_list() );
  EXPECT_EQ( nullptr, item->body() );
  EXPECT_FALSE( item->is_signed() );
  EXPECT_EQ( nullptr, item->range() );
  EXPECT_EQ( VpiVarType::None, item->data_type() );
  EXPECT_THROW( item->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item->terminal_num() );
  EXPECT_THROW( item->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtExpr*>{},
	     item->terminal_list() );
  EXPECT_EQ( nullptr, item->path_decl() );
  EXPECT_EQ( nullptr, item->expr() );
  EXPECT_EQ( 0, item->then_declhead_num() );
  EXPECT_THROW( item->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->then_declhead_list() );
  EXPECT_EQ( 0, item->then_item_num() );
  EXPECT_THROW( item->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->then_item_list() );
  EXPECT_EQ( 0, item->else_declhead_num() );
  EXPECT_THROW( item->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtDeclHead*>{},
	     item->else_declhead_list() );
  EXPECT_EQ( 0, item->else_item_num() );
  EXPECT_THROW( item->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtItem*>{},
	     item->else_item_list() );
  EXPECT_EQ( 0, item->caseitem_num() );
  EXPECT_THROW( item->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtGenCaseItem*>{},
	     item->caseitem_list() );
  EXPECT_EQ( nullptr, item->loop_var() );
  EXPECT_EQ( nullptr, item->init_expr() );
  EXPECT_EQ( nullptr, item->next_expr() );
}

TEST_F(ParserTest, Inst)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr = parser.new_IntConst(fr1, 123U);
  auto con = parser.new_OrderedCon(expr);
  auto con_list = parser.new_list<const PtConnection>();
  con_list->push_back(con);
  parser.new_Inst(fr0, con_list);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_EQ( nullptr, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 1, inst->port_num() );
  EXPECT_EQ( con, inst->port(0) );
  EXPECT_THROW( inst->port(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{con},
	     inst->port_list() );
}

TEST_F(ParserTest, Inst1)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  parser.new_Inst(fr0, expr1);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_EQ( nullptr, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 1, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  EXPECT_THROW( inst->port(1),
		std::out_of_range );
}

TEST_F(ParserTest, Inst2)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  parser.new_Inst(fr0, expr1, expr2);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_EQ( nullptr, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 2, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  EXPECT_THROW( inst->port(2),
		std::out_of_range );
}

TEST_F(ParserTest, Inst3)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.new_IntConst(fr3, 456U);
  parser.new_Inst(fr0, expr1, expr2, expr3);

  auto fr4 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_EQ( nullptr, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 3, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  auto con3 = inst->port(2);
  EXPECT_EQ( nullptr, con3->name() );
  EXPECT_EQ( expr3, con3->expr() );
  EXPECT_THROW( inst->port(3),
		std::out_of_range );
}

TEST_F(ParserTest, Inst4)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 30, 1, 39);
  auto expr3 = parser.new_IntConst(fr3, 456U);
  auto fr4 = make_file_region(1, 40, 1, 49);
  auto expr4 = parser.new_IntConst(fr3, 456U);
  parser.new_Inst(fr0, expr1, expr2, expr3, expr4);

  auto fr5 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr5, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_EQ( nullptr, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 4, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  auto con3 = inst->port(2);
  EXPECT_EQ( nullptr, con3->name() );
  EXPECT_EQ( expr3, con3->expr() );
  auto con4 = inst->port(3);
  EXPECT_EQ( nullptr, con4->name() );
  EXPECT_EQ( expr4, con4->expr() );
  EXPECT_THROW( inst->port(4),
		std::out_of_range );
}

TEST_F(ParserTest, InstN)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr = parser.new_IntConst(fr1, 123U);
  auto con = parser.new_OrderedCon(expr);
  auto con_list = parser.new_list<const PtConnection>();
  con_list->push_back(con);
  parser.new_InstN(fr0, name, con_list);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 1, inst->port_num() );
  EXPECT_EQ( con, inst->port(0) );
  EXPECT_THROW( inst->port(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{con},
	     inst->port_list() );
}

TEST_F(ParserTest, InstN1)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  parser.new_InstN(fr0, name, expr1);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 1, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  EXPECT_THROW( inst->port(1),
		std::out_of_range );
}

TEST_F(ParserTest, InstN2)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  parser.new_InstN(fr0, name, expr1, expr2);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 2, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  EXPECT_THROW( inst->port(2),
		std::out_of_range );
}

TEST_F(ParserTest, InstN3)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto name = "inst_name";
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.new_IntConst(fr3, 456U);
  parser.new_InstN(fr0, name, expr1, expr2, expr3);

  auto fr4 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 3, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  auto con3 = inst->port(2);
  EXPECT_EQ( nullptr, con3->name() );
  EXPECT_EQ( expr3, con3->expr() );
  EXPECT_THROW( inst->port(3),
		std::out_of_range );
}

TEST_F(ParserTest, InstN4)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 30, 1, 39);
  auto expr3 = parser.new_IntConst(fr3, 456U);
  auto fr4 = make_file_region(1, 40, 1, 49);
  auto expr4 = parser.new_IntConst(fr3, 456U);
  parser.new_InstN(fr0, name, expr1, expr2, expr3, expr4);

  auto fr5 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr5, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( nullptr, inst->range() );
  EXPECT_EQ( 4, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  auto con3 = inst->port(2);
  EXPECT_EQ( nullptr, con3->name() );
  EXPECT_EQ( expr3, con3->expr() );
  auto con4 = inst->port(3);
  EXPECT_EQ( nullptr, con4->name() );
  EXPECT_EQ( expr4, con4->expr() );
  EXPECT_THROW( inst->port(4),
		std::out_of_range );
}

TEST_F(ParserTest, InstV)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto frl = make_file_region(1, 10, 1, 19);
  auto left = parser.new_IntConst(frl, 0U);
  auto frr = make_file_region(1, 20, 1, 29);
  auto right = parser.new_IntConst(frr, 15U);
  auto range = parser.new_Range(FileRegion(frl, frr), left, right);
  auto name = "inst_name";
  auto fr1 = make_file_region(1, 30, 1, 39);
  auto expr = parser.new_IntConst(fr1, 123U);
  auto con = parser.new_OrderedCon(expr);
  auto con_list = parser.new_list<const PtConnection>();
  con_list->push_back(con);
  parser.new_InstV(fr0, name, range, con_list);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( range, inst->range() );
  EXPECT_EQ( 1, inst->port_num() );
  EXPECT_EQ( con, inst->port(0) );
  EXPECT_THROW( inst->port(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const PtConnection*>{con},
	     inst->port_list() );
}

TEST_F(ParserTest, InstV1)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto left = parser.new_IntConst(frl, 0U);
  auto frr = make_file_region(1, 20, 1, 29);
  auto right = parser.new_IntConst(frr, 15U);
  auto range = parser.new_Range(FileRegion(frl, frr), left, right);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  parser.new_InstV(fr0, name, range, expr1);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( range, inst->range() );
  EXPECT_EQ( 1, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  EXPECT_THROW( inst->port(1),
		std::out_of_range );
}

TEST_F(ParserTest, InstV2)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto left = parser.new_IntConst(frl, 0U);
  auto frr = make_file_region(1, 20, 1, 29);
  auto right = parser.new_IntConst(frr, 15U);
  auto range = parser.new_Range(FileRegion(frl, frr), left, right);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  parser.new_InstV(fr0, name, range, expr1, expr2);

  auto fr3 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( range, inst->range() );
  EXPECT_EQ( 2, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  EXPECT_THROW( inst->port(2),
		std::out_of_range );
}

TEST_F(ParserTest, InstV3)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto left = parser.new_IntConst(frl, 0U);
  auto frr = make_file_region(1, 20, 1, 29);
  auto right = parser.new_IntConst(frr, 15U);
  auto range = parser.new_Range(FileRegion(frl, frr), left, right);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto expr3 = parser.new_IntConst(fr3, 456U);
  parser.new_InstV(fr0, name, range, expr1, expr2, expr3);

  auto fr4 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr3, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( range, inst->range() );
  EXPECT_EQ( 3, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  auto con3 = inst->port(2);
  EXPECT_EQ( nullptr, con3->name() );
  EXPECT_EQ( expr3, con3->expr() );
  EXPECT_THROW( inst->port(3),
		std::out_of_range );
}

TEST_F(ParserTest, InstV4)
{
  auto fr0 = make_file_region(1, 1, 1, 9);
  auto name = "inst_name";
  auto frl = make_file_region(1, 10, 1, 19);
  auto left = parser.new_IntConst(frl, 0U);
  auto frr = make_file_region(1, 20, 1, 29);
  auto right = parser.new_IntConst(frr, 15U);
  auto range = parser.new_Range(FileRegion(frl, frr), left, right);
  auto fr1 = make_file_region(1, 10, 1, 19);
  auto expr1 = parser.new_IntConst(fr1, 123U);
  auto fr2 = make_file_region(1, 20, 1, 29);
  auto expr2 = parser.new_IntConst(fr2, 456U);
  auto fr3 = make_file_region(1, 30, 1, 39);
  auto expr3 = parser.new_IntConst(fr3, 456U);
  auto fr4 = make_file_region(1, 40, 1, 49);
  auto expr4 = parser.new_IntConst(fr3, 456U);
  parser.new_InstV(fr0, name, range, expr1, expr2, expr3, expr4);

  auto fr5 = make_file_region(1, 2, 3, 4);
  auto def_name = "inst1";
  auto item = parser.new_MuH(fr5, def_name);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( 1, item->inst_num() );
  auto inst = item->inst(0);
  EXPECT_THROW( item->inst(1),
		std::out_of_range );
  ASSERT_TRUE( inst != nullptr );
  EXPECT_EQ( fr0, inst->file_region() );
  EXPECT_STREQ( name, inst->name() );
  EXPECT_EQ( range, inst->range() );
  EXPECT_EQ( 4, inst->port_num() );
  auto con1 = inst->port(0);
  EXPECT_EQ( nullptr, con1->name() );
  EXPECT_EQ( expr1, con1->expr() );
  auto con2 = inst->port(1);
  EXPECT_EQ( nullptr, con2->name() );
  EXPECT_EQ( expr2, con2->expr() );
  auto con3 = inst->port(2);
  EXPECT_EQ( nullptr, con3->name() );
  EXPECT_EQ( expr3, con3->expr() );
  auto con4 = inst->port(3);
  EXPECT_EQ( nullptr, con4->name() );
  EXPECT_EQ( expr4, con4->expr() );
  EXPECT_THROW( inst->port(4),
		std::out_of_range );
}

END_NAMESPACE_YM_VERILOG
