/// @file Parser_inst.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtFactory.h"
#include "parser/PtItem.h"
#include "parser/PtExpr.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// gate/module/UDP instance の生成
//////////////////////////////////////////////////////////////////////

// @brief gate instance 文のヘッダの生成
PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type
)
{
  return mFactory.new_GateH(fr, type, nullptr, nullptr,
			    PtInstArray(mAlloc, mInstList));
}

// @brief gate instance 文のヘッダの生成 (strength付き)
PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type,
  const AstStrength* strength
)
{
  return mFactory.new_GateH(fr, type, strength, nullptr,
			    PtInstArray(mAlloc, mInstList));
}

// @brief gate instance 文のヘッダの生成 (遅延付き)
PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type,
  const AstDelay* delay
)
{
  return mFactory.new_GateH(fr, type, nullptr, delay,
			    PtInstArray(mAlloc, mInstList));
}

// @brief gate instance 文のヘッダの生成 (strength, 遅延付き)
PtItem*
Parser::new_GateH(
  const FileRegion& fr,
  VpiPrimType type,
  const AstStrength* strength,
  const AstDelay* delay
)
{
  return mFactory.new_GateH(fr, type, strength, delay,
			    PtInstArray(mAlloc, mInstList));
}

// @brief module instance/UDP instance 文のヘッダの生成
PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name
)
{
  auto item = mFactory.new_MuH(fr, def_name, nullptr, nullptr,
			       PtInstArray(mAlloc, mInstList));
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength付き)
PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  const AstStrength* strength
)
{
  auto item = mFactory.new_MuH(fr, def_name, strength, nullptr,
			       PtInstArray(mAlloc, mInstList));
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成 (遅延付き)
PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  const AstDelay* delay
)
{
  auto item = mFactory.new_MuH(fr, def_name, nullptr, delay,
			       PtInstArray(mAlloc, mInstList));
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成 (strength, 遅延付き)
PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  const AstStrength* strength,
  const AstDelay* delay
)
{
  auto item = mFactory.new_MuH(fr, def_name, strength, delay,
			       PtInstArray(mAlloc, mInstList));
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP instance 文のヘッダの生成
PtItem*
Parser::new_MuH(
  const FileRegion& fr,
  const char* def_name,
  PtConnectionList* con_list
)
{
  auto item = mFactory.new_MuH(fr, def_name,
			       con_list->to_array(mAlloc),
			       PtInstArray(mAlloc, mInstList));
  reg_defname(def_name);
  return item;
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  PtConnectionList* con_list
)
{
  auto item = mFactory.new_Inst(fr, nullptr, nullptr,
				con_list->to_array(mAlloc));
  mInstList.push_back(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const AstExpr* expr1
)
{
  auto convec = mFactory.new_OrderedConArray(expr1);
  auto item = mFactory.new_Inst(fr, nullptr, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2);
  auto item = mFactory.new_Inst(fr, nullptr, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2, expr3);
  auto item = mFactory.new_Inst(fr, nullptr, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief module instance/UDP/gate instance の要素の生成
void
Parser::new_Inst(
  const FileRegion& fr,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3,
  const AstExpr* expr4
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2, expr3, expr4);
  auto item = mFactory.new_Inst(fr, nullptr, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  PtConnectionList* con_list
)
{
  auto item = mFactory.new_Inst(fr, name, nullptr,
				con_list->to_array(mAlloc));
  mInstList.push_back(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const AstExpr* expr1
)
{
  auto convec = mFactory.new_OrderedConArray(expr1);
  auto item = mFactory.new_Inst(fr, name, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2);
  auto item = mFactory.new_Inst(fr, name, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2, expr3);
  auto item = mFactory.new_Inst(fr, name, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstN(
  const FileRegion& fr,
  const char* name,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3,
  const AstExpr* expr4
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2, expr3, expr4);
  auto item = mFactory.new_Inst(fr, name, nullptr,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const AstRange* range,
  PtConnectionList* con_list
)
{
  auto item = mFactory.new_Inst(fr, name, range,
				con_list->to_array(mAlloc));
  mInstList.push_back(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1
)
{
  auto convec = mFactory.new_OrderedConArray(expr1);
  auto item = mFactory.new_Inst(fr, name, range,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2);
  auto item = mFactory.new_Inst(fr, name, range,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2, expr3);
  auto item = mFactory.new_Inst(fr, name, range,
				std::move(convec));
  mInstList.push_back(item);
}

// @brief 名前と範囲付き module instance/UDP/gate instance の要素の生成
void
Parser::new_InstV(
  const FileRegion& fr,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3,
  const AstExpr* expr4
)
{
  auto convec = mFactory.new_OrderedConArray(expr1, expr2, expr3, expr4);
  auto item = mFactory.new_Inst(fr, name, range,
				std::move(convec));
  mInstList.push_back(item);
}

END_NAMESPACE_YM_VERILOG
