
/// @file ParserTest.cc
/// @brief ParserTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ParserTest.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtItem.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, Generate)
{
  parser.init_generate();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto name = "net1";
  parser.new_DeclItem(fr1, name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_generate();
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_STREQ( nullptr, item->name() );
  EXPECT_EQ( AstItem::Generate, item->type() );
  EXPECT_THROW( item->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item->strength() );
  EXPECT_EQ( nullptr, item->delay() );
  EXPECT_EQ( 0, item->paramassign_num() );
  EXPECT_THROW( item->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item->paramassign_list() );
  EXPECT_EQ( 0, item->defparam_num() );
  EXPECT_THROW( item->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item->defparam_list() );
  EXPECT_EQ( 0, item->contassign_num() );
  EXPECT_THROW( item->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item->contassign_list() );
  EXPECT_EQ( 0, item->inst_num() );
  EXPECT_THROW( item->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item->inst_list() );
  EXPECT_FALSE( item->automatic() );
  EXPECT_EQ( 0, item->ioitem_num() );
  EXPECT_EQ( 0, item->iohead_num() );
  EXPECT_THROW( item->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item->iohead_list() );
  EXPECT_EQ( 1, item->declhead_num() );
  EXPECT_EQ( net_decl, item->declhead(0) );
  EXPECT_THROW( item->declhead(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
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
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item->terminal_list() );
  EXPECT_EQ( nullptr, item->path_decl() );
  EXPECT_EQ( nullptr, item->expr() );
  EXPECT_EQ( 0, item->then_declhead_num() );
  EXPECT_THROW( item->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item->then_declhead_list() );
  EXPECT_EQ( 0, item->then_item_num() );
  EXPECT_THROW( item->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item->then_item_list() );
  EXPECT_EQ( 0, item->else_declhead_num() );
  EXPECT_THROW( item->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item->else_declhead_list() );
  EXPECT_EQ( 0, item->else_item_num() );
  EXPECT_THROW( item->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item->else_item_list() );
  EXPECT_EQ( 0, item->caseitem_num() );
  EXPECT_THROW( item->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstGenCaseItem*>{},
	     item->caseitem_list() );
  EXPECT_EQ( nullptr, item->loop_var() );
  EXPECT_EQ( nullptr, item->init_expr() );
  EXPECT_EQ( nullptr, item->next_expr() );
}

TEST_F(ParserTest, GenBlock1)
{
  parser.init_generate(); // for generate
  parser.init_generate(); // for gen block
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto name = "net1";
  parser.new_DeclItem(fr1, name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  auto fr0 = make_file_region(1, 2, 3, 4);
  parser.end_generate(); // for gen block
  parser.new_GenBlock(fr0);
  parser.end_generate(); // for generate
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 1, item->item_num() );
  EXPECT_THROW( item->item(1),
		std::out_of_range );
  auto item0 = item->item(0);
  EXPECT_EQ( fr0, item0->file_region() );
  EXPECT_EQ( AstItem::GenBlock, item0->type() );
  EXPECT_EQ( nullptr, item0->name() );
  EXPECT_THROW( item0->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item0->strength() );
  EXPECT_EQ( nullptr, item0->delay() );
  EXPECT_EQ( 0, item0->paramassign_num() );
  EXPECT_THROW( item0->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item0->paramassign_list() );
  EXPECT_EQ( 0, item0->defparam_num() );
  EXPECT_THROW( item0->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item0->defparam_list() );
  EXPECT_EQ( 0, item0->contassign_num() );
  EXPECT_THROW( item0->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item0->contassign_list() );
  EXPECT_EQ( 0, item0->inst_num() );
  EXPECT_THROW( item0->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item0->inst_list() );
  EXPECT_FALSE( item0->automatic() );
  EXPECT_EQ( 0, item0->ioitem_num() );
  EXPECT_EQ( 0, item0->iohead_num() );
  EXPECT_THROW( item0->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item0->iohead_list() );
  EXPECT_EQ( 1, item0->declhead_num() );
  EXPECT_EQ( net_decl, item0->declhead(0) );
  EXPECT_THROW( item0->declhead(1),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl},
	     item0->declhead_list() );
  EXPECT_EQ( 0, item0->item_num() );
  EXPECT_THROW( item0->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->item_list() );
  EXPECT_EQ( nullptr, item0->body() );
  EXPECT_FALSE( item0->is_signed() );
  EXPECT_EQ( nullptr, item0->range() );
  EXPECT_EQ( VpiVarType::None, item0->data_type() );
  EXPECT_THROW( item0->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item0->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item0->terminal_num() );
  EXPECT_THROW( item0->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item0->terminal_list() );
  EXPECT_EQ( nullptr, item0->path_decl() );
  EXPECT_EQ( nullptr, item0->expr() );
  EXPECT_EQ( 0, item0->then_declhead_num() );
  EXPECT_THROW( item0->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->then_declhead_list() );
  EXPECT_EQ( 0, item0->then_item_num() );
  EXPECT_THROW( item0->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->then_item_list() );
  EXPECT_EQ( 0, item0->else_declhead_num() );
  EXPECT_THROW( item0->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->else_declhead_list() );
  EXPECT_EQ( 0, item0->else_item_num() );
  EXPECT_THROW( item0->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->else_item_list() );
  EXPECT_EQ( 0, item0->caseitem_num() );
  EXPECT_THROW( item0->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstGenCaseItem*>{},
	     item0->caseitem_list() );
  EXPECT_EQ( nullptr, item0->loop_var() );
  EXPECT_EQ( nullptr, item0->init_expr() );
  EXPECT_EQ( nullptr, item0->next_expr() );
}

TEST_F(ParserTest, GenBlock2)
{
  parser.init_generate();
  parser.init_generate();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(fr1, net_name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_generate();
  auto fr0 = make_file_region(1, 2, 3, 4);
  auto name = "block1";
  parser.new_GenBlock(fr0, name);
  parser.end_generate();
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 1, item->item_num() );
  EXPECT_THROW( item->item(1),
		std::out_of_range );
  auto item0 = item->item(0);
  EXPECT_EQ( fr0, item0->file_region() );
  EXPECT_EQ( AstItem::GenBlock, item0->type() );
  EXPECT_EQ( name, item0->name() );
  EXPECT_THROW( item0->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item0->strength() );
  EXPECT_EQ( nullptr, item0->delay() );
  EXPECT_EQ( 0, item0->paramassign_num() );
  EXPECT_THROW( item0->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item0->paramassign_list() );
  EXPECT_EQ( 0, item0->defparam_num() );
  EXPECT_THROW( item0->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item0->defparam_list() );
  EXPECT_EQ( 0, item0->contassign_num() );
  EXPECT_THROW( item0->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item0->contassign_list() );
  EXPECT_EQ( 0, item0->inst_num() );
  EXPECT_THROW( item0->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item0->inst_list() );
  EXPECT_FALSE( item0->automatic() );
  EXPECT_EQ( 0, item0->ioitem_num() );
  EXPECT_EQ( 0, item0->iohead_num() );
  EXPECT_THROW( item0->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item0->iohead_list() );
  EXPECT_EQ( 1, item0->declhead_num() );
  EXPECT_THROW( item0->declhead(1),
		std::out_of_range );
  EXPECT_EQ( net_decl, item0->declhead(0) );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl},
	     item0->declhead_list() );
  EXPECT_EQ( 0, item0->item_num() );
  EXPECT_THROW( item0->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->item_list() );
  EXPECT_EQ( nullptr, item0->body() );
  EXPECT_FALSE( item0->is_signed() );
  EXPECT_EQ( nullptr, item0->range() );
  EXPECT_EQ( VpiVarType::None, item0->data_type() );
  EXPECT_THROW( item0->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item0->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item0->terminal_num() );
  EXPECT_THROW( item0->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item0->terminal_list() );
  EXPECT_EQ( nullptr, item0->path_decl() );
  EXPECT_EQ( nullptr, item0->expr() );
  EXPECT_EQ( 0, item0->then_declhead_num() );
  EXPECT_THROW( item0->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->then_declhead_list() );
  EXPECT_EQ( 0, item0->then_item_num() );
  EXPECT_THROW( item0->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->then_item_list() );
  EXPECT_EQ( 0, item0->else_declhead_num() );
  EXPECT_THROW( item0->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->else_declhead_list() );
  EXPECT_EQ( 0, item0->else_item_num() );
  EXPECT_THROW( item0->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->else_item_list() );
  EXPECT_EQ( 0, item0->caseitem_num() );
  EXPECT_THROW( item0->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstGenCaseItem*>{},
	     item0->caseitem_list() );
  EXPECT_EQ( nullptr, item0->loop_var() );
  EXPECT_EQ( nullptr, item0->init_expr() );
  EXPECT_EQ( nullptr, item0->next_expr() );
}

