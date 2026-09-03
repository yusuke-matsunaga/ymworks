
/// @file CptStmt_Ctrl.cc
/// @brief CptStmt_Ctrl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt_Ctrl.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Ctrl
//////////////////////////////////////////////////////////////////////

// コントロールを返す．
const PtControl*
CptStmt_Ctrl::control() const
{
  return mControl;
}

// 本体を取り出す．
const PtStmt*
CptStmt_Ctrl::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_DelayControl
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_DelayControl::type() const
{
  return AstStmt::DelayControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_EventControl
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_EventControl::type() const
{
  return AstStmt::EventControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Wait
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Wait::type() const
{
  return AstStmt::Wait;
}

// 条件を返す．
const PtExpr*
CptStmt_Wait::expr() const
{
  return mExpr;
}

// 実行すべき本体を返す．
const PtStmt*
CptStmt_Wait::body() const
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
  const PtControl* delay,
  const PtStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_DelayControl));
  return new (p) CptStmt_DelayControl(file_region, delay, body);
}

// event control 文を生成する．
PtStmt*
PtFactory::new_EcStmt(
  const FileRegion& file_region,
  const PtControl* event,
  const PtStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_EventControl));
  return new (p) CptStmt_EventControl(file_region, event, body);
}

// wait 文を生成する．
PtStmt*
PtFactory::new_Wait(
  const FileRegion& file_region,
  const PtExpr* cond,
  const PtStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Wait));
  return new (p) CptStmt_Wait(file_region, cond, body);
}

END_NAMESPACE_YM_VERILOG
