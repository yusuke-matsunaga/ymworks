
/// @file StmtGen_main.cc
/// @brief StmtGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym/vl/AstStmt.h"
#include "ym/vl/VlStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス StmtGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StmtGen::StmtGen(
  Elaborator& elab
) : ElbProxy{elab}
{
}

// @brief デストラクタ
StmtGen::~StmtGen()
{
}

// @brief スコープに関係するステートメントの実体化を行う．
void
StmtGen::phase1_stmt(
  const VlScope* parent,
  const AstStmt& ast_stmt,
  bool cf
)
{
  // 具体的には以下の処理を行う．
  //
  // 1. 内部にステートメントを持つステートメントは再帰する．
  // 2. 自身がスコープとなるもの (named-begin, named-fork) はスコープ
  //    を生成し，phase2 用のキューに登録す．
  if ( ast_stmt.is_invalid() ) {
    throw std::logic_error{"ast_stmt.is_invalid()"};
  }

  switch ( ast_stmt.type() ) {
  case AstStmt::Disable:
  case AstStmt::Enable:
  case AstStmt::SysEnable:
  case AstStmt::Assign:
  case AstStmt::NbAssign:
  case AstStmt::Event:
  case AstStmt::Null:
  case AstStmt::PcAssign:
  case AstStmt::Deassign:
  case AstStmt::Force:
  case AstStmt::Release:
    // 無視
    break;

  case AstStmt::DelayControl:
  case AstStmt::EventControl:
  case AstStmt::Wait:
  case AstStmt::Forever:
  case AstStmt::Repeat:
  case AstStmt::While:
  case AstStmt::For:
    phase1_stmt(parent, ast_stmt.body());
    break;

  case AstStmt::If:
    phase1_stmt(parent, ast_stmt.body());
    if ( ast_stmt.else_body().is_valid() ) {
      phase1_stmt(parent, ast_stmt.else_body());
    }
    break;

  case AstStmt::Case:
  case AstStmt::CaseX:
  case AstStmt::CaseZ:
    for ( auto ast_item: ast_stmt.caseitem_list() ) {
      phase1_stmt(parent, ast_item.body());
    }
    break;

  case AstStmt::ParBlock:
  case AstStmt::SeqBlock:
    for ( auto ast_stmt1: ast_stmt.stmt_list() ) {
      phase1_stmt(parent, ast_stmt1);
    }
    break;

  case AstStmt::NamedParBlock:
  case AstStmt::NamedSeqBlock:
    {
      auto block_scope = new_StmtBlockScope(parent, ast_stmt);

      for ( auto ast_stmt1: ast_stmt.stmt_list() ) {
	phase1_stmt(block_scope, ast_stmt1);
      }
      if ( cf ) {
	phase2_namedblock(block_scope, ast_stmt.declhead_list());
      }
      else {
	auto stub = make_stub<StmtGen,
			      const VlScope*,
			      const AstDeclHeadList&>(this,
						      &StmtGen::phase2_namedblock,
						      block_scope,
						      ast_stmt.declhead_list());
	add_phase2stub(stub);
      }
    }
    break;

  default:
    throw std::logic_error{"Should not be reached"};
  }
}

// @brief ステートメントの実体化を行う．
const VlStmt*
StmtGen::instantiate_stmt(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  if ( ast_stmt.is_invalid() ) {
    return nullptr;
  }

  const VlStmt* stmt{nullptr};
  switch ( ast_stmt.type() ) {
  case AstStmt::Disable:
    stmt = instantiate_disable(parent, process, ast_stmt);
    break;

  case AstStmt::Enable:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_enable(parent, process, env, ast_stmt);
    break;

  case AstStmt::SysEnable:
    if ( env.inside_constant_function() ) {
      // 無視する．
      // といっても nullptr を返すとまずいので NULL_STMT を返す．
      stmt = instantiate_nullstmt(parent, process, ast_stmt);
    }
    else {
      stmt = instantiate_sysenable(parent, process, env, ast_stmt);
    }
    break;

  case AstStmt::Assign:
    stmt = instantiate_assign(parent, process, env, ast_stmt, true);
    break;

  case AstStmt::NbAssign:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_assign(parent, process, env, ast_stmt, false);
    break;

  case AstStmt::Event:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_eventstmt(parent, process, ast_stmt);
    break;

  case AstStmt::Null:
    stmt = instantiate_nullstmt(parent, process, ast_stmt);
    break;

  case AstStmt::PcAssign:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_pca(parent, process, env, ast_stmt);
    break;

  case AstStmt::Deassign:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_deassign(parent, process, env, ast_stmt);
    break;

  case AstStmt::Force:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_force(parent, process, env, ast_stmt);
    break;

  case AstStmt::Release:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_release(parent, process, env, ast_stmt);
    break;

  case AstStmt::DelayControl:
  case AstStmt::EventControl:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_ctrlstmt(parent, process, env, ast_stmt);
    break;

  case AstStmt::Wait:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_wait(parent, process, env, ast_stmt);
    break;

  case AstStmt::Forever:
    stmt = instantiate_forever(parent, process, env, ast_stmt);
    break;

  case AstStmt::Repeat:
    stmt = instantiate_repeat(parent, process, env, ast_stmt);
    break;

  case AstStmt::While:
    stmt = instantiate_while(parent, process, env, ast_stmt);
    break;

  case AstStmt::For:
    stmt = instantiate_for(parent, process, env, ast_stmt);
    break;

  case AstStmt::If:
    stmt = instantiate_if(parent, process, env, ast_stmt);
    break;

  case AstStmt::Case:
  case AstStmt::CaseX:
  case AstStmt::CaseZ:
    stmt = instantiate_case(parent, process, env, ast_stmt);
    break;

  case AstStmt::ParBlock:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_parblock(parent, process, env, ast_stmt);
    break;

  case AstStmt::SeqBlock:
    stmt = instantiate_seqblock(parent, process, env, ast_stmt);
    break;

  case AstStmt::NamedParBlock:
    if ( env.inside_function() ) {
      log_mgr().error_illegal_stmt_in_function(__FILE__, __LINE__, ast_stmt);
    }
    stmt = instantiate_namedparblock(parent, process, env, ast_stmt);
    break;

  case AstStmt::NamedSeqBlock:
    stmt = instantiate_namedseqblock(parent, process, env, ast_stmt);
    break;

  default:
    throw std::logic_error{"Should not be reached"};
  }
  if ( stmt ) {
    // attribute instance の生成
    auto attr_list = attribute_list(ast_stmt);
    elb_mgr().reg_attr(stmt, attr_list);
  }

  return stmt;
}

END_NAMESPACE_YM_VERILOG