TEST_F(ParserTest, GenIf)
{
  parser.init_generate();
  parser.init_genif();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(fr1, net_name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_genif();
  auto fr3 = make_file_region(1, 1, 1, 1);
  auto expr = parser.new_IntConst(fr3, 1U);
  auto fr0 = make_file_region(1, 2, 3, 4);
  parser.new_GenIf(fr0, expr);
  parser.end_generate();
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 1, item->item_num() );
  auto item0 = item->item(0);
  EXPECT_EQ( fr0, item0->file_region() );
  EXPECT_EQ( AstItem::GenIf, item0->type() );
  EXPECT_EQ( nullptr, item0->name() );
  EXPECT_THROW( item->item(1),
		std::out_of_range );
  EXPECT_THROW( item0->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item0->strength() );
  EXPECT_EQ( nullptr, item0->delay() );
  EXPECT_EQ( 0, item0->paramassign_num() );
  EXPECT_THROW( item0->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item0->paramassign_list() );
  EXPECT_EQ( 0, item0->defparam_num() );
  EXPECT_THROW( item0->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item0->defparam_list() );
  EXPECT_EQ( 0, item0->contassign_num() );
  EXPECT_THROW( item0->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item0->contassign_list() );
  EXPECT_EQ( 0, item0->inst_num() );
  EXPECT_THROW( item0->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item0->inst_list() );
  EXPECT_FALSE( item0->automatic() );
  EXPECT_EQ( 0, item0->ioitem_num() );
  EXPECT_EQ( 0, item0->iohead_num() );
  EXPECT_THROW( item0->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item0->iohead_list() );
  EXPECT_EQ( 0, item0->declhead_num() );
  EXPECT_THROW( item0->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->declhead_list() );
  EXPECT_EQ( 0, item0->item_num() );
  EXPECT_THROW( item0->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->item_list() );
  EXPECT_EQ( nullptr, item0->body() );
  EXPECT_FALSE( item0->is_signed() );
  EXPECT_EQ( nullptr, item0->range() );
  EXPECT_EQ( VpiVarType::None, item0->data_type() );
  EXPECT_THROW( item0->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item0->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item0->terminal_num() );
  EXPECT_THROW( item0->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item0->terminal_list() );
  EXPECT_EQ( nullptr, item0->path_decl() );
  EXPECT_EQ( expr, item0->expr() );
  EXPECT_EQ( 1, item0->then_declhead_num() );
  EXPECT_THROW( item0->then_declhead(1),
		std::out_of_range );
  EXPECT_EQ( net_decl, item0->then_declhead(0) );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl},
	     item0->then_declhead_list() );
  EXPECT_EQ( 0, item0->then_item_num() );
  EXPECT_THROW( item0->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->then_item_list() );
  EXPECT_EQ( 0, item0->else_declhead_num() );
  EXPECT_THROW( item0->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->else_declhead_list() );
  EXPECT_EQ( 0, item0->else_item_num() );
  EXPECT_THROW( item0->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->else_item_list() );
  EXPECT_EQ( 0, item0->caseitem_num() );
  EXPECT_THROW( item0->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstGenCaseItem*>{},
	     item0->caseitem_list() );
  EXPECT_EQ( nullptr, item0->loop_var() );
  EXPECT_EQ( nullptr, item0->init_expr() );
  EXPECT_EQ( nullptr, item0->next_expr() );
}

