
/// @file CptCtrlStmt.cc
/// @brief CptCtrlStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptCtrlStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptCtrlStmt
//////////////////////////////////////////////////////////////////////

// コントロールを返す．
const AstControl*
CptCtrlStmt::control() const
{
  return mControl;
}

// 本体を取り出す．
const AstStmt*
CptCtrlStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDcStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptDcStmt::type() const
{
  return DelayControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptEcStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptEcStmt::type() const
{
  return EventControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptWait
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptWait::type() const
{
  return Wait;
}

// 条件を返す．
const AstExpr*
CptWait::expr() const
{
  return mExpr;
}

// 実行すべき本体を返す．
const AstStmt*
CptWait::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// delay control 文を生成する．
PtStmt*
PtFactory::new_DcStmt(
  const FileRegion& file_region,
  const AstControl* delay,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptDcStmt));
  return new (p) CptDcStmt(file_region, delay, body);
}

// event control 文を生成する．
PtStmt*
PtFactory::new_EcStmt(
  const FileRegion& file_region,
  const AstControl* event,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptEcStmt));
  return new (p) CptEcStmt(file_region, event, body);
}

// wait 文を生成する．
PtStmt*
PtFactory::new_Wait(
  const FileRegion& file_region,
  const AstExpr* cond,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptWait));
  return new (p) CptWait(file_region, cond, body);
}

END_NAMESPACE_YM_VERILOG
