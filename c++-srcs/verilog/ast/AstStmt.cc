
/// @file AstStmt.cc
/// @brief AstStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstStmt
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstStmt::json_obj() const
{
  auto jobj = AstHierNamedBase::json_obj();
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstCaseItem::json_obj() const
{
  auto jobj = AstBase::json_obj();
  return jobj;
}

END_NAMESPACE_YM_VERILOG
