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

// @brief defparam 文の要素の生成 (階層つき識別子)
void
Parser::new_DefParam(
  const FileRegion& fr,
  PtHierName* hname,
  const AstExpr* value
)


//////////////////////////////////////////////////////////////////////
// continuous assign の生成
//////////////////////////////////////////////////////////////////////

// @brief continuous assign 文のヘッダの生成
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr
)

// @brief continuous assign 文のヘッダの生成 (strengthつき)
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr,
  const AstStrength* strength
)

// @brief continuous assign 文のヘッダの生成 (遅延付き)
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr,
  const AstDelay* delay
)

// @brief continuous assign 文のヘッダの生成 (strength, 遅延付き)
PtItem*
Parser::new_ContAssignH(
  const FileRegion& fr,
  const AstStrength* strength,
  const AstDelay* delay
)

// @brief continuous assign 文の生成
void
Parser::new_ContAssign(
  const FileRegion& fr,
  const AstExpr* lhs,
  const AstExpr* rhs
)


//////////////////////////////////////////////////////////////////////
// process の生成
//////////////////////////////////////////////////////////////////////

// @brief initial 文の生成
PtItem*
Parser::new_Initial(
  const FileRegion& fr,
  const AstStmt* body
)

// @brief always 文の生成
PtItem*
Parser::new_Always(
  const FileRegion& fr,
  const AstStmt* body
)


//////////////////////////////////////////////////////////////////////
// task/function の生成
//////////////////////////////////////////////////////////////////////

// @brief task/function 定義の開始
void
Parser::init_tf()

// @brief task/function 定義の終了
void
Parser::end_tf()

// @brief task 文の生成
PtItem*
Parser::new_Task(
  const FileRegion& fr,
  const char* name,
  bool automatic,
  const AstStmt* stmt
)

// @brief 1ビット型 function 文の生成
PtItem*
Parser::new_Function(
  const FileRegion& fr,
  const char* name,
  bool automatic,
  bool sign,
  const AstStmt* stmt
)

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

// @brief path 仕様を生成する．
void
Parser::new_SpecPath(
  const FileRegion& fr,
  VpiSpecPathType id,
  const AstExpr* expr,
  const AstPathDecl* path_decl
)

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

// @brief path delay value の生成 (値が1個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value
)

// @brief path delay value の生成 (値が2個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2
)

// @brief path delay value の生成 (値が3個)
PtPathDelay*
Parser::new_PathDelay(
  const FileRegion& fr,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)

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

END_NAMESPACE_YM_VERILOG
