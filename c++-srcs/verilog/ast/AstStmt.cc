
/// @file AstStmt.cc
/// @brief AstStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstStmt.h"
#include "ym/vl/AstCaseItem.h"
#include "ym/vl/AstControl.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstDeclHead.h"
#include "ym/vl/AstNameBranch.h"
#include "parser/PtStmt.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstStmt
//////////////////////////////////////////////////////////////////////

// @brief クラスの型の取得
AstStmt::Type
AstStmt::type() const
{
  _check_ptr();
  return mPtr->type();
}

// @brief ステートメントの種類を表す文字列の取得
const char*
AstStmt::stmt_name() const
{
  switch ( type() ) {
  case Disable:       return "disable statement";
  case Enable:        return "task enable statement";
  case SysEnable:     return "system task enable statement";
  case DelayControl:  return "delay control statement";
  case EventControl:  return "event control statement";
  case Assign:        return "assignment";
  case NbAssign:      return "nonblocking assignment";
  case Event:         return "event statement";
  case Null:          return "null statement";
  case If:            return "if statement";
  case Case:          return "case statement";
  case CaseX:         return "casex statement";
  case CaseZ:         return "casez statement";
  case Wait:          return "wait statement";
  case Forever:       return "forever statement";
  case Repeat:        return "repeat statement";
  case While:         return "while statement";
  case For:           return "for-loop statement";
  case PcAssign:      return "procedural continuous assignment";
  case Deassign:      return "deassign statement";
  case Force:         return "force statement";
  case Release:       return "release statement";
  case NamedParBlock:
  case ParBlock:      return "parallel block";
  case NamedSeqBlock:
  case SeqBlock:      return "sequential block";
  }
  return "";
}

// @brief 本体のステートメントの取得
AstStmt
AstStmt::body() const
{
  _check_ptr();
  return AstStmt(mPtr->body());
}

// @brief 式の取得
AstExpr
AstStmt::expr() const
{
  _check_ptr();
  return AstExpr(mPtr->expr());
}

// @brief 引数のリストの取得
AstExprList
AstStmt::arg_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->arg_top()));
}

// @brief コントロールの取得
AstControl
AstStmt::control() const
{
  _check_ptr();
  return AstControl(mPtr->control());
}

// @brief 左辺式の取得
AstExpr
AstStmt::lhs() const
{
  _check_ptr();
  return AstExpr(mPtr->lhs());
}

// @brief 右辺式の取得
AstExpr
AstStmt::rhs() const
{
  _check_ptr();
  return AstExpr(mPtr->rhs());
}

// @brief イベントプライマリの取得
AstExpr
AstStmt::primary() const
{
  _check_ptr();
  return AstExpr(mPtr->primary());
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
AstStmt
AstStmt::else_body() const
{
  _check_ptr();
  return AstStmt(mPtr->else_body());
}

// @brief case item のリストの取得
AstCaseItemList
AstStmt::caseitem_list() const
{
  _check_ptr();
  return AstCaseItemList(AstCaseItem(mPtr->caseitem_top()));
}

// @brief 初期化代入文の取得
AstStmt
AstStmt::init_stmt() const
{
  _check_ptr();
  return AstStmt(mPtr->init_stmt());
}

// @brief 繰り返し代入文の取得
AstStmt
AstStmt::next_stmt() const
{
  _check_ptr();
  return AstStmt(mPtr->next_stmt());
}

// @brief 宣言ヘッダのリストの取得
AstDeclHeadList
AstStmt::declhead_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->declhead_top()));
}

