
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"
#include "ym/vl/AstIOItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス LogMgr
//////////////////////////////////////////////////////////////////////

// @brief IO 宣言に aux_type と宣言が重複している．
void
LogMgr::error_dup_type(
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
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
  error(file, line,
	ast_item.file_region(),
	"ELAB_IMPNET_WITH_INIT",
	buf.str());
}

END_NAMESPACE_YM_VERILOG
