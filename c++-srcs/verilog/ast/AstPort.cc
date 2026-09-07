
/// @file AstPort.cc
/// @brief AstPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstPort.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtPort.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstPort
//////////////////////////////////////////////////////////////////////

// @brief 外向の名前の取得
const char*
AstPort::ext_name() const
{
  _check_ptr();
  return mPtr->ext_name();
}

// @brief 内部の結線を表す式の取得
AstExpr
AstPort::expr() const
{
  _check_ptr();
  return AstExpr(mPtr->expr());
}

// @brief 内部のポート結線のリストの取得
AstExprList
AstPort::portref_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->portref_top()));
}

#if 0
// @brief 内部のポート結線の向きの取得
VpiDir
AstPort::portref_dir(
  SizeType index
) const
{
  _check_ptr();
  return mPtr->portref_dir(index);
}
#endif

// @brief 適切な値を持っている時 true を返す．
bool
AstPort::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstPort::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstPort::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstPort
AstPort::next() const
{
  _check_ptr();
  return AstPort(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstPort::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  JsonUtils::add(jobj, "ext_name", ext_name());
  JsonUtils::add(jobj, "expr", expr());
  JsonUtils::add_list(jobj, "portref_list", portref_list());
}

END_NAMESPACE_YM_VERILOG
