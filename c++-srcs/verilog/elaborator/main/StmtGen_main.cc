
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

#include "ym/pt/PtStmt.h"
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"
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
  const PtStmt* pt_stmt,
  bool cf
)
{
  // 具体的には以下の処理を行う．
  //
  // 1. 内部にステートメントを持つステートメントは再帰する．
  // 2. 自身がスコープとなるもの (named-begin, named-fork) はスコープ
  //    を生成し，phase2 用のキューに登録す．
  if ( pt_stmt == nullptr ) {
    throw std::logic_error{"pt_stmt == nullptr"};
  }

  switch ( pt_stmt->type() ) {
  case PtStmtType::Disable:
  case PtStmtType::Enable:
  case PtStmtType::SysEnable:
  case PtStmtType::Assign:
  case PtStmtType::NbAssign:
  case PtStmtType::Event:
  case PtStmtType::Null:
  case PtStmtType::PcAssign:
  case PtStmtType::Deassign:
  case PtStmtType::Force:
  case PtStmtType::Release:
    // 無視
    break;

  case PtStmtType::DelayControl:
  case PtStmtType::EventControl:
  case PtStmtType::Wait:
  case PtStmtType::Forever:
  case PtStmtType::Repeat:
  case PtStmtType::White:
  case PtStmtType::For:
    phase1_stmt(parent, pt_stmt->body());
    break;

  case PtStmtType::If:
    phase1_stmt(parent, pt_stmt->body());
    if ( pt_stmt->else_body() ) {
      phase1_stmt(parent, pt_stmt->else_body());
    }
    break;

  case PtStmtType::Case:
  case PtStmtType::CaseX:
  case PtStmtType::CaseZ:
    for ( auto pt_item: pt_stmt->caseitem_list() ) {
      phase1_stmt(parent, pt_item->body());
    }
    break;

  case PtStmtType::ParBlock:
  case PtStmtType::SeqBlock:
    for ( auto pt_stmt1: pt_stmt->stmt_list() ) {
      phase1_stmt(parent, pt_stmt1);
    }
    break;

  case PtStmtType::NamedParBlock:
  case PtStmtType::NamedSeqBlock:
    {
      auto block_scope{new_StmtBlockScope(parent, pt_stmt)};

      for ( auto pt_stmt1: pt_stmt->stmt_list() ) {
	phase1_stmt(block_scope, pt_stmt1);
      }
      if ( cf ) {
	phase2_namedblock(block_scope, pt_stmt->declhead_list());
      }
      else {
	auto stub = make_stub<StmtGen,
			      const VlScope*,
			      const std::vector<const PtDeclHead*>&>(this,
								&StmtGen::phase2_namedblock,
								block_scope,
								pt_stmt->declhead_list());
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
  const PtStmt* pt_stmt
)
{
  if ( pt_stmt == nullptr ) {
    return nullptr;
  }

  const VlStmt* stmt{nullptr};
  switch ( pt_stmt->type() ) {
  case PtStmtType::Disable:
    stmt = instantiate_disable(parent, process, pt_stmt);
    break;

  case PtStmtType::Enable:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_enable(parent, process, env, pt_stmt);
    break;

  case PtStmtType::SysEnable:
    if ( env.inside_constant_function() ) {
      // 無視する．
      // といっても nullptr を返すとまずいので NULL_STMT を返す．
      stmt = instantiate_nullstmt(parent, process, pt_stmt);
    }
    else {
      stmt = instantiate_sysenable(parent, process, env, pt_stmt);
    }
    break;

  case PtStmtType::Assign:
    stmt = instantiate_assign(parent, process, env, pt_stmt, true);
    break;

  case PtStmtType::NbAssign:
    if ( env.inside_function() ) {
      throw std::logic_error{"env.inside_function()"};
    }
    stmt = instantiate_assign(parent, process, env, pt_stmt, false);
    break;

  case PtStmtType::Event:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_eventstmt(parent, process, pt_stmt);
    break;

  case PtStmtType::Null:
    stmt = instantiate_nullstmt(parent, process, pt_stmt);
    break;

  case PtStmtType::PcAssign:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_pca(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Deassign:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_deassign(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Force:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_force(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Release:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_release(parent, process, env, pt_stmt);
    break;

  case PtStmtType::DelayControl:
  case PtStmtType::EventControl:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_ctrlstmt(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Wait:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_wait(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Forever:
    stmt = instantiate_forever(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Repeat:
    stmt = instantiate_repeat(parent, process, env, pt_stmt);
    break;

  case PtStmtType::White:
    stmt = instantiate_while(parent, process, env, pt_stmt);
    break;

  case PtStmtType::For:
    stmt = instantiate_for(parent, process, env, pt_stmt);
    break;

  case PtStmtType::If:
    stmt = instantiate_if(parent, process, env, pt_stmt);
    break;

  case PtStmtType::Case:
  case PtStmtType::CaseX:
  case PtStmtType::CaseZ:
    stmt = instantiate_case(parent, process, env, pt_stmt);
    break;

  case PtStmtType::ParBlock:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_parblock(parent, process, env, pt_stmt);
    break;

  case PtStmtType::SeqBlock:
    stmt = instantiate_seqblock(parent, process, env, pt_stmt);
    break;

  case PtStmtType::NamedParBlock:
    if ( env.inside_function() ) {
      goto error;
    }
    stmt = instantiate_namedparblock(parent, process, env, pt_stmt);
    break;

  case PtStmtType::NamedSeqBlock:
    stmt = instantiate_namedseqblock(parent, process, env, pt_stmt);
    break;

  default:
    throw std::logic_error{"Should not be reached"};
  }
  if ( stmt ) {
    // attribute instance の生成
    auto attr_list = attribute_list(pt_stmt);
    mgr().reg_attr(stmt, attr_list);
  }

  return stmt;

 error:
  // 使えません．
  {
    std::ostringstream buf;
    buf << pt_stmt->stmt_name()
	<< " : cannot be used in a constant function.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    pt_stmt->file_region(),
		    MsgType::Error,
		    "ELAB",
		    buf.str());
  }
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// PtStmt の派生クラスから ElbStmt を生成する関数
//////////////////////////////////////////////////////////////////////

// @brief disable statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_disable(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt
)
{
  const auto& fr = pt_stmt->file_region();

  // disable はモジュール境界を越えない？
  // 仕様書には何も書いていないのでたぶん越えられる．
  auto handle = mgr().find_obj_up(parent, pt_stmt, nullptr);
  if ( !handle ) {
    std::ostringstream buf;
    buf << pt_stmt->fullname() << " : Not found.";
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
  auto stmt = mgr().new_DisableStmt(parent, process, pt_stmt, scope);
  return stmt;
}

// @brief enable の実体化を行う．
const VlStmt*
StmtGen::instantiate_enable(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  const auto& fr = pt_stmt->file_region();

  // タスクを探し出して設定する．
  // タスク名の探索はモジュール境界を越える．
  auto handle = mgr().find_obj_up(parent, pt_stmt, nullptr);
  if ( !handle ) {
    std::ostringstream buf;
    buf << pt_stmt->fullname() << " : Not found.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    fr,
		    MsgType::Error,
		    "ELAB",
		    buf.str());
    return nullptr;
  }
  if ( handle->type() != VpiObjType::Task ) {
    std::ostringstream buf;
    buf << pt_stmt->fullname() << " : Not a task.";
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
  arg_list.reserve(pt_stmt->arg_num());
  for ( auto pt_expr: pt_stmt->arg_list() ) {
    auto expr = instantiate_expr(parent, env, pt_expr);
    if ( !expr ) {
      // エラーが起った．
      return nullptr;
    }
    arg_list.push_back(expr);
  }

  // task call ステートメントの生成
  auto stmt = mgr().new_TaskCall(parent, process, pt_stmt, task, arg_list);
  return stmt;
}

// @brief system enable 文の実体化を行う．
const VlStmt*
StmtGen::instantiate_sysenable(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  const auto& fr = pt_stmt->file_region();
  auto name = pt_stmt->name();

  // UserSystf を取り出す．
  auto user_systf = mgr().find_user_systf(name);
  if ( user_systf == nullptr ) {
    ErrorGen::no_such_systask(__FILE__, __LINE__, pt_stmt);
  }

  // 引数の数のチェック
  auto n = pt_stmt->arg_num();
  if ( !user_systf->check_n_of_args(n) ) {
    ErrorGen::n_of_arguments_mismatch(__FILE__, __LINE__, pt_stmt);
  }

  // 引数を生成する．
  std::vector<ElbExpr*> arg_list(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto pt_expr = pt_stmt->arg(i);
    // 空の引数があるのでエラーと区別する．
    ElbExpr* arg = nullptr;
    if ( pt_expr ) {
      arg = instantiate_arg(parent, env, pt_expr);
      arg_list.push_back(arg);
    }
    else {
      arg_list.push_back(nullptr);
    }
    if ( !user_systf->check_argument(i, arg) ) {
      ErrorGen::illegal_argument_type(__FILE__, __LINE__, pt_expr);
    }
    arg_list[i] = arg;
  }

  // system task call ステートメントの生成
  auto stmt = mgr().new_SysTaskCall(parent, process, pt_stmt,
				    user_systf, arg_list);
  return stmt;
}

// @brief delay / event control statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_ctrlstmt(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const PtStmt* pt_stmt
)
{
  auto pt_body = pt_stmt->body();
  auto body = instantiate_stmt(parent, process, env, pt_body);
  auto pt_control = pt_stmt->control();
  auto control = instantiate_control(parent, env, pt_control);

  if ( !body || !control ) {
    return nullptr;
  }

  // delay / event control ステートメントの生成
  auto stmt = mgr().new_CtrlStmt(parent, process, pt_stmt, control, body);

  return stmt;
}

// @brief コントロールを生成する．
const VlControl*
StmtGen::instantiate_control(
  const VlScope* parent,
  const ElbEnv& env,
  const PtControl* pt_control
)
{
  if ( pt_control == nullptr ) {
    return nullptr;
  }

  if ( pt_control->type() == PtCtrlType::Delay ) {
    auto delay = instantiate_expr(parent, env, pt_control->delay());
    if ( delay ) {
      return mgr().new_DelayControl(pt_control, delay);
    }
    return nullptr;
  }

  // イベントリストの生成を行う．
  SizeType event_num = pt_control->event_num();
  std::vector<ElbExpr*> event_list;
  event_list.reserve(event_num);
  for ( auto pt_expr: pt_control->event_list() ) {
    auto expr = instantiate_event_expr(parent, env, pt_expr);
    if ( !expr ) {
      return nullptr;
    }
    event_list.push_back(expr);
  }

  if ( pt_control->type() == PtCtrlType::Event ) {
    return mgr().new_EventControl(pt_control, event_list);
  }

  auto rep = instantiate_expr(parent, env, pt_control->rep_expr());
  if ( !rep ) {
    return nullptr;
  }
  return mgr().new_RepeatControl(pt_control, rep, event_list);
}

// @brief event statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_eventstmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt
)
{
  auto pt_expr = pt_stmt->primary();
  auto named_event = instantiate_namedevent(parent, pt_expr);
  if ( !named_event ) {
    return nullptr;
  }

  auto stmt = mgr().new_EventStmt(parent, process, pt_stmt, named_event);
  return stmt;
}

// @brief null statement の実体化を行う．
const VlStmt*
StmtGen::instantiate_nullstmt(
  const VlScope* parent,
  const VlProcess* process,
  const PtStmt* pt_stmt
)
{
  auto stmt = mgr().new_NullStmt(parent, process, pt_stmt);
  return stmt;
}

END_NAMESPACE_YM_VERILOG