TEST_F(ParserTest, GenIfElse)
{
  parser.init_generate();
  parser.init_genif();
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(fr1, net_name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_genif();
  parser.init_generate();
  auto fr3 = make_file_region(3, 3, 3, 3);
  auto net_name2 = "net2";
  parser.new_DeclItem(fr3, net_name2);
  auto fr4 = make_file_region(3, 3, 4, 4);
  auto net_decl2 = parser.new_NetH(fr4, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl2, nullptr);
  parser.end_genelse();
  auto fr5 = make_file_region(1, 1, 1, 1);
  auto expr = parser.new_IntConst(fr5, 1U);
  auto fr0 = make_file_region(1, 2, 3, 4);
  parser.new_GenIfElse(fr0, expr);
  parser.end_generate();
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 1, item->item_num() );
  auto item0 = item->item(0);
  EXPECT_EQ( fr0, item0->file_region() );
  EXPECT_EQ( AstItem::GenIf, item0->type() );
  EXPECT_EQ( nullptr, item0->name() );
  EXPECT_THROW( item->item(1),
		std::out_of_range );
  EXPECT_THROW( item0->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item0->strength() );
  EXPECT_EQ( nullptr, item0->delay() );
  EXPECT_EQ( 0, item0->paramassign_num() );
  EXPECT_THROW( item0->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item0->paramassign_list() );
  EXPECT_EQ( 0, item0->defparam_num() );
  EXPECT_THROW( item0->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item0->defparam_list() );
  EXPECT_EQ( 0, item0->contassign_num() );
  EXPECT_THROW( item0->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item0->contassign_list() );
  EXPECT_EQ( 0, item0->inst_num() );
  EXPECT_THROW( item0->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item0->inst_list() );
  EXPECT_FALSE( item0->automatic() );
  EXPECT_EQ( 0, item0->ioitem_num() );
  EXPECT_EQ( 0, item0->iohead_num() );
  EXPECT_THROW( item0->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item0->iohead_list() );
  EXPECT_EQ( 0, item0->declhead_num() );
  EXPECT_THROW( item0->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->declhead_list() );
  EXPECT_EQ( 0, item0->item_num() );
  EXPECT_THROW( item0->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->item_list() );
  EXPECT_EQ( nullptr, item0->body() );
  EXPECT_FALSE( item0->is_signed() );
  EXPECT_EQ( nullptr, item0->range() );
  EXPECT_EQ( VpiVarType::None, item0->data_type() );
  EXPECT_THROW( item0->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item0->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item0->terminal_num() );
  EXPECT_THROW( item0->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item0->terminal_list() );
  EXPECT_EQ( nullptr, item0->path_decl() );
  EXPECT_EQ( expr, item0->expr() );
  EXPECT_EQ( 1, item0->then_declhead_num() );
  EXPECT_THROW( item0->then_declhead(1),
		std::out_of_range );
  EXPECT_EQ( net_decl, item0->then_declhead(0) );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl},
	     item0->then_declhead_list() );
  EXPECT_EQ( 0, item0->then_item_num() );
  EXPECT_THROW( item0->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->then_item_list() );
  EXPECT_EQ( 1, item0->else_declhead_num() );
  EXPECT_THROW( item0->else_declhead(1),
		std::out_of_range );
  EXPECT_EQ( net_decl2, item0->else_declhead(0) );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl2},
	     item0->else_declhead_list() );
  EXPECT_EQ( 0, item0->else_item_num() );
  EXPECT_THROW( item0->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->else_item_list() );
  EXPECT_EQ( 0, item0->caseitem_num() );
  EXPECT_THROW( item0->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstGenCaseItem*>{},
	     item0->caseitem_list() );
  EXPECT_EQ( nullptr, item0->loop_var() );
  EXPECT_EQ( nullptr, item0->init_expr() );
  EXPECT_EQ( nullptr, item0->next_expr() );
}

