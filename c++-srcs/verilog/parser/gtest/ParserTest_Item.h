#ifndef PARSERTEST_ITEM_H
#define PARSERTEST_ITEM_H

/// @file ParserTest_Item.h
/// @brief ParserTest_Item のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ParserTest_Item ParserTest_Item.h "ParserTest_Item.h"
/// @brief ParserTest の AstItem 用のテスト環境
//////////////////////////////////////////////////////////////////////
class ParserTest_Item :
  public ParserTest
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // DefParam 型のテスト
  void
  check_item_DefParam(
    const AstItem* item,
    const std::vector<const AstDefParam*>& defparam_list
  )
  {
    check_item_common(item, AstItem::DefParam);
    EXPECT_EQ( defparam_list.size(), item->defparam_num() );
    EXPECT_EQ( defparam_list, item->defparam_list() );
  }

  // ContAssign 型のテスト
  void
  check_item_ContAssign(
    const AstItem* item,
    const AstStrength* strength,
    const AstDelay* delay,
    const std::vector<const AstContAssign*>& ca_list
  )
  {
    check_item_common(item, AstItem::ContAssign);
    EXPECT_EQ( strength, item->strength() );
    EXPECT_EQ( delay, item->delay() );
    EXPECT_EQ( ca_list.size(), item->contassign_num() );
    EXPECT_EQ( ca_list, item->contassign_list() );
  }

  // Initial のテスト
  void
  check_item_Initial(
    const AstItem* item,
    const AstStmt* body
  )
  {
    check_item_common(item, AstItem::Initial);
    EXPECT_EQ( body, item->body() );
  }

  // Always のテスト
  void
  check_item_Always(
    const AstItem* item,
    const AstStmt* body
  )
  {
    check_item_common(item, AstItem::Always);
    EXPECT_EQ( body, item->body() );
  }

  // Task のテスト
  void
  check_item_Task(
    const AstItem* item,
    bool automatic,
    const std::vector<const AstIOHead*>& iohead_list,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list,
    const AstStmt* body
  )
  {
    check_item_common(item, AstItem::Task);
    EXPECT_EQ( body, item->body() );
    check_item_container(item, declhead_list, item_list);
    EXPECT_EQ( automatic, item->automatic() );
    EXPECT_EQ( iohead_list.size(), item->iohead_num() );
    EXPECT_EQ( iohead_list, item->iohead_list() );
  }

  // Func のテスト
  void
  check_item_Func(
    const AstItem* item,
    bool is_signed,
    const AstRange* range,
    VpiVarType data_type,
    bool automatic,
    const std::vector<const AstIOHead*>& iohead_list,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list,
    const AstStmt* body
  )
  {
    check_item_common(item, AstItem::Func);
    EXPECT_EQ( body, item->body() );
    check_item_container(item, declhead_list, item_list);
    EXPECT_EQ( automatic, item->automatic() );
    EXPECT_EQ( iohead_list.size(), item->iohead_num() );
    EXPECT_EQ( iohead_list, item->iohead_list() );
    EXPECT_EQ( is_signed, item->is_signed() );
    EXPECT_EQ( range, item->range() );
    EXPECT_EQ( data_type, item->data_type() );
  }

  /// GateInst 型のテスト
  void
  check_item_gateinst(
    const AstItem* item,
    VpiPrimType prim_type,
    const std::vector<const AstInst*>& inst_list,
    const AstStrength* strength = nullptr,
    const AstDelay* delay = nullptr
  )
  {
    check_item_common(item, AstItem::GateInst);
    EXPECT_EQ( prim_type, item->prim_type() );
    EXPECT_EQ( strength, item->strength() );
    EXPECT_EQ( delay, item->delay() );
    EXPECT_EQ( inst_list.size(), item->inst_num() );
    EXPECT_EQ( inst_list, item->inst_list() );
  }

  // MuInst のテスト
  void
  check_item_MuInst(
    const AstItem* item,
    const std::vector<const AstConnection*>& con_list,
    const std::vector<const AstInst*>& inst_list
  )
  {
    check_item_common(item, AstItem::MuInst);
    EXPECT_EQ( con_list.size(), item->paramassign_num() );
    EXPECT_EQ( con_list, item->paramassign_list() );
    EXPECT_EQ( inst_list.size(), item->inst_num() );
    EXPECT_EQ( inst_list, item->inst_list() );
  }

  // SpecItem のテスト
  void
  check_item_SpecItem(
    const AstItem* item,
    VpiSpecItemType specitem_type,
    const std::vector<const AstExpr*>& terminal_list
  )
  {
    check_item_common(item, AstItem::SpecItem);
    EXPECT_EQ( specitem_type, item->specitem_type() );
    EXPECT_EQ( terminal_list.size(), item->terminal_num() );
    EXPECT_EQ( terminal_list, item->terminal_list() );
  }

  // SpecPath のテスト
  void
  check_item_SpecPath(
    const AstItem* item,
    VpiSpecPathType specpath_type,
    const AstExpr* expr,
    const AstPathDecl* path_decl
  )
  {
    check_item_common(item, AstItem::SpecPath);
    EXPECT_EQ( specpath_type, item->specpath_type() );
    EXPECT_EQ( path_decl, item->path_decl() );
    EXPECT_EQ( expr, item->cond_expr() );
  }

  // Generate のテスト
  void
  check_item_Generate(
    const AstItem* item,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list
  )
  {
    check_item_common(item, AstItem::Generate);
    check_item_container(item, declhead_list, item_list);
  }

  // GenBlock のテスト
  void
  check_item_GenBlock(
    const AstItem* item,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list
  )
  {
    check_item_common(item, AstItem::GenBlock);
    check_item_container(item, declhead_list, item_list);
  }

  // GenIf のテスト
  void
  check_item_GenIf(
    const AstItem* item,
    const std::vector<const AstDeclHead*>& then_declhead_list,
    const std::vector<const AstItem*>& then_item_list,
    const std::vector<const AstDeclHead*>& else_declhead_list,
    const std::vector<const AstItem*>& else_item_list
  )
  {
    check_item_common(item, AstItem::GenIf);
    EXPECT_EQ( then_declhead_list.size(), item->then_declhead_num() );
    EXPECT_EQ( then_declhead_list, item->then_declhead_list() );
    EXPECT_EQ( then_item_list.size(), item->then_item_num() );
    EXPECT_EQ( then_item_list, item->then_item_list() );
    EXPECT_EQ( else_declhead_list.size(), item->else_declhead_num() );
    EXPECT_EQ( else_declhead_list, item->else_declhead_list() );
    EXPECT_EQ( else_item_list.size(), item->else_item_num() );
    EXPECT_EQ( else_item_list, item->else_item_list() );
  }

  // GenCase のテスト
  void
  check_item_GenCase(
    const AstItem* item,
    const AstExpr* expr,
    const std::vector<const AstGenCaseItem*>& gci_list
  )
  {
    check_item_common(item, AstItem::GenCase);
    EXPECT_EQ( gci_list.size(), item->caseitem_num() );
    EXPECT_EQ( gci_list, item->caseitem_list() );
    EXPECT_EQ( expr, item->cond_expr() );
  }

  // GenFor のテスト
  void
  check_item_GenFor(
    const AstItem* item,
    const char* loop_var,
    const AstExpr* init_expr,
    const AstExpr* cond_expr,
    const AstExpr* next_expr,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list
  )
  {
    check_item_common(item, AstItem::GenFor);
    EXPECT_STREQ( loop_var, item->loop_var() );
    EXPECT_EQ( init_expr, item->init_expr() );
    EXPECT_EQ( cond_expr, item->cond_expr() );
    EXPECT_EQ( next_expr, item->next_expr() );
    check_item_container(item, declhead_list, item_list);
  }

  // 内部に要素を持つタイプのテスト
  void
  check_item_container(
    const AstItem* item,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list
  )
  {
    EXPECT_EQ( declhead_list.size(), item->declhead_num() );
    EXPECT_EQ( declhead_list, item->declhead_list() );
    EXPECT_EQ( item_list.size(), item->item_num() );
    EXPECT_EQ( item_list, item->item_list() );
  }

  // 共通のテスト
  void
  check_item_common(
    const AstItem* item,
    AstItem::Type type
  )
  {
    ASSERT_TRUE( item != nullptr );
    EXPECT_EQ( type, item->type() );
    if ( type != AstItem::SpecPath &&
	 type != AstItem::GenIf &&
	 type != AstItem::GenCase ) {
      check_item_no_cond_expr(item);
    }
    if ( type != AstItem::Initial &&
	 type != AstItem::Always &&
	 type != AstItem::Task &&
	 type != AstItem::Func ) {
      check_item_no_body(item);
    }
    if ( type != AstItem::Task &&
	 type != AstItem::Func &&
	 type != AstItem::Generate &&
	 type != AstItem::GenBlock &&
	 type != AstItem::GenFor ) {
      check_item_no_container(item);
    }
    if ( type != AstItem::ContAssign &&
	 type != AstItem::GateInst ) {
      check_item_no_strength_delay(item);
    }
    if ( type != AstItem::DefParam ) {
      check_item_no_DefParam(item);
    }
    if ( type != AstItem::ContAssign ) {
      check_item_no_ContAssign(item);
    }
    if ( type != AstItem::Task ) {
      check_item_no_Task(item);
    }
    if ( type != AstItem::Func ) {
      check_item_no_Func(item);
    }
    if ( type != AstItem::GateInst ) {
      check_item_no_GateInst(item);
    }
    if ( type != AstItem::MuInst ) {
      check_item_no_MuInst(item);
    }
    if ( type != AstItem::SpecItem ) {
      check_item_no_SpecItem(item);
    }
    if ( type != AstItem::SpecPath ) {
      check_item_no_SpecPath(item);
    }
    if ( type != AstItem::GenIf ) {
      check_item_no_GenIf(item);
    }
    if ( type != AstItem::GenCase ) {
      check_item_no_GenCase(item);
    }
    if ( type != AstItem::GenFor ) {
      check_item_no_GenFor(item);
    }
  }

  void
  check_item_no_cond_expr(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->cond_expr(),
		  std::logic_error );
  }

  void
  check_item_no_body(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->body(),
		  std::logic_error );
  }

  // 内部に要素を持たない時のテスト
  void
  check_item_no_container(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->declhead_list(),
		  std::logic_error );
    EXPECT_THROW( item->item_list(),
		  std::logic_error );
  }

  // strength/delay を持たない時のテスト
  void
  check_item_no_strength_delay(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->strength(),
		  std::logic_error );
    EXPECT_THROW( item->delay(),
		  std::logic_error );
  }

  // DefParam 型でない場合のテスト
  void
  check_item_no_DefParam(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->paramassign_list(),
		  std::logic_error );
    EXPECT_THROW( item->defparam_list(),
		  std::logic_error );
  }

  // ContAssign 型でない場合のテスト
  void
  check_item_no_ContAssign(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->contassign_list(),
		  std::logic_error );
  }

  // Task でない場合のテスト
  void
  check_item_no_Task(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->automatic(),
		  std::logic_error );
    EXPECT_THROW( item->iohead_list(),
		  std::logic_error );
  }

  // Func でない場合のテスト
  void
  check_item_no_Func(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->is_signed(),
		  std::logic_error );
    EXPECT_THROW( item->range(),
		  std::logic_error );
    EXPECT_THROW( item->data_type(),
		  std::logic_error );
  }

  // GateInst でない場合のテスト
  void
  check_item_no_GateInst(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->prim_type(),
		  std::logic_error );
  }

  // MuInst でない場合のテスト
  void
  check_item_no_MuInst(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->inst_list(),
		  std::logic_error );
  }

  // SpecItem でない時のテスト
  void
  check_item_no_SpecItem(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->specitem_type(),
		  std::logic_error );
    EXPECT_THROW( item->specpath_type(),
		  std::logic_error );
  }

  // SpecPath でない時のテスト
  void
  check_item_no_SpecPath(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->terminal_list(),
		  std::logic_error );
    EXPECT_THROW( item->path_decl(),
		  std::logic_error );
    EXPECT_THROW( item->cond_expr(),
		  std::logic_error );
  }

  // GenIf でない時のテスト
  void
  check_item_no_GenIf(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->then_declhead_list(),
		  std::logic_error );
    EXPECT_THROW( item->then_item_list(),
		  std::logic_error );
    EXPECT_THROW( item->else_declhead_list(),
		  std::logic_error );
    EXPECT_THROW( item->else_item_list(),
		  std::logic_error );
  }

  // GenCase でない時のテスト
  void
  check_item_no_GenCase(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->caseitem_list(),
		  std::logic_error );
  }

  // GenFor でない時のテスト
  void
  check_item_no_GenFor(
    const AstItem* item
  )
  {
    EXPECT_THROW( item->loop_var(),
		  std::logic_error );
    EXPECT_THROW( item->init_expr(),
		  std::logic_error );
    EXPECT_THROW( item->next_expr(),
		  std::logic_error );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstDefParam 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_Defparam(
    const AstDefParam* defparam,
    const char* name,
    const AstExpr* expr,
    const std::vector<const char*>& nb_list = {}
  )
  {
    EXPECT_STREQ( name, defparam->name() );
    auto n = nb_list.size();
    EXPECT_EQ( n, defparam->namebranch_num() );
    SizeType index = 0;
    std::string exp_name;
    for ( auto nb: defparam->namebranch_list() ) {
      EXPECT_STREQ( nb_list[index], nb->name() );
      exp_name += nb->name();
      exp_name += ".";
      ++ index;
    }
    exp_name += defparam->name();
    EXPECT_EQ( exp_name, defparam->decompile_name() );
    EXPECT_EQ( expr, defparam->expr() );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstContAssign 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_ContAssign(
    const AstContAssign* contassign,
    const AstExpr* lhs,
    const AstExpr* rhs
  )
  {
    EXPECT_EQ( lhs, contassign->lhs() );
    EXPECT_EQ( rhs, contassign->rhs() );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstInst 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_Inst(
    const AstInst* inst,
    const AstRange* range,
    const std::vector<const AstConnection*>& con_list
  )
  {
    EXPECT_EQ( range, inst->range() );
    EXPECT_EQ( con_list.size(), inst->port_num() );
    EXPECT_EQ( con_list, inst->port_list() );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstGenCaseItem 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_GenCaseItem(
    const AstGenCaseItem* item,
    const std::vector<const AstExpr*>& label_list,
    const std::vector<const AstDeclHead*>& declhead_list,
    const std::vector<const AstItem*>& item_list
  )
  {
    EXPECT_EQ( label_list.size(), item->label_num() );
    EXPECT_EQ( label_list, item->label_list() );
    EXPECT_EQ( declhead_list.size(), item->declhead_num() );
    EXPECT_EQ( declhead_list, item->declhead_list() );
    EXPECT_EQ( item_list.size(), item->item_num() );
    EXPECT_EQ( item_list, item->item_list() );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstPathDecl 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_PathDecl(
    const AstPathDecl* pathdecl,
    int edge,
    const std::vector<const AstExpr*>& input_list,
    int input_pol,
    VpiPathType op,
    const std::vector<const AstExpr*>& output_list,
    int output_pol,
    const AstExpr* expr,
    const AstPathDelay* path_delay
  )
  {
    EXPECT_EQ( edge, pathdecl->edge() );
    EXPECT_EQ( input_list.size(), pathdecl->input_num() );
    EXPECT_EQ( input_list, pathdecl->input_list() );
    EXPECT_EQ( input_pol, pathdecl->input_pol() );
    EXPECT_EQ( op, pathdecl->op() );
    EXPECT_EQ( output_list.size(), pathdecl->output_num() );
    EXPECT_EQ( output_list, pathdecl->output_list() );
    EXPECT_EQ( output_pol, pathdecl->output_pol() );
    EXPECT_EQ( expr, pathdecl->expr() );
    EXPECT_EQ( path_delay, pathdecl->path_delay() );
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PARSERTEST_ITEM_H
