
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"

#include "elaborator/ObjHandle.h"
#include "ym/vl/AstControl.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstStmt.h"
#include "ym/vl/VlExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief function 内で使えないステートメント
void
LogMgr::error_illegal_stmt_in_function(
  const char* file,
  int line,
  const AstStmt& ast_stmt
)
{
  error(file, line,
	ast_stmt.file_region(),
	"ELAB_ILLEAGAL_STMT_IN_FUNC",
	"Illegal statement inside function block");
}

// @brief 該当するタスクが存在しない．
void
LogMgr::error_task_not_found(
  const char* file,
  int line,
  const AstStmt& ast_stmt
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_stmt.decompile_name()
      << "\": Not found";
  error(file, line,
	ast_stmt.file_region(),
	"ELAB_TASK_NOT_FOUND",
	buf.str());
}


// @brief 該当するシステムタスクが存在しない．
void
LogMgr::error_systask_not_found(
  const char* file,
  int line,
  const AstStmt& ast_stmt
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_stmt.name()
      << "\": No such system task";
  error(file, line,
	ast_stmt.file_region(),
	"ELAB_SYSTASK_NOT_FOUND",
	buf.str());
}

// @brief 対象がタスクではなかった．
void
LogMgr::error_not_a_task(
  const char* file,
  int line,
  const FileRegion& file_region,
  const ObjHandle* handle
)
{
  std::ostringstream buf;
  buf << handle->full_name()
      << " : Not a task";
  error(file, line,
	file_region,
	"ELAB_NOT_A_TASK",
	buf.str());
}

// @brief 対象が名前付きブロックではなかった．
void
LogMgr::error_not_a_namedblock(
  const char* file,
  int line,
  const FileRegion& file_region,
  const ObjHandle* handle
)
{
  std::ostringstream buf;
  buf << handle->full_name()
      << " : Not a namedblock, nor a task";
  error(file, line,
	file_region,
	"ELAB_NOT_A_NAMEDBLOCK",
	buf.str());
}

// @brief 引数の数が合わない．
void
LogMgr::error_argument_num_mismatch(
  const char* file,
  int line,
  const AstStmt& ast_stmt
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_stmt.decompile_name()
      << "(";
  const char* sep = "";
  for ( auto ast_expr: ast_stmt.arg_list() ) {
    buf << sep << ast_expr.decompile();
    sep = ", ";
  }
  buf << ")\": # of arguments mismatch";
  error(file, line,
	ast_stmt.file_region(),
	"ELAB_ARG_NUM_MISMATCH",
	buf.str());
}

// @brief function の内部ではコントロールを持てない．
void
LogMgr::error_ctrl_in_function(
  const char* file,
  int line,
  const AstControl& ast_ctrl
)
{
  error(file, line,
	ast_ctrl.file_region(),
	"ELAB_CTRL_IN_FUNCTION",
	"Control inside function is not allowed");
}

// @brief case 文の評価式に real 型は使えない．
void
LogMgr::error_real_in_case_expr(
  const char* file,  ///< [in] ファイル名
  int line,	       ///< [in] 行番号
  const VlExpr* expr ///< [in] 式
)
{
  std::ostringstream buf;
  buf << "\""
      << expr->decompile()
      << "\": Real type cannot be used in case expression";
  error(file, line,
	expr->file_region(),
	"ELAB_REAL_IN_CASE_EXPR",
	buf.str());
}

/// @brief case 文のラベル式に real 型は使えない．
void
LogMgr::error_real_in_case_label(
  const char* file,  ///< [in] ファイル名
  int line,	       ///< [in] 行番号
  const VlExpr* expr ///< [in] 式
)
{
  std::ostringstream buf;
  buf << "\""
      << expr->decompile()
      << "\": Real type cannot be used in case label";
  error(file, line,
	expr->file_region(),
	"ELAB_REAL_IN_CASE_LABEL",
	buf.str());
}

END_NAMESPACE_YM_VERILOG
