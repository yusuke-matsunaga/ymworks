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

#include "ym/vl/AstAttrInst.h"
#include "ym/vl/AstAttrSpec.h"
#include "ym/vl/AstCaseItem.h"
#include "ym/vl/AstConnection.h"
#include "ym/vl/AstContAssign.h"
#include "ym/vl/AstControl.h"
#include "ym/vl/AstDeclHead.h"
#include "ym/vl/AstDeclItem.h"
#include "ym/vl/AstDefParam.h"
#include "ym/vl/AstDelay.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstGenCaseItem.h"
#include "ym/vl/AstInst.h"
#include "ym/vl/AstIOHead.h"
#include "ym/vl/AstIOItem.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstModule.h"
#include "ym/vl/AstNameBranch.h"
#include "ym/vl/AstPart.h"
#include "ym/vl/AstPathDecl.h"
#include "ym/vl/AstPathDelay.h"
#include "ym/vl/AstPort.h"
#include "ym/vl/AstRange.h"
#include "ym/vl/AstStmt.h"
#include "ym/vl/AstStrength.h"
#include "ym/vl/AstUdp.h"
#include "ym/vl/AstUdpEntry.h"
#include "ym/vl/AstUdpValue.h"

#endif // YM_VL_AST_H
