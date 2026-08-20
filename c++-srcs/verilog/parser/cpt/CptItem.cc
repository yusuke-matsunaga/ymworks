
/// @file CptItem.cc
/// @brief CptItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptItem
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
const char*
CptItem::name() const
{
  return nullptr;
}

// @brief パラメータ割り当ての要素数の取得
SizeType
CptItem::paramassign_num() const
{
  throw std::logic_error{"paramassign_num(): Does not have paramassign_list"};
}

// @brief パラメータ割り当ての取得
const AstConnection*
CptItem::paramassign(
  SizeType index
) const
{
  throw std::logic_error{"paramassign(): Does not have paramassign_list"};
}

// @brief パラメータ割り当てのリストの取得
AstConnectionVec
CptItem::paramassign_list() const
{
  throw std::logic_error{"paramassign_list(): Does not have paramassign_list"};
}

// @brief defparam の要素数の取得
SizeType
CptItem::defparam_num() const
{
  throw std::logic_error{"defparam_num(): Does not have defparam_list"};
}

// @brief defparam の取得
const AstDefParam*
CptItem::defparam(
  SizeType index
) const
{
  throw std::logic_error{"defparam(): Does not have defparam_list"};
}

// @brief defparam リストの取得
AstDefParamVec
CptItem::defparam_list() const
{
  throw std::logic_error{"defparam_list(): Does not have defparam_list"};
}

// @brief continuous assign の要素数の取得
SizeType
CptItem::contassign_num() const
{
  throw std::logic_error{"contassign_num(): Does not have contassign_list"};
}

// @brief continuous assign の取得
const AstContAssign*
CptItem::contassign(
  SizeType index
) const
{
  throw std::logic_error{"contassign(): Does not have contassign_list"};
}

// @brief continuous assign リストの取得
AstContAssignVec
CptItem::contassign_list() const
{
  throw std::logic_error{"contassign_list(): Does not have contassign_list"};
}

// @brief プリミティブタイプの取得
VpiPrimType
CptItem::prim_type() const
{
  throw std::logic_error{"prim_type(): Not an INSTANCE type"};
}

// @brief strength の取得
const AstStrength*
CptItem::strength() const
{
  throw std::logic_error{"strength(): Does not have strength attribute"};
}

