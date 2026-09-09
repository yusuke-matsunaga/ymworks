
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbGenvar.h"
#include "elaborator/ElbError.h"
#include "elaborator/ElbParameter.h"
#include "elaborator/ElbPrimitive.h"
#include "elaborator/ObjHandle.h"
#include "elaborator/RangeVal.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/VlContAssign.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlDeclArray.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 同名のモジュール定義がある．
void
LogMgr::error_module_redefined(
  const char* file,
  int line,
  const AstModule& ast_module,
  const FileRegion& prev_file_region
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_module.name()
      << "\": Module redefined. Previous location is "
      << prev_file_region;
  throw ElbError(file, line,
		 ast_module.file_region(),
		 "ELAB_MODULE_REDEFINED",
		 buf.str());
}

// @brief 同名の関数定義がある．
void
LogMgr::error_function_redefined(
  const char* file,
  int line,
  const AstItem& ast_funcdef,
  const FileRegion& prev_file_region
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_funcdef.name()
      << "\": Function redefined. Previous location is "
      << prev_file_region;
  throw ElbError(file, line,
		 ast_funcdef.file_region(),
		 "ELAB_FUNCTION_REDEFINED",
		 buf.str());
}

// @brief 未解決の defparam 文がある．
void
LogMgr::error_defparam_unresolved(
  const char* file,
  int line,
  const AstDefParam& ast_defparam
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_defparam.decompile_name()
      << "\": Defparam unresolved";
  throw ElbError(file, line,
		 ast_defparam.file_region(),
		 "ELAB_DEFPARAM_UNRESOLVED",
		 buf.str());
}

// @brief パラメータポートの割り当て数が多すぎる．
void
LogMgr::error_too_many_params(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  throw ElbError(file, line,
		 file_region,
		 "ELAB_TOO_MANY_PARAMS",
		 "Too many parameters");
}

// @brief パラメータポートに現れるパラメータが存在しない．
void
LogMgr::error_param_not_found(
  const char* file,
  int line,
  const AstConnection& ast_con
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_con.name()
      << "\": No such parameter.";
  throw ElbError(file, line,
		 ast_con.file_region(),
		 "ELAB_PARAM_NOT_FOUND",
		 buf.str());
}

// @brief ポートに配列が使われている．
void
LogMgr::error_array_in_port_connection(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  throw ElbError(file, line,
		 file_region,
		 "ELAB_ARRAY_IN_PORT_CONNECTION",
		 "Array shall not be connected to a module port.");
}

// @brief ポートに使われている要素が宣言要素でなかった．
void
LogMgr::error_illegal_port(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  throw ElbError(file, line,
		 file_region,
		 "ELAB_ILLEGA_PORT",
		 "Illegal type for port connection.");
}

// @brief 重複した名前を持つ．
void
LogMgr::error_dup_name(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* name,
  const FileRegion& prev_loc
)
{
  std::ostringstream buf;
  buf << "\"" << name
      << "\": redefined. previous location is "
      << prev_loc;
  throw ElbError(file, line,
		 loc,
		 "ELAB_DUP_NAME",
		 buf.str());
}

// @brief 対象の要素が見つからない．
void
LogMgr::error_not_found(
  const char* file,
  int line,
  const FileRegion& file_region,
  const char* name
)
{
  std::ostringstream buf;
  buf << "\"" << name
      << "\": Not found";
  throw ElbError(file, line,
		 file_region,
		 "ELAB_NOT_FOUND",
		 buf.str());
}

// @brief IO 宣言に aux_type と宣言が重複している．
void
LogMgr::error_duplicate_type(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  const FileRegion& prev_loc
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_item.name()
      << "\": Having an aux-type declaration"
      << ", while it also has another declaration in "
      << prev_loc << ".";
  throw ElbError(file, line,
		 ast_item.file_region(),
		 "ELAB_DUP_TYPE",
		 buf.str());
}

// @brief IO 宣言に配列型の要素が現れている．
void
LogMgr::error_array_in_io_decl(
  const char* file,
  int line,
  const AstIOItem& ast_item
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_item.name()
      << "\": Array object shall not be connected to IO port.";
  throw ElbError(file, line,
		 ast_item.file_region(),
		 "ELAB_ARRAY_IN_IO",
		 buf.str());
}

