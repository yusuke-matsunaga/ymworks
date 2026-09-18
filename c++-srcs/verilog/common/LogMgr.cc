
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"
#include "elaborator/ElbError.h"
#include "ym/vl/AstCaseItem.h"
#include "ym/vl/AstConnection.h"
#include "ym/vl/AstDefParam.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstModule.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE
static
const int debug_msg = 1;
END_NONAMESPACE

// @brief 全メッセージ数を得る．
SizeType
LogMgr::msg_num() const
{
  return MsgMgr::msg_num();
}

// @brief エラーメッセージ数を得る．
SizeType
LogMgr::error_num() const
{
  return MsgMgr::error_num();
}

// @brief 警告メッセージ数を得る．
SizeType
LogMgr::warning_num() const
{
  return MsgMgr::warning_num();
}

// @brief 情報メッセージ数を得る．
SizeType
LogMgr::info_num() const
{
  return MsgMgr::info_num();
}

// @brief 失敗メッセージ数を得る．
SizeType
LogMgr::fail_num() const
{
  return MsgMgr::fail_num();
}

// @brief デバッグメッセージ数を得る．
SizeType
LogMgr::debug_num() const
{
  return MsgMgr::debug_num();
}

// @brief エラー用の汎用関数
void
LogMgr::error(
  const char* file,
  int line,
  FileRegion file_region,
  const char* label,
  std::string message
)
{
  put_msg(file, line,
	  file_region,
	  MsgType::Error,
	  label,
	  message);
  throw ElbError(file, line,
		 file_region,
		 label,
		 message);
}

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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
	file_region,
	"ELAB_NOT_FOUND",
	buf.str());
}

// @brief case 文のデフォルトラベルが2つ以上存在する．
void
LogMgr::error_dup_default_label(
  const char* file,
  int line,
  const AstCaseItem& caseitem
)
{
  error(file, line,
	caseitem.file_region(),
	"PARS_DUP_DEFAULT_LABEL",
	"More than one 'default' fault");
}

// @brief GenFor の変数名が異なる．
void
LogMgr::error_genvar_mismatch(
  const char* file,
  int line,
  const FileRegion& file_region,
  const char* loop_var,
  const char* next_var
)
{
  std::ostringstream buf;
  buf << "Lhs of the increment statement ("
      << next_var
      << ") does not match with Lhs of the initial statement ("
      << loop_var
      << ")";
  error(file, line,
	file_region,
	"PARSE_GENFOR_VAR_MISMATCH",
	buf.str());
}

// @brief メッセージを出力する．
void
LogMgr::put_msg(
  const char* file,
  int line,
  const FileRegion& file_region,
  MsgType type,
  const char* label,
  const std::string& message
)
{
  if ( debug_msg ) {
    std::cout << file_region << std::endl
	      << type << std::endl
	      << label << std::endl
	      << message << std::endl;
  }
  MsgMgr::put_msg(file, line,
		  file_region,
		  type,
		  label,
		  message);
}

END_NAMESPACE_YM_VERILOG
