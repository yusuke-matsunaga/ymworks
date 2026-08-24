
/// @file CptStmt.cc
/// @brief CptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"
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
  throw std::logic_error{"namebranch_list(): typemismatch"};
}

// @brief 名前の取得
const char*
CptStmt::name() const
{
  throw std::logic_error{"name(): typemismatch"};
}

// @brief 引数の数の取得
SizeType
CptStmt::arg_num() const
{
  throw std::logic_error{"arg_num(): type mismatch"};
}

// @brief 引数の取得
const AstExpr*
CptStmt::arg(
  SizeType index
) const
{
  throw std::logic_error{"arg(): type mismatch"};
}

// @brief 引数のリストの取得
AstExprVec
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

// @brief case item のリストの要素数の取得
SizeType
CptStmt::caseitem_num() const
{
  throw std::logic_error{"caseitem_num(): type mismatch"};
}

// @brief case item の取得
const AstCaseItem*
CptStmt::caseitem(
  SizeType index
) const
{
  throw std::logic_error{"caseitem(): type mismatch"};
}

// @brief case item のリストの取得
AstCaseItemVec
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

// @brief 宣言ヘッダ配列の要素数の取得
SizeType
CptStmt::declhead_num() const
{
  throw std::logic_error{"declhead_num(): type mismatch"};
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptStmt::declhead(
  SizeType index
) const
{
  throw std::logic_error{"declhead(): type mismatch"};
}

// @brief 宣言ヘッダのリストの取得
AstDeclHeadVec
CptStmt::declhead_list() const
{
  throw std::logic_error{"declhead_list(): type mismatch"};
}

// @brief 子供のステートメントリストの要素数の取得
SizeType
CptStmt::stmt_num() const
{
  throw std::logic_error{"stmt_num(): type mismatch"};
}

// @brief 子供のステートメントの取得
const AstStmt*
CptStmt::stmt(
  SizeType index
) const
{
  throw std::logic_error{"stmt(): type mismatch"};
}

// @brief 子供のステートメントのリストの取得
AstStmtVec
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

// @brief 引数の数の取得
SizeType
CptEnableBase::arg_num() const
{
  return mArgList->size();
}

// @brief 引数の取得
const AstExpr*
CptEnableBase::arg(
  SizeType index
) const
{
  return mArgList->expr(index);
}

// @brief 引数のリストの取得
AstExprVec
CptEnableBase::arg_list() const
{
  return mArgList->to_vector();
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

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptEnable::namebranch_list() const
{
  return AstNameBranchList();
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
// クラス CptCtrlStmt
//////////////////////////////////////////////////////////////////////

// コントロールを返す．
const AstControl*
CptCtrlStmt::control() const
{
  return mControl;
}

// 本体を取り出す．
const AstStmt*
CptCtrlStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDcStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptDcStmt::type() const
{
  return DelayControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptEcStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptEcStmt::type() const
{
  return EventControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptWait
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptWait::type() const
{
  return Wait;
}

// 条件を返す．
const AstExpr*
CptWait::expr() const
{
  return mExpr;
}

// 実行すべき本体を返す．
const AstStmt*
CptWait::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptAssignBase
//////////////////////////////////////////////////////////////////////

// lhs を得る．
const AstExpr*
CptAssignBase::lhs() const
{
  return mLhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptAssign::type() const
{
  return Assign;
}

// rhs を得る．
const AstExpr*
CptAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptAssignC
//////////////////////////////////////////////////////////////////////

// コントロールを返す．
const AstControl*
CptAssignC::control() const
{
  return mControl;
}


//////////////////////////////////////////////////////////////////////
// クラス CptNbAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptNbAssign::type() const
{
  return NbAssign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptNbAssignC
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptNbAssignC::type() const
{
  return NbAssign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPcAssign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptPcAssign::type() const
{
  return PcAssign;
}

// 右辺式を返す．
const AstExpr*
CptPcAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDeassign
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptDeassign::type() const
{
  return Deassign;
}


//////////////////////////////////////////////////////////////////////
// クラス CptForce
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptForce::type() const
{
  return Force;
}


//////////////////////////////////////////////////////////////////////
// クラス CptRelease
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptRelease::type() const
{
  return Release;
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
// クラス CptIf
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptIf::type() const
{
  return If;
}

// 条件式を返す．
const AstExpr*
CptIf::expr() const
{
  return mCond;
}

// 成り立ったとき実行されるステートメント
const AstStmt*
CptIf::body() const
{
  return mThen;
}


//////////////////////////////////////////////////////////////////////
// クラス CptIfElse
//////////////////////////////////////////////////////////////////////

// 成り立たなかったとき実行されるステートメント
const AstStmt*
CptIfElse::else_body() const
{
  return mElse;
}


//////////////////////////////////////////////////////////////////////
// クラス CptCase
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptCase::type() const
{
  return Case;
}

// 比較される式を返す．
const AstExpr*
CptCase::expr() const
{
  return mExpr;
}

// @brief case item のリストの要素数の取得
SizeType
CptCase::caseitem_num() const
{
  return mCaseItemList.size();
}

// @brief case item の取得
const AstCaseItem*
CptCase::caseitem(
  SizeType index
) const
{
  return mCaseItemList[index];
}

// @brief case item のリストの取得
AstCaseItemVec
CptCase::caseitem_list() const
{
  return mCaseItemList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptCaseX
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptCaseX::type() const
{
  return CaseX;
}


//////////////////////////////////////////////////////////////////////
// クラス CptCaseZ
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptCaseZ::type() const
{
  return CaseZ;
}


//////////////////////////////////////////////////////////////////////
// クラス CptCaseItem
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルのリストの要素数の取得
SizeType
CptCaseItem::label_num() const
{
  return mLabelList->size();
}

// @brief ラベルの取得
const AstExpr*
CptCaseItem::label(
  SizeType index
) const
{
  return mLabelList->expr(index);
}

// @brief ラベルリストの取得
AstExprVec
CptCaseItem::label_list() const
{
  return mLabelList->to_vector();
}

// 本体のステートメント得る．
const AstStmt*
CptCaseItem::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// LoopStmt のベースクラス
//////////////////////////////////////////////////////////////////////

// 実行すべき本体を返す．
const AstStmt*
CptLoopStmt::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptForever
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptForever::type() const
{
  return Forever;
}


//////////////////////////////////////////////////////////////////////
// クラス CptRepeat
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptRepeat::type() const
{
  return Repeat;
}

// 繰り返し式を返す．
const AstExpr*
CptRepeat::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptWhile::type() const
{
  return While;
}


//////////////////////////////////////////////////////////////////////
// クラス CptFor
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptFor::type() const
{
  return For;
}

// 初期化代入式を取出す
const AstStmt*
CptFor::init_stmt() const
{
  return mInit;
}

// 繰り返し代入式を取出す
const AstStmt*
CptFor::next_stmt() const
{
  return mNext;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmtBlock
//////////////////////////////////////////////////////////////////////

// @brief 子供のステートメントリストの要素数の取得
SizeType
CptStmtBlock::stmt_num() const
{
  return mStmtList.size();
}

// @brief 子供のステートメントの取得
const AstStmt*
CptStmtBlock::stmt(
  SizeType index
) const
{
  return mStmtList[index];
}

// @brief 子供のステートメントのリストの取得
AstStmtVec
CptStmtBlock::stmt_list() const
{
  return mStmtList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmtBlockN
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptStmtBlockN::name() const
{
  return mName;
}

// @brief 宣言ヘッダ配列の要素数の取得
SizeType
CptStmtBlockN::declhead_num() const
{
  return mDeclHeadList.size();
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptStmtBlockN::declhead(
  SizeType index
) const
{
  return mDeclHeadList[index];
}

// @brief 宣言ヘッダのリストの取得
AstDeclHeadVec
CptStmtBlockN::declhead_list() const
{
  return mDeclHeadList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptParBlock
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptParBlock::type() const
{
  return ParBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス CptParBlockN
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptParBlockN::type() const
{
  return NamedParBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス CptSeqBlock
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptSeqBlock::type() const
{
  return SeqBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス CptSeqBlockN
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptSeqBlockN::type() const
{
  return NamedSeqBlock;
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
  const AstExprList* arg_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptEnable));
  return new (p) CptEnable(file_region, name, arg_list);
}

PtStmt*
PtFactory::new_Enable(
  const FileRegion& file_region,
  const PtHierName& hname,
  const AstExprList* arg_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptEnableH));
  return new (p) CptEnableH(file_region, hname, arg_list);

}

// system task enable 文を生成する．
PtStmt*
PtFactory::new_SysEnable(
  const FileRegion& file_region,
  const char* task_name,
  const AstExprList* arg_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptSysEnable));
  return new (p) CptSysEnable(file_region, task_name, arg_list);
}

// delay control 文を生成する．
PtStmt*
PtFactory::new_DcStmt(
  const FileRegion& file_region,
  const AstControl* delay,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptDcStmt));
  return new (p) CptDcStmt(file_region, delay, body);
}

// event control 文を生成する．
PtStmt*
PtFactory::new_EcStmt(
  const FileRegion& file_region,
  const AstControl* event,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptEcStmt));
  return new (p) CptEcStmt(file_region, event, body);
}

// wait 文を生成する．
PtStmt*
PtFactory::new_Wait(
  const FileRegion& file_region,
  const AstExpr* cond,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptWait));
  return new (p) CptWait(file_region, cond, body);
}

// assign 文を生成する．
PtStmt*
PtFactory::new_Assign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptAssign));
  return new (p) CptAssign(file_region, lhs, rhs);
}

// assign 文を生成する．
PtStmt*
PtFactory::new_Assign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs,
  const AstControl* control
)
{
  void* p = mAlloc.get_memory(sizeof(CptAssignC));
  return new (p) CptAssignC(file_region, lhs, rhs, control);
}

// nonblocking assign 文を生成する．
PtStmt*
PtFactory::new_NbAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptNbAssign));
  return new (p) CptNbAssign(file_region, lhs, rhs);
}

