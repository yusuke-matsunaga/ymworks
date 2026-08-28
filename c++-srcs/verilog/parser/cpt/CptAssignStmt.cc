
/// @file CptStmt.cc
/// @brief CptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptAssignStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptAssignBase
//////////////////////////////////////////////////////////////////////

// lhs を得る．
const AstExpr*
CptAssignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptAssign::type() const
{
  return Assign;
}

// rhs を得る．
const AstExpr*
CptAssign::rhs() const
{
  return mRhs;
}

// @brief コントロールを返す．
const AstControl*
CptAssign::control() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptAssignC
//////////////////////////////////////////////////////////////////////

// コントロールを返す．
const AstControl*
CptAssignC::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptNbAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptNbAssign::type() const
{
  return NbAssign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptNbAssignC
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptNbAssignC::type() const
{
  return NbAssign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPcAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptPcAssign::type() const
{
  return PcAssign;
}

// 右辺式を返す．
const AstExpr*
CptPcAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDeassign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptDeassign::type() const
{
  return Deassign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptForce
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptForce::type() const
{
  return Force;
}


//////////////////////////////////////////////////////////////////////
// クラス CptRelease
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptRelease::type() const
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
  void* p = mAlloc.get_memory(sizeof(CptAssign));
  return new (p) CptAssign(file_region, lhs, rhs);
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
  void* p = mAlloc.get_memory(sizeof(CptAssignC));
  return new (p) CptAssignC(file_region, lhs, rhs, control);
}

// nonblocking assign 文を生成する．
PtStmt*
PtFactory::new_NbAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptNbAssign));
  return new (p) CptNbAssign(file_region, lhs, rhs);
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
  void* p = mAlloc.get_memory(sizeof(CptNbAssignC));
  return new (p) CptNbAssignC(file_region, lhs, rhs, control);
}

// procedural continuous assignment 文を生成する．
PtStmt*
PtFactory::new_PcAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptPcAssign));
  return new (p) CptPcAssign(file_region, lhs, rhs);
}

// deassign 文を生成する．
PtStmt*
PtFactory::new_Deassign(
  const FileRegion& file_region,
  const AstExpr* lhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptDeassign));
  return new (p) CptDeassign(file_region, lhs);
}

// force 文を生成する．
PtStmt*
PtFactory::new_Force(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptForce));
  return new (p) CptForce(file_region, lhs, rhs);
}

// release 文を生成する．
PtStmt*
PtFactory::new_Release(
  const FileRegion& file_region,
  const AstExpr* lhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptRelease));
  return new (p) CptRelease(file_region, lhs);
}

END_NAMESPACE_YM_VERILOG
