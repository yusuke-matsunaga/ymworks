
/// @file StmtGen_isc.cc
/// @brief StmtGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"

#include "ym/vl/AstStmt.h"

#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbUserSystf.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbError.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス StmtGen
//////////////////////////////////////////////////////////////////////

// @brief disable statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_disable(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt
)
{
  const auto& fr = ast_stmt.file_region();

  // disable はモジュール境界を越えない？
  // 仕様書には何も書いていないのでたぶん越えられる．
  auto handle = elb_mgr().find_obj_up(parent, ast_stmt, nullptr);
  if ( handle == nullptr ) {
    log_mgr().error_task_not_found(__FILE__, __LINE__, ast_stmt);
  }

  auto type = handle->type();
  if ( type != VpiObjType::NamedBegin &&
       type != VpiObjType::NamedFork &&
       type != VpiObjType::Task ) {
    log_mgr().error_not_a_namedblock(__FILE__, __LINE__, fr, handle);
  }

  auto scope = handle->scope();
  return elb_mgr().new_DisableStmt(parent, process, ast_stmt, scope);
}

// @brief enable の実体化を行う．
const VlStmt*
StmtGen::instantiate_enable(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  const auto& fr = ast_stmt.file_region();

  // タスクを探し出して設定する．
  // タスク名の探索はモジュール境界を越える．
  auto handle = elb_mgr().find_obj_up(parent, ast_stmt, nullptr);
  if ( handle == nullptr ) {
    log_mgr().error_task_not_found(__FILE__, __LINE__, ast_stmt);
  }
  if ( handle->type() != VpiObjType::Task ) {
    log_mgr().error_not_a_task(__FILE__, __LINE__, fr, handle);
  }

  auto task = handle->taskfunc();
  if ( task == nullptr ) {
    // handle->type == VpiObjType::Task なのでありえない．
    throw std::logic_error{"task == nullptr"};
  }

  // 引数を生成する．
  std::vector<ElbExpr*> arg_list;
  arg_list.reserve(ast_stmt.arg_list().size());
  for ( auto ast_expr: ast_stmt.arg_list() ) {
    auto expr = instantiate_expr(parent, env, ast_expr);
    arg_list.push_back(expr);
  }

  // task call ステートメントの生成
  return elb_mgr().new_TaskCall(parent, process, ast_stmt, task, arg_list);
}

// @brief system enable 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_sysenable(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  const auto& fr = ast_stmt.file_region();
  auto name = ast_stmt.name();

  // UserSystf を取り出す．
  auto user_systf = elb_mgr().find_user_systf(name);
  if ( user_systf == nullptr ) {
    log_mgr().error_systask_not_found(__FILE__, __LINE__, ast_stmt);
  }

  // 引数の数のチェック
  auto n = ast_stmt.arg_list().size();
  if ( !user_systf->check_n_of_args(n) ) {
    log_mgr().error_argument_num_mismatch(__FILE__, __LINE__, ast_stmt);
  }

  // 引数を生成する．
  std::vector<ElbExpr*> arg_list;
  arg_list.reserve(n);
  for ( auto ast_expr: ast_stmt.arg_list() ) {
    // 空の引数の場合は nullptr を渡す．
    ElbExpr* arg = nullptr;
    if ( ast_expr.is_valid() ) {
      arg = instantiate_arg(parent, env, ast_expr);
    }
    if ( !user_systf->check_argument(arg_list.size(), arg) ) {
      log_mgr().error_argument_type_mismatch(__FILE__, __LINE__, ast_expr);
    }
    arg_list.push_back(arg);
  }

  // system task call ステートメントの生成
  return elb_mgr().new_SysTaskCall(parent, process, ast_stmt,
				   user_systf, arg_list);
}

// @brief delay / event control statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_ctrlstmt(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  auto ast_body = ast_stmt.body();
  auto body = instantiate_stmt(parent, process, env, ast_body);
  auto ast_control = ast_stmt.control();

  auto control = instantiate_control(parent, env, ast_control);

  // delay / event control ステートメントの生成
  return elb_mgr().new_CtrlStmt(parent, process, ast_stmt, control, body);
}

// @brief コントロールを生成する．
const VlControl*
StmtGen::instantiate_control(
  const VlScope* parent,
  const ElbEnv& env,
  const AstControl& ast_control
)
{
  if ( ast_control.is_invalid() ) {
    return nullptr;
  }

  // 基本的にコントロールの生成でエラーが起きたらエラーを記録して nullptr を返す．
  try {
    if ( env.inside_function() ) {
      // 関数内ではコントロールは使えない．
      log_mgr().error_ctrl_in_function(__FILE__, __LINE__, ast_control);
    }

    if ( ast_control.type() == AstControl::Delay ) {
      auto delay = instantiate_expr(parent, env, ast_control.delay());
      return elb_mgr().new_DelayControl(ast_control, delay);
    }

    // イベントリストの生成を行う．
    SizeType event_num = ast_control.event_list().size();
    std::vector<ElbExpr*> event_list;
    event_list.reserve(event_num);
    for ( auto ast_expr: ast_control.event_list() ) {
      auto expr = instantiate_event_expr(parent, env, ast_expr);
      event_list.push_back(expr);
    }

    if ( ast_control.type() == AstControl::Event ) {
      return elb_mgr().new_EventControl(ast_control, event_list);
    }

    auto rep = instantiate_expr(parent, env, ast_control.rep_expr());
    return elb_mgr().new_RepeatControl(ast_control, rep, event_list);
  }
  catch ( const ElbError& error ) {
    return nullptr;
  }
}

// @brief event statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_eventstmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt
)
{
  auto ast_expr = ast_stmt.primary();
  auto named_event = instantiate_namedevent(parent, ast_expr);
  return elb_mgr().new_EventStmt(parent, process, ast_stmt, named_event);
}

// @brief null statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_nullstmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt
)
{
  return elb_mgr().new_NullStmt(parent, process, ast_stmt);
}

END_NAMESPACE_YM_VERILOG
