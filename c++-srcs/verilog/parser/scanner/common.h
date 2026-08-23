#ifndef PARSER_COMMON_H
#define PARSER_COMMON_H

/// @file parser_common.h
/// @brief YACC の生成したヘッダファイルをインクルードする為のファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/Ast.h"
#include "parser/PtFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

class Parser;
class PtHierName;

#include "verilog_grammer.hh"

END_NAMESPACE_YM_VERILOG

#endif // PARSER_COMMON_H
