
/// @file StmtGen_assign.cc
/// @brief ElbMgr の実装ファイル(assign statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 代入系ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// 代入文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_assign(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt,
  bool block
)
{
  ElbVarLhsEnv env1{env};

  auto ast_lhs = ast_stmt.lhs();
  auto lhs = instantiate_lhs(parent, env1, ast_lhs);

  auto ast_rhs = ast_stmt.rhs();
  auto rhs = instantiate_rhs(parent, env, ast_rhs, lhs);

  auto ast_control = ast_stmt.control();
  auto control = instantiate_control(parent, env, ast_control);

  return elb_mgr().new_Assignment(parent, process, ast_stmt,
				  lhs, rhs, block, control);
}

// @brief procedural continuous assign 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_pca(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  ElbPcaLhsEnv env1{env};

  auto ast_lhs = ast_stmt.lhs();
  auto lhs = instantiate_lhs(parent, env1, ast_lhs);

  auto ast_rhs = ast_stmt.rhs();
  auto rhs = instantiate_rhs(parent, env, ast_rhs, lhs);

  return elb_mgr().new_AssignStmt(parent, process, ast_stmt, lhs, rhs);
}

// @brief deassign 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_deassign(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  ElbPcaLhsEnv env1{env};

  auto ast_lhs = ast_stmt.lhs();
  auto lhs = instantiate_lhs(parent, env1, ast_lhs);

  return elb_mgr().new_DeassignStmt(parent, process, ast_stmt, lhs);
}

// @brief force 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_force(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  ElbForceLhsEnv env1{env};

  auto ast_lhs = ast_stmt.lhs();
  auto lhs = instantiate_lhs(parent, env1, ast_lhs);

  auto ast_rhs = ast_stmt.rhs();
  auto rhs = instantiate_rhs(parent, env, ast_rhs, lhs);

  return elb_mgr().new_ForceStmt(parent, process, ast_stmt, lhs, rhs);
}

// @brief release 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_release(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  ElbForceLhsEnv env1{env};

  auto ast_lhs = ast_stmt.lhs();
  auto lhs = instantiate_lhs(parent, env1, ast_lhs);

  return elb_mgr().new_ReleaseStmt(parent, process, ast_stmt, lhs);
}

END_NAMESPACE_YM_VERILOG