// @brief IO 宣言に不適切な型の要素が現れている．
void
LogMgr::error_illegal_io_decl(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  bool is_module
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_item.name()
      << "\": Should be a ";
  if ( is_module ) {
    buf << "net, ";
  }
  buf << "reg or integer/time variable.";
  throw ElbError(file, line,
		 ast_item.file_region(),
		 "ELAB_ILLEGAL_IO",
		 buf.str());
}

// @brief IO宣言と要素宣言の範囲が異なる．
void
LogMgr::error_conflict_io_range(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  ElbDecl* decl
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_item.name()
      << "\": Conflictive range declaration.";
  throw ElbError(file, line,
		 ast_item.file_region(),
		 "ELAB_CONFLICT_IO_RANGE",
		 buf.str());
}

// @brief 暗黙のネット宣言は許されていない．
void
LogMgr::error_no_impnet(
  const char* file,
  int line,
  const AstIOItem& ast_item
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_item.name()
      << "\": Implicit declaration is inhibited "
      << " because default_nettype = \"none\".";
  throw ElbError(file, line,
		 ast_item.file_region(),
		 "ELAB_NO_IMPNET",
		 buf.str());
}

// @brief 暗黙のネット宣言は初期値を持てない．
void
LogMgr::error_impnet_with_init(
  const char* file,
  int line,
  const AstIOItem& ast_item
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_item.name()
      << "\": Implicit net declaration cannot have initial value.";
  throw ElbError(file, line,
		 ast_item.file_region(),
		 "ELAB_IMPNET_WITH_INIT",
		 buf.str());
}

// @brief defparam の割り当て対象がパラメータではなかった．
void
LogMgr::error_not_a_parameter(
  const char* file,
  int line,
  const FileRegion& file_region,
  const std::string& name
)
{
  std::ostringstream buf;
  buf << "\"" << name
      << "\" is not a parameter.";
  throw ElbError(file, line,
		 file_region,
		 "ELAB_NOT_A_PARAMETER",
		 buf.str());
}

// @brief defparam の割り当て対象が localparameter だった．
void
LogMgr::error_localparam_override(
  const char* file,
  int line,
  const AstDefParam& ast_defparam
)
{
  std::ostringstream buf;
  buf << "\"" << ast_defparam.decompile_name()
      << "\" is a localparam, which shall not be override.";
  throw ElbError(file, line,
		 ast_defparam.file_region(),
		 "ELAB_LOCALPARM_OVERRIDE",
		 buf.str());
}

// @brief 名無しのモジュールインスタンス
void
LogMgr::error_noname_module(
  const char* file,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file, line,
		 ast_inst.file_region(),
		 "ELAB_NONAME_MODULE",
		 "Module instance should have a name");
}

// @brief モジュールの依存関係がループしている．
void
LogMgr::error_cyclic_dependency(
  const char* file,
  int line,
  const AstModule& ast_module
)
{
  std::ostringstream buf;
  buf << ast_module.name() << " : instantiated within itself.";
  throw ElbError(file, line,
		 ast_module.file_region(),
		 "ELAB_CYCLIC_DEPENDENCY",
		 buf.str());
}

// @brief UDP インスタンスにパラメータ割り当てがある．
void
LogMgr::error_udp_with_paramassign(
  const char* file,        ///< [in] ファイル名
  int line,                ///< [in] 行番号
  const AstItem& ast_head  ///< [in] インスタンス記述のヘッダ
)
{
  throw ElbError(file, line,
		 ast_head.file_region(),
		 "ELAB_UDP_WITH_PARAMASSIGN",
		 "UDP instance cannot have parameter list.");
}

// @brief セルインスタンスにパラメータ割り当てがある．
void
LogMgr::error_cell_with_paramassign(
  const char* file,        ///< [in] ファイル名
  int line,                ///< [in] 行番号
  const AstItem& ast_head  ///< [in] インスタンス記述のヘッダ
)
{
  throw ElbError(file, line,
		 ast_head.file_region(),
		 "ELAB_CELL_WITH_PARAMASSIGN",
		 "Cell instance cannot have parameter list.");
}

// @brief モジュールインスタンスのポート数が合わない．
void
LogMgr::error_port_num_mismatch(
  const char* file,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file, line,
		 ast_inst.file_region(),
		 "ELAB_PORT_NUM_MISMATCH",
		 "# of ports mismatch");
}

// @brief ポートが見つからない．
void
LogMgr::error_port_not_found(
  const char* file,
  int line,
  const AstConnection& ast_conn
)
{
  std::ostringstream buf;
  buf << "\"" << ast_conn.name()
      << "\": no such port";
  throw ElbError(file, line,
		 ast_conn.file_region(),
		 "ELAB_PORT_NOT_FOUND",
		 buf.str());
}

