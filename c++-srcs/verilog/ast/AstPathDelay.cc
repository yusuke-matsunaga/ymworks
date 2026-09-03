
/// @file AstPathDelay.cc
/// @brief AstPathDelay の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstPathDelay.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtPathDelay.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstPathDelay
//////////////////////////////////////////////////////////////////////

// @brief 値を取り出す．
AstExpr
AstPathDelay::value(
  SizeType pos
) const
{
  _check_ptr();
  return AstExpr(mPtr->value(pos));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstPathDelay::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstPathDelay::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstPathDelay::key() const
{
  return _key();
}

// @brief json_obj() の下請け関数
void
AstPathDelay::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  #warning "TODO: 仕様確認"
}

END_NAMESPACE_YM_VERILOG
