
/// @file AstPathDecl.cc
/// @brief AstPathDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstPathDecl.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstPathDelay.h"
#include "parser/PtPathDecl.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstPathDecl
//////////////////////////////////////////////////////////////////////

// @brief edge_descriptor の取得
int
AstPathDecl::edge() const
{
  _check_ptr();
  return mPtr->edge();
}

// @brief 入力のリストの取得
AstExprList
AstPathDecl::input_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->input_top()));
}

// @brief 入力の極性の取得
int
AstPathDecl::input_pol() const
{
  _check_ptr();
  return mPtr->input_pol();
}

// @brief パス記述子(?)の取得
VpiPathType
AstPathDecl::op() const
{
  _check_ptr();
  return mPtr->op();
}

// @brief 出力リストの取得
AstExprList
AstPathDecl::output_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->output_top()));
}

// @brief 出力の極性の取得
int
AstPathDecl::output_pol() const
{
  _check_ptr();
  return mPtr->output_pol();
}

// @brief 式の取得
AstExpr
AstPathDecl::expr() const
{
  _check_ptr();
  return AstExpr(mPtr->expr());
}

// @brief path_delay_value の取得
AstPathDelay
AstPathDecl::path_delay() const
{
  _check_ptr();
  return AstPathDelay(mPtr->path_delay());
}

// @brief 適切な値を持っている時 true を返す．
bool
AstPathDecl::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstPathDecl::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstPathDecl::key() const
{
  return _key();
}

// @brief json_obj() の下請け関数
void
AstPathDecl::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  jobj.add("edge", JsonValue(edge()));
  jobj.add("input_list", input_list().json_obj());
  jobj.add("input_pol", JsonValue(input_pol()));
  const char* type_str = nullptr;
  switch ( op() ) {
  case VpiPathType::Full:     type_str = "Full"; break;
  case VpiPathType::Parallel: type_str = "Parallel"; break;
  }
  jobj.add("op", JsonValue(type_str));
  jobj.add("output_list", output_list().json_obj());
  jobj.add("output_pol", JsonValue(output_pol()));
  JsonUtils::add(jobj, "expr", expr());
  JsonUtils::add(jobj, "path_delay", path_delay());
}

END_NAMESPACE_YM_VERILOG
