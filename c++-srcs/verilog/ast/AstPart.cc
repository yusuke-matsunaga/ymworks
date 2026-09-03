
/// @file AstPart.cc
/// @brief AstPart の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstPart.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtPart.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstPart
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
AstPart::mode() const
{
  _check_ptr();
  return mPtr->mode();
}

// @brief 1番目の式を取り出す．
AstExpr
AstPart::left() const
{
  _check_ptr();
  return AstExpr(mPtr->left());
}

// @brief 2番めの式を取り出す．
AstExpr
AstPart::right() const
{
  _check_ptr();
  return AstExpr(mPtr->right());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstPart::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstPart::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstPart::key() const
{
  return _key();
}

// @brief json_obj() の下請け関数
void
AstPart::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);

  const char* type_str = nullptr;
  switch ( mode() ) {
  case VpiRangeMode::Const: type_str = "Const"; break;
  case VpiRangeMode::Plus:  type_str = "Plus"; break;
  case VpiRangeMode::Minus: type_str = "Minus"; break;
  default: break;
  }
  jobj.add("mode", JsonValue(type_str));
  jobj.add("left", left().json_obj());
  jobj.add("right", right().json_obj());
}

END_NAMESPACE_YM_VERILOG
