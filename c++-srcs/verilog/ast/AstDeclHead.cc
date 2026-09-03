
/// @file AstDeclHead.cc
/// @brief AstDeclHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstDeclHead.h"
#include "ym/vl/AstDeclItem.h"
#include "ym/vl/AstRange.h"
#include "ym/vl/AstStrength.h"
#include "ym/vl/AstDelay.h"
#include "parser/PtDeclHead.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstDeclHead
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素の型の取得
AstDeclHead::Type
AstDeclHead::type() const
{
  _check_ptr();
  return mPtr->type();
}

// @brief 符号の取得
bool
AstDeclHead::is_signed() const
{
  _check_ptr();
  return mPtr->is_signed();
}

// @brief 範囲Bの取得
AstRange
AstDeclHead::range() const
{
  _check_ptr();
  return AstRange(mPtr->range());
}

// @brief データ型の取得
VpiVarType
AstDeclHead::data_type() const
{
  _check_ptr();
  return mPtr->data_type();
}

// @brief ネット型の取得
VpiNetType
AstDeclHead::net_type() const
{
  _check_ptr();
  return mPtr->net_type();
}

// @brief vectored|scalared 属性の取得
VpiVsType
AstDeclHead::vs_type() const
{
  _check_ptr();
  return mPtr->vs_type();
}

// @brief strength の取得
AstStrength
AstDeclHead::strength() const
{
  _check_ptr();
  return AstStrength(mPtr->strength());
}

// @brief delay の取得
AstDelay
AstDeclHead::delay() const
{
  _check_ptr();
  return AstDelay(mPtr->delay());
}

// @brief 要素のリストを返す．
AstDeclItemList
AstDeclHead::item_list() const
{
  _check_ptr();
  return AstDeclItemList(AstDeclItem(mPtr->item_top()));
}

// @brief 適切な値を持っている時 true を返す．
bool
AstDeclHead::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstDeclHead::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstDeclHead::key() const
{
  return _key();
}

// @brief 次の要素を返す．
AstDeclHead
AstDeclHead::next() const
{
  _check_ptr();
  return AstDeclHead(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstDeclHead::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  switch ( type() ) {
  case Param:      jobj.add("type", JsonValue("Param")); break;
  case LocalParam: jobj.add("type", JsonValue("LocalParam")); break;
  case Reg:        jobj.add("type", JsonValue("Reg")); break;
  case Var:        jobj.add("type", JsonValue("Var")); break;
  case Genvar:     jobj.add("type", JsonValue("Genvar")); break;
  case Net:        jobj.add("type", JsonValue("Net")); break;
  case Event:      jobj.add("type", JsonValue("Eevent")); break;
  case SpecParam:  jobj.add("type", JsonValue("SpecParam")); break;
  }
  if ( range().is_valid() ) {
    jobj.add("is_signed", JsonValue(is_signed()));
    jobj.add("range", range().json_obj());
  }
  JsonUtils::add(jobj, "data_type", data_type());
  JsonUtils::add(jobj, "net_type", net_type());
  switch ( vs_type() ) {
  case VpiVsType::None:     break;
  case VpiVsType::Vectored: jobj.add("vs_type", JsonValue("Vectored")); break;
  case VpiVsType::Scalared: jobj.add("vs_type", JsonValue("Scalared")); break;
  }
  JsonUtils::add(jobj, "strength", strength());
  JsonUtils::add(jobj, "delay", delay());
  jobj.add("item_list", item_list().json_obj());
}

END_NAMESPACE_YM_VERILOG
