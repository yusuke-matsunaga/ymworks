
/// @file AstItem.cc
/// @brief AstItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstItem.h"
#include "ym/vl/AstConnection.h"
#include "ym/vl/AstContAssign.h"
#include "ym/vl/AstDeclHead.h"
#include "ym/vl/AstDefParam.h"
#include "ym/vl/AstDelay.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstGenCaseItem.h"
#include "ym/vl/AstInst.h"
#include "ym/vl/AstIOHead.h"
#include "ym/vl/AstPathDecl.h"
#include "ym/vl/AstPathDelay.h"
#include "ym/vl/AstRange.h"
#include "ym/vl/AstStmt.h"
#include "ym/vl/AstStrength.h"
#include "parser/PtItem.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstItem
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
AstItem::Type
AstItem::type() const
{
  _check_ptr();
  return mPtr->type();
}

// @brief 条件式の取得
AstExpr
AstItem::cond_expr() const
{
  _check_ptr();
  return AstExpr(mPtr->cond_expr());
}

// @brief 本体のステートメントの取得
AstStmt
AstItem::body() const
{
  _check_ptr();
  return AstStmt(mPtr->body());
}

// @brief strength の取得
AstStrength
AstItem::strength() const
{
  _check_ptr();
  return AstStrength(mPtr->strength());
}

// @brief delay の取得
AstDelay
AstItem::delay() const
{
  _check_ptr();
  return AstDelay(mPtr->delay());
}

// @brief defparam リストの取得
AstDefParamList
AstItem::defparam_list() const
{
  _check_ptr();
  return AstDefParamList(AstDefParam(mPtr->defparam_top()));
}

// @brief continuous assign リストの取得
AstContAssignList
AstItem::contassign_list() const
{
  _check_ptr();
  return AstContAssignList(AstContAssign(mPtr->contassign_top()));
}

// @brief プリミティブタイプの取得
VpiPrimType
AstItem::prim_type() const
{
  _check_ptr();
  return mPtr->prim_type();
}

// @brief パラメータ割り当てのリストの取得
AstConnectionList
AstItem::paramassign_list() const
{
  _check_ptr();
  return AstConnectionList(AstConnection(mPtr->paramassign_top()));
}

// @brief module/UDP/gate instance リストの取得
AstInstList
AstItem::inst_list() const
{
  _check_ptr();
  return AstInstList(AstInst(mPtr->inst_top()));
}

// @brief 宣言ヘッダリストの取得
AstDeclHeadList
AstItem::declhead_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->declhead_top()));
}

// @brief item リストの取得
AstItemList
AstItem::item_list() const
{
  _check_ptr();
  return AstItemList(AstItem(mPtr->item_top()));
}

// @brief automatic 情報の取得
bool
AstItem::automatic() const
{
  _check_ptr();
  return mPtr->automatic();
}

// @brief IO宣言ヘッダリストの取得
AstIOHeadList
AstItem::iohead_list() const
{
  _check_ptr();
  return AstIOHeadList(AstIOHead(mPtr->iohead_top()));
}

// @brief IO要素の数
SizeType
AstItem::ioitem_num() const
{
  return mPtr->ioitem_num();
}

// @brief 符号の取得
bool
AstItem::is_signed() const
{
  _check_ptr();
  return mPtr->is_signed();
}

// @brief 範囲の取得
AstRange
AstItem::range() const
{
  _check_ptr();
  return AstRange(mPtr->range());
}

// @brief 戻値のデータ型の取得
VpiVarType
AstItem::data_type() const
{
  _check_ptr();
  return mPtr->data_type();
}

// @brief constant function の展開中の印をつける．
void
AstItem::set_in_use() const
{
  _check_ptr();
  mPtr->set_in_use();
}

// @brief constant function の展開中の印を消す．
void
AstItem::clear_in_use() const
{
  _check_ptr();
  mPtr->clear_in_use();
}