// @brief 子供のステートメントのリストの取得
AstStmtList
AstStmt::stmt_list() const
{
  _check_ptr();
  return AstStmtList(AstStmt(mPtr->stmt_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstStmt::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstStmt::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstStmt::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstStmt::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
AstStmt::namebranch_list() const
{
  _check_ptr();
  return AstNameBranchList(AstNameBranch(mPtr->namebranch_top()));
}

// @brief 次の要素を返す．
AstStmt
AstStmt::next() const
{
  _check_ptr();
  return AstStmt(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstStmt::json_sub(
  JsonValue& jobj
) const
{
  AstHierNamedBase::json_sub(jobj);
  switch ( type() ) {
  case Disable:       json_sub_disable(jobj); break;
  case Enable:        json_sub_enable(jobj, "Enable"); break;
  case SysEnable:     json_sub_enable(jobj, "SysEnable"); break;
  case DelayControl:  json_sub_control(jobj, "DelayControl"); break;
  case EventControl:  json_sub_control(jobj, "EventControl"); break;
  case Assign:        json_sub_assign(jobj, "Assign"); break;
  case NbAssign:      json_sub_assign(jobj, "NbAssign"); break;
  case Event:         json_sub_event(jobj); break;
  case Null:          json_sub_null(jobj); break;
  case If:            json_sub_if(jobj); break;
  case Case:          json_sub_case(jobj, "Case"); break;
  case CaseX:         json_sub_case(jobj, "CaseX"); break;
  case CaseZ:         json_sub_case(jobj, "CaseZ"); break;
  case Wait:          json_sub_wait(jobj); break;
  case Forever:       json_sub_forever(jobj); break;
  case Repeat:        json_sub_repeat(jobj); break;
  case While:         json_sub_while(jobj); break;
  case For:           json_sub_for(jobj); break;
  case PcAssign:      json_sub_pcassign(jobj, "PcAssign"); break;
  case Deassign:      json_sub_deassign(jobj, "Deassign"); break;
  case Force:         json_sub_pcassign(jobj, "Force"); break;
  case Release:       json_sub_deassign(jobj, "Release"); break;
  case ParBlock:      json_sub_block(jobj, "ParBlock"); break;
  case SeqBlock:      json_sub_block(jobj, "SeqBlock"); break;
  case NamedParBlock: json_sub_namedblock(jobj, "NamedParBlock"); break;
  case NamedSeqBlock: json_sub_namedblock(jobj, "NamedSeqBlock"); break;
  }
}

// @brief Disable 用の json_sub()
void
AstStmt::json_sub_disable(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Disable"));
}

// @brief Enable 用の json_sub()
void
AstStmt::json_sub_enable(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("arg_list", arg_list().json_obj());
}

// @brief EventControl/DelayControl 用の json_sub()
void
AstStmt::json_sub_control(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("body", body().json_obj());
  jobj.add("control", control().json_obj());
}

// @brief Assign/NbAssign 用の json_sub()
void
AstStmt::json_sub_assign(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("lhs", lhs().json_obj());
  jobj.add("rhs", rhs().json_obj());
  JsonUtils::add(jobj, "control", control());
}

// @brief Event 用の json_sub()
void
AstStmt::json_sub_event(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Event"));
  jobj.add("primary", primary().json_obj());
}

// @brief Null 用の json_sub()
void
AstStmt::json_sub_null(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Null"));
}

// @brief If 用の json_sub()
void
AstStmt::json_sub_if(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("If"));
  jobj.add("expr", expr().json_obj());
  jobj.add("body", body().json_obj());
  JsonUtils::add(jobj, "else_body", else_body());
}

// @brief Case 用の json_sub()
void
AstStmt::json_sub_case(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("expr", expr().json_obj());
  jobj.add("caseitem_list", caseitem_list().json_obj());
}

// @brief Wait 用の json_sub()
void
AstStmt::json_sub_wait(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Wati"));
  jobj.add("expr", expr().json_obj());
}

// @brief Forever 用の json_sub()
void
AstStmt::json_sub_forever(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Wait"));
  jobj.add("body", body().json_obj());
}

// @brief Repeat 用の json_sub()
void
AstStmt::json_sub_repeat(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Repat"));
  jobj.add("expr", expr().json_obj());
  jobj.add("body", body().json_obj());
}

// @brief While 用の json_sub()
void
AstStmt::json_sub_while(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("While"));
  jobj.add("expr", expr().json_obj());
  jobj.add("body", body().json_obj());
}

// @brief For 用の json_sub()
void
AstStmt::json_sub_for(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("For"));
  jobj.add("expr", expr().json_obj());
  jobj.add("init_stmt", init_stmt().json_obj());
  jobj.add("next_stmt", next_stmt().json_obj());
  jobj.add("body", body().json_obj());
}

// @brief PcAssign/Force 用の json_sub()
void
AstStmt::json_sub_pcassign(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("lhs", lhs().json_obj());
  jobj.add("rhs", rhs().json_obj());
}

// @brief Deassign 用の json_sub()
void
AstStmt::json_sub_deassign(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("lhs", lhs().json_obj());
}

// @brief ParBlock/SeqBlock 用の json_sub()
void
AstStmt::json_sub_block(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("stmt_list", stmt_list().json_obj());
}

// @brief NamedParBlock/NamedSeqBlock 用の json_sub()
void
AstStmt::json_sub_namedblock(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  jobj.add("stmt_list", stmt_list().json_obj());
}

END_NAMESPACE_YM_VERILOG
