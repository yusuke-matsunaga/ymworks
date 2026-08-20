
/// @file StmtGen_block.cc
/// @brief ElbMgr の実装ファイル(block statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int debug = 0;
#define dout std::cerr

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// ブロックステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief ブロックスコープ内の宣言要素の生成を行う．
void
StmtGen::phase2_namedblock(
  const VlScope* parent,
  const AstDeclHeadVec& ast_head_list
)
{
  if ( debug ) {
    dout << std::endl
	 << "phase2_namedblock( "
	 << parent->full_name()
	 << " ["
	 << std::hex << reinterpret_cast<PtrIntType>(parent) << std::dec
	 << "] )" << std::endl;
  }

  instantiate_decl(parent, ast_head_list);

  if ( debug ) {
    dout << "phase2_namedblock end" << std::endl
	 << std::endl;
  }
}

// @brief parallel block のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_parblock(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  auto stmt_list = instantiate_stmt_list(parent, process, env, ast_stmt);
  return mgr().new_Fork(parent, process, ast_stmt, stmt_list);
}

// @brief sequential block のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_seqblock(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  auto stmt_list = instantiate_stmt_list(parent, process, env, ast_stmt);
  return mgr().new_Begin(parent, process, ast_stmt, stmt_list);
}

// @brief 名前つき parallel block のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_namedparblock(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  auto block = mgr().find_namedobj(parent, ast_stmt->name());
  if ( block == nullptr ) {
    throw std::logic_error{"block == nullptr"};
  }

  auto stmt_list = instantiate_stmt_list(block, process, env, ast_stmt);
  return mgr().new_NamedFork(block, process, ast_stmt, stmt_list);
}

// @brief 名前つき sequential block のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_namedseqblock(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  auto block = mgr().find_namedobj(parent, ast_stmt->name());
  if ( block == nullptr ) {
    throw std::logic_error{"block == nullptr"};
  }

  auto stmt_list = instantiate_stmt_list(block, process, env, ast_stmt);
  return mgr().new_NamedBegin(block, process, ast_stmt, stmt_list);
}

// @brief Stmt のリストのインスタンス化を行う．
std::vector<const VlStmt*>
StmtGen::instantiate_stmt_list(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  SizeType stmt_num = ast_stmt->stmt_num();
  std::vector<const VlStmt*> stmt_list;
  stmt_list.reserve(stmt_num);
  for ( auto ast_stmt1: ast_stmt->stmt_list() ) {
    auto stmt1 = instantiate_stmt(parent, process, env, ast_stmt1);
    if ( !stmt1 ) {
      return std::vector<const VlStmt*>{};
    }
    stmt_list.push_back(stmt1);
  }

  return stmt_list;
}

// @brief ステートメントブロックのスコープを生成する．
const VlScope*
StmtGen::new_StmtBlockScope(
  const VlScope* parent,
  const AstStmt* ast_stmt
)
{
  return mgr().new_StmtBlockScope(parent, ast_stmt);
}

END_NAMESPACE_YM_VERILOG
