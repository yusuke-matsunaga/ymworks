
/// @file PtFactory.cc
/// @brief PtFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "parser/PtFactory.h"
#include "parser/PtExpr.h"
#include "parser/PtHierName.h"
#include "parser/PtMisc.h"
#include "alloc/Alloc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
PtFactory::PtFactory(
  Alloc& alloc
) : mAlloc{alloc}
{
}

// @brief デストラクタ
PtFactory::~PtFactory()
{
}

END_NAMESPACE_YM_VERILOG
