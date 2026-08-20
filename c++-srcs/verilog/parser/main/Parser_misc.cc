/// @file Parser_misc.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief ディレイコントロールの生成
PtControl*
Parser::new_DelayControl(
  const FileRegion& fr,
  const AstExpr* value
)
{
  return mFactory.new_DelayControl(fr, value);
}

// @brief イベントコントロールの生成
PtControl*
Parser::new_EventControl(
  const FileRegion& fr
)
{
  return mFactory.new_EventControl(fr);
}

// @brief イベントコントロールの生成
PtControl*
Parser::new_EventControl(
  const FileRegion& fr,
  const char* event_name,
  const FileRegion& name_loc
)
{
  auto expr = new_Primary(name_loc, event_name);
  return mFactory.new_EventControl(fr, expr);
}

// @brief イベントコントロールの生成
PtControl*
Parser::new_EventControl(
  const FileRegion& fr,
  PtHierName* event_name,
  const FileRegion& name_loc
)
{
  auto expr = new_Primary(name_loc, event_name);
  return mFactory.new_EventControl(fr, expr);
}

// @brief イベントコントロールの生成
PtControl*
Parser::new_EventControl(
  const FileRegion& fr,
  PtExprList* event_list
)
{
  return mFactory.new_EventControl(fr, event_list);
}

// @brief リピートコントロールの生成
PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const AstExpr* rep
)
{
  return mFactory.new_RepeatControl(fr, rep);
}

// @brief リピートコントロールの生成
PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const AstExpr* rep,
  const char* event_name,
  const FileRegion& name_loc
)
{
  auto event = new_Primary(name_loc, event_name);
  return mFactory.new_RepeatControl(fr, rep, event);
}

// @brief リピートコントロールの生成
PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const AstExpr* rep,
  PtHierName* event_name,
  const FileRegion& name_loc
)
{
  auto event = new_Primary(name_loc, event_name);
  return mFactory.new_RepeatControl(fr, rep, event);
}

// @brief リピートコントロールの生成
PtControl*
Parser::new_RepeatControl(
  const FileRegion& fr,
  const AstExpr* rep,
  PtExprList* event_list
)
{
  return mFactory.new_RepeatControl(fr, rep, event_list);
}

// @brief 順序つき結合子の生成
PtConnection*
Parser::new_OrderedCon(
  const AstExpr* expr
)
{
  return mFactory.new_OrderedCon(expr);
}

// @brief 順序つき結合子の生成
PtConnection*
Parser::new_OrderedCon(
  const FileRegion& fr,
  const AstExpr* expr,
  PtAttrInstList* ai_list
)
{
  auto con = mFactory.new_OrderedCon(fr, expr);
  reg_attrinst(con, ai_list);
  return con;
}

// @brief 名前付き結合子の生成
PtConnection*
Parser::new_NamedCon(
  const FileRegion& fr,
  const char* name,
  const AstExpr* expr,
  PtAttrInstList* ai_list
)
{
  auto con = mFactory.new_NamedCon(fr, name, expr);
  reg_attrinst(con, ai_list);
  return con;
}

// @brief strength の生成
PtStrength*
Parser::new_Strength(
  const FileRegion& fr,
  VpiStrength value0,
  VpiStrength value1
)
{
  return mFactory.new_Strength(fr, value0, value1);
}

// @brief charge strength の生成
PtStrength*
Parser::new_Strength(
  const FileRegion& fr,
  VpiStrength value
)
{
  return mFactory.new_Strength(fr, value);
}

// @brief 遅延値の生成 (1つの値)
PtDelay*
Parser::new_Delay(
  const FileRegion& fr,
  const AstExpr* value1
)
{
  return mFactory.new_Delay(fr, value1);
}

// @brief 遅延値の生成 (2つの値)
PtDelay*
Parser::new_Delay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  return mFactory.new_Delay(fr, value1, value2);
}

// @brief 遅延値の生成 (3つの値)
PtDelay*
Parser::new_Delay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)
{
  return mFactory.new_Delay(fr, value1, value2, value3);
}

// @brief attribute instance の生成
PtAttrInst*
Parser::new_AttrInst(
  const FileRegion& fr,
  PtAttrSpecList* as_list
)
{
  return mFactory.new_AttrInst(fr, as_list);
}

// @brief attribute spec の生成
PtAttrSpec*
Parser::new_AttrSpec(
  const FileRegion& fr,
  const char* name,
  const AstExpr* expr
)
{
  return mFactory.new_AttrSpec(fr, name, expr);
}

END_NAMESPACE_YM_VERILOG
