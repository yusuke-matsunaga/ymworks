
/// @file AstConnection.cc
/// @brief AstConnection の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstConnection.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtConnection.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstConnection
//////////////////////////////////////////////////////////////////////

// @brief 名前の取得
const char*
AstConnection::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 式の取得
AstExpr
AstConnection::expr() const
{
  _check_ptr();
  return AstExpr(mPtr->expr());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstConnection::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstConnection::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstConnection::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstConnection
AstConnection::next() const
{
  _check_ptr();
  return AstConnection(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstConnection::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  JsonUtils::add(jobj, "name", name());
  JsonUtils::add(jobj, "expr", expr());
}

END_NAMESPACE_YM_VERILOG
