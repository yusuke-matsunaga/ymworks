
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

BEGIN_NONAMESPACE

// @brief AstExpr 系のエラーの下請け関数
std::string
expr_common(
  const AstExpr& ast_expr,
  const std::string& msg
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": "
      << msg;
  return buf.str();
}

END_NONAMESPACE


// @brief int 型が要求されている所で互換性のない型があった．
void
LogMgr::error_int_required(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Integer value is required");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_INT_REQUIRED",
	msg);
}

// @brief int 型が要求されている所で互換性のない型があった．
void
LogMgr::error_int_required(
  const char* file,
  int line,
  const VlExpr* expr
)
{
  std::ostringstream buf;
  buf << "\""
      << expr->decompile()
      << "\": "
      << "Integer value is required";
  error(file, line,
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
  error(file, line,
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
  auto msg = expr_common(ast_expr, "Edge descriptor in an expression");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ILLEGAL_EDGE_DESCRIPTOR",
	msg);
}

// @brief real 型のオペランドをとれない
void
LogMgr::error_illegal_real_type(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Shall not have a real-type operand");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ILLEGAL_REAL_TYPE",
	msg);
}

// @brief 該当する関数が存在しない．
void
LogMgr::error_function_not_found(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Not found");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_FUNCTION_NOT_FOUND",
	msg);
}

// @brief 該当するシステム関数が存在しない．
void
LogMgr::error_sysfunc_not_found(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Not found");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SYSFUNC_NOT_FOUND",
	msg);
}

// @brief 関数ではない．
void
LogMgr::error_not_a_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Function expected");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_NOT_A_FUNCTION",
	msg);
}

// @brief 引数の数が合わない．
void
LogMgr::error_argument_num_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "# of argments mismatch");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ARG_NUM_MISMATCH",
	msg);
}

// @brief 引数の型が合わない．
void
LogMgr::error_argument_type_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Argument type mismatch");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ARG_TYPE_MISMATCH",
	msg);
}

// @brief オブジェクトの型が不適切
void
LogMgr::error_illegal_object(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Illegal type");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ILLEGAL_OBJECT",
	msg);
}

// @brief オブジェクトが named-event でなかった
void
LogMgr::error_not_a_namedevent(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Named event expected");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_NOT_A_NAMEDEVENT",
	msg);
}

// @brief 要素の範囲の順番と範囲指定の順番が異なる．
void
LogMgr::error_range_order(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Range order mismatch");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_RANGE_ORDER",
	msg);
}

// @brief named-event に対する範囲指定
void
LogMgr::error_select_for_namedevent(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Named event cannot have a part/bit selection");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SELECT_FOR_NAMEDEVENT",
	msg);
}

// @brief assign/deassign に不適切なビット/範囲指定
void
LogMgr::error_select_in_pca(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Bit/part-select shall not be used"
			 " in LHS of assign/deassign statement.");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SELECT_FOR_PCA",
	msg);
}

// @brief force/release に不適切なビット/範囲指定
void
LogMgr::error_select_in_force(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Bit/part-select shall not be used"
			 " in LHS of force/release statement.");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SELECT_FOR_FORCE",
	msg);
}

// @brief assign/deassign に不適切な配列要素
void
LogMgr::error_array_in_pca(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Array element shall not be used"
			 " in LHS of assign/deassign statement.");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ARRAY_IN_PCA",
	msg);
}

// @brief force/release に不適切な配列要素
void
LogMgr::error_array_in_force(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Array element shall not be used"
			 " in LHS of force/release statement.");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ARRAY_IN_FORCE",
	msg);
}

// @brief 配列の次元が合わない
void
LogMgr::error_dimension_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Dimension mismatch");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_DIM_MISMATCH",
	msg);
}

// @brief real 型に対するビット選択あるいは部分選択があった
void
LogMgr::error_select_for_real(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Real type cannot have a part/bit selection");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SELECT_FOR_REAL",
	msg);
}

// @brief constant function 中にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Sysfunc cannot be used in constant function");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SYSFUNC_IN_CF",
	msg);
}

// @brief constant expression 中にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_ce(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Sysfunc cannot be used in constant expression");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SYSFUNC_IN_CE",
	msg);
}

// @brief 定数関数は自己再帰できない．
void
LogMgr::error_uses_itself(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Uses itself");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_USES_ITSELF",
	msg);
}

// @brief 定数関数ではない．
void
LogMgr::error_not_a_constant_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Not a constant function");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_NOT_A_CF",
	msg);
}

// @brief オブジェクトの型が constant function 用として不適切
void
LogMgr::error_illegal_object_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Illegal object type inside constant function");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ILLEGAL_TYPE_IN_CF",
	msg);
}

// @brief 階層名が constant expression 中にあった
void
LogMgr::error_hname_in_ce(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Hierarchical name shall not be used"
			 " inside constant expression");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_HNAME_IN_CE",
	msg);
}

// @brief 階層名が constant function 中にあった
void
LogMgr::error_hname_in_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Hierarchical name shall not be used"
			 " inside constant function");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_HNAME_IN_CF",
	msg);
}

// @brief オブジェクトが parameter でなかった
void
LogMgr::error_not_a_parameter(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Parameter type expected");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_NOT_A_PARAMETER",
	msg);
}

// @brief イベント式の根元に定数
void
LogMgr::error_illegal_constant_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Constant shall not be used in event description");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_CONST_IN_EVENT",
	msg);
}

// @brief イベント式の根元に関数呼び出し
void
LogMgr::error_illegal_funccall_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Function call shall not be used in event description");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_FUNCCALL_IN_EVENT",
	msg);
}

// @brief イベント式の根元にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Sysfunc call shall not be used in event description");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SYSFUNCCALL_IN_EVENT",
	msg);
}

// @brief 左辺式で用いることのできない演算子
void
LogMgr::error_illegal_operator_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Illegal operator in LHS");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_ILLEGAL_OPERATOR_IN_LHS",
	msg);
}

// @brief 左辺式に定数
void
LogMgr::error_illegal_constant_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr, "Constant shall not be in LHS");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_CONST_IN_LHS",
	msg);
}

// @brief 左辺式に関数呼び出し
void
LogMgr::error_illegal_funccall_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Function call shall not be used in LHS");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_FUNCCALL_IN_LHS",
	msg);
}

// @brief 左辺式にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  auto msg = expr_common(ast_expr,
			 "Sysfunc call shall not be used in LHS");
  error(file, line,
	ast_expr.file_region(),
	"ELAB_SYSFUNCCALL_IN_LHS",
	msg);
}

END_NAMESPACE_YM_VERILOG
