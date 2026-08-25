
/// @file ParserTest_item1.cc
/// @brief ParserTest_item1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest_Item.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest_Item, DefParam0)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.factory().new_DefParamH(fr, {});

  check_item_DefParam(item,
		      std::vector<const AstDefParam*>{});
}

TEST_F(ParserTest_Item, DefParam1)
{
  auto fr1 = make_file_region(1, 1, 1, 10);
  auto fr2 = make_file_region(1, 14, 1, 20);
  auto fr3 = make_file_region(1, 30, 1, 40);

  auto val = parser.factory().new_IntConst(fr3, 123U);
  auto name = "param1";
  auto defparam1 = parser.factory().new_DefParam(fr2, name, val);
  auto item = parser.factory().new_DefParamH(fr1, defparam1);

  check_item_DefParam(item,
		      std::vector<const AstDefParam*>{defparam1});
}

#if 0
TEST_F(ParserTest_Item, DefParam2)
{
  auto fr1 = make_file_region(1, 1, 1, 10);
  auto fr2 = make_file_region(1, 14, 1, 19);
  auto fr3 = make_file_region(1, 30, 1, 39);

  parser.init_defparam_list();
  auto val = parser.factory().new_IntConst(fr3, 123U);
  auto head = "head";
  auto name = "param1";
  auto hname = parser.new_HierName(head, name);
  auto defparam1 = parser.factory().new_DefParam(fr2, hname, val);
  parser.add_defparam(defparam1);
  auto item = parser.factory().new_DefParamH(fr1, parser.defparam_list());

  check_item_DefParam(item,
		      std::vector<const AstDefParam*>{defparam1});

  EXPECT_EQ( 1, defparam1->namebranch_num() );
  auto branch = defparam1->namebranch_list().front();
  EXPECT_STREQ( head, branch->name() );
  EXPECT_EQ( "head.param1", defparam1->decompile_name() );
  EXPECT_EQ( std::vector<const AstNameBranch*>{branch},
	     defparam1->namebranch_list() );
  EXPECT_EQ( val, defparam1->expr() );
}

TEST_F(ParserTest_Item, ContAssignH)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto item = parser.factory().new_ContAssignH(fr, {});

  check_item_ContAssign(item,
			nullptr,
			nullptr,
			std::vector<const AstContAssign*>{});
}

TEST_F(ParserTest_Item, ContAssignHS)
{
  auto fr1 = make_file_region(1, 2, 1, 4);
  auto fr2 = make_file_region(1, 10, 1, 20);
  auto str = parser.factory().new_Strength(fr2,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  auto item = parser.factory().new_ContAssignH(fr1, str, {});

  check_item_ContAssign(item,
			str,
			nullptr,
			std::vector<const AstContAssign*>{});
}

TEST_F(ParserTest_Item, ContAssignHD)
{
  auto fr2 = make_file_region(1, 10, 1, 20);
  auto fr1 = make_file_region(1, 1, 1, 4);
  auto val = parser.factory().new_IntConst(fr1, 1U);
  auto delay = parser.factory().new_Delay(fr1, val);
  auto item = parser.factory().new_ContAssignH(fr2, delay, {});

  check_item_ContAssign(item,
			nullptr,
			delay,
			std::vector<const AstContAssign*>{});
}

TEST_F(ParserTest_Item, ContAssignHSD)
{
  auto fr1 = make_file_region(1, 2, 1, 4);
  auto fr2 = make_file_region(1, 10, 1, 19);
  auto str = parser.factory().new_Strength(fr2,
					   VpiStrength::SupplyDrive,
					   VpiStrength::StrongDrive);
  auto fr3 = make_file_region(1, 20, 1, 29);
  auto val = parser.factory().new_IntConst(fr3, 1U);
  auto delay = parser.factory().new_Delay(fr3, val);
  auto item = parser.factory().new_ContAssignH(fr1, str, delay, {});

  check_item_ContAssign(item,
			str,
			delay,
			std::vector<const AstContAssign*>{});
}

TEST_F(ParserTest_Item, ContAssign)
{
  auto fr1 = make_file_region(1, 1, 1, 10);
  auto fr2 = make_file_region(1, 14, 1, 20);
  auto fr3 = make_file_region(1, 30, 1, 40);

  parser.init_contassign_list();
  auto name = "net1";
  auto lhs = parser.factory().new_Primary(fr2, name);
  auto rhs = parser.factory().new_IntConst(fr3, 1234U);
  auto ca_fr = FileRegion(fr2, fr3);
  auto ca = parser.factory().new_ContAssign(ca_fr, lhs, rhs);
  parser.add_contassign(ca);
  auto fr = FileRegion(fr1, fr3);
  auto item = parser.factory().new_ContAssignH(fr, parser.contassign_list());

  check_item_ContAssign(item,
			nullptr,
			nullptr,
			AstContAssignVec{ca});

  EXPECT_EQ( ca_fr, ca->file_region() );
  EXPECT_EQ( lhs, ca->lhs() );
  EXPECT_EQ( rhs, ca->rhs() );
}

TEST_F(ParserTest_Item, Initial)
{
  auto fr1 = make_file_region(1, 1, 1, 9);
  auto fr2 = make_file_region(2, 10, 2, 19);
  auto body = parser.new_NullStmt(fr2);
  auto fr = FileRegion(fr1, fr2);
  auto item = parser.new_Initial(fr, body);

  ASSERT_TRUE( item != nullptr );
  EXPECT_EQ( fr, item->file_region() );
  EXPECT_EQ( nullptr, item->name() );
  EXPECT_EQ( AstItem::Initial, item->type() );
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
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
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
  EXPECT_EQ( AstItem::Always, item->type() );
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
  EXPECT_EQ( 0, item->declhead_num() );
  EXPECT_THROW( item->declhead(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstDeclHead*>{},
	     item->declhead_list() );
  EXPECT_EQ( 0, item->item_num() );
  EXPECT_THROW( item->item(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const AstItem*>{},
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
#endif

END_NAMESPACE_YM_VERILOG
