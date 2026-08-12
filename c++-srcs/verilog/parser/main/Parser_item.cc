/// @file Parser_item.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtFactory.h"
#include "parser/PtItem.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// defparam の生成
//////////////////////////////////////////////////////////////////////

// @brief defparam 文のヘッダの生成
PtItem*
Parser::new_DefParamH(
  const FileRegion& fr
)
{
  return mFactory.new_DefParamH(fr,
				PtDefParamArray(mAlloc, mDefParamList));
}

// @brief defparam 文の要素の生成
void
Parser::new_DefParam(
  const FileRegion& fr,
  const char* name,
  const AstExpr* value
)
{
  auto defparam = mFactory.new_DefParam(fr, name, value);
  mDefParamList.push_back(defparam);
}

// @brief defparam 文の要素の生成 (階層つき識別子)
void
Parser::new_DefParam(
  const FileRegion& fr,
  PtHierName* hname,
  const AstExpr* value
)
{
  auto defparam = mFactory.new_DefParam(fr, hname, value);
  mDefParamList.push_back(defparam);
}


//////////////////////////////////////////////////////////////////////
// continuous assign の生成
//////////////////////////////////////////////////////////////////////

// @brief continuous assign 文のヘッダの生成
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr
)
{
  return mFactory.new_ContAssignH(fr, nullptr, nullptr,
				  PtContAssignArray(mAlloc, mContAssignList));
}

// @brief continuous assign 文のヘッダの生成 (strengthつき)
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr,
  const AstStrength* strength
)
{
  return mFactory.new_ContAssignH(fr, strength, nullptr,
				  PtContAssignArray(mAlloc, mContAssignList));
}

// @brief continuous assign 文のヘッダの生成 (遅延付き)
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr,
  const AstDelay* delay
)
{
  return mFactory.new_ContAssignH(fr, nullptr, delay,
				  PtContAssignArray(mAlloc, mContAssignList));
}

// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr,
  const AstStrength* strength,
  const AstDelay* delay
)
{
  return mFactory.new_ContAssignH(fr, strength, delay,
				  PtContAssignArray(mAlloc, mContAssignList));
}

// @brief continuous assign 文の生成
void
Parser::new_ContAssign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  auto ca = mFactory.new_ContAssign(fr, lhs, rhs);
  mContAssignList.push_back(ca);
}


//////////////////////////////////////////////////////////////////////
// process の生成
//////////////////////////////////////////////////////////////////////

// @brief initial 文の生成
PtItem*
Parser::new_Initial(
  const FileRegion& fr,
  const AstStmt* body
)
{
  return mFactory.new_Initial(fr, body);
}

// @brief always 文の生成
PtItem*
Parser::new_Always(
  const FileRegion& fr,
  const AstStmt* body
)
{
  return mFactory.new_Always(fr, body);
}


//////////////////////////////////////////////////////////////////////
// task/function の生成
//////////////////////////////////////////////////////////////////////

// @brief task/function 定義の開始
void
Parser::init_tf()
{
  mCurIOHeadList = &mTfIOHeadList;
  push_declhead_list();

  mCurIOHeadList->clear();
  mIOItemList.clear();
  cur_declhead_list().clear();
  mDeclItemList.clear();
}

// @brief task/function 定義の終了
void
Parser::end_tf()
{
  mCurIOHeadList = &mModuleIOHeadList;
  mCurDeclArray = pop_declhead_list();
}

// @brief task 文の生成
PtItem*
Parser::new_Task(
  const FileRegion& fr,
  const char* name,
  bool automatic,
  const AstStmt* stmt
)
{
  return mFactory.new_Task(fr, name, automatic,
			   PtIOHeadArray(mAlloc, mTfIOHeadList, true),
			   PtDeclHeadArray(mAlloc, mCurDeclArray, true),
			   stmt);
}

// @brief 1ビット型 function 文の生成
PtItem*
Parser::new_Function(
  const FileRegion& fr,
  const char* name,
  bool automatic,
  bool sign,
  const AstStmt* stmt
)
{
  return mFactory.new_Function(fr, name, automatic,
			       sign,
			       PtIOHeadArray(mAlloc, mTfIOHeadList, true),
			       PtDeclHeadArray(mAlloc, mCurDeclArray, true),
			       stmt);
}

