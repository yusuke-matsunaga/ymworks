
/// @file CptStmt.cc
/// @brief CptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtDecl.h"
#include "parser/PtFactory.h"
#include "parser/PtHierName.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// クラス CptStmt
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptStmt::file_region() const
{
  return mFileRegion;
}

// @brief ステートメントの種類を表す文字列の取得
const char*
CptStmt::stmt_name() const
{
  switch ( type() ) {
  case Disable:       return "disable statement";
  case Enable:        return "task enable statement";
  case SysEnable:     return "system task enable statement";
  case DelayControl:  return "delay control statement";
  case EventControl:  return "event control statement";
  case Assign:        return "assignment";
  case NbAssign:      return "nonblocking assignment";
  case Event:         return "event statement";
  case Null:          return "null statement";
  case If:            return "if statement";
  case Case:          return "case statement";
  case CaseX:         return "casex statement";
  case CaseZ:         return "casez statement";
  case Wait:          return "wait statement";
  case Forever:       return "forever statement";
  case Repeat:        return "repeat statement";
  case While:         return "while statement";
  case For:           return "for-loop statement";
  case PcAssign:      return "procedural continuous assignment";
  case Deassign:      return "deassign statement";
  case Force:         return "force statement";
  case Release:       return "release statement";
  case NamedParBlock:
  case ParBlock:      return "parallel block";
  case NamedSeqBlock:
  case SeqBlock:      return "sequential block";
  }
  return "";
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptStmt::namebranch_list() const
{
  return AstNameBranchList(nullptr);
}

// @brief 名前の取得
const char*
CptStmt::name() const
{
  return nullptr;
}

// @brief 引数のリストの取得
AstExprList
CptStmt::arg_list() const
{
  throw std::logic_error{"arg_list(): type mismatch"};
}

// @brief コントロールの取得
const AstControl*
CptStmt::control() const
{
  throw std::logic_error{"control(): type miscmatch"};
}

// @brief 本体のステートメントの取得
const AstStmt*
CptStmt::body() const
{
  throw std::logic_error{"body(): type mismatch"};
}

// @brief 式の取得
const AstExpr*
CptStmt::expr() const
{
  throw std::logic_error{"expr(): type mismatch"};
}

// @brief 左辺式の取得
const AstExpr*
CptStmt::lhs() const
{
  throw std::logic_error{"lhs(): type mismatch"};
}

// @brief 右辺式の取得
const AstExpr*
CptStmt::rhs() const
{
  throw std::logic_error{"rhs(): type mismatch"};
}

// @brief イベントプライマリの取得
const AstExpr*
CptStmt::primary() const
{
  throw std::logic_error{"primary(): type mismatch"};
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
const AstStmt*
CptStmt::else_body() const
{
  throw std::logic_error{"else_body(): type mismatch"};
}

// @brief case item のリストの取得
AstCaseItemList
CptStmt::caseitem_list() const
{
  throw std::logic_error{"caseitem_list(): type mismatch"};
}

// @brief 初期化代入文の取得
const AstStmt*
CptStmt::init_stmt() const
{
  throw std::logic_error{"init_stmt(): type mismatch"};
}

// @brief 繰り返し代入文の取得
const AstStmt*
CptStmt::next_stmt() const
{
  throw std::logic_error{"next_stmt(): type mismatch"};
  return nullptr;
}

// @brief 宣言ヘッダのリストの取得
AstDeclHeadList
CptStmt::declhead_list() const
{
  throw std::logic_error{"declhead_list(): type mismatch"};
}

// @brief 子供のステートメントのリストの取得
AstStmtList
CptStmt::stmt_list() const
{
  throw std::logic_error{"stmt_list(): type mismatch"};
}


//////////////////////////////////////////////////////////////////////
// クラス CptDisable
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptDisable::type() const
{
  return Disable;
}

// 末尾の名前を返す．
const char*
CptDisable::name() const
{
  return mName;
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptDisable::namebranch_list() const
{
  return AstNameBranchList();
}


//////////////////////////////////////////////////////////////////////
// クラス CptDisableH
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptDisableH::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptEnableBase
//////////////////////////////////////////////////////////////////////

// 末尾の名前を返す．
const char*
CptEnableBase::name() const
{
  return mName;
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptEnableBase::namebranch_list() const
{
  return AstNameBranchList();
}

// @brief 引数のリストの取得
AstExprList
CptEnableBase::arg_list() const
{
  return AstExprList(mArgTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptEnable
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptEnable::type() const
{
  return Enable;
}


//////////////////////////////////////////////////////////////////////
// クラス CptEnableH
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptEnableH::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptSysEnable
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptSysEnable::type() const
{
  return SysEnable;
}


//////////////////////////////////////////////////////////////////////
// クラス CptEventStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptEventStmt::type() const
{
  return Event;
}

// イベントプライマリを返す．
const AstExpr*
CptEventStmt::primary() const
{
  return mPrimary;
}


//////////////////////////////////////////////////////////////////////
// クラス CptNullStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptNullStmt::type() const
{
  return Null;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// disable 文を生成する．
PtStmt*
PtFactory::new_Disable(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptDisable));
  return new (p) CptDisable(file_region, name);
}

PtStmt*
PtFactory::new_Disable(
  const FileRegion& file_region,
  const PtHierName& hname
)
{
  void* p = mAlloc.get_memory(sizeof(CptDisableH));
  return new (p) CptDisableH(file_region, hname);
}

// enable 文を生成する．
PtStmt*
PtFactory::new_Enable(
  const FileRegion& file_region,
  const char* name,
  PtExpr* arg_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptEnable));
  return new (p) CptEnable(file_region, name, arg_top);
}

PtStmt*
PtFactory::new_Enable(
  const FileRegion& file_region,
  const PtHierName& hname,
  PtExpr* arg_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptEnableH));
  return new (p) CptEnableH(file_region, hname, arg_top);

}

// system task enable 文を生成する．
PtStmt*
PtFactory::new_SysEnable(
  const FileRegion& file_region,
  const char* task_name,
  PtExpr* arg_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptSysEnable));
  return new (p) CptSysEnable(file_region, task_name, arg_top);
}

// event 文を生成する．
PtStmt*
PtFactory::new_EventStmt(
  const FileRegion& file_region,
  const AstExpr* event
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventStmt));
  return new (p) CptEventStmt(file_region, event);
}

// null 文を生成する．
PtStmt*
PtFactory::new_NullStmt(
  const FileRegion& file_region
)
{
  void* p = mAlloc.get_memory(sizeof(CptNullStmt));
  return new (p) CptNullStmt(file_region);
}


END_NAMESPACE_YM_VERILOG
