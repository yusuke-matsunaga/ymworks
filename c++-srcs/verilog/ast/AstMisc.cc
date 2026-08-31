
/// @file AstMisc.cc
/// @brief AstMisc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstNameBranch
//////////////////////////////////////////////////////////////////////

// @brief インデックスを含めた名前を返す．
std::string
AstNameBranch::decompile() const
{
  std::ostringstream buf;
  buf << name();
  if ( has_index() ) {
    buf << "[" << index() << "]";
  }
  return buf.str();
}

// @brief 内容を JsonValue に変換する．
JsonValue
AstNameBranch::json_obj() const
{
  auto jobj = JsonValue::object();
  jobj.add("name", JsonValue(name()));
  if ( has_index() ) {
    jobj.add("index", JsonValue(index()));
  }
  return jobj;
}

END_NAMESPACE_YM_VERILOG