// @brief 範囲指定型 function 文の生成
PtItem*
Parser::new_SizedFunc(
  const FileRegion& fr,
  const char* name,
  bool automatic,
  bool sign,
  const AstRange* range,
  const AstStmt* stmt
)
{
  return mFactory.new_SizedFunc(fr, name, automatic,
				sign, range,
				PtIOHeadArray(mAlloc, mTfIOHeadList, true),
				PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				stmt);
}

// @brief 組み込み型 function 文の生成
PtItem*
Parser::new_TypedFunc(
  const FileRegion& fr,
  const char* name,
  bool automatic,
  bool sign,
  VpiVarType func_type,
  const AstStmt* stmt
)
{
  return mFactory.new_TypedFunc(fr, name, automatic,
				sign, func_type,
				PtIOHeadArray(mAlloc, mTfIOHeadList, true),
				PtDeclHeadArray(mAlloc, mCurDeclArray, true),
				stmt);
}


//////////////////////////////////////////////////////////////////////
// spec item の生成
//////////////////////////////////////////////////////////////////////

// @brief specify block item の生成
void
Parser::new_SpecItem(
  const FileRegion& fr,
  VpiSpecItemType id,
  PtExprList* terminal_list
)
{
  auto item = mFactory.new_SpecItem(fr, id,
				    terminal_list->to_array(mAlloc));
  add_item(item);
}

// @brief path 仕様を生成する．
void
Parser::new_SpecPath(
  const FileRegion& fr,
  VpiSpecPathType id,
  const AstExpr* expr,
  const AstPathDecl* path_decl
)
{
  auto item = mFactory.new_SpecPath(fr, id, expr, path_decl);
  add_item(item);
}

// @brief パス記述の生成
PtPathDecl*
Parser::new_PathDecl(
  const FileRegion& fr,
  int edge,
  PtExprList* input_list,
  int input_pol,
  VpiPathType op,
  PtExprList* output_list,
  int output_pol,
  const AstExpr* expr,
  const AstPathDelay* path_delay
)
{
  return mFactory.new_PathDecl(fr, edge,
			       input_list->to_array(mAlloc), input_pol,
			       op,
			       output_list->to_array(mAlloc), output_pol,
			       expr, path_delay);
}

// @brief パス記述の生成
PtPathDecl*
Parser::new_PathDecl(
  const FileRegion& fr,
  int edge,
  PtExprList* input_list,
  int input_pol,
  VpiPathType op,
  const AstExpr* output,
  int output_pol,
  const AstExpr* expr,
  const AstPathDelay* path_delay
)
{
  return mFactory.new_PathDecl(fr, edge,
			       input_list->to_array(mAlloc), input_pol,
			       op,
			       PtExprArray(mAlloc, output), output_pol,
			       expr, path_delay);
}

// @brief path delay value の生成 (値が1個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value
)
{
  return mFactory.new_PathDelay(fr, value);
}

// @brief path delay value の生成 (値が2個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  return mFactory.new_PathDelay(fr, value1, value2);
}

// @brief path delay value の生成 (値が3個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)
{
  return mFactory.new_PathDelay(fr, value1, value2, value3);
}

// @brief path delay value の生成 (値が6個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3,
  const AstExpr* value4,
  const AstExpr* value5,
  const AstExpr* value6
)
{
  return mFactory.new_PathDelay(fr,
				value1, value2, value3,
				value4, value5, value6);
}

// @brief path delay value の生成 (値が12個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3,
  const AstExpr* value4,
  const AstExpr* value5,
  const AstExpr* value6,
  const AstExpr* value7,
  const AstExpr* value8,
  const AstExpr* value9,
  const AstExpr* value10,
  const AstExpr* value11,
  const AstExpr* value12
)
{
  return mFactory.new_PathDelay(fr,
				value1, value2, value3,
				value4, value5, value6,
				value7, value8, value9,
				value10, value11, value12);
}

END_NAMESPACE_YM_VERILOG
