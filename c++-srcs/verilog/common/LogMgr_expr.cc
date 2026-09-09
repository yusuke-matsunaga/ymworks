
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"

#include "elaborator/ElbError.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief int 型が要求されている所で互換性のない型があった．
void
LogMgr::error_int_required(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_INT_REQUIRED",
	      "Integer value is required");
}

// @brief int 型が要求されている所で互換性のない型があった．
void
LogMgr::error_int_required(
  const char* file,
  int line,
  const VlExpr* expr
)
{
  // この関数だけ VlExpr* なので expr_common() を使えない．
  std::ostringstream buf;
  buf << "\""
      << expr->decompile()
      << "\": "
      << "Integer value is required";
  throw ElbError(file, line,
		 expr->file_region(),
		 "ELAB_INT_REQUIRED",
		 buf.str());
}

// @brief ビットベクタ型が要求されている所で互換性のない型があった．
void
LogMgr::error_bv_required(
  const char* file,
  int line,
  const FileRegion& loc
)
{
  throw ElbError(file, line,
		 loc,
		 "ELAB_BV_REQUIRED",
		 "Bit vector value is required");
}

// @brief 通常の式中に edge descriptor
void
LogMgr::error_illegal_edge_descriptor(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ILLEGAL_EDGE_DESCRIPTOR",
		 "Edge descriptor in an expression");
}

// @brief real 型のオペランドをとれない
void
LogMgr::error_illegal_real_type(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ILLEGAL_REAL_TYPE",
	      "Shall not have a real-type operand");
}

// @brief 該当する関数が存在しない．
void
LogMgr::error_function_not_found(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_FUNCTION_NOT_FOUND",
	      " Not found");
}

// @brief 該当するシステム関数が存在しない．
void
LogMgr::error_sysfunc_not_found(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SYSFUNC_NOT_FOUND",
	      "Not found");
}

// @brief 関数ではない．
void
LogMgr::error_not_a_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_NOT_A_FUNCTION",
	      "Function expected");
}

// @brief 引数の数が合わない．
void
LogMgr::error_argument_num_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ARG_NUM_MISMATCH",
	      "# of argments mismatch");
}

// @brief 引数の型が合わない．
void
LogMgr::error_argument_type_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ARG_TYPE_MISMATCH",
	      "Argument type mismatch");
}

// @brief オブジェクトの型が不適切
void
LogMgr::error_illegal_object(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ILLEGAL_OBJECT",
	      "Illegal type");
}

// @brief オブジェクトが named-event でなかった
void
LogMgr::error_not_a_namedevent(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_NOT_A_NAMEDEVENT",
	      "Named event expected");
}

// @brief 要素の範囲の順番と範囲指定の順番が異なる．
void
LogMgr::error_range_order(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_RANGE_ORDER",
	      "Range order mismatch");
}

// @brief named-event に対する範囲指定
void
LogMgr::error_select_for_namedevent(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SELECT_FOR_NAMEDEVENT",
	      "Named event cannot have a part/bit selection");
}

// @brief assign/deassign に不適切なビット/範囲指定
void
LogMgr::error_select_in_pca(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SELECT_FOR_PCA",
	      "Bit/part-select shall not be used"
	      " in LHS of assign/deassign statement.");
}

// @brief force/release に不適切なビット/範囲指定
void
LogMgr::error_select_in_force(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SELECT_FOR_FORCE",
	      "Bit/part-select shall not be used"
	      " in LHS of force/release statement.");
}

// @brief assign/deassign に不適切な配列要素
void
LogMgr::error_array_in_pca(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ARRAY_IN_PCA",
	      "Array element shall not be used"
	      " in LHS of assign/deassign statement.");
}

// @brief force/release に不適切な配列要素
void
LogMgr::error_array_in_force(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ARRAY_IN_FORCE",
	      "Array element shall not be used"
	      " in LHS of force/release statement.");
}

// @brief 配列の次元が合わない
void
LogMgr::error_dimension_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_DIM_MISMATCH",
	      "Dimension mismatch");
}

// @brief real 型に対するビット選択あるいは部分選択があった
void
LogMgr::error_select_for_real(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SELECT_FOR_REAL",
	      "Real type cannot have a part/bit selection");
}

// @brief constant function 中にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SYSFUNC_IN_CF",
	      "Sysfunc cannot be used in constant function");
}

// @brief constant expression 中にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_ce(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SYSFUNC_IN_CE",
	      "Sysfunc cannot be used in constant expression");
}

// @brief 定数関数は自己再帰できない．
void
LogMgr::error_uses_itself(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_USES_ITSELF",
	      "Uses itself");
}

// @brief 定数関数ではない．
void
LogMgr::error_not_a_constant_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_NOT_A_CF",
	      "Not a constant function");
}

// @brief オブジェクトの型が constant function 用として不適切
void
LogMgr::error_illegal_object_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ILLEGAL_TYPE_IN_CF",
	      "Illegal object type inside constant function");
}

// @brief 階層名が constant expression 中にあった
void
LogMgr::error_hname_in_ce(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_HNAME_IN_CE",
	      "Hierarchical name shall not be used"
	      " inside constant expression");
}

// @brief 階層名が constant function 中にあった
void
LogMgr::error_hname_in_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_HNAME_IN_CF",
	      "Hierarchical name shall not be used"
	      " inside constant function");
}

// @brief オブジェクトが parameter でなかった
void
LogMgr::error_not_a_parameter(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_NOT_A_PARAMETER",
	      "Parameter type expected");
}

// @brief イベント式の根元に定数
void
LogMgr::error_illegal_constant_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_CONST_IN_EVENT",
	      "Constant shall not be used in event description");
}

// @brief イベント式の根元に関数呼び出し
void
LogMgr::error_illegal_funccall_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_FUNCCALL_IN_EVENT",
	      "Function call shall not be used in event description");
}

// @brief イベント式の根元にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SYSFUNCCALL_IN_EVENT",
	      "Sysfunc call shall not be used in event description");
}

// @brief 左辺式で用いることのできない演算子
void
LogMgr::error_illegal_operator_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_ILLEGAL_OPERATOR_IN_LHS",
	      "Illegal operator in LHS");
}

// @brief 左辺式に定数
void
LogMgr::error_illegal_constant_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_CONST_IN_LHS",
	      "Constant shall not be in LHS");
}

// @brief 左辺式に関数呼び出し
void
LogMgr::error_illegal_funccall_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_FUNCCALL_IN_LHS",
	      "Function call shall not be used in LHS");
}

// @brief 左辺式にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  expr_common(file, line, ast_expr,
	      "ELAB_SYSFUNCCALL_IN_LHS",
	      "Sysfunc call shall not be used in LHS");
}

// @brief AstExpr 系のエラーの下請け関数
void
LogMgr::expr_common(
  const char* file,
  int line,
  const AstExpr& ast_expr,
  const char* label,
  const std::string& msg
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": "
      << msg;
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 label,
		 buf.str());
}

END_NAMESPACE_YM_VERILOG
