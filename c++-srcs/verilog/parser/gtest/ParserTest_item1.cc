
/// @file ParserTest_item1.cc
/// @brief ParserTest_item1 の実装ファイル
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

TEST_F(ParserTest, DefParamH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.new_DefParamH(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( PtItemType::DefParam, item->type() );
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

TEST_F(ParserTest, DefParam1)
{
  auto fr1 = make_file_region(1, 1, 1, 10);
  auto fr2 = make_file_region(1, 14, 1, 20);
  auto fr3 = make_file_region(1, 30, 1, 40);

  parser.init_defparam();
  auto val = parser.new_IntConst(fr3, 123U);
  auto name = "param1";
  parser.new_DefParam(fr2, name, val);
  auto item = parser.new_DefParamH(fr1);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( PtItemType::DefParam, item->type() );
  EXPECT_EQ( 1, item->defparam_num() );
  auto defparam = item->defparam(0);
  EXPECT_THROW( item->defparam(1),
		std::out_of_range );
  ASSERT_TRUE( defparam != nullptr );
  EXPECT_EQ( 0, defparam->namebranch_num() );
  EXPECT_THROW( defparam->namebranch(0),
		std::out_of_range );
  EXPECT_EQ( name, defparam->fullname() );
  EXPECT_EQ( std::vector<const PtNameBranch*>{},
	     defparam->namebranch_list() );
  EXPECT_EQ( val, defparam->expr() );
}

TEST_F(ParserTest, DefParam2)
{
  auto fr1 = make_file_region(1, 1, 1, 10);
  auto fr2 = make_file_region(1, 14, 1, 19);
  auto fr3 = make_file_region(1, 30, 1, 39);

  parser.init_defparam();
  auto val = parser.new_IntConst(fr3, 123U);
  auto head = "head";
  auto name = "param1";
  auto hname = parser.new_HierName(head, name);
  parser.new_DefParam(fr2, hname, val);
  auto item = parser.new_DefParamH(fr1);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( PtItemType::DefParam, item->type() );
  EXPECT_EQ( 1, item->defparam_num() );
  auto defparam = item->defparam(0);
  EXPECT_THROW( item->defparam(1),
		std::out_of_range );
  ASSERT_TRUE( defparam != nullptr );
  EXPECT_EQ( 1, defparam->namebranch_num() );
  auto branch = defparam->namebranch(0);
  EXPECT_STREQ( head, branch->name() );
  EXPECT_THROW( defparam->namebranch(1),
		std::out_of_range );
  EXPECT_EQ( "head.param1", defparam->fullname() );
  EXPECT_EQ( std::vector<const PtNameBranch*>{branch},
	     defparam->namebranch_list() );
  EXPECT_EQ( val, defparam->expr() );
}

TEST_F(ParserTest, ContAssignH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.new_ContAssignH(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( nullptr, item->name() );
  EXPECT_EQ( PtItemType::ContAssign, item->type() );
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

TEST_F(ParserTest, ContAssignHS)
{
  auto fr1 = make_file_region(1, 2, 1, 4);
  auto fr2 = make_file_region(1, 10, 1, 20);
  auto str = parser.new_Strength(fr2,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto item = parser.new_ContAssignH(fr1, str);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr1, item->file_region() );
  EXPECT_EQ( nullptr, item->name() );
  EXPECT_EQ( PtItemType::ContAssign, item->type() );
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

TEST_F(ParserTest, ContAssignHD)
{
  auto fr2 = make_file_region(1, 10, 1, 20);
  auto fr1 = make_file_region(1, 1, 1, 4);
  auto val = parser.new_IntConst(fr1, 1U);
  auto delay = parser.new_Delay(fr1, val);
  auto item = parser.new_ContAssignH(fr2, delay);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr2, item->file_region() );
  EXPECT_EQ( PtItemType::ContAssign, item->type() );
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

TEST_F(ParserTest, ContAssignHSD)
{
  auto fr1 = make_file_region(1, 2, 1, 4);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto str = parser.new_Strength(fr2,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto val = parser.new_IntConst(fr3, 1U);
  auto delay = parser.new_Delay(fr3, val);
  auto item = parser.new_ContAssignH(fr1, str, delay);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr1, item->file_region() );
  EXPECT_EQ( PtItemType::ContAssign, item->type() );
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

TEST_F(ParserTest, ContAssign)
{
  auto fr1 = make_file_region(1, 1, 1, 10);
  auto fr2 = make_file_region(1, 14, 1, 20);
  auto fr3 = make_file_region(1, 30, 1, 40);

  parser.init_contassign();
  auto name = "net1";
  auto lhs = parser.new_Primary(fr2, name);
  auto rhs = parser.new_IntConst(fr3, 1234U);
  auto ca_fr = FileRegion(fr2, fr3);
  parser.new_ContAssign(ca_fr, lhs, rhs);
  auto fr = FileRegion(fr1, fr3);
  auto item = parser.new_ContAssignH(fr);

  ASSERT_TRUE( item != nullptr );
  ASSERT_EQ( PtItemType::ContAssign, item->type() );
  EXPECT_EQ( 1, item->contassign_num() );
  auto ca = item->contassign(0);
  ASSERT_TRUE( ca != nullptr );
  EXPECT_THROW( item->contassign(1),
		std::out_of_range );
  EXPECT_EQ( ca_fr, ca->file_region() );
  EXPECT_EQ( lhs, ca->lhs() );
  EXPECT_EQ( rhs, ca->rhs() );
}

TEST_F(ParserTest, Initial)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto body = parser.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  auto item = parser.new_Initial(fr, body);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( nullptr, item->name() );
  EXPECT_EQ( PtItemType::Initial, item->type() );
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
  EXPECT_EQ( body, item->body() );
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

TEST_F(ParserTest, Always)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto body = parser.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  auto item = parser.new_Always(fr, body);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( nullptr, item->name() );
  EXPECT_EQ( PtItemType::Always, item->type() );
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
  EXPECT_EQ( body, item->body() );
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

END_NAMESPACE_YM_VERILOG
