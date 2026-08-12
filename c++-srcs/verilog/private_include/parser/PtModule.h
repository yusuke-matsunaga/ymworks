#ifndef PTMODULE_H
#define PTMODULE_H

/// @file PtModule.h
/// @brief PtModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstModule.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtModule PtModule.h "parser/PtModule.h"
/// @brief AstModule の実装クラス
//////////////////////////////////////////////////////////////////////
class PtModule :
  public AstModule
{
};

END_NAMESPACE_YM_VERILOG

#endif // PTMODULE_H
