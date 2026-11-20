
/// @file VlDumperImpl_stmt.cc
/// @brief VlDumperImpl の実装ファイル (stmt担当)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlDumperImpl.h"
#include "ym/vl/VlMgr.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlProcess.h"
#include "ym/vl/VlStmt.h"
#include "ym/vl/VlControl.h"
#include "ym/vl/VlModule.h"
#include "ym/vl/VlScope.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// initial/always の内容を出力する関数
// IEEE 1364-2001 p.656
void
VlDumperImpl::put(
  const char* label,
  const VlProcess* process
)
{
  const char* nm = nullptr;
  switch ( process->type() ) {
  case VpiObjType::Initial: nm = "Initial"; break;
  case VpiObjType::Always:  nm = "Always"; break;
  default: ASSERT_NOT_REACHED;
  }
  VlDumpHeader x(this, label, nm);

  put("FileRegion", process->file_region() );
  put("Module", process->parent_scope()->parent_module()->full_name() );
  put("Stmt", process->stmt() );
}

// statement の内容を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlStmt* stmt
)
{
  const char* nm = nullptr;
  switch ( stmt->type() ) {
  case VpiObjType::Begin:           nm = "Begin"; break;
  case VpiObjType::Fork:            nm = "Fork"; break;
  case VpiObjType::NamedBegin:      nm = "NamedBegin"; break;
  case VpiObjType::NamedFork:       nm = "NamedFork"; break;
  case VpiObjType::NullStmt:        nm = "NullStmt"; break;
  case VpiObjType::EventStmt:       nm = "EventStmt"; break;
  case VpiObjType::Assignment:      nm = "Assignment"; break;
  case VpiObjType::While:           nm = "While"; break;
  case VpiObjType::Repeat:          nm = "Repeat"; break;
  case VpiObjType::Wait:            nm = "Wait"; break;
  case VpiObjType::For:             nm = "For"; break;
  case VpiObjType::Forever:         nm = "Forever"; break;
  case VpiObjType::If:              nm = "If"; break;
  case VpiObjType::IfElse:          nm = "IfElse"; break;
  case VpiObjType::Case:            nm = "Case"; break;
  case VpiObjType::Force:           nm = "Force"; break;
  case VpiObjType::AssignStmt:      nm = "AssignStmt"; break;
  case VpiObjType::Deassign:        nm = "Deassign"; break;
  case VpiObjType::Release:         nm = "Release"; break;
  case VpiObjType::TaskCall:        nm = "TaskCall"; break;
  case VpiObjType::SysTaskCall:     nm = "SysTaskCall"; break;
  case VpiObjType::Disable:         nm = "Disable"; break;
  case VpiObjType::DelayControl:    nm = "DelayControl"; break;
  case VpiObjType::EventControl:    nm = "EventControl"; break;
  default: ASSERT_NOT_REACHED;
  }

  VlDumpHeader x(this, label, nm);

  put("FileRegion", stmt->file_region());
  put("vpiScope", stmt->parent_scope()->full_name() );

  switch ( stmt->type() ) {
  case VpiObjType::NamedBegin:
  case VpiObjType::NamedFork:
    // スコープとしての内容は別に出力されている．
  case VpiObjType::Begin:
  case VpiObjType::Fork:
    put("ChildStmt", stmt->child_stmt_list());
    break;

  case VpiObjType::EventStmt:
    put("vpiNamedEvent", stmt->named_event()->decompile());
    break;

  case VpiObjType::Assignment:
    put("Blocking", stmt->is_blocking() );
    put("Lhs", stmt->lhs() );
    put("Rhs", stmt->rhs() );
    put("control", stmt->control() );
    break;

  case VpiObjType::While:
    put("Condition", stmt->expr() );
    put("vpiStmt", stmt->body_stmt() );
    break;

  case VpiObjType::Repeat:
    put("Condition", stmt->expr() );
    put("vpiStmt", stmt->body_stmt() );
    break;

  case VpiObjType::Wait:
    put("Condition", stmt->expr() );
    put("vpiStmt", stmt->body_stmt() );
    break;

  case VpiObjType::For:
    put("vpiForInitStmt", stmt->init_stmt() );
    put("Condition", stmt->expr() );
    put("vpiForIncStmt", stmt->inc_stmt() );
    put("vpiStmt", stmt->body_stmt() );
    break;

  case VpiObjType::Forever:
    put("vpiStmt", stmt->body_stmt() );
    break;

  case VpiObjType::If:
    put("Condition", stmt->expr() );
    put("vpiStmt", stmt->body_stmt() );
    break;

  case VpiObjType::IfElse:
    put("Condition", stmt->expr() );
    put("vpiStmt", stmt->body_stmt() );
    put("vpiElseStmt", stmt->else_stmt() );
    break;

  case VpiObjType::Case:
    put("CaseType", stmt->case_type() );
    put("Condition", stmt->expr() );
    for ( int i = 0; i < stmt->caseitem_num(); ++ i ) {
      auto ci = stmt->caseitem(i);
      VlDumpHeader x(this, "vpiCaseItem", "CaseItem");
      put("FileRegion", ci->file_region() );
      for ( int i = 0; i < ci->expr_num(); ++ i ) {
	auto expr = ci->expr(i);
	put("Expr", expr);
      }
      put("vpiStmt", ci->body_stmt() );
    }
    break;

  case VpiObjType::AssignStmt:
  case VpiObjType::Force:
    put("Lhs", stmt->lhs() );
    put("Rhs", stmt->rhs() );
    break;

  case VpiObjType::Deassign:
  case VpiObjType::Release:
    put("Lhs", stmt->lhs() );
    break;

  case VpiObjType::TaskCall:
    put("Task", stmt->task()->full_name() );
    put("Argument", stmt->argument_list());
    break;

  case VpiObjType::SysTaskCall:
#if 0
    put("vpiUserDefn", handle.get_bool(vpiUserDefn));
#else
#warning "TODO: SystemTaskCall の userDefn"
#endif
    put("UserSystf", stmt->user_systf()->name() );
    put("Argument", stmt->argument_list());
    break;

  case VpiObjType::Disable:
    put("vpiExpr", stmt->scope()->full_name() );
    break;

  case VpiObjType::DelayControl:
  case VpiObjType::EventControl:
    put("control", stmt->control() );
    put("vpiStmt", stmt->body_stmt() );
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

// @brief delay/event/repeat control の内容を出力する関数
void
VlDumperImpl::put(
  const char* label,
  const VlControl* control
)
{
  if ( control == nullptr ) {
    if ( !nullptr_suppress_mode() ) {
      VlDumpHeader x(this, label, "null-pointer");
    }
    return;
  }

  if ( control->delay() ) {
    VlDumpHeader x(this, label, "DelayControl");
    put("FileRegion", control->file_region() );
    put("vpiDelay", control->delay() );
  }
  else if ( control->expr() ) {
    VlDumpHeader x(this, label, "RepeatControl");
    put("FileRegion", control->file_region() );
    put("Expr", control->expr() );
    for ( int i = 0; i < control->event_num(); ++ i ) {
      put("Condition", control->event(i) );
    }
  }
  else {
    VlDumpHeader x(this, label, "EventControl");
    put("FileRegion", control->file_region() );
    for ( int i = 0; i < control->event_num(); ++ i ) {
      put("Condition", control->event(i) );
    }
  }
}

END_NAMESPACE_YM_VERILOG
