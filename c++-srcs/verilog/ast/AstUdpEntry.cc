
/// @file AstUdpEntry.cc
/// @brief AstUdpEntry の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstUdpEntry.h"
#include "ym/vl/AstUdpValue.h"
#include "parser/PtUdpEntry.h"
#include "parser/PtUdpValue.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstUdpEntry
//////////////////////////////////////////////////////////////////////

// @brief 入力値のリストを取り出す．
AstUdpValueList
AstUdpEntry::input_list() const
{
  _check_ptr();
  return AstUdpValueList(AstUdpValue(mPtr->input_top()));
}

// @brief 現状態の値を取り出す．
AstUdpValue
AstUdpEntry::current() const
{
  _check_ptr();
  return AstUdpValue(mPtr->current());
}

// @brief 出力の値を取り出す．
AstUdpValue
AstUdpEntry::output() const
{
  _check_ptr();
  return AstUdpValue(mPtr->output());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstUdpEntry::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstUdpEntry::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstUdpEntry::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstUdpEntry
AstUdpEntry::next() const
{
  _check_ptr();
  return AstUdpEntry(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstUdpEntry::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("input_list", input_list().json_obj());
  JsonUtils::add(jobj, "current", current());
  jobj.add("output", output().json_obj());
}

END_NAMESPACE_YM_VERILOG
