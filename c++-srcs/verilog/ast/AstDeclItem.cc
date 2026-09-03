
/// @file AstDecl.cc
/// @brief AstDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstDeclItem.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstRange.h"
#include "parser/PtDeclItem.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstDeclItem
//////////////////////////////////////////////////////////////////////

// @brief 範囲のリストを返す．
AstRangeList
AstDeclItem::range_list() const
{
  _check_ptr();
  return AstRangeList(AstRange(mPtr->range_top()));
}

// @brief 初期値の取得
AstExpr
AstDeclItem::init_value() const
{
  _check_ptr();
  return AstExpr(mPtr->init_value());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstDeclItem::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstDeclItem::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstDeclItem::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstDeclItem::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 次の要素を返す．
AstDeclItem
AstDeclItem::next() const
{
  _check_ptr();
  return AstDeclItem(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstDeclItem::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  JsonUtils::add_list(jobj, "range_list", range_list());
  JsonUtils::add(jobj, "init_value", init_value());
}

END_NAMESPACE_YM_VERILOG
