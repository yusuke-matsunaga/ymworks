
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"

#include "ym/vl/AstConnection.h"
#include "ym/vl/AstDefParam.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstInst.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス LogMgr
//////////////////////////////////////////////////////////////////////

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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
	ast_inst.file_region(),
	"ELAB_MANY_GATE_CONN",
	"Too many port connections.");
}

END_NAMESPACE_YM_VERILOG
