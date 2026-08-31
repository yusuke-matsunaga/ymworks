
/// @file AstBase.cc
/// @brief AstBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstBase.h"
#include "ym/vl/AstMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstBase
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す JSON オブジェクトを返す．
JsonValue
AstBase::json_obj() const
{
  auto jobj = JsonValue::object();
  jobj.add("file_region", file_region().json_obj());
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstNamedBase
//////////////////////////////////////////////////////////////////////

// @brief 内容を表す JSON オブジェクトを返す．
JsonValue
AstNamedBase::json_obj() const
{
  auto jobj = AstBase::json_obj();
  if ( name() != nullptr ) {
    jobj.add("name", JsonValue(name()));
  }
  return jobj;
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
    buf << period << nb->decompile();
    period = ".";
  }
  if ( name() != nullptr ) {
    buf << period << name();
  }
  return buf.str();
}

// @brief 内容を表す JSON オブジェクトを返す．
JsonValue
AstHierNamedBase::json_obj() const
{
  auto jobj = AstNamedBase::json_obj();
  if ( namebranch_list().size() > 0 ) {
    jobj.add("namebranch_list", namebranch_list().json_obj());
  }
  return jobj;
}

END_NAMESPACE_YM_VERILOG
