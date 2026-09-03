
/// @file StmtGen_cond.cc
/// @brief ElbMgr の実装ファイル(conditional statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"

#include "elaborator/ElbExpr.h"

#include "ym/vl/AstStmt.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstCaseItem.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 条件ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief if 文の生成を行う．
const VlStmt*
StmtGen::instantiate_if(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_cond = ast_stmt.expr();
  auto cond = instantiate_expr(parent, env, ast_cond);

  auto ast_then = ast_stmt.body();
  auto then_stmt = instantiate_stmt(parent, process, env, ast_then);

  auto ast_else = ast_stmt.else_body();
  auto else_stmt = instantiate_stmt(parent, process, env, ast_else);

  if ( !cond || !then_stmt || ( ast_else.is_valid() && !else_stmt ) ) {
    // たぶんエラー
    return nullptr;
  }

  return mgr().new_IfStmt(parent, process, ast_stmt,
			  cond, then_stmt, else_stmt);
}

// @brief case 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_case(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  // 条件式の生成
  auto ast_cond = ast_stmt.expr();
  auto cond = instantiate_expr(parent, env, ast_cond);
  if ( !cond ) {
    // たぶんエラー
    return nullptr;
  }

  // この case 文に関係する全ての式のリスト
  // あとでサイズ調整をするために用いる．
  std::vector<ElbExpr*> expr_list;
  SizeType ne{0};
  for ( auto ast_item: ast_stmt.caseitem_list() ) {
    ne += ast_item.label_list().size();
  }
  expr_list.reserve(ne);

  // default caseitem を末尾にするために順序づけを行う．
  // Parser::check_default_label() で default が高々1個しかないことは確認済み．
  std::vector<AstCaseItem> ast_caseitem_list;
  {
    SizeType nc = ast_stmt.caseitem_list().size();
    ast_caseitem_list.reserve(nc);
    AstCaseItem default_caseitem;
    for ( auto ast_item: ast_stmt.caseitem_list() ) {
      if ( ast_item.label_list().size() > 0 ) {
	ast_caseitem_list.push_back(ast_item);
      }
      else {
	default_caseitem = ast_item;
      }
    }
    if ( default_caseitem.is_valid() ) {
      // default caseitem を末尾に置く．
      ast_caseitem_list.push_back(default_caseitem);
    }
  }

  // case-item の生成
  std::vector<const VlCaseItem*> caseitem_list;
  caseitem_list.reserve(ast_caseitem_list.size());
  for ( auto ast_item: ast_caseitem_list ) {
    auto ast_body = ast_item.body();
    auto body = instantiate_stmt(parent, process, env, ast_body);
    if ( ast_body.is_valid() && !body ) {
      // たぶんエラー
      return nullptr;
    }
    // ast_body が空の場合はあり．

    // ラベルの生成と設定
    SizeType n = ast_item.label_list().size();
    std::vector<ElbExpr*> label_list;
    label_list.reserve(n);
    for ( auto ast_expr: ast_item.label_list() ) {
      auto expr = instantiate_expr(parent, env, ast_expr);
      if ( !expr ) {
	// たぶんエラー
	return nullptr;
      }
      label_list.push_back(expr);
      expr_list.push_back(expr);
    }

    // caseitem の生成
    auto caseitem = mgr().new_CaseItem(ast_item, label_list, body);
    caseitem_list.push_back(caseitem);
  }

  // expr_list のサイズを合わせる．
  // ルールは
  // - どれか一つでも符号付きならすべて符号付き
  // - サイズは各要素の最大サイズ
  // 初期値は cond のものを用いる．
  auto value_type0 = cond->value_type();
  if ( value_type0.is_real_type() ) {
    // real 型は駄目
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cond->file_region(),
		    MsgType::Error,
		    "ELAB",
		    "Case expression should not be real-type.");
    return nullptr;
  }
  bool sign = value_type0.is_signed();
  SizeType size = value_type0.size();

  for ( auto expr: expr_list ) {
    auto value_type1 = expr->value_type();
    if ( value_type1.is_real_type() ) {
      // real 型は駄目
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      MsgType::Error,
		      "ELAB",
		      "Case-item expression should not be real-type.");
      return nullptr;
    }

    if ( value_type1.is_signed() ) {
      // 符号付き
      sign = true;
    }

    SizeType size1 = value_type1.size();
    if ( size < size1 ) {
      // 最大サイズの更新
      size = size1;
    }
  }

  VlValueType value_type{sign, true, size};
  cond->set_reqsize(value_type);
  for ( auto expr: expr_list ) {
    expr->set_reqsize(value_type);
  }

  // case statement の生成
  return mgr().new_CaseStmt(parent, process, ast_stmt,
			    cond, caseitem_list);
}

// @brief wait 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_wait(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_cond = ast_stmt.expr();
  auto cond = instantiate_expr(parent, env, ast_cond);

  auto ast_body = ast_stmt.body();
  auto body = instantiate_stmt(parent, process, env, ast_body);

  if ( !cond || !body ) {
    return nullptr;
  }

  return mgr().new_WaitStmt(parent, process, ast_stmt, cond, body);
}

// @brief forever 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_forever(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_body = ast_stmt.body();
  auto body = instantiate_stmt(parent, process, env, ast_body);

  if ( !body ) {
    return nullptr;
  }

  return mgr().new_ForeverStmt(parent, process, ast_stmt, body);
}

// @brief repeat 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_repeat(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_expr = ast_stmt.expr();
  auto expr = instantiate_expr(parent, env, ast_expr);

  auto ast_body = ast_stmt.body();
  auto body = instantiate_stmt(parent, process, env, ast_body);

  if ( !expr || !body ) {
    return nullptr;
  }

  return mgr().new_RepeatStmt(parent, process, ast_stmt, expr, body);
}

// @brief while 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_while(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_cond = ast_stmt.expr();
  auto cond = instantiate_expr(parent, env, ast_cond);

  auto ast_body = ast_stmt.body();
  auto body = instantiate_stmt(parent, process, env, ast_body);

  if ( !cond || !body ) {
    return nullptr;
  }

  return mgr().new_WhileStmt(parent, process, ast_stmt, cond, body);
}

// @brief for 文のインスタンス化を行う．
const VlStmt*
StmtGen::instantiate_for(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_cond = ast_stmt.expr();
  auto cond = instantiate_expr(parent, env, ast_stmt.expr());

  auto ast_init = ast_stmt.init_stmt();
  auto init = instantiate_stmt(parent, process, env, ast_init);

  auto ast_next = ast_stmt.next_stmt();
  auto next = instantiate_stmt(parent, process, env, ast_next);

  auto ast_body = ast_stmt.body();
  auto body = instantiate_stmt(parent, process, env, ast_body);

  if ( !cond || !init || !next || !body ) {
    return nullptr;
  }

  auto stmt = mgr().new_ForStmt(parent, process, ast_stmt, cond, init, next, body);
  return stmt;
}

END_NAMESPACE_YM_VERILOG