// nonblocking assign 文を生成する．
PtStmt*
PtFactory::new_NbAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs,
  const AstControl* control
)
{
  void* p = mAlloc.get_memory(sizeof(CptNbAssignC));
  return new (p) CptNbAssignC(file_region, lhs, rhs, control);
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

// if 文を生成する．
PtStmt*
PtFactory::new_If(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstStmt* then_body
)
{
  void* p = mAlloc.get_memory(sizeof(CptIf));
  return new (p) CptIf(file_region, expr, then_body);
}

// if-else 文を生成する．
PtStmt*
PtFactory::new_IfElse(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstStmt* then_body,
  const AstStmt* else_body
)
{
  void* p = mAlloc.get_memory(sizeof(CptIfElse));
  return new (p) CptIfElse(file_region, expr,
			   then_body, else_body);
}

// case 文を生成する．
PtStmt*
PtFactory::new_Case(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstCaseItemVec& caseitem_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptCase));
  return new (p) CptCase(file_region, expr,
			 PtCaseItemArray(mAlloc, caseitem_list));
}

// casex 文を生成する．
PtStmt*
PtFactory::new_CaseX(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstCaseItemVec& caseitem_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseX));
  return new (p) CptCaseX(file_region, expr,
			  PtCaseItemArray(mAlloc, caseitem_list));
}

