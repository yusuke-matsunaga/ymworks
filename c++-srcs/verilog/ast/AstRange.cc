
/// @file AstRange.cc
/// @brief AstRange の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstRange.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtRange.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstRange
//////////////////////////////////////////////////////////////////////

// @brief 範囲の MSB を取り出す．
AstExpr
AstRange::left() const
{
  _check_ptr();
  return AstExpr(mPtr->left());
}

// @brief 範囲の LSB を取り出す．
AstExpr
AstRange::right() const
{
  _check_ptr();
  return AstExpr(mPtr->right());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstRange::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstRange::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstRange::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstRange
AstRange::next() const
{
  _check_ptr();
  return AstRange(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstRange::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("left", left().json_obj());
  jobj.add("right", right().json_obj());
}

END_NAMESPACE_YM_VERILOG
