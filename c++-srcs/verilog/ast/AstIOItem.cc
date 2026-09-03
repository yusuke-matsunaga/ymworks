
/// @file AstIOItem.cc
/// @brief AstIOItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstIOItem.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtIOItem.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstIOItem
//////////////////////////////////////////////////////////////////////

// @brief 初期値の取得
AstExpr
AstIOItem::init_value() const
{
  _check_ptr();
  return AstExpr(mPtr->init_value());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstIOItem::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstIOItem::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstIOItem::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstIOItem::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 次の要素を返す．
AstIOItem
AstIOItem::next() const
{
  _check_ptr();
  return AstIOItem(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstIOItem::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  JsonUtils::add(jobj, "init_value", init_value());
}

END_NAMESPACE_YM_VERILOG
