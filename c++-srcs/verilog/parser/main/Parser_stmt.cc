/// @file Parser_stmt.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h"
#include "parser/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief disable 文の生成
PtStmt*
Parser::new_Disable(
  const FileRegion& fr,
  const char* name
)
{
  return mFactory.new_Disable(fr, name);
}

// @brief disable 文の生成 (階層付き識別子)
PtStmt*
Parser::new_Disable(
  const FileRegion& fr,
  PtHierName* hname
)
{
  return mFactory.new_Disable(fr, hname);
}

// @brief enable 文の生成
PtStmt*
Parser::new_Enable(
  const FileRegion& fr,
  const char* name,
  PtExprList* arg_list
)
{
  return mFactory.new_Enable(fr, name,
			     arg_list->to_array(mAlloc));
}

// @brief enable 文の生成 (階層付き識別子)
PtStmt*
Parser::new_Enable(
  const FileRegion& fr,
  PtHierName* hname,
  PtExprList* arg_list
)
{
  return mFactory.new_Enable(fr, hname,
			     arg_list->to_array(mAlloc));
}

// @brief system task enable 文の生成
PtStmt*
Parser::new_SysEnable(
  const FileRegion& fr,
  const char* name,
  PtExprList* arg_list
)
{
  return mFactory.new_SysEnable(fr, name,
				arg_list->to_array(mAlloc));
}

// @brief delay control 文の生成
PtStmt*
Parser::new_DcStmt(
  const FileRegion& fr,
  const AstControl* delay,
  const AstStmt* body
)
{
  return mFactory.new_DcStmt(fr, delay, body);
}

// @brief event control 文の生成
PtStmt*
Parser::new_EcStmt(
  const FileRegion& fr,
  const AstControl* event,
  const AstStmt* body
)
{
  return mFactory.new_EcStmt(fr, event, body);
}

// @brief wait 文の生成
PtStmt*
Parser::new_Wait(
  const FileRegion& fr,
  const AstExpr* cond,
  const AstStmt* body
)
{
  return mFactory.new_Wait(fr, cond, body);
}

// @brief assign 文の生成
PtStmt*
Parser::new_Assign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  return mFactory.new_Assign(fr, lhs, rhs);
}

// @brief control 付き assign 文の生成
PtStmt*
Parser::new_Assign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs,
  const AstControl* control
)
{
  return mFactory.new_Assign(fr, lhs, rhs, control);
}

// @brief nonblocking assign 文の生成
PtStmt*
Parser::new_NbAssign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  return mFactory.new_NbAssign(fr, lhs, rhs);
}

// @brief control 付き nonblocking assign 文の生成
PtStmt*
Parser::new_NbAssign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs,
  const AstControl* control
)
{
  return mFactory.new_NbAssign(fr, lhs, rhs, control);
}

// @brief event 文の生成
PtStmt*
Parser::new_EventStmt(
  const FileRegion& fr,
  const AstExpr* event
)
{
  return mFactory.new_EventStmt(fr, event);
}

// @brief null 文の生成
PtStmt*
Parser::new_NullStmt(
  const FileRegion& fr
)
{
  return mFactory.new_NullStmt(fr);
}

// @brief if 文の生成
PtStmt*
Parser::new_If(
  const FileRegion& fr,
  const AstExpr* expr,
  const AstStmt* then_body
)
{
  return mFactory.new_If(fr, expr, then_body);
}

// @brief if 文の生成
PtStmt*
Parser::new_If(
  const FileRegion& fr,
  const AstExpr* expr,
  const AstStmt* then_body,
  const AstStmt* else_body
)
{
  return mFactory.new_If(fr, expr, then_body, else_body);
}

// @brief case 文の生成
PtStmt*
Parser::new_Case(
  const FileRegion& fr,
  const AstExpr* expr,
  PtCaseItemList* caseitem_list
)
{
  return mFactory.new_Case(fr, expr,
			   caseitem_list->to_array(mAlloc));
}

// @brief casex 文の生成
PtStmt*
Parser::new_CaseX(
  const FileRegion& fr,
  const AstExpr* expr,
  PtCaseItemList* caseitem_list
)
{
  return mFactory.new_CaseX(fr, expr,
			    caseitem_list->to_array(mAlloc));
}

// @brief casez 文の生成
PtStmt*
Parser::new_CaseZ(
  const FileRegion& fr,
  const AstExpr* expr,
  PtCaseItemList* caseitem_list
)
{
  return mFactory.new_CaseZ(fr, expr,
			    caseitem_list->to_array(mAlloc));
}

// @brief case item の生成
PtCaseItem*
Parser::new_CaseItem(
  const FileRegion& fr,
  PtExprList* label_list,
  const AstStmt* body
)
{
  return mFactory.new_CaseItem(fr,
			       label_list->to_array(mAlloc),
			       body);
}

// @brief forever 文の生成
PtStmt*
Parser::new_Forever(
  const FileRegion& fr,
  const AstStmt* body
)
{
  return mFactory.new_Forever(fr, body);
}

// @brief repeat 文の生成
PtStmt*
Parser::new_Repeat(
  const FileRegion& fr,
  const AstExpr* expr,
  const AstStmt* body
)
{
  return mFactory.new_Repeat(fr, expr, body);
}

// @brief while 文の生成
PtStmt*
Parser::new_While(
  const FileRegion& fr,
  const AstExpr* cond,
  const AstStmt* body
)
{
  return mFactory.new_While(fr, cond, body);
}

// @brief for 文の生成
PtStmt*
Parser::new_For(
  const FileRegion& fr,
  const AstStmt* init,
  const AstExpr* cond,
  const AstStmt* next,
  const AstStmt* body
)
{
  return mFactory.new_For(fr, init, cond, next, body);
}

// @brief procedural assign 文の生成
PtStmt*
Parser::new_PcAssign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  return mFactory.new_PcAssign(fr, lhs, rhs);
}

// @brief deassign 文の生成
PtStmt*
Parser::new_Deassign(
  const FileRegion& fr,
  const AstExpr* lhs
)
{
  return mFactory.new_Deassign(fr, lhs);
}

// @brief force 文の生成
PtStmt*
Parser::new_Force(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  return mFactory.new_Force(fr, lhs, rhs);
}

// @brief release 文の生成
PtStmt*
Parser::new_Release(
  const FileRegion& fr,
  const AstExpr* lhs
)
{
  return mFactory.new_Release(fr, lhs);
}

// @brief parallel block の生成
PtStmt*
Parser::new_ParBlock(
  const FileRegion& fr,
  PtStmtList* stmt_list
)
{
  return mFactory.new_ParBlock(fr,
			       stmt_list->to_array(mAlloc));
}

// @brief 名前付き parallel block の生成
PtStmt*
Parser::new_NamedParBlock(
  const FileRegion& fr,
  const char* name,
  PtStmtList* stmt_list
)
{
  return mFactory.new_NamedParBlock(fr, name,
				    PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				    stmt_list->to_array(mAlloc));
}

// @brief sequential block の生成
PtStmt*
Parser::new_SeqBlock(
  const FileRegion& fr,
  PtStmtList* stmt_list
)
{
  return mFactory.new_SeqBlock(fr,
			       stmt_list->to_array(mAlloc));
}

// @brief 名前付き sequential block の生成
PtStmt*
Parser::new_NamedSeqBlock(
  const FileRegion& fr,
  const char* name,
  PtStmtList* stmt_list
)
{
  return mFactory.new_NamedSeqBlock(fr, name,
				    PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				    stmt_list->to_array(mAlloc));
}

END_NAMESPACE_YM_VERILOG
