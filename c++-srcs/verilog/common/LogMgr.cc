
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

// @brief int 型が要求されている所で互換性のない型があった．
void
LogMgr::error_int_required(
  const char* file,
  int line,
  const FileRegion& loc
)
{
  throw ElbError(file, line,
		 loc,
		 "ELAB_INT_REQUIRED",
		 "Integer value is required");
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
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ILLEGAL_REAL_TYPE",
		 "Shall not have a real-type operand");
}

// @brief 該当する関数が存在しない．
void
LogMgr::error_no_such_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": No such function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_NO_SUCH_FUNCTION",
		 buf.str());
}

// @brief 該当するシステム関数が存在しない．
void
LogMgr::error_no_such_sysfunction(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": No such system function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_NO_SUCH_SYSFUNC",
		 buf.str());
}

// @brief 該当するシステムタスクが存在しない．
void
LogMgr::error_no_such_systask(
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
		 "ELAB_NO_SUCH_SYSTASK",
		 buf.str());
}

// @brief 関数ではない．
void
LogMgr::error_not_a_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Not a function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_NOT_A_FUNCTION",
		 buf.str());
}

// @brief 引数の数が合わない．
void
LogMgr::error_argument_num_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": # of argments mismatch";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ARG_NUM_MISMATCH",
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

// @brief 引数の型が合わない．
void
LogMgr::error_argument_type_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Argument type mismatch";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ARG_TYPE_MISMATCH",
		 buf.str());
}

// @brief オブジェクトの型が不適切
void
LogMgr::error_illegal_object(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Illegal type";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ILLEGAL_OBJECT",
		 buf.str());
}

// @brief オブジェクトが named-event でなかった
void
LogMgr::error_not_a_namedevent(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Named event expected";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_NOT_A_NAMEDEVENT",
		 buf.str());
}

// @brief 要素の範囲の順番と範囲指定の順番が異なる．
void
LogMgr::error_range_order(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Range order mismatch";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_RANGE_ORDER",
		 buf.str());
}

// @brief named-event に対する範囲指定
void
LogMgr::error_select_for_namedevent(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Named event cannot have a part-select";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SELECT_FOR_NAMEDEVENT",
		 buf.str());
}

// @brief assign/deassign に不適切なビット/範囲指定
void
LogMgr::error_select_in_pca(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Bit/part-select shall not be used"
      << " in LHS of assign/deassign statement.";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SELECT_FOR_PCA",
		 buf.str());
}

// @brief force/release に不適切なビット/範囲指定
void
LogMgr::error_select_in_force(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Bit/part-select shall not be used"
      << " in LHS of force/release statement.";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SELECT_FOR_FORCE",
		 buf.str());
}

// @brief assign/deassign に不適切な配列要素
void
LogMgr::error_array_in_pca(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Array element shall not be used"
      << " in LHS of assign/deassign statement.";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ARRAY_IN_PCA",
		 buf.str());
}

// @brief force/release に不適切な配列要素
void
LogMgr::error_array_in_force(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Array element shall not be used"
      << " in LHS of force/release statement.";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ARRAY_IN_FORCE",
		 buf.str());
}

// @brief 配列の次元が合わない
void
LogMgr::error_dimension_mismatch(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Dimension mismatch";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_DIM_MISMATCH",
		 buf.str());
}

// @brief real 型に対するビット選択あるいは部分選択があった
void
LogMgr::error_select_for_real(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Real type cannot have a part-select";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SELECT_FOR_REAL",
		 buf.str());
}

// @brief constant function 中にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": sysfunc cannot be used in constant function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SYSFUNC_IN_CF",
		 buf.str());
}

// @brief constant expression 中にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_ce(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": sysfunc cannot be used in constant expression";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SYSFUNC_IN_CE",
		 buf.str());
}

// @brief 定数関数は自己再帰できない．
void
LogMgr::error_uses_itself(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Uses itself";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_USES_ITSELF",
		 buf.str());
}

