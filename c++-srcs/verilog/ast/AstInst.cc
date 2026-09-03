
/// @file AstInst.cc
/// @brief AstInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstInst.h"
#include "ym/vl/AstConnection.h"
#include "ym/vl/AstRange.h"
#include "parser/PtInst.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstInst
//////////////////////////////////////////////////////////////////////

// @brief 範囲の取得
AstRange
AstInst::range() const
{
  _check_ptr();
  return AstRange(mPtr->range());
}

// @brief ポートリストの取得
AstConnectionList
AstInst::port_list() const
{
  _check_ptr();
  return AstConnectionList(AstConnection(mPtr->port_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstInst::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstInst::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstInst::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstInst::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 次の要素を返す．
AstInst
AstInst::next() const
{
  _check_ptr();
  return AstInst(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstInst::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  JsonUtils::add(jobj, "range", range());
  jobj.add("port_list", port_list().json_obj());
}

END_NAMESPACE_YM_VERILOG
