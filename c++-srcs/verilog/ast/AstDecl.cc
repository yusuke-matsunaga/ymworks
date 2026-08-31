
/// @file AstDecl.cc
/// @brief AstDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstDecl.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstIOHead
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstIOHead::json_obj() const
{
  auto jobj = AstBase::json_obj();
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
  switch ( net_type() ) {
  case VpiNetType::Wire:    jobj.add("net_type", JsonValue("Wire")); break;
  case VpiNetType::Wand:    jobj.add("net_type", JsonValue("Wand")); break;
  case VpiNetType::Wor:     jobj.add("net_type", JsonValue("Wor")); break;
  case VpiNetType::Tri:     jobj.add("net_type", JsonValue("Tri")); break;
  case VpiNetType::Tri0:    jobj.add("net_type", JsonValue("Tri0")); break;
  case VpiNetType::Tri1:    jobj.add("net_type", JsonValue("Tri1")); break;
  case VpiNetType::TriReg:  jobj.add("net_type", JsonValue("TriReg")); break;
  case VpiNetType::TriAnd:  jobj.add("net_type", JsonValue("TriAnd")); break;
  case VpiNetType::TriOr:   jobj.add("net_type", JsonValue("TriOr")); break;
  case VpiNetType::Supply1: jobj.add("net_type", JsonValue("Supply1")); break;
  case VpiNetType::Supply0: jobj.add("net_type", JsonValue("Supply0")); break;
  case VpiNetType::None:    break;
  }
  switch ( var_type() ) {
  case VpiVarType::None:     break;
  case VpiVarType::Integer:  jobj.add("var_type", JsonValue("Integer")); break;
  case VpiVarType::Real:     jobj.add("var_type", JsonValue("Real")); break;
  case VpiVarType::Time:     jobj.add("var_type", JsonValue("Time")); break;
  case VpiVarType::Realtime: jobj.add("var_type", JsonValue("Realtime")); break;
  }
  if ( range() != nullptr ) {
    jobj.add("is_signed", JsonValue(is_signed()));
    jobj.add("range", range()->json_obj());
  }
  jobj.add("item_list", item_list().json_obj());
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstIOItem
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstIOItem::json_obj() const
{
  auto jobj = AstNamedBase::json_obj();
  if ( init_value() != nullptr ) {
    jobj.add("init_value", init_value()->json_obj());
  }
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstDeclHead
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstDeclHead::json_obj() const
{
  auto jobj = AstBase::json_obj();
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
  if ( range() != nullptr ) {
    jobj.add("is_signed", JsonValue(is_signed()));
    jobj.add("range", range()->json_obj());
  }
  switch ( data_type() ) {
  case VpiVarType::None:     break;
  case VpiVarType::Integer:  jobj.add("data_type", JsonValue("Integer")); break;
  case VpiVarType::Real:     jobj.add("data_type", JsonValue("Real")); break;
  case VpiVarType::Time:     jobj.add("data_type", JsonValue("Time")); break;
  case VpiVarType::Realtime: jobj.add("data_type", JsonValue("Realtime")); break;
  }
  switch ( net_type() ) {
  case VpiNetType::Wire:    jobj.add("net_type", JsonValue("Wire")); break;
  case VpiNetType::Wand:    jobj.add("net_type", JsonValue("Wand")); break;
  case VpiNetType::Wor:     jobj.add("net_type", JsonValue("Wor")); break;
  case VpiNetType::Tri:     jobj.add("net_type", JsonValue("Tri")); break;
  case VpiNetType::Tri0:    jobj.add("net_type", JsonValue("Tri0")); break;
  case VpiNetType::Tri1:    jobj.add("net_type", JsonValue("Tri1")); break;
  case VpiNetType::TriReg:  jobj.add("net_type", JsonValue("TriReg")); break;
  case VpiNetType::TriAnd:  jobj.add("net_type", JsonValue("TriAnd")); break;
  case VpiNetType::TriOr:   jobj.add("net_type", JsonValue("TriOr")); break;
  case VpiNetType::Supply1: jobj.add("net_type", JsonValue("Supply1")); break;
  case VpiNetType::Supply0: jobj.add("net_type", JsonValue("Supply0")); break;
  case VpiNetType::None:    break;
  }
  switch ( vs_type() ) {
  case VpiVsType::None:     break;
  case VpiVsType::Vectored: jobj.add("vs_type", JsonValue("Vectored")); break;
  case VpiVsType::Scalared: jobj.add("vs_type", JsonValue("Scalared")); break;
  }
  if ( strength() != nullptr ) {
    jobj.add("strength", strength()->json_obj());
  }
  if ( delay() != nullptr ) {
    jobj.add("delay", delay()->json_obj());
  }
  jobj.add("item_list", item_list().json_obj());
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstDeclItem
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstDeclItem::json_obj() const
{
  auto jobj = AstNamedBase::json_obj();
  if ( range_list().size() > 0 ) {
    jobj.add("range_list", range_list().json_obj());
  }
  if ( init_value() != nullptr ) {
    jobj.add("init_value", init_value()->json_obj());
  }
  return jobj;
}


//////////////////////////////////////////////////////////////////////
// クラス AstRange
//////////////////////////////////////////////////////////////////////

// @brief 内容を JsonValue に変換する．
JsonValue
AstRange::json_obj() const
{
  auto jobj = AstBase::json_obj();
  jobj.add("left", left()->json_obj());
  jobj.add("right", right()->json_obj());
  return jobj;
}

END_NAMESPACE_YM_VERILOG
