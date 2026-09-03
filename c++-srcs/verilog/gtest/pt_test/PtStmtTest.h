#ifndef PTSTMTTEST_H
#define PTSTMTTEST_H

/// @file PtStmtTest.h
/// @brief PtStmtTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtTest.h"
#include "parser/PtCaseItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtStmtTest PtStmtTest.h "PtStmtTest.h"
/// @brief PtStmt 用のテスト環境
//////////////////////////////////////////////////////////////////////
class PtStmtTest :
  public PtTest
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Disable 型のテスト
  void
  check_Disable(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list = {}
  )
  {
    check_common(stmt, file_region, AstStmt::Disable, name, nbspec_list);
  }

  /// @brief Enable 型のテスト
  void
  check_Enable(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const PtExpr*>& arg_list
  )
  {
    check_common(stmt, file_region, AstStmt::Enable, name);
    EXPECT_EQ( arg_list, to_vector(stmt->arg_top()) );
  }

  /// @brief Enable 型のテスト
  void
  check_Enable(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list,
    const std::vector<const PtExpr*>& arg_list
  )
  {
    check_common(stmt, file_region, AstStmt::Enable, name, nbspec_list);
    EXPECT_EQ( arg_list, to_vector(stmt->arg_top()) );
  }

  /// @brief SysEnable 型のテスト
  void
  check_SysEnable(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const PtExpr*>& arg_list
  )
  {
    check_common(stmt, file_region, AstStmt::SysEnable, name);
    EXPECT_EQ( arg_list, to_vector(stmt->arg_top()) );
  }

  /// @brief DelayControl 型のテスト
  void
  check_DelayControl(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtControl* control,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::DelayControl);
    EXPECT_EQ( control, stmt->control() );
    EXPECT_EQ( body, stmt->body() );
  }

  /// @brief EventControl 型のテスト
  void
  check_EventControl(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtControl* control,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::EventControl);
    EXPECT_EQ( control, stmt->control() );
    EXPECT_EQ( body, stmt->body() );
  }

  /// @brief Assign 型のテスト
  void
  check_Assign(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* lhs,
    const PtExpr* rhs,
    const PtControl* control = nullptr
  )
  {
    check_common(stmt, file_region, AstStmt::Assign);
    EXPECT_EQ( lhs, stmt->lhs() );
    EXPECT_EQ( rhs, stmt->rhs() );
    EXPECT_EQ( control, stmt->control() );
  }

  /// @brief NbAssign 型のテスト
  void
  check_NbAssign(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* lhs,
    const PtExpr* rhs,
    const PtControl* control = nullptr
  )
  {
    check_common(stmt, file_region, AstStmt::NbAssign);
    EXPECT_EQ( lhs, stmt->lhs() );
    EXPECT_EQ( rhs, stmt->rhs() );
    EXPECT_EQ( control, stmt->control() );
  }

  /// @brief Force 型のテスト
  void
  check_Force(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* lhs,
    const PtExpr* rhs
  )
  {
    check_common(stmt, file_region, AstStmt::Force);
    EXPECT_EQ( lhs, stmt->lhs() );
    EXPECT_EQ( rhs, stmt->rhs() );
  }

  /// @brief PcAssign 型のテスト
  void
  check_PcAssign(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* lhs,
    const PtExpr* rhs
  )
  {
    check_common(stmt, file_region, AstStmt::PcAssign);
    EXPECT_EQ( lhs, stmt->lhs() );
    EXPECT_EQ( rhs, stmt->rhs() );
  }

  /// @brief Release 型のテスト
  void
  check_Release(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* lhs
  )
  {
    check_common(stmt, file_region, AstStmt::Release);
    EXPECT_EQ( lhs, stmt->lhs() );
  }

  /// @brief Deassign 型のテスト
  void
  check_Deassign(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* lhs
  )
  {
    check_common(stmt, file_region, AstStmt::Deassign);
    EXPECT_EQ( lhs, stmt->lhs() );
  }

  /// @brief EventStmt 型のテスト
  void
  check_Event(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* primary
  )
  {
    check_common(stmt, file_region, AstStmt::Event);
    EXPECT_EQ( primary, stmt->primary() );
  }

  /// @brief Null 型のテスト
  void
  check_Null(
    const PtStmt* stmt,
    const FileRegion& file_region
  )
  {
    check_common(stmt, file_region, AstStmt::Null);
  }

  /// @brief If 型のテスト
  void
  check_If(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const PtStmt* body,
    const PtStmt* else_body = nullptr
  )
  {
    check_common(stmt, file_region, AstStmt::If);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( body, stmt->body() );
    EXPECT_EQ( else_body, stmt->else_body() );
  }

  /// @brief Case 型のテスト
  void
  check_Case(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const std::vector<const PtCaseItem*>& caseitem_list
  )
  {
    check_common(stmt, file_region, AstStmt::Case);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( caseitem_list, to_vector(stmt->caseitem_top()) );
  }

  /// @brief CaseX 型のテスト
  void
  check_CaseX(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const std::vector<const PtCaseItem*>& caseitem_list
  )
  {
    check_common(stmt, file_region, AstStmt::CaseX);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( caseitem_list, to_vector(stmt->caseitem_top()) );
  }

  /// @brief CaseZ 型のテスト
  void
  check_CaseZ(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const std::vector<const PtCaseItem*>& caseitem_list
  )
  {
    check_common(stmt, file_region, AstStmt::CaseZ);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( caseitem_list, to_vector(stmt->caseitem_top()) );
  }

  /// @brief Wait 型のテスト
  void
  check_Wait(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::Wait);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( body, stmt->body() );
  }

  /// @brief Forever 型のテスト
  void
  check_Forever(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::Forever);
    EXPECT_EQ( body, stmt->body() );
  }

  /// @brief Repeat 型のテスト
  void
  check_Repeat(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::Repeat);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( body, stmt->body() );
  }

  /// @brief While 型のテスト
  void
  check_While(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtExpr* expr,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::While);
    EXPECT_EQ( expr, stmt->expr() );
    EXPECT_EQ( body, stmt->body() );
  }

  /// @breif For 型のテスト
  void
  check_For(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const PtStmt* init_stmt,
    const PtExpr* cond,
    const PtStmt* next_stmt,
    const PtStmt* body
  )
  {
    check_common(stmt, file_region, AstStmt::For);
    EXPECT_EQ( init_stmt, stmt->init_stmt() );
    EXPECT_EQ( cond, stmt->expr() );
    EXPECT_EQ( next_stmt, stmt->next_stmt() );
    EXPECT_EQ( body, stmt->body() );
  }

  /// @brief ParBlock 型のテスト
  void
  check_ParBlock(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const std::vector<const PtStmt*>& stmt_list
  )
  {
    check_common(stmt, file_region, AstStmt::ParBlock);
    EXPECT_EQ( stmt_list, to_vector(stmt->stmt_top()) );
  }

  /// @brief ParBlock 型のテスト
  void
  check_SeqBlock(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const std::vector<const PtStmt*>& stmt_list
  )
  {
    check_common(stmt, file_region, AstStmt::SeqBlock);
    EXPECT_EQ( stmt_list, to_vector(stmt->stmt_top()) );
  }

  /// @brief NamedParBlock 型のテスト
  void
  check_NamedParBlock(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtStmt*>& stmt_list
  )
  {
    check_common(stmt, file_region, AstStmt::NamedParBlock, name);
    EXPECT_EQ( declhead_list, to_vector(stmt->declhead_top()) );
    EXPECT_EQ( stmt_list, to_vector(stmt->stmt_top()) );
  }

  /// @brief NamedSeqBlock 型のテスト
  void
  check_NamedSeqBlock(
    const PtStmt* stmt,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const PtDeclHead*>& declhead_list,
    const std::vector<const PtStmt*>& stmt_list
  )
  {
    check_common(stmt, file_region, AstStmt::NamedSeqBlock, name);
    EXPECT_EQ( declhead_list, to_vector(stmt->declhead_top()) );
    EXPECT_EQ( stmt_list, to_vector(stmt->stmt_top()) );
  }

  /// @brief CaseItem のテスト
  void
  check_CaseItem(
    const PtCaseItem* caseitem,
    const FileRegion& file_region,
    const std::vector<const PtExpr*>& label_list,
    const PtStmt* body
  )
  {
    check_Base(caseitem, file_region);
    EXPECT_EQ( label_list, to_vector(caseitem->label_top()) );
    EXPECT_EQ( body, caseitem->body() );
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通のチェック
  void
  check_common(
    const PtStmt* stmt,
    const FileRegion& file_region,
    AstStmt::Type type,
    const char* name = nullptr,
    const std::vector<NameBranchSpec>& nbspec_list = {}
  )
  {
    check_Base(stmt, file_region);
    EXPECT_STREQ( name, stmt->name() );
    check_namebranch_list(stmt->namebranch_top(), nbspec_list);
    EXPECT_EQ( type, stmt->type() );
    if ( type != AstStmt::DelayControl &&
	 type != AstStmt::EventControl &&
	 type != AstStmt::Wait &&
	 type != AstStmt::Forever &&
	 type != AstStmt::Repeat &&
	 type != AstStmt::While &&
	 type != AstStmt::For &&
	 type != AstStmt::If ) {
      EXPECT_THROW( stmt->body(),
		    std::logic_error );
    }
    if ( type != AstStmt::Wait &&
	 type != AstStmt::Repeat &&
	 type != AstStmt::While &&
	 type != AstStmt::For &&
	 type != AstStmt::If &&
	 type != AstStmt::Case &&
	 type != AstStmt::CaseX &&
	 type != AstStmt::CaseZ ) {
      EXPECT_THROW( stmt->expr(),
		    std::logic_error );
    }
    if ( type != AstStmt::Enable &&
	 type != AstStmt::SysEnable ) {
      EXPECT_THROW( stmt->arg_top(),
		    std::logic_error );
    }
    if ( type != AstStmt::DelayControl &&
	 type != AstStmt::EventControl &&
	 type != AstStmt::Assign &&
	 type != AstStmt::NbAssign ) {
      EXPECT_THROW( stmt->control(),
		    std::logic_error );
    }
    if ( type != AstStmt::Assign &&
	 type != AstStmt::NbAssign &&
	 type != AstStmt::Force &&
	 type != AstStmt::PcAssign &&
	 type != AstStmt::Release &&
	 type != AstStmt::Deassign ) {
      EXPECT_THROW( stmt->lhs(),
		    std::logic_error );
    }
    if ( type != AstStmt::Assign &&
	 type != AstStmt::NbAssign &&
	 type != AstStmt::Force &&
	 type != AstStmt::PcAssign ) {
      EXPECT_THROW( stmt->rhs(),
		    std::logic_error );
    }
    if ( type != AstStmt::Event ) {
      EXPECT_THROW( stmt->primary(),
		    std::logic_error );
    }
    if ( type != AstStmt::If ) {
      EXPECT_THROW( stmt->else_body(),
		    std::logic_error );
    }
    if ( type != AstStmt::Case &&
	 type != AstStmt::CaseX &&
	 type != AstStmt::CaseZ ) {
      EXPECT_THROW( stmt->caseitem_top(),
		    std::logic_error );
    }
    if ( type != AstStmt::For )  {
      EXPECT_THROW( stmt->init_stmt(),
		    std::logic_error );
      EXPECT_THROW( stmt->next_stmt(),
		    std::logic_error );
    }
    if ( type != AstStmt::NamedParBlock &&
	 type != AstStmt::NamedSeqBlock ) {
      EXPECT_THROW( stmt->declhead_top(),
		    std::logic_error );
      if ( type != AstStmt::ParBlock &&
	   type != AstStmt::SeqBlock ) {
	EXPECT_THROW( stmt->stmt_top(),
		      std::logic_error );
      }
    }
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PTSTMTTEST_H
