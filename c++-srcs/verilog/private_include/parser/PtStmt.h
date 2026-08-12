#ifndef PTSTMT_H
#define PTSTMT_H

/// @file PtStmt.h
/// @brief PtStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtStmt PtStmt.h "parser/PtStmt.h"
/// @brief AstStmt の実装クラス
//////////////////////////////////////////////////////////////////////
class PtStmt :
  public AstStmt
{
};


//////////////////////////////////////////////////////////////////////
/// @class PtCaseItem PtStmt.h "parser/PtStmt.h"
/// @brief AstCaseItem の実装クラス
//////////////////////////////////////////////////////////////////////
class PtCaseItem :
  public AstCaseItem
{
};

END_NAMESPACE_YM_VERILOG

#endif // PTSTMT_H
