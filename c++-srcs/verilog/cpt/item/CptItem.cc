
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
  throw std::logic_error{"name(): type mismatch"};
}

// @brief パラメータ割り当てのリストの先頭の取得
const PtConnection*
CptItem::paramassign_top() const
{
  throw std::logic_error{"paramassign_top(): type mismatch"};
}

// @brief defparam リストの取得
const PtDefParam*
CptItem::defparam_top() const
{
  throw std::logic_error{"defparam_top(): type mismatch"};
}

// @brief continuous assign リストの取得
const PtContAssign*
CptItem::contassign_top() const
{
  throw std::logic_error{"contassign_top(): type mismtach"};
}

// @brief プリミティブタイプの取得
VpiPrimType
CptItem::prim_type() const
{
  throw std::logic_error{"prim_type(): Not an INSTANCE type"};
}

// @brief strength の取得
const PtStrength*
CptItem::strength() const
{
  throw std::logic_error{"strength(): Does not have strength attribute"};
}

// @brief delay の取得
const PtDelay*
CptItem::delay() const
{
  throw std::logic_error{"delay(): Does not have delay attribute"};
}

// @brief module/UDP/gate instance リストの取得
const PtInst*
CptItem::inst_top() const
{
  throw std::logic_error{"inst_top(): type mismtach"};
}

// @brief 宣言ヘッダリストの取得
const PtDeclHead*
CptItem::declhead_top() const
{
  throw std::logic_error{"declhead_top(): type mismatch"};
}

// @brief item リストの取得
const PtItem*
CptItem::item_top() const
{
  throw std::logic_error{"item_top(): type mismatch"};
}

// @brief 本体のステートメントの取得
const PtStmt*
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

// @brief IO宣言ヘッダリストの取得
const PtIOHead*
CptItem::iohead_top() const
{
  throw std::logic_error{"iohead_top(): type mismatch"};
}

// @brief IO宣言の要素数の取得
SizeType
CptItem::ioitem_num() const
{
  throw std::logic_error{"ioitem_num(): type mismatch"};
}

// @brief 符号の取得
bool
CptItem::is_signed() const
{
  throw std::logic_error{"is_signed(): Not a Func type"};
}

// @brief 範囲の取得
const PtRange*
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

// @brief ターミナルリストの取得
const PtExpr*
CptItem::terminal_top() const
{
  throw std::logic_error{"terminal_top(): type mismatch"};
}

// @brief specify block path の種類の取得
VpiSpecPathType
CptItem::specpath_type() const
{
  throw std::logic_error{"specpath_type(): Not a SPECPATH type"};
}

// @brief 条件式の取得
const PtExpr*
CptItem::cond_expr() const
{
  throw std::logic_error{"cond_expr(): type mismatch"};
}

// @brief パス記述の取得
const PtPathDecl*
CptItem::path_decl() const
{
  throw std::logic_error{"Not a SpecPath type"};
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
const PtDeclHead*
CptItem::then_declhead_top() const
{
  throw std::logic_error{"then_declhead_top(): type mismatch"};
}

// @brief 条件が成り立った時に生成される要素リストの取得
const PtItem*
CptItem::then_item_top() const
{
  throw std::logic_error{"then_item_top(): type mismatch"};
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
const PtDeclHead*
CptItem::else_declhead_top() const
{
  throw std::logic_error{"else_declhead_top(): type mismatch"};
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
const PtItem*
CptItem::else_item_top() const
{
  throw std::logic_error{"else_item_top(): type mismatch"};
}

// @brief case item リストの取得
const PtGenCaseItem*
CptItem::caseitem_top() const
{
  throw std::logic_error{"caseitem_top(): Not a GenCase type"};
}

// @brief 繰り返し制御用の変数名の取得
const char*
CptItem::loop_var() const
{
  throw std::logic_error{"loop_var(): Not a GenFor type"};
}

// @brief 初期化文の右辺の取得
const PtExpr*
CptItem::init_expr() const
{
  throw std::logic_error{"init_expr(): Not a GenFor type"};
}

// @brief 増加文の右辺の取得
const PtExpr*
CptItem::next_expr() const
{
  throw std::logic_error{"next_expr(): Not a GenFor type"};
}

END_NAMESPACE_YM_VERILOG