// casez 文を生成する．
PtStmt*
PtFactory::new_CaseZ(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstCaseItemVec& caseitem_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseZ));
  return new (p) CptCaseZ(file_region, expr,
			  PtCaseItemArray(mAlloc, caseitem_list));
}

// case item を生成する．
PtCaseItem*
PtFactory::new_CaseItem(
  const FileRegion& file_region,
  const AstExprList* label_list,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptCaseItem));
  return new (p) CptCaseItem(file_region, label_list, body);
}

// forever 文を生成する．
PtStmt*
PtFactory::new_Forever(
  const FileRegion& file_region,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptForever));
  return new (p) CptForever(file_region, body);
}

// repeat 文を生成する．
PtStmt*
PtFactory::new_Repeat(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptRepeat));
  return new (p) CptRepeat(file_region, expr, body);
}

// while 文を生成する．
PtStmt*
PtFactory::new_While(
  const FileRegion& file_region,
  const AstExpr* cond,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptWhile));
  return new (p) CptWhile(file_region, cond, body);
}

// for 文を生成する．
PtStmt*
PtFactory::new_For(
  const FileRegion& file_region,
  const AstStmt* init,
  const AstExpr* cond,
  const AstStmt* next,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptFor));
  return new (p) CptFor(file_region, init, cond, next, body);
}

// procedural continuous assignment 文を生成する．
PtStmt*
PtFactory::new_PcAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptPcAssign));
  return new (p) CptPcAssign(file_region, lhs, rhs);
}

// deassign 文を生成する．
PtStmt*
PtFactory::new_Deassign(
  const FileRegion& file_region,
  const AstExpr* lhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptDeassign));
  return new (p) CptDeassign(file_region, lhs);
}

// force 文を生成する．
PtStmt*
PtFactory::new_Force(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptForce));
  return new (p) CptForce(file_region, lhs, rhs);
}

// release 文を生成する．
PtStmt*
PtFactory::new_Release(
  const FileRegion& file_region,
  const AstExpr* lhs
)
{
  void* p = mAlloc.get_memory(sizeof(CptRelease));
  return new (p) CptRelease(file_region, lhs);
}

// par block を生成する．
PtStmt*
PtFactory::new_ParBlock(
  const FileRegion& file_region,
  const AstStmtVec& stmt_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptParBlock));
  return new (p) CptParBlock(file_region,
			     PtStmtArray(mAlloc, stmt_list));
}

// par block を生成する．
PtStmt*
PtFactory::new_NamedParBlock(
  const FileRegion& file_region,
  const char* name,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstStmtVec& stmt_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptParBlockN));
  return new (p) CptParBlockN(file_region, name,
			      PtDeclHeadArray(mAlloc, declhead_list),
			      PtStmtArray(mAlloc, stmt_list));
}

// seq block を生成する．
PtStmt*
PtFactory::new_SeqBlock(
  const FileRegion& file_region,
  const AstStmtVec& stmt_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptSeqBlock));
  return new (p) CptSeqBlock(file_region,
			     PtStmtArray(mAlloc, stmt_list));
}

// seq block を生成する．
PtStmt*
PtFactory::new_NamedSeqBlock(
  const FileRegion& file_region,
  const char* name,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstStmtVec& stmt_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptSeqBlockN));
  return new (p) CptSeqBlockN(file_region, name,
			      PtDeclHeadArray(mAlloc, declhead_list),
			      PtStmtArray(mAlloc, stmt_list));
}

END_NAMESPACE_YM_VERILOG
