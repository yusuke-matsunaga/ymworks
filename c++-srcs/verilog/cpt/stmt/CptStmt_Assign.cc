
/// @file CptStmt_Assign.cc
/// @brief CptStmt_Assign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt_Assign.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStmt_AssignBase
//////////////////////////////////////////////////////////////////////

// lhs を得る．
const AstExpr*
CptStmt_AssignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Assign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Assign::type() const
{
  return Assign;
}

// rhs を得る．
const AstExpr*
CptStmt_Assign::rhs() const
{
  return mRhs;
}

// @brief コントロールを返す．
const AstControl*
CptStmt_Assign::control() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_AssignC
//////////////////////////////////////////////////////////////////////

// コントロールを返す．
const AstControl*
CptStmt_AssignC::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_NbAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_NbAssign::type() const
{
  return NbAssign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_NbAssignC
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_NbAssignC::type() const
{
  return NbAssign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_PcAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_PcAssign::type() const
{
  return PcAssign;
}

// 右辺式を返す．
const AstExpr*
CptStmt_PcAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Deassign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Deassign::type() const
{
  return Deassign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Force
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Force::type() const
{
  return Force;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Release
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Release::type() const
{
  return Release;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// assign 文を生成する．
PtStmt*
PtFactory::new_Assign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Assign));
  return new (p) CptStmt_Assign(file_region, lhs, rhs);
}

// assign 文を生成する．
PtStmt*
PtFactory::new_Assign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs,
  const AstControl* control
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_AssignC));
  return new (p) CptStmt_AssignC(file_region, lhs, rhs, control);
}

// nonblocking assign 文を生成する．
PtStmt*
PtFactory::new_NbAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_NbAssign));
  return new (p) CptStmt_NbAssign(file_region, lhs, rhs);
}

// nonblocking assign 文を生成する．
PtStmt*
PtFactory::new_NbAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs,
  const AstControl* control
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_NbAssignC));
  return new (p) CptStmt_NbAssignC(file_region, lhs, rhs, control);
}

// procedural continuous assignment 文を生成する．
PtStmt*
PtFactory::new_PcAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_PcAssign));
  return new (p) CptStmt_PcAssign(file_region, lhs, rhs);
}

// deassign 文を生成する．
PtStmt*
PtFactory::new_Deassign(
  const FileRegion& file_region,
  const AstExpr* lhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Deassign));
  return new (p) CptStmt_Deassign(file_region, lhs);
}

// force 文を生成する．
PtStmt*
PtFactory::new_Force(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Force));
  return new (p) CptStmt_Force(file_region, lhs, rhs);
}

// release 文を生成する．
PtStmt*
PtFactory::new_Release(
  const FileRegion& file_region,
  const AstExpr* lhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Release));
  return new (p) CptStmt_Release(file_region, lhs);
}

END_NAMESPACE_YM_VERILOG
