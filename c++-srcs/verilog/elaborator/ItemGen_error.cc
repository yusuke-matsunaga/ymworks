
/// @file ItemGen_error.cc
/// @brief ItemGen_error の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ItemGen.h"
#include "ElbError.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief defparam の割り当て対象がパラメータではなかった．
void
ItemGen::error_not_a_parameter(
  const char* file_name,
  int line,
  const AstDefParam& ast_defparam
)
{
  std::ostringstream buf;
  buf << "\"" << ast_defparam.decompile_name()
      << "\" is not a parameter.";
  throw ElbError(file_name, line,
		 ast_defparam.file_region(),
		 "ELAB_NOT_A_PARAMETER",
		 buf.str());
}

// @brief defparam の割り当て対象が localparameter だった．
void
ItemGen::error_localparam_override(
  const char* file_name,
  int line,
  const AstDefParam& ast_defparam
)
{
  std::ostringstream buf;
  buf << "\"" << ast_defparam.decompile_name()
      << "\" is a localparam, which shall not be override.";
  throw ElbError(file_name, line,
		 ast_defparam.file_region(),
		 "ELAB_LOCALPARM_OVERRIDE",
		 buf.str());
}

// @brief 名無しのモジュールインスタンス
void
ItemGen::error_noname_module(
  const char* file_name,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file_name, line,
		 ast_inst.file_region(),
		 "ELAB_NONAME_MODULE",
		 "Module instance should have a name");
}

// @brief モジュールの依存関係がループしている．
void
ItemGen::error_cyclic_dependency(
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
ItemGen::error_udp_with_paramassign(
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
ItemGen::error_cell_with_paramassign(
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
ItemGen::error_port_num_mismatch(
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
ItemGen::error_port_not_found(
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
ItemGen::error_real_type_in_port_list(
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
ItemGen::error_port_size_mismatch(
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

// @brief genvar が見つからなかった．
void
ItemGen::error_genvar_not_found(
  const char* file_name,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Not found";
  throw ElbError(file_name, line,
		 ast_genfor.file_region(),
		 "ELAB_GENVAR_NOT_FOUND",
		 buf.str());
}

// @brief genvar が見つからなかった．
void
ItemGen::error_not_a_genvar(
  const char* file_name,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Not a genvar";
  throw ElbError(file_name, line,
		 ast_genfor.file_region(),
		 "ELAB_NOT_A_GENVAR",
		 buf.str());
}

// @brief genvar が他のループで用いられている．
void
ItemGen::error_genvar_in_use(
  const char* file_name,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Already in use by another loop";
  throw ElbError(file_name, line,
		 ast_genfor.file_region(),
		 "ELAB_GENVAR_IN_USE",
		 buf.str());
}

// @brief genvar の値が負になった．
void
ItemGen::error_genvar_negative(
  const char* file_name,
  int line,
  const AstItem& ast_genfor
)
{
  std::ostringstream buf;
  buf << "\"" << ast_genfor.loop_var()
      << "\": Having negative value";
  throw ElbError(file_name, line,
		 ast_genfor.file_region(),
		 "ELAB_GENVAR_NEGATIVE",
		 buf.str());
}

// @brief gate instance のポート結合の数が少ない．
void
ItemGen::error_few_gate_conn(
  const char* file_name,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file_name, line,
		 ast_inst.file_region(),
		 "ELAB_FEW_GATE_CONN",
		 "Too few port connections.");
}

// @brief gate instance のポート結合の数が多い．
void
ItemGen::error_many_gate_conn(
  const char* file_name,
  int line,
  const AstInst& ast_inst
)
{
  throw ElbError(file_name, line,
		 ast_inst.file_region(),
		 "ELAB_MANY_GATE_CONN",
		 "Too many port connections.");
}

END_NAMESPACE_YM_VERILOG
