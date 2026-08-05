
/// @file ParserTest_item2.cc
/// @brief ParserTest_item2 の実装ファイル
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

TEST_F(ParserTest, Task)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto body = parser.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  for ( auto automatic: { true, false } ) {
    auto name = "task1";
    auto item = parser.new_Task(fr, name, automatic, body);

    ASSERT_TRUE( item != nullptr );
    EXPECT_EQ( fr, item->file_region() );
    EXPECT_STREQ( name, item->name() );
    EXPECT_EQ( PtItemType::Task, item->type() );
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
    EXPECT_EQ( automatic, item->automatic() );
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
}

TEST_F(ParserTest, Function)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto name = "func1";
  auto body = parser.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  for ( auto automatic: { true, false } ) {
    for ( auto sign: { true, false } ) {
      auto item = parser.new_Function(fr, name, automatic, sign, body);

      ASSERT_TRUE( item != nullptr );
      EXPECT_EQ( fr, item->file_region() );
      EXPECT_STREQ( name, item->name() );
      EXPECT_EQ( PtItemType::Func, item->type() );
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
      EXPECT_EQ( automatic, item->automatic() );
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
      EXPECT_EQ( sign, item->is_signed() );
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
  }
}

TEST_F(ParserTest, SizedFunc)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto fr4 = make_file_region(2, 10, 2, 19);
  auto name = "func1";
  auto left = parser.new_IntConst(fr2, 31U);
  auto right = parser.new_IntConst(fr3, 0U);
  auto range = parser.new_Range(FileRegion(fr2, fr3), left, right);
  auto body = parser.new_NullStmt(fr4);
  auto fr = FileRegion(fr1, fr4);
  for ( auto automatic: { true, false } ) {
    for ( auto sign: { true, false } ) {
      auto item = parser.new_SizedFunc(fr, name, automatic, sign, range, body);

      ASSERT_TRUE( item != nullptr );
      EXPECT_EQ( fr, item->file_region() );
      EXPECT_STREQ( name, item->name() );
      EXPECT_EQ( PtItemType::Func, item->type() );
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
      EXPECT_EQ( automatic, item->automatic() );
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
      EXPECT_EQ( sign, item->is_signed() );
      EXPECT_EQ( range, item->range() );
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
  }
}

TEST_F(ParserTest, TypedFunc)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr4 = make_file_region(2, 10, 2, 19);
  auto name = "func1";
  auto body = parser.new_NullStmt(fr4);
  auto fr = FileRegion(fr1, fr4);
  for ( auto automatic: { true, false } ) {
    for ( auto sign: { true, false } ) {
      for ( auto type: { VpiVarType::Integer, VpiVarType::Real,
			 VpiVarType::Time, VpiVarType::Realtime } ) {
	auto item = parser.new_TypedFunc(fr, name, automatic, sign, type, body);

	ASSERT_TRUE( item != nullptr );
	EXPECT_EQ( fr, item->file_region() );
	EXPECT_STREQ( name, item->name() );
	EXPECT_EQ( PtItemType::Func, item->type() );
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
	EXPECT_EQ( automatic, item->automatic() );
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
	EXPECT_EQ( sign, item->is_signed() );
	EXPECT_EQ( nullptr, item->range() );
	EXPECT_EQ( type, item->data_type() );
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
    }
  }
}

TEST_F(ParserTest, GateH)
{
  auto fr = make_file_region(1, 1, 2, 2);
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto item = parser.new_GateH(fr, type);

    ASSERT_TRUE( item != nullptr );
    EXPECT_EQ( fr, item->file_region() );
    EXPECT_EQ( nullptr, item->name() );
    EXPECT_EQ( PtItemType::GateInst, item->type() );
    EXPECT_EQ( type, item->prim_type() );
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
}

TEST_F(ParserTest, GateHS)
{
  auto fr1 = make_file_region(1, 2, 3, 4);
  auto str = parser.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr = make_file_region(1, 1, 2, 2);
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto item = parser.new_GateH(fr, type, str);

    ASSERT_TRUE( item != nullptr );
    EXPECT_EQ( fr, item->file_region() );
    EXPECT_EQ( nullptr, item->name() );
    EXPECT_EQ( PtItemType::GateInst, item->type() );
    EXPECT_EQ( type, item->prim_type() );
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
}

TEST_F(ParserTest, GateHD)
{
  auto fr1 = make_file_region(1, 2, 3, 4);
  auto val = parser.new_IntConst(fr1, 1U);
  auto delay = parser.new_Delay(fr1, val);
  auto fr = make_file_region(1, 1, 2, 2);
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto item = parser.new_GateH(fr, type, delay);

    ASSERT_TRUE( item != nullptr );
    EXPECT_EQ( fr, item->file_region() );
    EXPECT_EQ( nullptr, item->name() );
    EXPECT_EQ( PtItemType::GateInst, item->type() );
    EXPECT_EQ( type, item->prim_type() );
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
}

TEST_F(ParserTest, GateHSD)
{
  auto fr1 = make_file_region(1, 2, 3, 4);
  auto str = parser.new_Strength(fr1,
				 VpiStrength::SupplyDrive,
				 VpiStrength::StrongDrive);
  auto fr2 = make_file_region(5, 6, 7, 8);
  auto val = parser.new_IntConst(fr2, 1U);
  auto delay = parser.new_Delay(fr2, val);
  auto fr = make_file_region(1, 1, 2, 2);
  for ( auto type: { VpiPrimType::And, VpiPrimType::Nand, VpiPrimType::Nor,
	VpiPrimType::Or, VpiPrimType::Xor, VpiPrimType::Xnor,
	VpiPrimType::Buf, VpiPrimType::Not, VpiPrimType::Bufif0,
	VpiPrimType::Bufif1, VpiPrimType::Notif0, VpiPrimType::Notif1,
	VpiPrimType::Nmos, VpiPrimType::Pmos, VpiPrimType::Cmos,
	VpiPrimType::Rnmos, VpiPrimType::Rpmos, VpiPrimType::Rcmos,
	VpiPrimType::Rtran, VpiPrimType::Rtranif0, VpiPrimType::Rtranif1,
	VpiPrimType::Tran, VpiPrimType::Tranif0, VpiPrimType::Tranif1,
	VpiPrimType::Pullup, VpiPrimType::Pulldown,
	VpiPrimType::Seq, VpiPrimType::Comb, VpiPrimType::Cell } ) {
    auto item = parser.new_GateH(fr, type, str, delay);

    ASSERT_TRUE( item != nullptr );
    EXPECT_EQ( fr, item->file_region() );
    EXPECT_EQ( nullptr, item->name() );
    EXPECT_EQ( PtItemType::GateInst, item->type() );
    EXPECT_EQ( type, item->prim_type() );
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
}

END_NAMESPACE_YM_VERILOG
