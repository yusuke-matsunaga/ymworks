
/// @file ModuleGen.cc
/// @brief ElbMgr の実装ファイル (module のインスタンス化関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ModuleGen.h"
#include "ElbParamCon.h"
#include "ElbError.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ModuleGen
//////////////////////////////////////////////////////////////////////

// @brief 同名のモジュール定義がある．
void
ModuleGen::error_module_redefined(
  const char* file,
  int line,
  const AstModule& ast_module,
  const FileRegion& prev_file_region
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_module.name()
      << "\": Redefined. previous definition is "
      << prev_file_region;
  throw ElbError(file, line,
		 ast_module.file_region(),
		 "ELAB_MODULE_REDEFINED",
		 buf.str());
}

// @brief パラメータポートの割り当て数が多すぎる．
void
ModuleGen::error_too_many_params(
  const char* file,
  int line,
  const std::vector<ElbParamCon>& param_con_list
)
{
  auto last = param_con_list.back();
  throw ElbError(file, line,
		 last.mAstCon.file_region(),
		 "ELAB_TOO_MANY_PARAMS",
		 "Too many parameters");
}

// @brief パラメータポートに現れるパラメータが存在しない．
void
ModuleGen::error_param_not_found(
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
ModuleGen::error_array_in_port_connection(
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
ModuleGen::error_illegal_port(
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

// @brief 添字が範囲外
void
ModuleGen::warning_index_out_of_range(
  const FileRegion& file_region
)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Index is out of range.");
}

// @brief 左の範囲が範囲外
void
ModuleGen::warning_left_index_out_of_range(
  const FileRegion& file_region
)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Left index is out of range.");
}

// @brief 右の範囲が範囲外
void
ModuleGen::warning_right_index_out_of_range(
  const FileRegion& file_region
)
{
  MsgMgr::put_msg(__FILE__, __LINE__,
		  file_region,
		  MsgType::Warning,
		  "ELAB",
		  "Right index is out of range.");
}

END_NAMESPACE_YM_VERILOG
