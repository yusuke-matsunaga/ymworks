#ifndef PTITEMTEST_H
#define PTITEMTEST_H

/// @file PtItemTest.h
/// @brief PtItemTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtTest.h"
#include "parser/PtPathDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtItemTest PtItemTest.h "PtItemTest.h"
/// @brief PtItem 用のテスト環境
//////////////////////////////////////////////////////////////////////
class PtItemTest :
  public PtTest
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // DefParamH 型のテスト
  void
  check_DefParamH(
    const PtItem* item,
    const FileRegion& file_region,
    const std::vector<const PtDefParam*>& defparam_list
  )
  {
    check_common(item, file_region, AstItem::DefParam);
    EXPECT_EQ( defparam_list, to_vector(item->defparam_top()) );
  }

  // ContAssign 型のテスト
  void
  check_ContAssignH(
    const PtItem* item,
    const FileRegion& file_region,
    const PtStrength* strength,
    const PtDelay* delay,
    const std::vector<const PtContAssign*>& ca_list
  )
  {
    check_common(item, file_region, AstItem::ContAssign);
    EXPECT_EQ( strength, item->strength() );
    EXPECT_EQ( delay, item->delay() );
    EXPECT_EQ( ca_list, to_vector(item->contassign_top()) );
  }

  // Initial のテスト
  void
  check_Initial(
    const PtItem* item,
    const FileRegion& file_region,
    const PtStmt* body
  )
  {
    check_common(item, file_region, AstItem::Initial);
    EXPECT_EQ( body, item->body() );
  }

  // Always のテスト
  void
  check_Always(
    const PtItem* item,
    const FileRegion& file_region,
    const PtStmt* body
  )
  {
    check_common(item, file_region, AstItem::Always);
    EXPECT_EQ( body, item->body() );
  }

  // Task のテスト
  void
  check_Task(
    const PtItem* item,
    const FileRegion& file_region,
    const char* name,
    bool automatic,
    const std::vector<const PtIOHead*>& iohead_list,
    const std::vector<const PtDeclHead*>& declhead_list,
    const PtStmt* body
  )
  {
    check_common(item, file_region, AstItem::Task, name);
    EXPECT_EQ( body, item->body() );
    EXPECT_EQ( automatic, item->automatic() );
    EXPECT_EQ( iohead_list, to_vector(item->iohead_top()) );
    EXPECT_EQ( declhead_list, to_vector(item->declhead_top()) );
  }

  // Func のテスト
  void
  check_Function(
    const PtItem* item,
    const FileRegion& file_region,
    const char* name,
    bool is_signed,
    const PtRange* range,
    VpiVarType data_type,
    bool automatic,
    const std::vector<const PtIOHead*>& iohead_list,
    const std::vector<const PtDeclHead*>& declhead_list,
    const PtStmt* body
  )
  {
    check_common(item, file_region, AstItem::Func, name);
    EXPECT_EQ( body, item->body() );
    EXPECT_EQ( automatic, item->automatic() );
    EXPECT_EQ( iohead_list, to_vector(item->iohead_top()) );
    EXPECT_EQ( declhead_list, to_vector(item->declhead_top()) );
    EXPECT_EQ( is_signed, item->is_signed() );
    EXPECT_EQ( range, item->range() );
    EXPECT_EQ( data_type, item->data_type() );
  }

  /// GateInst 型のテスト
  void
  check_GateH(
    const PtItem* item,
    const FileRegion& file_region,
    VpiPrimType prim_type,
    const std::vector<const PtInst*>& inst_list,
    const PtStrength* strength = nullptr,
    const PtDelay* delay = nullptr
  )
  {
    check_common(item, file_region, AstItem::GateInst);
    EXPECT_EQ( prim_type, item->prim_type() );
    EXPECT_EQ( strength, item->strength() );
    EXPECT_EQ( delay, item->delay() );
    EXPECT_EQ( inst_list, to_vector(item->inst_top()) );
  }

  // MuInst のテスト
  void
  check_MuH(
    const PtItem* item,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const PtConnection*>& con_list,
    const std::vector<const PtInst*>& inst_list,
    const PtStrength* strength = nullptr,
    const PtDelay* delay = nullptr
  )
  {
    check_common(item, file_region, AstItem::MuInst, name);
    EXPECT_EQ( con_list, to_vector(item->paramassign_top()) );
    EXPECT_EQ( strength, item->strength() );
    EXPECT_EQ( delay, item->delay() );
    EXPECT_EQ( inst_list, to_vector(item->inst_top()) );
  }

  // SpecItem のテスト
  void
  check_SpecItem(
    const PtItem* item,
    const FileRegion& file_region,
    VpiSpecItemType specitem_type,
    const std::vector<const PtExpr*>& terminal_list
  )
  {
    check_common(item, file_region, AstItem::SpecItem);
    EXPECT_EQ( specitem_type, item->specitem_type() );
    EXPECT_EQ( terminal_list, to_vector(item->terminal_top()) );
  }

  // SpecPath のテスト
  void
  check_SpecPath(
    const PtItem* item,
    const FileRegion& file_region,
    VpiSpecPathType specpath_type,
    const PtExpr* expr,
    const PtPathDecl* path_decl
  )
  {
    check_common(item, file_region, AstItem::SpecPath);
    EXPECT_EQ( specpath_type, item->specpath_type() );
    EXPECT_EQ( path_decl, item->path_decl() );
    EXPECT_EQ( expr, item->cond_expr() );
  }

  // Generate のテスト
  void
  check_Generate(
    const PtItem* item,
    const FileRegion& file_region,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtItem*>& item_list
  )
  {
    check_common(item, file_region, AstItem::Generate);
    check_container(item, declhead_list, item_list);
  }

  // GenBlock のテスト
  void
  check_GenBlock(
    const PtItem* item,
    const FileRegion& file_region,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtItem*>& item_list
  )
  {
    check_common(item, file_region, AstItem::GenBlock);
    check_container(item, declhead_list, item_list);
  }

  // GenBlock のテスト
  void
  check_GenBlock(
    const PtItem* item,
    const FileRegion& file_region,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtItem*>& item_list,
    const char* name
  )
  {
    check_common(item, file_region, AstItem::GenBlock, name);
    check_container(item, declhead_list, item_list);
  }

  // GenIf のテスト
  void
  check_GenIf(
    const PtItem* item,
    const FileRegion& file_region,
    const PtExpr* cond_expr,
    const std::vector<const PtDeclHead*>& then_declhead_list,
    const std::vector<const PtItem*>& then_item_list,
    const std::vector<const PtDeclHead*>& else_declhead_list,
    const std::vector<const PtItem*>& else_item_list
  )
  {
    check_common(item, file_region, AstItem::GenIf);
    EXPECT_EQ( cond_expr, item->cond_expr() );
    EXPECT_EQ( then_declhead_list, to_vector(item->then_declhead_top()) );
    EXPECT_EQ( then_item_list, to_vector(item->then_item_top()) );
    EXPECT_EQ( else_declhead_list, to_vector(item->else_declhead_top()) );
    EXPECT_EQ( else_item_list, to_vector(item->else_item_top()) );
  }

  // GenCase のテスト
  void
  check_GenCase(
    const PtItem* item,
    const FileRegion& file_region,
    const PtExpr* expr,
    const std::vector<const PtGenCaseItem*>& gci_list
  )
  {
    check_common(item, file_region, AstItem::GenCase);
    EXPECT_EQ( gci_list, to_vector(item->caseitem_top()) );
    EXPECT_EQ( expr, item->cond_expr() );
  }

  // GenFor のテスト
  void
  check_GenFor(
    const PtItem* item,
    const FileRegion& file_region,
    const char* name,
    const char* loop_var,
    const PtExpr* init_expr,
    const PtExpr* cond_expr,
    const PtExpr* next_expr,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtItem*>& item_list
  )
  {
    check_common(item, file_region, AstItem::GenFor, name);
    EXPECT_STREQ( loop_var, item->loop_var() );
    EXPECT_EQ( init_expr, item->init_expr() );
    EXPECT_EQ( cond_expr, item->cond_expr() );
    EXPECT_EQ( next_expr, item->next_expr() );
    check_container(item, declhead_list, item_list);
  }

  // 内部に要素を持つタイプのテスト
  void
  check_container(
    const PtItem* item,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtItem*>& item_list
  )
  {
    EXPECT_EQ( declhead_list, to_vector(item->declhead_top()) );
    EXPECT_EQ( item_list, to_vector(item->item_top()) );
  }

  // 共通のテスト
  void
  check_common(
    const PtItem* item,
    const FileRegion& file_region,
    AstItem::Type type
  )
  {
    ASSERT_TRUE( item != nullptr );
    check_Base(item, file_region);
    EXPECT_THROW( item->name(),
		  std::logic_error );
    check_common_sub(item, type);
  }

  // 共通のテスト
  void
  check_common(
    const PtItem* item,
    const FileRegion& file_region,
    AstItem::Type type,
    const char* name
  )
  {
    ASSERT_TRUE( item != nullptr );
    check_Base(item, file_region);
    EXPECT_STREQ( name, item->name() );
    check_common_sub(item, type);
  }

  /// @biref check_common() の下請け関数
  void
  check_common_sub(
    const PtItem* item,
    AstItem::Type type
  )
  {
    EXPECT_EQ( type, item->type() );
    if ( type != AstItem::SpecPath &&
	 type != AstItem::GenIf &&
	 type != AstItem::GenCase &&
	 type != AstItem::GenFor ) {
      EXPECT_THROW( item->cond_expr(),
		    std::logic_error );
    }
    if ( type != AstItem::Initial &&
	 type != AstItem::Always &&
	 type != AstItem::Task &&
	 type != AstItem::Func ) {
      EXPECT_THROW( item->body(),
		    std::logic_error );
    }
    if ( type != AstItem::Task &&
	 type != AstItem::Func &&
	 type != AstItem::Generate &&
	 type != AstItem::GenBlock &&
	 type != AstItem::GenFor ) {
      EXPECT_THROW( item->declhead_top(),
		    std::logic_error );
      EXPECT_THROW( item->item_top(),
		    std::logic_error );
    }
    if ( type != AstItem::ContAssign &&
	 type != AstItem::GateInst &&
	 type != AstItem::MuInst) {
      EXPECT_THROW( item->strength(),
		    std::logic_error );
      EXPECT_THROW( item->delay(),
		    std::logic_error );
    }
    if ( type != AstItem::DefParam ) {
      EXPECT_THROW( item->defparam_top(),
		    std::logic_error );
    }
    if ( type != AstItem::ContAssign ) {
      EXPECT_THROW( item->contassign_top(),
		    std::logic_error );
    }
    if ( type != AstItem::Task &&
	 type != AstItem::Func ) {
      EXPECT_THROW( item->automatic(),
		    std::logic_error );
      EXPECT_THROW( item->iohead_top(),
		    std::logic_error );
    }
    if ( type != AstItem::Func ) {
      EXPECT_THROW( item->is_signed(),
		    std::logic_error );
      EXPECT_THROW( item->range(),
		    std::logic_error );
      EXPECT_THROW( item->data_type(),
		    std::logic_error );
    }
    if ( type != AstItem::GateInst ) {
      EXPECT_THROW( item->prim_type(),
		    std::logic_error );
    }
    if ( type != AstItem::MuInst ) {
      EXPECT_THROW( item->paramassign_top(),
		    std::logic_error );
    }
    if ( type != AstItem::MuInst &&
	 type != AstItem::GateInst ) {
      EXPECT_THROW( item->inst_top(),
		    std::logic_error );
    }
    if ( type != AstItem::SpecItem ) {
      EXPECT_THROW( item->specitem_type(),
		    std::logic_error );
      EXPECT_THROW( item->specpath_type(),
		    std::logic_error );
    }
    if ( type != AstItem::SpecPath ) {
      EXPECT_THROW( item->terminal_top(),
		    std::logic_error );
      EXPECT_THROW( item->path_decl(),
		    std::logic_error );
    }
    if ( type != AstItem::GenIf ) {
      EXPECT_THROW( item->then_declhead_top(),
		    std::logic_error );
      EXPECT_THROW( item->then_item_top(),
		    std::logic_error );
      EXPECT_THROW( item->else_declhead_top(),
		    std::logic_error );
      EXPECT_THROW( item->else_item_top(),
		    std::logic_error );
    }
    if ( type != AstItem::GenCase ) {
      EXPECT_THROW( item->caseitem_top(),
		    std::logic_error );
    }
    if ( type != AstItem::GenFor ) {
      EXPECT_THROW( item->loop_var(),
		    std::logic_error );
      EXPECT_THROW( item->init_expr(),
		    std::logic_error );
      EXPECT_THROW( item->next_expr(),
		    std::logic_error );
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // PtDefParam 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_DefParam(
    const PtDefParam* defparam,
    const FileRegion& fr,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_vec,
    const PtExpr* expr
  )
  {
    check_Base(defparam, fr);
    EXPECT_STREQ( name, defparam->name() );
    check_namebranch_list(defparam->namebranch_top(), nbspec_vec);
    EXPECT_EQ( expr, defparam->expr() );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // PtContAssign 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_ContAssign(
    const PtContAssign* contassign,
    const FileRegion& fr,
    const PtExpr* lhs,
    const PtExpr* rhs
  )
  {
    check_Base(contassign, fr);
    EXPECT_EQ( lhs, contassign->lhs() );
    EXPECT_EQ( rhs, contassign->rhs() );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // PtInst 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_Inst(
    const PtInst* inst,
    const FileRegion& fr,
    const char* name,
    const PtRange* range,
    const std::vector<const PtConnection*>& con_list
  )
  {
    check_Base(inst, fr);
    EXPECT_STREQ( name, inst->name() );
    EXPECT_EQ( range, inst->range() );
    EXPECT_EQ( con_list, to_vector(inst->port_top()) );
  }

  void
  check_Inst(
    const PtInst* inst,
    const FileRegion& fr,
    const char* name,
    const PtRange* range,
    const std::vector<const PtExpr*>& expr_list
  )
  {
    check_Base(inst, fr);
    EXPECT_STREQ( name, inst->name() );
    EXPECT_EQ( range, inst->range() );
    auto con_list = to_vector(inst->port_top());
    ASSERT_EQ( expr_list.size(), con_list.size() );
    for ( SizeType i = 0; i < expr_list.size(); ++ i ) {
      auto con = con_list[i];
      EXPECT_EQ( nullptr, con->name() );
      EXPECT_EQ( expr_list[i], con->expr() );
    }
  }


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_GenCaseItem(
    const PtGenCaseItem* item,
    const FileRegion& fr,
    const std::vector<const PtExpr*>& label_list,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtItem*>& item_list
  )
  {
    check_Base(item, fr);
    EXPECT_EQ( label_list, to_vector(item->label_top()) );
    EXPECT_EQ( declhead_list, to_vector(item->declhead_top()) );
    EXPECT_EQ( item_list, to_vector(item->item_top()) );
  }


public:
  //////////////////////////////////////////////////////////////////////
  // PtPathDecl 用のテスト
  //////////////////////////////////////////////////////////////////////

  void
  check_PathDecl(
    const PtPathDecl* pathdecl,
    const FileRegion& fr,
    int edge,
    const std::vector<const PtExpr*>& input_list,
    int input_pol,
    VpiPathType op,
    const std::vector<const PtExpr*>& output_list,
    int output_pol,
    const PtExpr* expr,
    const PtPathDelay* path_delay
  )
  {
    check_Base(pathdecl, fr);
    EXPECT_EQ( edge, pathdecl->edge() );
    EXPECT_EQ( input_list, to_vector(pathdecl->input_top()) );
    EXPECT_EQ( input_pol, pathdecl->input_pol() );
    EXPECT_EQ( op, pathdecl->op() );
    EXPECT_EQ( output_list, to_vector(pathdecl->output_top()) );
    EXPECT_EQ( output_pol, pathdecl->output_pol() );
    EXPECT_EQ( expr, pathdecl->expr() );
    EXPECT_EQ( path_delay, pathdecl->path_delay() );
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTITEMTEST_H
