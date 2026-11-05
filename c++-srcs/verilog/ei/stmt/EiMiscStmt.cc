
/// @file EiMiscStmt.cc
/// @brief EiMiscStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiMiscStmt.h"

#include "elaborator/ElbDecl.h"
#include "elaborator/ElbExpr.h"

#include "ym/vl/VlUserSystf.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlControl.h"

#include "ym/vl/AstStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief イベント文を生成する．
const VlStmt*
ElbMgr::new_EventStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  ElbExpr* named_event
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiEventStmt(parent, process, ast_stmt,
			      named_event);
    });
}

// @brief nullptr ステートメントを生成する．
const VlStmt*
ElbMgr::new_NullStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiNullStmt(parent, process, ast_stmt);
    });
}

// @brief タスクコール文を生成する．
const VlStmt*
ElbMgr::new_TaskCall(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlTaskFunc* task,
  const std::vector<ElbExpr*>& arg_array
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiTaskCall(parent, process, ast_stmt,
			     task, arg_array);
    });
}

// @brief システムタスクコール文を生成する．
const VlStmt*
ElbMgr::new_SysTaskCall(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlUserSystf* user_systf,
  const std::vector<ElbExpr*>& arg_array
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiSysTaskCall(parent, process, ast_stmt,
				user_systf, arg_array);
    });
}

// @brief disable 文を生成する．
const VlStmt*
ElbMgr::new_DisableStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlScope* target
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiDisableStmt(parent, process, ast_stmt,
				target);
    });
}

// @brief コントロール文を生成する．
const VlStmt*
ElbMgr::new_CtrlStmt(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const VlControl* control,
  const VlStmt* body
)
{
  return new_stmt(
    [&]() -> const VlStmt* {
      return new EiCtrlStmt(parent, process, ast_stmt,
			    control, body);
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EiEventStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiEventStmt::type() const
{
  return VpiObjType::EventStmt;
}

// @brief named event を返す．
const VlExpr*
EiEventStmt::named_event() const
{
  return mEvent;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNullStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiNullStmt::type() const
{
  return VpiObjType::NullStmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTcBase
//////////////////////////////////////////////////////////////////////

// @brief 引数の数を返す．
SizeType
EiTcBase::argument_num() const
{
  return mArgumentList.size();
}

// @brief 引数の取得
const VlExpr*
EiTcBase::argument(
  SizeType pos
) const
{
  if ( pos >= argument_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mArgumentList[pos];
}

// @brief 引数の取得
std::vector<const VlExpr*>
EiTcBase::argument_list() const
{
  return std::vector<const VlExpr*>{mArgumentList.begin(), mArgumentList.end()};
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskCall
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiTaskCall::type() const
{
  return VpiObjType::TaskCall;
}

// @brief task の実体を返す．
const VlTaskFunc*
EiTaskCall::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス SysTaskCall
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiSysTaskCall::type() const
{
  return VpiObjType::SysTaskCall;
}

// @brief user systf クラスへのポインタを返す．
const VlUserSystf*
EiSysTaskCall::user_systf() const
{
  return mUserSystf;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDisableStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiDisableStmt::type() const
{
  return VpiObjType::Disable;
}

// @brief expr を返す．
const VlScope*
EiDisableStmt::target_scope() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiCtrlStmt
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiCtrlStmt::type() const
{
  return mControl->type();
}

// @brief コントロールを返す．
const VlControl*
EiCtrlStmt::control() const
{
  return mControl;
}

// @brief 本文のステートメントを返す．
const VlStmt*
EiCtrlStmt::body_stmt() const
{
  return mBodyStmt;
}

END_NAMESPACE_YM_VERILOG
