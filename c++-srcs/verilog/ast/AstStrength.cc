
/// @file AstStrength.cc
/// @brief AstStrength の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstStrength.h"
#include "parser/PtStrength.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstStrength
//////////////////////////////////////////////////////////////////////

// @brief drive strength0 の取得
VpiStrength
AstStrength::drive0() const
{
  _check_ptr();
  return mPtr->drive0();
}

// @brief drive strength1 の取得
VpiStrength
AstStrength::drive1() const
{
  _check_ptr();
  return mPtr->drive1();
}

// @brief charge strength の取得
VpiStrength
AstStrength::charge() const
{
  _check_ptr();
  return mPtr->charge();
}

// @brief 適切な値を持っている時 true を返す．
bool
AstStrength::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstStrength::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstStrength::key() const
{
  return _key();
}

// @brief json_obj() の下請け関数
void
AstStrength::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  JsonUtils::add(jobj, "driver0", drive0());
  JsonUtils::add(jobj, "driver1", drive1());
  JsonUtils::add(jobj, "charge", charge());
}

END_NAMESPACE_YM_VERILOG
