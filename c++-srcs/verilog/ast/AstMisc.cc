
/// @file AstMisc.cc
/// @brief AstMisc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstMisc.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstControl
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstControl::json_obj() const
{
  auto jobj = AstBase::json_obj();
  switch ( type() ) {
  case Delay:
    jobj.add("type", JsonValue("Delay"));
    jobj.add("delay", delay()->json_obj());
    break;

  case Event:
    jobj.add("type", JsonValue("Event"));
    jobj.add("event_list", event_list().json_obj());
    break;

  case Repeat:
    jobj.add("type", JsonValue("Repeat"));
    jobj.add("rep_expr", rep_expr()->json_obj());
    jobj.add("event_list", event_list().json_obj());
    break;
  }
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstConnection
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstConnection::json_obj() const
{
  auto jobj = AstBase::json_obj();
  if ( name() != nullptr ) {
    jobj.add("name", JsonValue(name()));
  }
  if ( expr() != nullptr ) {
    jobj.add("expr", expr()->json_obj());
  }
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstStrength
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

inline
void
add_strength(
  JsonValue& jobj,
  const char* label,
  VpiStrength str
)
{
  if ( str != VpiStrength::NoStrength ) {
    std::ostringstream buf;
    buf << str;
    jobj.add(label, JsonValue(buf.str()));
  }
}

END_NONAMESPACE

// @brief 内容を JsonValue に変換する．
JsonValue
AstStrength::json_obj() const
{
  auto jobj = AstBase::json_obj();
  add_strength(jobj, "drive0", drive0());
  add_strength(jobj, "drive1", drive1());
  add_strength(jobj, "charge", charge());
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstDelay
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

inline
void
add_delay(
  JsonValue& jobj,
  const char* label,
  const AstExpr* expr
)
{
  if ( expr != nullptr ) {
    jobj.add(label, expr->json_obj());
  }
}

END_NONAMESPACE

// @brief 内容を JsonValue に変換する．
JsonValue
AstDelay::json_obj() const
{
  auto jobj = AstBase::json_obj();
  add_delay(jobj, "value0", value0());
  add_delay(jobj, "value1", value1());
  add_delay(jobj, "value2", value2());
  return jobj;
}


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


//////////////////////////////////////////////////////////////////////
// クラス AstAttrInst
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstAttrInst::json_obj() const
{
  auto jobj = AstBase::json_obj();
  jobj.add("attrspec_list", attrspec_list().json_obj());
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstAttrSpec
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstAttrSpec::json_obj() const
{
  auto jobj = AstNamedBase::json_obj();
  if ( expr() != nullptr ) {
    jobj.add("expr", expr()->json_obj());
  }
  return jobj;
}

END_NAMESPACE_YM_VERILOG
