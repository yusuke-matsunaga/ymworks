/// @file Parser_decl.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtDeclItem の生成
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素の生成
void
Parser::new_DeclItem(
  const FileRegion& fr,
  const char* name
)

// @brief 初期値付き宣言要素の生成
void
Parser::new_DeclItem(
  const FileRegion& fr,
  const char* name,
  const AstExpr* init_value
)

// @brief 配列型宣言要素の生成
void
Parser::new_DeclItem(
  const FileRegion& fr,
  const char* name,
  PtRangeList* range_list
)


//////////////////////////////////////////////////////////////////////
// PtRange の生成
//////////////////////////////////////////////////////////////////////

// @brief 範囲の生成
PtRange*
Parser::new_Range(
  const FileRegion& fr,
  const AstExpr* msb,
  const AstExpr* lsb
)

END_NAMESPACE_YM_VERILOG