TEST_F(ParserTest, GenCase)
{
  parser.init_generate();

  auto caseitem_list = parser.new_gencaseitem_list();
  parser.init_generate(); // for gen caseitem
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(fr1, net_name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_generate(); // for gen caseitem
  auto fr3 = make_file_region(1, 1, 1, 1);
  auto expr = parser.new_IntConst(fr3, 1U);
  auto expr_list = parser.new_expr_list();
  expr_list->push_back(expr);
  auto fr0 = make_file_region(1, 2, 3, 4);
  auto caseitem0 = parser.new_GenCaseItem(fr0, expr_list);
  caseitem_list->push_back(caseitem0);

  parser.init_generate();
  parser.end_generate();
  auto fr4 = make_file_region(4, 4, 4, 4);
  // default の場合
  auto null_list = parser.new_expr_list();
  auto caseitem1 = parser.new_GenCaseItem(fr4, null_list);
  caseitem_list->push_back(caseitem1);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto case_expr = parser.new_IntConst(fr5, 123U);
  auto fr6 = make_file_region(6, 6, 6, 6);
  parser.new_GenCase(fr6, case_expr, caseitem_list);
  parser.end_generate();
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 1, item->item_num() );
  auto item0 = item->item(0);
  EXPECT_EQ( fr6, item0->file_region() );
  EXPECT_EQ( AstItem::GenCase, item0->type() );
  EXPECT_EQ( nullptr, item0->name() );
  EXPECT_THROW( item->item(1),
		std::out_of_range );
  EXPECT_THROW( item0->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item0->strength() );
  EXPECT_EQ( nullptr, item0->delay() );
  EXPECT_EQ( 0, item0->paramassign_num() );
  EXPECT_THROW( item0->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item0->paramassign_list() );
  EXPECT_EQ( 0, item0->defparam_num() );
  EXPECT_THROW( item0->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item0->defparam_list() );
  EXPECT_EQ( 0, item0->contassign_num() );
  EXPECT_THROW( item0->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item0->contassign_list() );
  EXPECT_EQ( 0, item0->inst_num() );
  EXPECT_THROW( item0->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item0->inst_list() );
  EXPECT_FALSE( item0->automatic() );
  EXPECT_EQ( 0, item0->ioitem_num() );
  EXPECT_EQ( 0, item0->iohead_num() );
  EXPECT_THROW( item0->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item0->iohead_list() );
  EXPECT_EQ( 0, item0->declhead_num() );
  EXPECT_THROW( item0->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->declhead_list() );
  EXPECT_EQ( 0, item0->item_num() );
  EXPECT_THROW( item0->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->item_list() );
  EXPECT_EQ( nullptr, item0->body() );
  EXPECT_FALSE( item0->is_signed() );
  EXPECT_EQ( nullptr, item0->range() );
  EXPECT_EQ( VpiVarType::None, item0->data_type() );
  EXPECT_THROW( item0->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item0->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item0->terminal_num() );
  EXPECT_THROW( item0->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item0->terminal_list() );
  EXPECT_EQ( nullptr, item0->path_decl() );
  EXPECT_EQ( case_expr, item0->expr() );
  EXPECT_EQ( 0, item0->then_declhead_num() );
  EXPECT_THROW( item0->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->then_declhead_list() );
  EXPECT_EQ( 0, item0->then_item_num() );
  EXPECT_THROW( item0->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->then_item_list() );
  EXPECT_EQ( 0, item0->else_declhead_num() );
  EXPECT_THROW( item0->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->else_declhead_list() );
  EXPECT_EQ( 0, item0->else_item_num() );
  EXPECT_THROW( item0->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->else_item_list() );
  EXPECT_EQ( 2, item0->caseitem_num() );
  EXPECT_THROW( item0->caseitem(2),
		std::out_of_range );
  EXPECT_EQ( caseitem0, item0->caseitem(0) );
  EXPECT_EQ( caseitem1, item0->caseitem(1) );
  auto exp_caseitem_list = std::vector<const AstGenCaseItem*>{caseitem0, caseitem1};
  EXPECT_EQ( exp_caseitem_list, item0->caseitem_list() );
  EXPECT_EQ( nullptr, item0->loop_var() );
  EXPECT_EQ( nullptr, item0->init_expr() );
  EXPECT_EQ( nullptr, item0->next_expr() );
}

