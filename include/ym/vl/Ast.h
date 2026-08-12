#ifndef YM_VL_AST_H
#define YM_VL_AST_H

/// @file ym/vl/Ast.h
/// @brief 抽象構文木のクラス Ast のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup AstGroup 構文木の構成要素
/// @ingroup VlGroup
///
/// パース木の構成要素

#include "ym/FileRegion.h"
#include "ym/verilog.h"

#include "ym/vl/AstUdp.h"
#include "ym/vl/AstModule.h"
#include "ym/vl/AstDecl.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstStmt.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"

#endif // YM_VL_AST_H
