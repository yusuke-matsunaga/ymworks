
/// @file AstDefParam.cc
/// @brief AstDefParam の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstDefParam.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstNameBranch.h"
#include "parser/PtDefParam.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstDefParam
//////////////////////////////////////////////////////////////////////

// @brief 値の取得
AstExpr
AstDefParam::expr() const
{
  _check_ptr();
  return AstExpr(mPtr->expr());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstDefParam::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstDefParam::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstDefParam::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstDefParam::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
AstDefParam::namebranch_list() const
{
  _check_ptr();
  return AstNameBranchList(AstNameBranch(mPtr->namebranch_top()));
}

// @brief 次の要素を返す．
AstDefParam
AstDefParam::next() const
{
  _check_ptr();
  return AstDefParam(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstDefParam::json_sub(
  JsonValue& jobj
) const
{
  AstHierNamedBase::json_sub(jobj);
  jobj.add("expr", expr().json_obj());
}

END_NAMESPACE_YM_VERILOG