// @brief 定数関数ではない．
void
LogMgr::error_not_a_constant_function(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Not a constant function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_NOT_A_CF",
		 buf.str());
}

// @brief オブジェクトの型が constant function 用として不適切
void
LogMgr::error_illegal_object_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Illegal object type inside constant function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ILLEGAL_TYPE_IN_CF",
		 buf.str());
}

// @brief 階層名が constant expression 中にあった
void
LogMgr::error_hname_in_ce(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Hierarchical name shall not be used"
      << " inside constant expression";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_HNAME_IN_CE",
		 buf.str());
}

// @brief 階層名が constant function 中にあった
void
LogMgr::error_hname_in_cf(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Hierarchical name shall not be used"
      << " inside constant function";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_HNAME_IN_CF",
		 buf.str());
}

// @brief オブジェクトが parameter でなかった
void
LogMgr::error_not_a_parameter(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Parameter type expected";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_NOT_A_PARAMETER",
		 buf.str());
}

// @brief イベント式の根元に定数
void
LogMgr::error_illegal_constant_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Constant shall not be used in event description";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_CONST_IN_EVENT",
		 buf.str());
}

// @brief イベント式の根元に関数呼び出し
void
LogMgr::error_illegal_funccall_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Function call shall not be used in event description";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_FUNCCALL_IN_EVENT",
		 buf.str());
}

// @brief イベント式の根元にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_event_expression(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Sysfunc call shall not be used in event description";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SYSFUNCCALL_IN_EVENT",
		 buf.str());
}

// @brief 左辺式で用いることのできない演算子
void
LogMgr::error_illegal_operator_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Illegal operator in LHS";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_ILLEGAL_OPERATOR_IN_LHS",
		 buf.str());
}

// @brief 左辺式に定数
void
LogMgr::error_illegal_constant_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Constant shall not be in LHS";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_CONST_IN_LHS",
		 buf.str());
}

// @brief 左辺式に関数呼び出し
void
LogMgr::error_illegal_funccall_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Function call shall not be used in LHS";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_FUNCCALL_IN_LHS",
		 buf.str());
}

// @brief 左辺式にシステム関数呼び出し
void
LogMgr::error_illegal_sysfunccall_in_lhs(
  const char* file,
  int line,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_expr.decompile()
      << "\": Sysfunc call shall not be used in LHS";
  throw ElbError(file, line,
		 ast_expr.file_region(),
		 "ELAB_SYSFUNCCALL_IN_LHS",
		 buf.str());
}

// @brief 添字が範囲外
void
LogMgr::warning_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  put_warning(file, line,
	      file_region,
	      "ELAB_INDEX_OUT_OF_RANGE",
	      "Index is out of range.");
}

// @brief 左の範囲が範囲外
void
LogMgr::warning_left_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  put_warning(file, line,
	      file_region,
	      "ELAB_LEFT_INDEX_OUT_OF_RANGE",
	      "Left index is out of range.");
}

// @brief 右の範囲が範囲外
void
LogMgr::warning_right_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  put_warning(file, line,
	      file_region,
	      "ELAB_RIGHT_INDEX_OUT_OF_RANGE",
	      "Right index is out of range.");
}

// @brief モジュール配列のインスタンス生成
void
LogMgr::info_module_array(
  const char* file,
  int line,
  const AstItem& ast_head,
  const AstInst& ast_inst,
  const RangeVal& range
)
{
  std::ostringstream buf;
  buf << "Instantiating module array \"" << ast_inst.name() << "\" of \""
      << ast_head.name() << "\" [" << range.left << " : " << range.right << "].";
  put_info(file, line,
	   ast_inst.file_region(),
	   "ELAB_MODULE_ARRAY_INSTANTIATE",
	   buf.str());
}

// @brief モジュールのインスタンス生成
void
LogMgr::info_module(
  const char* file,
  int line,
  ElbModule* module
)
{
  std::ostringstream buf;
  buf << "\"" << module->full_name() << "\" has been created.";
  put_info(file, line,
	   module->file_region(),
	   "ELAB_MODULE_INSTANTIATE",
	   buf.str());
}

