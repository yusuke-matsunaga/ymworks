#ifndef PTEXPR_H
#define PTEXPR_H

/// @file PtExpr.h
/// @brief PtExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtExpr PtExpr.h "parser/PtExpr.h"
/// @brief AstExpr の実装クラス
//////////////////////////////////////////////////////////////////////
class PtExpr :
  public AstExpr
{
};


//////////////////////////////////////////////////////////////////////
/// @class PtPart PtExr.h "parser/PtExpr.h"
/// @brief AstPart の実装クラス
//////////////////////////////////////////////////////////////////////
class PtPart :
  public AstPart
{
};

END_NAMESPACE_YM_VERILOG

#endif // ASTEXPRIMPL_H
