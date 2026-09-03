
/// @file AstAttrSpec.cc
/// @brief AstAttrSpec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstAttrSpec.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtAttrSpec.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstAttrSpec
//////////////////////////////////////////////////////////////////////

// @brief 式の取得
AstExpr
AstAttrSpec::expr() const
{
  _check_ptr();
  return AstExpr(mPtr->expr());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstAttrSpec::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstAttrSpec::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstAttrSpec::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstAttrSpec::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 次の要素を返す．
AstAttrSpec
AstAttrSpec::next() const
{
  _check_ptr();
  return AstAttrSpec(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstAttrSpec::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  JsonUtils::add(jobj, "expr", expr());
}

END_NAMESPACE_YM_VERILOG