// @brief ポート結合式に real タイプの式が使われている．
void
LogMgr::error_real_type_in_port_list(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_REAL_TYPE_IN_PORT_LIST",
		 "Real expression cannot connect to module port.");
}

// @brief モジュールインスタンスのポート結合式のビット幅が合わない．
void
LogMgr::error_port_size_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_PORT_SIZE_MISMATCH",
		 "Port size does not match with the expression.");
}

// @brief UDP instance に名前付きポート割り当てがあった．
void
LogMgr::error_named_port_in_udp_instance(
  const char* file,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file, line,
		 ast_inst.file_region(),
		 "ELAB_NAMED_PORT_IN_UDP",
		 "UDP instance cannot have named port list");
}

// @brief cell instance のピン名が合わない．
void
LogMgr::error_pin_name_not_found(
  const char* file,
  int line,
  const AstConnection& ast_con
)
{
  auto pin_name = ast_con.name();
  std::ostringstream buf;
  buf << "\""
      << pin_name
      << "\": No such pin.";
  throw ElbError(file, line,
		 ast_con.file_region(),
		 "ELAB_PIN_NAME_NOT_FOUND",
		 buf.str());
}

// @brief 空のポート式
void
LogMgr::error_empty_port_expression(
  const char* file,
  int line,
  const AstConnection& ast_con
)
{
  throw ElbError(file, line,
		 ast_con.file_region(),
		 "ELAB_EMPTY_PORT",
		 "Empty expression in UDP/primitive instance port is not allowed.");
}

// @brief genvar が見つからなかった．
void
LogMgr::error_genvar_not_found(
  const char* file,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Not found";
  throw ElbError(file, line,
		 ast_genfor.file_region(),
		 "ELAB_GENVAR_NOT_FOUND",
		 buf.str());
}

// @brief genvar が見つからなかった．
void
LogMgr::error_not_a_genvar(
  const char* file,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Not a genvar";
  throw ElbError(file, line,
		 ast_genfor.file_region(),
		 "ELAB_NOT_A_GENVAR",
		 buf.str());
}

// @brief genvar が他のループで用いられている．
void
LogMgr::error_genvar_in_use(
  const char* file,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Already in use by another loop";
  throw ElbError(file, line,
		 ast_genfor.file_region(),
		 "ELAB_GENVAR_IN_USE",
		 buf.str());
}

// @brief genvar の値が負になった．
void
LogMgr::error_genvar_negative(
  const char* file,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Having negative value";
  throw ElbError(file, line,
		 ast_genfor.file_region(),
		 "ELAB_GENVAR_NEGATIVE",
		 buf.str());
}

// @brief gate instance のポート結合の数が少ない．
void
LogMgr::error_few_gate_conn(
  const char* file,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file, line,
		 ast_inst.file_region(),
		 "ELAB_FEW_GATE_CONN",
		 "Too few port connections.");
}

// @brief gate instance のポート結合の数が多い．
void
LogMgr::error_many_gate_conn(
  const char* file,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file, line,
		 ast_inst.file_region(),
		 "ELAB_MANY_GATE_CONN",
		 "Too many port connections.");
}

// @brief function 内で使えないステートメント
void
LogMgr::error_illegal_stmt_in_function(
  const char* file,
  int line,
  const AstStmt& ast_stmt
)
{
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
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
  throw ElbError(file, line,
		 expr->file_region(),
		 "ELAB_REAL_IN_CASE_LABEL",
		 buf.str());
}

// @brief エラーメッセージを出力する．
void
LogMgr::put_error(
  const ElbError& error
)
{
  put_error(error.file(), error.line(),
	    error.file_region(),
	    error.label().c_str(),
	    error.message());
}

// @brief 失敗メッセージを出力する．
void
LogMgr::put_error(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)
{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Error,
		  label,
		  msg);
}

// @brief 失敗メッセージを出力する．
void
LogMgr::put_failure(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)
{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Failure,
		  label,
		  msg);
}

// @brief 警告メッセージを出力する．
void
LogMgr::put_warning(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)

{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Warning,
		  label,
		  msg);
}

// @brief 情報メッセージを出力する．
void
LogMgr::put_info(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)
{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Info,
		  label,
		  msg);
}

// @brief デバッグメッセージを出力する．
void
LogMgr::put_debug(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)
{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Debug,
		  label,
		  msg);
}

END_NAMESPACE_YM_VERILOG
