
/// @file AstGenCaseItem.cc
/// @brief AstGenCaseItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstGenCaseItem.h"
#include "ym/vl/AstDeclHead.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstItem.h"
#include "parser/PtGenCaseItem.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstGenCaseItem
//////////////////////////////////////////////////////////////////////

// @brief ラベルリストの取得
AstExprList
AstGenCaseItem::label_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->label_top()));
}

// @brief 宣言リストの取得
AstDeclHeadList
AstGenCaseItem::declhead_list() const
{
  _check_ptr();
  return AstDeclHeadList(AstDeclHead(mPtr->declhead_top()));
}

// @brief item リストの取得
AstItemList
AstGenCaseItem::item_list() const
{
  _check_ptr();
  return AstItemList(AstItem(mPtr->item_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstGenCaseItem::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstGenCaseItem::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstGenCaseItem::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstGenCaseItem
AstGenCaseItem::next() const
{
  _check_ptr();
  return AstGenCaseItem(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstGenCaseItem::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("label_list", label_list().json_obj());
  JsonUtils::add_list(jobj, "declhead_list", declhead_list());
  JsonUtils::add_list(jobj, "item_list", item_list());
}

END_NAMESPACE_YM_VERILOG
