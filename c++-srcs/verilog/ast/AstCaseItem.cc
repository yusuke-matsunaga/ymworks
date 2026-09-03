
/// @file AstCaseItem.cc
/// @brief AstCaseItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstCaseItem.h"
#include "ym/vl/AstStmt.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtCaseItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief ラベルリストの取得
AstExprList
AstCaseItem::label_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->label_top()));
}

// @brief 本体のステートメントの取得
AstStmt
AstCaseItem::body() const
{
  _check_ptr();
  return AstStmt(mPtr->body());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstCaseItem::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstCaseItem::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstCaseItem::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstCaseItem
AstCaseItem::next() const
{
  _check_ptr();
  return AstCaseItem(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstCaseItem::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("label_list", label_list().json_obj());
  jobj.add("body", body().json_obj());
}

END_NAMESPACE_YM_VERILOG
