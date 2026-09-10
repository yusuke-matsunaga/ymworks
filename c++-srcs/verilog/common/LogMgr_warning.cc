
/// @file LogMgr.cc
/// @brief LogMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "common/LogMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 添字が範囲外
void
LogMgr::warning_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  warning(file, line,
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
  warning(file, line,
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
  warning(file, line,
	  file_region,
	  "ELAB_RIGHT_INDEX_OUT_OF_RANGE",
	  "Right index is out of range.");
}

END_NAMESPACE_YM_VERILOG
