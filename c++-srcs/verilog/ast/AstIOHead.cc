
/// @file AstIOHead.cc
/// @brief AstDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstIOHead.h"
#include "ym/vl/AstIOItem.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstRange.h"
#include "parser/PtIOHead.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstIOHead
//////////////////////////////////////////////////////////////////////

// @brief 方向の取得
VpiDir
AstIOHead::direction() const
{
  _check_ptr();
  return mPtr->direction();
}

// @brief 補助的な型の取得
VpiAuxType
AstIOHead::aux_type() const
{
  _check_ptr();
  return mPtr->aux_type();
}

// @brief 補助的なネット型の取得
VpiNetType
AstIOHead::net_type() const
{
  _check_ptr();
  return mPtr->net_type();
}

// @brief 補助的な変数型の取得
VpiVarType
AstIOHead::var_type() const
{
  _check_ptr();
  return mPtr->var_type();
}

// @brief 符号の取得
bool
AstIOHead::is_signed() const
{
  _check_ptr();
  return mPtr->is_signed();
}

// @brief 範囲の取得
AstRange
AstIOHead::range() const
{
  _check_ptr();
  return AstRange(mPtr->range());
}

// @brief 要素のリストを返す．
AstIOItemList
AstIOHead::item_list() const
{
  _check_ptr();
  return AstIOItemList(AstIOItem(mPtr->item_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstIOHead::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstIOHead::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstIOHead::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstIOHead
AstIOHead::next() const
{
  _check_ptr();
  return AstIOHead(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstIOHead::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  switch ( direction() ) {
  case VpiDir::Input:  jobj.add("direction", JsonValue("Input")); break;
  case VpiDir::Output: jobj.add("direction", JsonValue("Output")); break;
  case VpiDir::Inout:  jobj.add("direction", JsonValue("Inout")); break;
  default: break;
  }
  switch ( aux_type() ) {
  case VpiAuxType::None: break;
  case VpiAuxType::Net:  jobj.add("aux_type", JsonValue("Net")); break;
  case VpiAuxType::Reg:  jobj.add("aux_type", JsonValue("Reg")); break;
  case VpiAuxType::Var:  jobj.add("aux_type", JsonValue("Var")); break;
  }
  JsonUtils::add(jobj, "net_type", net_type());
  JsonUtils::add(jobj, "var_type", var_type());
  if ( range().is_valid() ) {
    jobj.add("is_signed", JsonValue(is_signed()));
    jobj.add("range", range().json_obj());
  }
  jobj.add("item_list", item_list().json_obj());
}

END_NAMESPACE_YM_VERILOG