TEST_F(ParserTest, GenFor)
{
  parser.init_generate(); // for generate
  parser.init_generate(); // for gen for
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(fr1, net_name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_generate(); // for gen for
  auto fr3 = make_file_region(1, 1, 1, 1);
  auto init_expr = parser.new_IntConst(fr3, 1U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto cond_expr = parser.new_IntConst(fr4, 2U);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto next_expr = parser.new_IntConst(fr5, 3U);
  auto varname = "i";
  auto blockname = "block1";
  auto fr0 = make_file_region(1, 2, 3, 4);
  parser.new_GenFor(fr0, varname, init_expr, cond_expr,
		    varname, next_expr, blockname);
  parser.end_generate(); // for generate
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 1, item->item_num() );
  auto item0 = item->item(0);
  EXPECT_EQ( fr0, item0->file_region() );
  EXPECT_EQ( AstItem::GenFor, item0->type() );
  EXPECT_STREQ( blockname, item0->name() );
  EXPECT_THROW( item->item(1),
		std::out_of_range );
  EXPECT_THROW( item0->prim_type(),
		std::logic_error );
  EXPECT_EQ( nullptr, item0->strength() );
  EXPECT_EQ( nullptr, item0->delay() );
  EXPECT_EQ( 0, item0->paramassign_num() );
  EXPECT_THROW( item0->paramassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstConnection*>{},
	     item0->paramassign_list() );
  EXPECT_EQ( 0, item0->defparam_num() );
  EXPECT_THROW( item0->defparam(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDefParam*>{},
	     item0->defparam_list() );
  EXPECT_EQ( 0, item0->contassign_num() );
  EXPECT_THROW( item0->contassign(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstContAssign*>{},
	     item0->contassign_list() );
  EXPECT_EQ( 0, item0->inst_num() );
  EXPECT_THROW( item0->inst(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstInst*>{},
	     item0->inst_list() );
  EXPECT_FALSE( item0->automatic() );
  EXPECT_EQ( 0, item0->ioitem_num() );
  EXPECT_EQ( 0, item0->iohead_num() );
  EXPECT_THROW( item0->iohead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstIOHead*>{},
	     item0->iohead_list() );
  EXPECT_EQ( 1, item0->declhead_num() );
  EXPECT_THROW( item0->declhead(1),
		std::out_of_range );
  EXPECT_EQ( net_decl, item0->declhead(0) );
  EXPECT_EQ( std::vector<const AstDeclHead*>{net_decl},
	     item0->declhead_list() );
  EXPECT_EQ( 0, item0->item_num() );
  EXPECT_THROW( item0->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->item_list() );
  EXPECT_EQ( nullptr, item0->body() );
  EXPECT_FALSE( item0->is_signed() );
  EXPECT_EQ( nullptr, item0->range() );
  EXPECT_EQ( VpiVarType::None, item0->data_type() );
  EXPECT_THROW( item0->specitem_type(),
		std::logic_error );
  EXPECT_THROW( item0->specpath_type(),
		std::logic_error );
  EXPECT_EQ( 0, item0->terminal_num() );
  EXPECT_THROW( item0->terminal(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstExpr*>{},
	     item0->terminal_list() );
  EXPECT_EQ( nullptr, item0->path_decl() );
  EXPECT_EQ( cond_expr, item0->expr() );
  EXPECT_EQ( 0, item0->then_declhead_num() );
  EXPECT_THROW( item0->then_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->then_declhead_list() );
  EXPECT_EQ( 0, item0->then_item_num() );
  EXPECT_THROW( item0->then_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->then_item_list() );
  EXPECT_EQ( 0, item0->else_declhead_num() );
  EXPECT_THROW( item0->else_declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item0->else_declhead_list() );
  EXPECT_EQ( 0, item0->else_item_num() );
  EXPECT_THROW( item0->else_item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
	     item0->else_item_list() );
  EXPECT_EQ( 0, item0->caseitem_num() );
  EXPECT_THROW( item0->caseitem(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstGenCaseItem*>{},
	     item0->caseitem_list() );
  EXPECT_STREQ( varname, item0->loop_var() );
  EXPECT_EQ( init_expr, item0->init_expr() );
  EXPECT_EQ( next_expr, item0->next_expr() );
}

TEST_F(ParserTest, GenFor_bad)
{
  parser.init_generate(); // for generate
  parser.init_generate(); // for gen for
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto net_name = "net1";
  parser.new_DeclItem(fr1, net_name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = parser.new_NetH(fr2, VpiNetType::Wire, false);
  parser.add_decl_head(net_decl, nullptr);
  parser.end_generate(); // for gen for
  auto fr3 = make_file_region(1, 1, 1, 1);
  auto init_expr = parser.new_IntConst(fr3, 1U);
  auto fr4 = make_file_region(4, 4, 4, 4);
  auto cond_expr = parser.new_IntConst(fr4, 2U);
  auto fr5 = make_file_region(5, 5, 5, 5);
  auto next_expr = parser.new_IntConst(fr5, 3U);
  auto varname = "i";
  auto nextname = "j";
  auto blockname = "block1";
  auto fr0 = make_file_region(1, 2, 3, 4);
  parser.new_GenFor(fr0, varname, init_expr, cond_expr,
		    nextname, next_expr, blockname);
  parser.end_generate(); // for generate
  auto fr = make_file_region(1, 2, 5, 6);
  auto item = parser.new_Generate(fr);
  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( 0, item->item_num() );
}

END_NAMESPACE_YM_VERILOG
