
/// @file LogMgr_udp.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"
#include "ym/vl/AstUdpEntry.h"
#include "ym/vl/AstUdpValue.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス LogMgr
//////////////////////////////////////////////////////////////////////

// @brief UDP のテーブルサイズが合わない．
void
LogMgr::error_udp_tablesize_mismatch(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  error(file, line,
	file_region,
	"ELAB_UDP_TABLESIZE_MISMATCH",
	"Number of input symbols mimatch.");
}

// @brief 組み合わせ回路用UDPに遷移シンボルは使えない．
void
LogMgr::error_udp_trans_sym(
  const char* file,
  int line,
  const AstUdpValue& ast_val
)
{
  std::ostringstream buf;
  buf << ast_val.symbol().to_string()
      << " : Transition symbol is not allowed for combinational UDP";
  error(file, line,
	ast_val.file_region(),
	"ELAB_UDP_TRANS_SYM",
	buf.str());
}

// @brief UDPテーブルの入力フィールドに不適切なシンボル．
void
LogMgr::error_udp_nc_sym_in_input(
  const char* file,
  int line,
  const AstUdpValue& ast_val
)
{
  std::ostringstream buf;
  buf << ast_val.symbol().to_string()
      << " : NC symbol is not allowed in input field.";
  error(file, line,
	ast_val.file_region(),
	"ELAB_UDP_NC_SYM_IN_INPUT",
	buf.str());
}

// @brief 組み合わせ回路用UDPに現状態はない．
void
LogMgr::error_udp_wrong_cur_state(
  const char* file,
  int line,
  const AstUdpEntry& ast_udp_entry
)
{
  error(file, line,
	ast_udp_entry.file_region(),
	"ELAB_UDP_WRONG_CUR_STATE",
	"Combinational UDP cannot not have "
	"\'current state\' value.");
}

// @brief UDPテーブルの出力フィールドに不適切なシンボル
void
LogMgr::error_udp_composite_sym_in_output(
  const char* file,
  int line,
  const AstUdpValue& ast_val
)
{
  std::ostringstream buf;
  buf << ast_val.symbol().to_string()
      << " : Composite symbol is not allowed in output field.";
  error(file, line,
	ast_val.file_region(),
	"ELAB_UDP_COMPOSITE_SYM_IN_OUTPUT",
	buf.str());
}

// @brief UDPテーブルの出力フィールドに遷移シンボルは使えない．
void
LogMgr::error_udp_trans_sym_in_output(
  const char* file,
  int line,
  const AstUdpValue& ast_val
)
{
  std::ostringstream buf;
  buf << ast_val.symbol().to_string()
      << " : Transition symbol is not allowed in output field.";
  error(file, line,
	ast_val.file_region(),
	"ELAB_UDP_TRANS_SYM_IN_OUTPUT",
	buf.str());
}

// @brief UDPテーブルの1行に複数の遷移シンボル
void
LogMgr::error_udp_dup_trans_sym(
  const char* file,
  int line,
  const AstUdpEntry& ast_udp_entry
)
{
  error(file, line,
	ast_udp_entry.file_region(),
	"ELAB_UDP_DUP_TRANS_SYM",
	"More than one transition symbols "
	"in the same row.");
}

// @brief 順序回路用UDPなのに現状態がない．
void
LogMgr::error_udp_no_cur_state(
  const char* file,
  int line,
  const AstUdpEntry& ast_udp_entry
)
{
  error(file, line,
	ast_udp_entry.file_region(),
	"ELAB_UDP_NO_CURRENT_STATE",
	"Sequential UDP requires \'current state\' value.");
}

// @brief UDPテーブルの現状態フィールドに遷移シンボルは使えない．
void
LogMgr::error_udp_trans_sym_in_cur_state(
  const char* file,
  int line,
  const AstUdpValue& ast_val
)
{
  std::ostringstream buf;
  buf << ast_val.symbol().to_string()
      << " : Transition symbol is not allowed in current state field.";
  error(file, line,
	ast_val.file_region(),
	"ELAB_UDP_TRANS_SYM_IN_CUR_STATE",
	buf.str());
}

// @brief UDPテーブルの現状態フィールドに NCシンボルは使えない．
void
LogMgr::error_udp_nc_sym_in_cur_state(
  const char* file,
  int line,
  const AstUdpValue& ast_val
)
{
  std::ostringstream buf;
  buf << ast_val.symbol().to_string()
      << " : NC symbol is not allowed in for current state field.";
  error(file, line,
	ast_val.file_region(),
	"ELAB_UDP_NC_SYM_IN_CUR_STATE",
	buf.str());
}

END_NAMESPACE_YM_VERILOG