// @brief delay の取得
const AstDelay*
CptItem::delay() const
{
  throw std::logic_error{"delay(): Does not have delay attribute"};
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptItem::inst_num() const
{
  throw std::logic_error{"inst_num(): Does not have inst_list"};
}

// @brief module/UDP/gate instance リストの取得
const AstInst*
CptItem::inst(
  SizeType index
) const
{
  throw std::logic_error{"inst(): Does not have inst_list"};
}

// @brief module/UDP/gate instance リストの取得
AstInstVec
CptItem::inst_list() const
{
  throw std::logic_error{"inst_list(): Does not have inst_list"};
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptItem::declhead_num() const
{
  throw std::logic_error{"declhead_num(): Does not have declhead_list"};
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptItem::declhead(
  SizeType index
) const
{
  throw std::logic_error{"declhead(): Does not have declhead_list"};
}

// @brief 宣言ヘッダリストの取得
AstDeclHeadVec
CptItem::declhead_list() const
{
  throw std::logic_error{"declhead_list(): Does not have declhead_list"};
}

// @brief item の要素数の取得
SizeType
CptItem::item_num() const
{
  throw std::logic_error{"item_num(): Does not have item_list"};
}

// @brief item の取得
const AstItem*
CptItem::item(
  SizeType index
) const
{
  throw std::logic_error{"item(): Does not have item_list"};
}

// @brief item リストの取得
AstItemVec
CptItem::item_list() const
{
  throw std::logic_error{"item_list(): Does not have item_list"};
}

// @brief 本体のステートメントの取得
const AstStmt*
CptItem::body() const
{
  throw std::logic_error{"body(): Does not have body"};
}

// @brief automatic 情報の取得
bool
CptItem::automatic() const
{
  throw std::logic_error{"automatic(): type mismatch"};
}

// @brief IO宣言の要素数の取得
SizeType
CptItem::ioitem_num() const
{
  throw std::logic_error{"ioitem_num(): type mismatch"};
}

// @brief IO宣言ヘッダリストの要素数の取得
SizeType
CptItem::iohead_num() const
{
  throw std::logic_error{"iohead_num(): Not a Task/Func type"};
}

// @brief IO宣言ヘッダの取得
const AstIOHead*
CptItem::iohead(
  SizeType index
) const
{
  throw std::logic_error{"iohead(): Not a Task/Func type"};
}

// @brief IO宣言ヘッダリストの取得
AstIOHeadVec
CptItem::iohead_list() const
{
  throw std::logic_error{"iohead_list(): Not a Task/Func type"};
}

// @brief 符号の取得
bool
CptItem::is_signed() const
{
  throw std::logic_error{"is_signed(): Not a Func type"};
}

// @brief 範囲の取得
const AstRange*
CptItem::range() const
{
  throw std::logic_error{"range(): Not a Func type"};
}

// @brief 戻値のデータ型の取得
VpiVarType
CptItem::data_type() const
{
  throw std::logic_error{"data_type(): Not a Func type"};
}

// @brief constant function の展開中の印をつける．
void
CptItem::set_in_use() const
{
  throw std::logic_error{"set_in_use(): Not a Func type"};
}

// @brief constant function の展開中の印を消す．
void
CptItem::clear_in_use() const
{
  throw std::logic_error{"clear_in_use(): Not a Func type"};
}

// @brief 使用中(constant function として展開中)のチェック
bool
CptItem::is_in_use() const
{
  throw std::logic_error{"is_in_use(): Not a Func type"};
}

// @brief specify block item の種類の取得
VpiSpecItemType
CptItem::specitem_type() const
{
  throw std::logic_error{"specitem_type(): Not a SpecItem type"};
}

// @brief ターミナルの要素数の取得
SizeType
CptItem::terminal_num() const
{
  throw std::logic_error{"terminal_num(): Not a SpecItem type"};
}

// @brief ターミナルの取得
const AstExpr*
CptItem::terminal(
  SizeType index
) const
{
  throw std::logic_error{"terminal(): Not a SpecItem type"};
}

// @brief ターミナルリストの取得
AstExprVec
CptItem::terminal_list() const
{
  throw std::logic_error{"terminal_list(): Not a SpecItem type"};
}

// @brief specify block path の種類の取得
VpiSpecPathType
CptItem::specpath_type() const
{
  throw std::logic_error{"specpath_type(): Not a SPECPATH type"};
}

// @brief 条件式の取得
const AstExpr*
CptItem::cond_expr() const
{
  throw std::logic_error{"cond_expr(): type mismatch"};
}

// @brief パス記述の取得
const AstPathDecl*
CptItem::path_decl() const
{
  throw std::logic_error{"Not a SpecPath type"};
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
SizeType
CptItem::then_declhead_num() const
{
  throw std::logic_error{"then_declhead_num(): Not a GenIf type"};
}

// @brief 条件が成り立った時に生成される宣言ヘッダの取得i
const AstDeclHead*
CptItem::then_declhead(
  SizeType index
) const
{
  throw std::logic_error{"then_declhead(): Not a GenIf type"};
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
AstDeclHeadVec
CptItem::then_declhead_list() const
{
  throw std::logic_error{"then_declhead_list(): Not a GenIf type"};
}

// @brief 条件が成り立ったときに生成される要素数の取得
SizeType
CptItem::then_item_num() const
{
  throw std::logic_error{"then_item_num(): Not a GenIf type"};
}

// @brief 条件が成り立った時に生成される要素の取得
const AstItem*
CptItem::then_item(
  SizeType index
) const
{
  throw std::logic_error{"then_item(): Not a GenIf type"};
}

// @brief 条件が成り立った時に生成される要素リストの取得
AstItemVec
CptItem::then_item_list() const
{
  throw std::logic_error{"then_item_list(): Not a GenIf type"};
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
CptItem::else_declhead_num() const
{
  throw std::logic_error{"else_declhead_num(): Not a GenIf type"};
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダの取得
const AstDeclHead*
CptItem::else_declhead(
  SizeType index
) const
{
  throw std::logic_error{"else_declhead(): Not a GenIf type"};
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadVec
CptItem::else_declhead_list() const
{
  throw std::logic_error{"else_declhead_list(): Not a GenIf type"};
}

// @brief 条件が成り立たなかったときに生成される要素数の取得
SizeType
CptItem::else_item_num() const
{
  throw std::logic_error{"else_item_num(): Not a GenIf type"};
}

// @brief 条件が成り立たなかった時に生成される要素の取得
const AstItem*
CptItem::else_item(
  SizeType index
) const
{
  throw std::logic_error{"else_item(): Not a GenIf type"};
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemVec
CptItem::else_item_list() const
{
  throw std::logic_error{"else_item_list(): Not a GenIf type"};
}

// @brief case item のリストの要素数の取得
SizeType
CptItem::caseitem_num() const
{
  throw std::logic_error{"caseitem_num(): Not a GenCase type"};
}

// @brief case item の取得
const AstGenCaseItem*
CptItem::caseitem(
  SizeType index
) const
{
  throw std::logic_error{"caseitem(): Not a GenCase type"};
}

// @brief case item リストの取得
AstGenCaseItemVec
CptItem::caseitem_list() const
{
  throw std::logic_error{"caseitem_list(): Not a GenCase type"};
}

// @brief 繰り返し制御用の変数名の取得
const char*
CptItem::loop_var() const
{
  throw std::logic_error{"loop_var(): Not a GenFor type"};
}

// @brief 初期化文の右辺の取得
const AstExpr*
CptItem::init_expr() const
{
  throw std::logic_error{"init_expr(): Not a GenFor type"};
}

// @brief 増加文の右辺の取得
const AstExpr*
CptItem::next_expr() const
{
  throw std::logic_error{"next_expr(): Not a GenFor type"};
}

END_NAMESPACE_YM_VERILOG