// @brief 使用中(constant function として展開中)のチェック
bool
AstItem::is_in_use() const
{
  _check_ptr();
  return mPtr->is_in_use();
}

// @brief specify block item の種類の取得
VpiSpecItemType
AstItem::specitem_type() const
{
  _check_ptr();
  return mPtr->specitem_type();
}

// @brief ターミナルリストの取得
AstExprList
AstItem::terminal_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->terminal_top()));
}

// @brief specify block path の種類の取得
VpiSpecPathType
AstItem::specpath_type() const
{
  _check_ptr();
  return mPtr->specpath_type();
}

// @brief パス記述の取得
AstPathDecl
AstItem::path_decl() const
{
  _check_ptr();
  return AstPathDecl(mPtr->path_decl());
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
AstDeclHeadList
AstItem::then_declhead_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->then_declhead_top()));
}

// @brief 条件が成り立った時に生成される要素リストの取得
AstItemList
AstItem::then_item_list() const
{
  _check_ptr();
  return AstItemList(AstItem(mPtr->then_item_top()));
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadList
AstItem::else_declhead_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->else_declhead_top()));
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemList
AstItem::else_item_list() const
{
  _check_ptr();
  return AstItemList(AstItem(mPtr->else_item_top()));
}

// @brief case item リストの取得
AstGenCaseItemList
AstItem::caseitem_list() const
{
  _check_ptr();
  return AstGenCaseItemList(AstGenCaseItem(mPtr->caseitem_top()));
}

// @brief 繰り返し制御用の変数名の取得
const char*
AstItem::loop_var() const
{
  _check_ptr();
  return mPtr->loop_var();
}

// @brief 初期化文の右辺の取得
AstExpr
AstItem::init_expr() const
{
  _check_ptr();
  return AstExpr(mPtr->init_expr());
}

// @brief 増加文の右辺の取得
AstExpr
AstItem::next_expr() const
{
  _check_ptr();
  return AstExpr(mPtr->next_expr());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstItem::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstItem::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstItem::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstItem::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 次の要素を返す．
AstItem
AstItem::next() const
{
  _check_ptr();
  return AstItem(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstItem::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  switch ( type() ) {
  case DefParam:   json_sub_defparam(jobj); break;
  case ContAssign: json_sub_contassign(jobj); break;
  case Initial:    json_sub_process(jobj, "Initial"); break;
  case Always:     json_sub_process(jobj, "Always"); break;
  case Task:       json_sub_task(jobj); break;
  case Func:       json_sub_func(jobj); break;
  case GateInst:   json_sub_gateinst(jobj); break;
  case MuInst:     json_sub_muinst(jobj); break;
  case SpecItem:   json_sub_specitem(jobj); break;
  case SpecPath:   json_sub_specpath(jobj); break;
  case Generate:   json_sub_generate(jobj); break;
  case GenBlock:   json_sub_genblock(jobj); break;
  case GenIf:      json_sub_genif(jobj); break;
  case GenCase:    json_sub_gencase(jobj); break;
  case GenFor:     json_sub_genfor(jobj); break;
  }
}

// @brief DefParam 用の json_sub()
void
AstItem::json_sub_defparam(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("DefParam"));
  jobj.add("defparam_list", defparam_list().json_obj());
}

// @brief ContAssign 用の json_sub()
void
AstItem::json_sub_contassign(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("ContAssign"));
  JsonUtils::add(jobj, "strength", strength());
  JsonUtils::add(jobj, "delay", delay());
  jobj.add("contassign_list", contassign_list().json_obj());
}

// @brief Initial/Process 用の json_sub()
void
AstItem::json_sub_process(
  JsonValue& jobj,
  const char* label
) const
{
  jobj.add("type", JsonValue(label));
  jobj.add("body", body().json_obj());
}

// @brief Task 用の json_sub()
void
AstItem::json_sub_task(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Task"));
  jobj.add("automatic", JsonValue(automatic()));
  jobj.add("iohead_list", iohead_list().json_obj());
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  jobj.add("body", body().json_obj());
}

