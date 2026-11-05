
/// @file EiAssignment.cc
/// @brief EiAssignment の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiAssignment.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/VlControl.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief 代入文を生成する．
const VlStmt*
ElbMgr::new_Assignment(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs,
  bool block,
  const VlControl* control
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      if ( block ) {
	return new EiAssignment(parent, process, ast_stmt,
				lhs, rhs, control);
      }
      else {
	return new EiNbAssignment(parent, process, ast_stmt,
				  lhs, rhs, control);
      }
    });
}

// @brief assign ステートメントを生成する．
const VlStmt*
ElbMgr::new_AssignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiAssignStmt(parent, process, ast_stmt,
			      lhs, rhs);
    });
}

// @brief deassign ステートメントを生成する．
const VlStmt*
ElbMgr::new_DeassignStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* lhs
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiDeassignStmt(parent, process, ast_stmt,
				lhs);
    });
}

// @brief force ステートメントを生成する．
const VlStmt*
ElbMgr::new_ForceStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* lhs,
  const VlExpr* rhs
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiForceStmt(parent, process, ast_stmt,
			     lhs, rhs);
    });
}

// @brief release ステートメントを生成する．
const VlStmt*
ElbMgr::new_ReleaseStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlExpr* lhs
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiReleaseStmt(parent, process, ast_stmt,
			       lhs);
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignBase
//////////////////////////////////////////////////////////////////////

// @brief 左辺を返す．
const VlExpr*
EiAssignBase::lhs() const
{
  return mLhs;
}

// @brief 右辺を返す．
const VlExpr*
EiAssignBase::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNbAssignment
//////////////////////////////////////////////////////////////////////

// @brief クラスの型の取得
VpiObjType
EiNbAssignment::type() const
{
  return VpiObjType::Assignment;
}

// @brief control を返す．nullptr の場合もありうる．
const VlControl*
EiNbAssignment::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignment
//////////////////////////////////////////////////////////////////////

// @brief 代入のブロッキング/ノンブロッキングの区別の取得
bool
EiAssignment::is_blocking() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiAssignStmt
//////////////////////////////////////////////////////////////////////

// @brief このクラスの型を返す．
VpiObjType
EiAssignStmt::type() const
{
  return VpiObjType::AssignStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiForceStmt
//////////////////////////////////////////////////////////////////////

// @brief このクラスの型を返す．
VpiObjType
EiForceStmt::type() const
{
  return VpiObjType::Force;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeassignBase
//////////////////////////////////////////////////////////////////////

// @brief 左辺を返す．
const VlExpr*
EiDeassignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeassignStmt
//////////////////////////////////////////////////////////////////////

// @brief このクラスの型を返す．
VpiObjType
EiDeassignStmt::type() const
{
  return VpiObjType::Deassign;
}


//////////////////////////////////////////////////////////////////////
// クラス EiReleaseStmt
//////////////////////////////////////////////////////////////////////

// @brief このクラスの型を返す．
VpiObjType
EiReleaseStmt::type() const
{
  return VpiObjType::Release;
}

END_NAMESPACE_YM_VERILOG
