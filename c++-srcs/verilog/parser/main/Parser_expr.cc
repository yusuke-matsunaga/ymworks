/// @file Parser_expr.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "parser/Parser.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 単項演算子の生成
PtExpr*
Parser::new_Opr(
  const FileRegion& fr,
  VpiOpType type,
  const AstExpr* opr,
  const AstAttrInstList* ai_list
)
{
  auto expr = mFactory.new_Opr(fr, type, opr);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief 二項演算子の生成
PtExpr*
Parser::new_Opr(
  const FileRegion& fr,
  VpiOpType type,
  const AstExpr* opr1,
  const AstExpr* opr2,
  const AstAttrInstList* ai_list
)
{
  auto expr = mFactory.new_Opr(fr, type, opr1, opr2);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief 三項演算子の生成
PtExpr*
Parser::new_Opr(
  const FileRegion& fr,
  VpiOpType type,
  const AstExpr* opr1,
  const AstExpr* opr2,
  const AstExpr* opr3,
  const AstAttrInstList* ai_list
)
{
  auto expr = mFactory.new_Opr(fr, type, opr1, opr2, opr3);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief concatination 演算子の生成
PtExpr*
Parser::new_Concat(
  const FileRegion& fr,
  const AstExprList* expr_list
)
{
  return mFactory.new_Concat(fr, expr_list);
}

// @brief multi-concatination 演算子の生成
PtExpr*
Parser::new_MultiConcat(
  const FileRegion& fr,
  const AstExpr* rep,
  const AstExprList* expr_list
)
{
  return mFactory.new_MultiConcat(fr, rep, expr_list);
}

// @brief min/typ/max delay 演算子の生成
PtExpr*
Parser::new_MinTypMax(
  const FileRegion& fr,
  const AstExpr* val0,
  const AstExpr* val1,
  const AstExpr* val2
)
{
  return mFactory.new_MinTypMax(fr, val0, val1, val2);
}

// @brief primary の生成
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  const char* name
)
{
  return mFactory.new_Primary(fr, name);
}

// @brief インデックス付き primary の生成
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  const char* name,
  const AstExprList* index_array
)
{
  return mFactory.new_Primary(fr, name, index_array);
}

// @brief 範囲指定付き primary の生成
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  const char* name,
  const AstPart* part
)
{
  return mFactory.new_Primary(fr, name, part);
}

// @brief インデックスと範囲指定付き primary の生成
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  const char* name,
  const AstExprList* index_list,
  const AstPart* part
)
{
  return mFactory.new_Primary(fr, name, index_list, part);
}

// @brief primary の生成 (階層付き)
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  PtHierName* hname
)
{
  return mFactory.new_Primary(fr, hname);
}

// @brief インデックス付き primary の生成 (階層付き)
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  PtHierName* hname,
  const AstExprList* index_list
)
{
  return mFactory.new_Primary(fr, hname, index_list);
}

// @brief 範囲指定付き primary の生成 (階層付き)
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  PtHierName* hname,
  const AstPart* part
)
{
  return mFactory.new_Primary(fr, hname, part);
}

// @brief インデックスと範囲指定付き primary の生成 (階層付き)
PtExpr*
Parser::new_Primary(
  const FileRegion& fr,
  PtHierName* hname,
  const AstExprList* index_list,
  const AstPart* part
)
{
  return mFactory.new_Primary(fr, hname, index_list, part);
}

// @brief constant primary の生成
PtExpr*
Parser::new_CPrimary(
  const FileRegion& fr,
  const char* name,
  const AstExpr* index
)
{
  auto index_list = new_ExprList({index});
  return mFactory.new_CPrimary(fr, name, index_list);
}

// @brief constant primary の生成
PtExpr*
Parser::new_CPrimary(
  const FileRegion& fr,
  const char* name,
  const AstExprList* index_list
)
{
  return mFactory.new_CPrimary(fr, name, index_list);
}

// @brief 範囲指定付き constant primary の生成
PtExpr*
Parser::new_CPrimary(
  const FileRegion& fr,
  const char* name,
  const AstPart* part
)
{
  return mFactory.new_CPrimary(fr, name, part);
}

// @brief インデックス付き constant primary の生成 (階層付き)
PtExpr*
Parser::new_CPrimary(
  const FileRegion& fr,
  PtHierName* hname,
  const AstExprList* index_list
)
{
  return mFactory.new_CPrimary(fr, hname, index_list);
}

// @brief function call の生成
PtExpr*
Parser::new_FuncCall(
  const FileRegion& fr,
  const char* name,
  const AstExprList* arg_list,
  const AstAttrInstList* ai_list
)
{
  auto expr = mFactory.new_FuncCall(fr, name, arg_list);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief function call の生成 (階層付き)
PtExpr*
Parser::new_FuncCall(
  const FileRegion& fr,
  PtHierName* hname,
  const AstExprList* arg_list,
  const AstAttrInstList* ai_list
)
{
  auto expr = mFactory.new_FuncCall(fr, hname, arg_list);
  reg_attrinst(expr, ai_list);
  return expr;
}

// @brief system function call の生成
PtExpr*
Parser::new_SysFuncCall(
  const FileRegion& fr,
  const char* name,
  const AstExprList* arg_list
)
{
  return mFactory.new_SysFuncCall(fr, name, arg_list);
}

// @brief 整数型の定数の生成
PtExpr*
Parser::new_IntConst(
  const FileRegion& fr,
  SizeType value
)
{
  return mFactory.new_IntConst(fr, value);
}

// @brief 整数型の定数の生成
PtExpr*
Parser::new_IntConst(
  const FileRegion& fr,
  const char* value
)
{
  return mFactory.new_IntConst(fr, value);
}

// @brief 基底付き整数型の定数の生成
PtExpr*
Parser::new_IntConst(
  const FileRegion& fr,
  VpiConstType const_type,
  const char* value
)
{
  return mFactory.new_IntConst(fr, const_type, value);
}

// @brief サイズと基底付き定数の生成
PtExpr*
Parser::new_IntConst(
  const FileRegion& fr,
  SizeType size,
  VpiConstType const_type,
  const char* value
)
{
  return mFactory.new_IntConst(fr, size, const_type, value);
}

// @brief 実数型の定数の生成
PtExpr*
Parser::new_RealConst(
  const FileRegion& fr,
  double value
)
{
  return mFactory.new_RealConst(fr, value);
}

// @brief 文字列型の定数の生成
PtExpr*
Parser::new_StringConst(
  const FileRegion& fr,
  const char* value
)
{
  return mFactory.new_StringConst(fr, value);
}

// @brief 範囲指定の生成
PtPart*
Parser::new_Part(
  const FileRegion& fr,
  VpiRangeMode mode,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  return mFactory.new_Part(fr, mode, expr1, expr2);
}

END_NAMESPACE_YM_VERILOG
