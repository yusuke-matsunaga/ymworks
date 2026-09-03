
/// @file AstDelay.cc
/// @brief AstDelay の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstDelay.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtDelay.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstDelay
//////////////////////////////////////////////////////////////////////

// @brief 値の取得
AstExpr
AstDelay::value0() const
{
  _check_ptr();
  return AstExpr(mPtr->value0());
}

// @brief 値の取得
AstExpr
AstDelay::value1() const
{
  _check_ptr();
  return AstExpr(mPtr->value1());
}

// @brief 値の取得
AstExpr
AstDelay::value2() const
{
  _check_ptr();
  return AstExpr(mPtr->value2());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstDelay::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstDelay::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstDelay::key() const
{
  return _key();
}

// @brief json_obj() の下請け関数
void
AstDelay::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  JsonUtils::add(jobj, "value0", value0());
  JsonUtils::add(jobj, "value1", value1());
  JsonUtils::add(jobj, "value2", value2());
}

END_NAMESPACE_YM_VERILOG
