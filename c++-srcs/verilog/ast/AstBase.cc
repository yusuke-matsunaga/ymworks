
/// @file AstBase.cc
/// @brief AstBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "ym/vl/AstNameBranch.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstBase
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す JSON オブジェクトを返す．
JsonValue
AstBase::json_obj() const
{
  if ( is_invalid() ) {
    return JsonValue::null();
  }
  auto jobj = JsonValue::object();
  json_sub(jobj);
  return jobj;
}

// @brief json_obj() の下請け関数
void
AstBase::json_sub(
  JsonValue& jobj
) const
{
  jobj.add("file_region", file_region().json_obj());
}


//////////////////////////////////////////////////////////////////////
// クラス AstNamedBase
//////////////////////////////////////////////////////////////////////

// @brief json_obj() の下請け関数
void
AstNamedBase::json_sub(
  JsonValue& jobj
) const
{
  AstBase::json_sub(jobj);
  JsonUtils::add(jobj, "name", name());
}


//////////////////////////////////////////////////////////////////////
// クラス AstHierNamedBase
//////////////////////////////////////////////////////////////////////

// @brief 階層名を展開する．
std::string
AstHierNamedBase::decompile_name() const
{
  std::ostringstream buf;
  const char* period = "";
  for ( auto nb: namebranch_list() ) {
    buf << period << nb.decompile();
    period = ".";
  }
  if ( name() != nullptr ) {
    buf << period << name();
  }
  return buf.str();
}


// @brief json_obj() の下請け関数
void
AstHierNamedBase::json_sub(
  JsonValue& jobj
) const
{
  AstNamedBase::json_sub(jobj);
  JsonUtils::add_list(jobj, "namebranch_list", namebranch_list());
}

END_NAMESPACE_YM_VERILOG