// @brief IO宣言のインスタンス生成
void
LogMgr::info_iodecl(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  const VlScope* scope
)
{
  std::ostringstream buf;
  buf << "IODecl(" << ast_item.name() << ")@"
      << scope->full_name() << " created.";
  put_info(file, line,
	   ast_item.file_region(),
	   "INFO_IODECL",
	   buf.str());
}

// @brief パラメータのインスタンス生成
void
LogMgr::info_param(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Parameter(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_PARAM",
	   buf.str());
}

// @brief ネット配列のインスタンス生成
void
LogMgr::info_net_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "NetArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_NET_ARRAY",
	   buf.str());
}

// @brief ネットのインスタンス生成
void
LogMgr::info_net(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Net(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_NET",
	   buf.str());
}

// @brief Reg配列のインスタンス生成
void
LogMgr::info_reg_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "RegArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_REG_ARRAY",
	   buf.str());
}

// @brief Regのインスタンス生成
void
LogMgr::info_reg(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Reg(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_REG",
	   buf.str());
}

// @brief Var配列のインスタンス生成
void
LogMgr::info_var_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "VarArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_VAR_ARRAY",
	   buf.str());
}

// @brief Varのインスタンス生成
void
LogMgr::info_var(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Var(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_VAR",
	   buf.str());
}

// @brief イベント配列のインスタンス生成
void
LogMgr::info_event_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
  std::ostringstream buf;
  buf << "EventArray(" << decl_array->full_name() << ") created.";
  put_info(file, line,
	   decl_array->file_region(),
	   "INFO_EVENT_ARRAY",
	   buf.str());
}

// @brief イベントのインスタンス生成
void
LogMgr::info_event(
  const char* file,
  int line,
  const VlDecl* decl
)
{
  std::ostringstream buf;
  buf << "Event(" << decl->full_name() << ") created.";
  put_info(file, line,
	   decl->file_region(),
	   "INFO_EVENT",
	   buf.str());
}

// @brief genvarのインスタンス生成
void
LogMgr::info_genvar(
  const char* file,
  int line,
  ElbGenvar* genvar
)
{
  std::ostringstream buf;
  buf << "Genvar(" << genvar->full_name() << ") created.";
  put_info(file, line,
	   genvar->file_region(),
	   "INFO_GENVER",
	   buf.str());
}

// @brief defparam の生成
void
LogMgr::info_defparam(
  const char* file,
  int line,
  const FileRegion& file_region,
  ElbParameter* param,
  const AstExpr& ast_expr
)
{
  std::ostringstream buf;
  buf << "DefParam("
      << param->full_name()
      << " = "
      << ast_expr.decompile()
      << ")";
  put_info(file, line,
	   file_region,
	   "INFO_DEFPARAM",
	   buf.str());
}

// @brief continuous assign の生成
void
LogMgr::info_contassign(
  const char* file,
  int line,
  const VlContAssign* cont_assign
)
{
  std::ostringstream buf;
  buf << "ContAssign("
      << cont_assign->lhs()->decompile()
      << " = "
      << cont_assign->rhs()->decompile()
      << ")";
  put_info(file, line,
	   cont_assign->file_region(),
	   "INFO_CONTASSIGN",
	   buf.str());
}

// @brief プリミティブ配列インスタンスの生成
void
LogMgr::info_prim_array(
  const char* file,
  int line,
  ElbPrimArray* prim_array
)
{
  std::ostringstream buf;
  buf << "PrimArray("
      << prim_array->full_name()
      << ")";
  put_info(file, line,
	   prim_array->file_region(),
	   "INFO_PRIMARRAY",
	   buf.str());
}

// @brief プリミティブインスタンスの生成
void
LogMgr::info_primitive(
  const char* file,
  int line,
  ElbPrimitive* prim
)
{
  std::ostringstream buf;
  buf << "Primitive("
      << prim->full_name()
      << ")";
  put_info(file, line,
	   prim->file_region(),
	   "INFO_PRIMITIVE",
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
