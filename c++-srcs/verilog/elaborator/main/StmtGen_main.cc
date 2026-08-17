
/// @file StmtGen_main.cc
/// @brief ElbMgr の実装ファイル(statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "StmtGen.h"
#include "ElbEnv.h"
#include "ElbStub.h"
#include "ErrorGen.h"

#include "ym/vl/AstStmt.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"
#include "ym/vl/VlStmt.h"

#include "elaborator/ElbTaskFunc.h"
#include "elaborator/ElbUserSystf.h"
#include "elaborator/ElbExpr.h"

#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス StmtGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StmtGen::StmtGen(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : ElbProxy{elab, elb_mgr}
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
  const AstStmt* ast_stmt,
  bool cf
)
{
  // 具体的には以下の処理を行う．
  //
  // 1. 内部にステートメントを持つステートメントは再帰する．
  // 2. 自身がスコープとなるもの (named-begin, named-fork) はスコープ
  //    を生成し，phase2 用のキューに登録す．
  if ( ast_stmt == nullptr ) {
    throw std::logic_error{"ast_stmt == nullptr"};
  }

  switch ( ast_stmt->type() ) {
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
    phase1_stmt(parent, ast_stmt->body());
    break;

  case AstStmt::If:
    phase1_stmt(parent, ast_stmt->body());
    if ( ast_stmt->else_body() ) {
      phase1_stmt(parent, ast_stmt->else_body());
    }
    break;

  case AstStmt::Case:
  case AstStmt::CaseX:
  case AstStmt::CaseZ:
    for ( auto ast_item: ast_stmt->caseitem_list() ) {
      phase1_stmt(parent, ast_item->body());
    }
    break;

  case AstStmt::ParBlock:
  case AstStmt::SeqBlock:
    for ( auto ast_stmt1: ast_stmt->stmt_list() ) {
      phase1_stmt(parent, ast_stmt1);
    }
    break;

  case AstStmt::NamedParBlock:
  case AstStmt::NamedSeqBlock:
    {
      auto block_scope{new_StmtBlockScope(parent, ast_stmt)};

      for ( auto ast_stmt1: ast_stmt->stmt_list() ) {
	phase1_stmt(block_scope, ast_stmt1);
      }
      if ( cf ) {
	phase2_namedblock(block_scope, ast_stmt->declhead_list());
      }
      else {
	auto stub = make_stub<StmtGen,
			      const VlScope*,
			      const std::vector<const AstDeclHead*>&>(this,
								&StmtGen::phase2_namedblock,
								block_scope,
								ast_stmt->declhead_list());
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
  const AstStmt* ast_stmt
)
{
  if ( ast_stmt == nullptr ) {
    return nullptr;
  }

  const VlStmt* stmt{nullptr};
  switch ( ast_stmt->type() ) {
  case AstStmt::Disable:
    stmt = instantiate_disable(parent, process, ast_stmt);
    break;

  case AstStmt::Enable:
    if ( env.inside_function() ) {
      goto error;
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
      throw std::logic_error{"env.inside_function()"};
    }
    stmt = instantiate_assign(parent, process, env, ast_stmt, false);
    break;

  case AstStmt::Event:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_eventstmt(parent, process, ast_stmt);
    break;

  case AstStmt::Null:
    stmt = instantiate_nullstmt(parent, process, ast_stmt);
    break;

  case AstStmt::PcAssign:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_pca(parent, process, env, ast_stmt);
    break;

  case AstStmt::Deassign:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_deassign(parent, process, env, ast_stmt);
    break;

  case AstStmt::Force:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_force(parent, process, env, ast_stmt);
    break;

  case AstStmt::Release:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_release(parent, process, env, ast_stmt);
    break;

  case AstStmt::DelayControl:
  case AstStmt::EventControl:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_ctrlstmt(parent, process, env, ast_stmt);
    break;

  case AstStmt::Wait:
    if ( env.inside_function() ) {
      goto error;
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
      goto error;
    }
    stmt = instantiate_parblock(parent, process, env, ast_stmt);
    break;

  case AstStmt::SeqBlock:
    stmt = instantiate_seqblock(parent, process, env, ast_stmt);
    break;

  case AstStmt::NamedParBlock:
    if ( env.inside_function() ) {
      goto error;
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
    mgr().reg_attr(stmt, attr_list);
  }

  return stmt;

 error:
  // 使えません．
  {
    std::ostringstream buf;
    buf << ast_stmt->stmt_name()
	<< " : cannot be used in a constant function.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    ast_stmt->file_region(),
		    MsgType::Error,
		    "ELAB",
		    buf.str());
  }
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// AstStmt の派生クラスから ElbStmt を生成する関数
//////////////////////////////////////////////////////////////////////

// @brief disable statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_disable(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt
)
{
  const auto& fr = ast_stmt->file_region();

  // disable はモジュール境界を越えない？
  // 仕様書には何も書いていないのでたぶん越えられる．
  auto handle = mgr().find_obj_up(parent, ast_stmt, nullptr);
  if ( !handle ) {
    std::ostringstream buf;
    buf << ast_stmt->decompile_name() << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  auto type = handle->type();
  if ( type != VpiObjType::NamedBegin &&
       type != VpiObjType::NamedFork &&
       type != VpiObjType::Task ) {
    std::ostringstream buf;
    buf << handle->full_name()
	<< " : Not a named block, nor a task.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  auto scope = handle->scope();
  return mgr().new_DisableStmt(parent, process, ast_stmt, scope);
}

// @brief enable の実体化を行う．
const VlStmt*
StmtGen::instantiate_enable(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  const auto& fr = ast_stmt->file_region();

  // タスクを探し出して設定する．
  // タスク名の探索はモジュール境界を越える．
  auto handle = mgr().find_obj_up(parent, ast_stmt, nullptr);
  if ( !handle ) {
    std::ostringstream buf;
    buf << ast_stmt->decompile_name() << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }
  if ( handle->type() != VpiObjType::Task ) {
    std::ostringstream buf;
    buf << ast_stmt->decompile_name() << " : Not a task.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }

  auto task = handle->taskfunc();
  if ( task == nullptr ) {
    throw std::logic_error{"task == nullptr"};
  }

  // 引数を生成する．
  std::vector<ElbExpr*> arg_list;
  arg_list.reserve(ast_stmt->arg_num());
  for ( auto ast_expr: ast_stmt->arg_list() ) {
    auto expr = instantiate_expr(parent, env, ast_expr);
    if ( !expr ) {
      // エラーが起った．
      return nullptr;
    }
    arg_list.push_back(expr);
  }

  // task call ステートメントの生成
  return mgr().new_TaskCall(parent, process, ast_stmt, task, arg_list);
}

// @brief system enable 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_sysenable(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  const auto& fr = ast_stmt->file_region();
  auto name = ast_stmt->name();

  // UserSystf を取り出す．
  auto user_systf = mgr().find_user_systf(name);
  if ( user_systf == nullptr ) {
    ErrorGen::no_such_systask(__FILE__, __LINE__, ast_stmt);
  }

  // 引数の数のチェック
  auto n = ast_stmt->arg_num();
  if ( !user_systf->check_n_of_args(n) ) {
    ErrorGen::n_of_arguments_mismatch(__FILE__, __LINE__, ast_stmt);
  }

  // 引数を生成する．
  std::vector<ElbExpr*> arg_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto ast_expr = ast_stmt->arg(i);
    // 空の引数があるのでエラーと区別する．
    ElbExpr* arg = nullptr;
    if ( ast_expr ) {
      arg = instantiate_arg(parent, env, ast_expr);
      arg_list.push_back(arg);
    }
    else {
      arg_list.push_back(nullptr);
    }
    if ( !user_systf->check_argument(i, arg) ) {
      ErrorGen::illegal_argument_type(__FILE__, __LINE__, ast_expr);
    }
    arg_list[i] = arg;
  }

  // system task call ステートメントの生成
  return mgr().new_SysTaskCall(parent, process, ast_stmt,
			       user_systf, arg_list);
}

// @brief delay / event control statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_ctrlstmt(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt* ast_stmt
)
{
  auto ast_body = ast_stmt->body();
  auto body = instantiate_stmt(parent, process, env, ast_body);
  auto ast_control = ast_stmt->control();
  auto control = instantiate_control(parent, env, ast_control);

  if ( !body || !control ) {
    return nullptr;
  }

  // delay / event control ステートメントの生成
  return mgr().new_CtrlStmt(parent, process, ast_stmt, control, body);
}

// @brief コントロールを生成する．
const VlControl*
StmtGen::instantiate_control(
  const VlScope* parent,
  const ElbEnv& env,
  const AstControl* ast_control
)
{
  if ( ast_control == nullptr ) {
    return nullptr;
  }

  if ( ast_control->type() == AstControl::Delay ) {
    auto delay = instantiate_expr(parent, env, ast_control->delay());
    if ( delay ) {
      return mgr().new_DelayControl(ast_control, delay);
    }
    return nullptr;
  }

  // イベントリストの生成を行う．
  SizeType event_num = ast_control->event_num();
  std::vector<ElbExpr*> event_list;
  event_list.reserve(event_num);
  for ( auto ast_expr: ast_control->event_list() ) {
    auto expr = instantiate_event_expr(parent, env, ast_expr);
    if ( !expr ) {
      return nullptr;
    }
    event_list.push_back(expr);
  }

  if ( ast_control->type() == AstControl::Event ) {
    return mgr().new_EventControl(ast_control, event_list);
  }

  auto rep = instantiate_expr(parent, env, ast_control->rep_expr());
  if ( !rep ) {
    return nullptr;
  }
  return mgr().new_RepeatControl(ast_control, rep, event_list);
}

// @brief event statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_eventstmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt
)
{
  auto ast_expr = ast_stmt->primary();
  auto named_event = instantiate_namedevent(parent, ast_expr);
  if ( !named_event ) {
    return nullptr;
  }

  return mgr().new_EventStmt(parent, process, ast_stmt, named_event);
}

// @brief null statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_nullstmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt* ast_stmt
)
{
  return mgr().new_NullStmt(parent, process, ast_stmt);
}

END_NAMESPACE_YM_VERILOG
