
/// @file AstControl.cc
/// @brief AstControl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstControl.h"
#include "ym/vl/AstExpr.h"
#include "parser/PtControl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstControl
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
AstControl::Type
AstControl::type() const
{
  _check_ptr();
  return mPtr->type();
}

// @brief 遅延式の取得
AstExpr
AstControl::delay() const
{
  _check_ptr();
  return AstExpr(mPtr->delay());
}

// @brief イベントリストの取得
AstExprList
AstControl::event_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->event_top()));
}

// @brief 繰り返し数の取得
AstExpr
AstControl::rep_expr() const
{
  _check_ptr();
  return AstExpr(mPtr->rep_expr());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstControl::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstControl::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstControl::key() const
{
  return _key();
}

// @brief json_obj() の下請け関数
void
AstControl::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  switch ( type() ) {
  case Delay:
    jobj.add("type", JsonValue("Delay"));
    jobj.add("delay", delay().json_obj());
    break;

  case Event:
    jobj.add("type", JsonValue("Event"));
    jobj.add("event_list", event_list().json_obj());
    break;

  case Repeat:
    jobj.add("type", JsonValue("Repeat"));
    jobj.add("rep_expr", rep_expr().json_obj());
    jobj.add("event_list", event_list().json_obj());
    break;
  }
}

END_NAMESPACE_YM_VERILOG
