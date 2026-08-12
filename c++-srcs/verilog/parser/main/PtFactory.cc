
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

// @brief 順序つき結合子のベクタの生成
PtConnectionArray
PtFactory::new_OrderedConArray(
  const AstExpr* expr
)
{
  auto con1 = new_OrderedCon(expr);
  return PtConnectionArray(mAlloc, con1);
}

// @brief 順序つき結合子のベクタの生成
PtConnectionArray
PtFactory::new_OrderedConArray(
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  return PtConnectionArray(mAlloc, con1, con2);
}

// @brief 順序つき結合子のベクタの生成
PtConnectionArray
PtFactory::new_OrderedConArray(
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  auto con3 = new_OrderedCon(expr3);
  return PtConnectionArray(mAlloc, con1, con2, con3);
}

// @brief 順序つき結合子のベクタの生成
PtConnectionArray
PtFactory::new_OrderedConArray(
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3,
  const AstExpr* expr4
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  auto con3 = new_OrderedCon(expr3);
  auto con4 = new_OrderedCon(expr4);
  return PtConnectionArray(mAlloc, con1, con2, con3, con4);
}

// @brief 階層名の生成
PtHierName*
PtFactory::new_HierName(
  const AstNameBranch* nb,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(PtHierName));
  return new (p) PtHierName(nb, name);
}

END_NAMESPACE_YM_VERILOG