// @brief Func 用の json_sub()
void
AstItem::json_sub_func(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Func"));
  jobj.add("automatic", JsonValue(automatic()));
  if ( range().is_valid() ) {
    jobj.add("is_signed", JsonValue(is_signed()));
    jobj.add("range", range().json_obj());
  }
  JsonUtils::add(jobj, "data_type", data_type());
  jobj.add("iohead_list", iohead_list().json_obj());
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  jobj.add("body", body().json_obj());
}

// @brief GateInst 用の json_sub()
void
AstItem::json_sub_gateinst(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("GateInst"));
  JsonUtils::add(jobj, "prim_type", prim_type());
  jobj.add("inst_list", inst_list().json_obj());
}

// @brief MuInst 用の json_sub()
void
AstItem::json_sub_muinst(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("MuInst"));
  JsonUtils::add_list(jobj, "paramassign_list", paramassign_list());
  jobj.add("inst_list", inst_list().json_obj());
}

// @brief SpecItem 用の json_sub()
void
AstItem::json_sub_specitem(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("SpecItem"));
  const char* type_str = nullptr;
  switch ( specitem_type() ) {
  case VpiSpecItemType::PulsestyleOnEvent:  type_str = "PulsestyleOnEvent"; break;
  case VpiSpecItemType::PulsestyleOnDetect: type_str = "PulsestyleOnDetect"; break;
  case VpiSpecItemType::Showcancelled:      type_str = "Showcancelled"; break;
  case VpiSpecItemType::Noshowcancelled:    type_str = "Noshowcancelled"; break;
  }
  jobj.add("specitem_type", JsonValue(type_str));
  jobj.add("terminal_list", terminal_list().json_obj());
}

// @brief SpecPath 用の json_sub()
void
AstItem::json_sub_specpath(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("SpecPath"));
  const char* type_str = nullptr;
  switch ( specpath_type() ) {
  case VpiSpecPathType::Null:   type_str = "Null"; break;
  case VpiSpecPathType::If:     type_str = "If"; break;
  case VpiSpecPathType::Ifnone: type_str = "Ifnone"; break;
  }
  jobj.add("specpath_type", JsonValue(type_str));
  jobj.add("cond_expr", cond_expr().json_obj());
  jobj.add("path_decl", path_decl().json_obj());
}

// @brief Generate 用の json_sub()
void
AstItem::json_sub_generate(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("Generate"));
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  JsonUtils::add_list(jobj, "item_list", item_list());
}

// @brief GenBlock 用の json_sub()
void
AstItem::json_sub_genblock(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("GenBlock"));
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  JsonUtils::add_list(jobj, "item_list", item_list());
}

// @brief GenIf 用の json_sub()
void
AstItem::json_sub_genif(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("GenIf"));
  jobj.add("cond_expr", cond_expr().json_obj());
  JsonUtils::add_list(jobj, "then_declhead_list", then_declhead_list());
  JsonUtils::add_list(jobj, "then_item_list", then_item_list());
  JsonUtils::add_list(jobj, "else_declhead_list", else_declhead_list());
  JsonUtils::add_list(jobj, "else_item_list", else_item_list());
}

// @brief GenCase 用の json_sub()
void
AstItem::json_sub_gencase(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("GenCase"));
  jobj.add("cond_expr", cond_expr().json_obj());
  jobj.add("caseitem_list", caseitem_list().json_obj());
}

// @brief GenFor 用の json_sub()
void
AstItem::json_sub_genfor(
  JsonValue& jobj
) const
{
  jobj.add("type", JsonValue("GenFor"));
  jobj.add("cond_expr", cond_expr().json_obj());
  jobj.add("loop_var", JsonValue(loop_var()));
  jobj.add("init_expr", init_expr().json_obj());
  jobj.add("next_expr", next_expr().json_obj());
  jobj.add("body", body().json_obj());
}

END_NAMESPACE_YM_VERILOG
