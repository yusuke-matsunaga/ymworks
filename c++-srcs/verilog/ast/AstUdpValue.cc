
/// @file AstUdpValue.cc
/// @brief AstUdpValue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstUdpValue.h"
#include "ym/vl/VlUdpVal.h"
#include "parser/PtUdpValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstUdpValue
//////////////////////////////////////////////////////////////////////

// @brief 値を取り出す．
VlUdpVal
AstUdpValue::symbol() const
{
  _check_ptr();
  return mPtr->symbol();
}

// @brief 適切な値を持っている時 true を返す．
bool
AstUdpValue::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstUdpValue::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstUdpValue::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstUdpValue
AstUdpValue::next() const
{
  _check_ptr();
  return AstUdpValue(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstUdpValue::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("symbol", JsonValue(symbol().to_string()));
}


END_NAMESPACE_YM_VERILOG
