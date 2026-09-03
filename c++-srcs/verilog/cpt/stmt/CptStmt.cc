
/// @file CptStmt.cc
/// @brief CptStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtDeclHead.h"
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

// @brief 階層ブランチのリストを返す．
const PtNameBranch*
CptStmt::namebranch_top() const
{
  return nullptr;
}

// @brief 名前の取得
const char*
CptStmt::name() const
{
  return nullptr;
}

// @brief 引数のリストの取得
const PtExpr*
CptStmt::arg_top() const
{
  throw std::logic_error{"arg_top(): type mismatch"};
}

// @brief コントロールの取得
const PtControl*
CptStmt::control() const
{
  throw std::logic_error{"control(): type miscmatch"};
}

// @brief 本体のステートメントの取得
const PtStmt*
CptStmt::body() const
{
  throw std::logic_error{"body(): type mismatch"};
}

// @brief 式の取得
const PtExpr*
CptStmt::expr() const
{
  throw std::logic_error{"expr(): type mismatch"};
}

// @brief 左辺式の取得
const PtExpr*
CptStmt::lhs() const
{
  throw std::logic_error{"lhs(): type mismatch"};
}

// @brief 右辺式の取得
const PtExpr*
CptStmt::rhs() const
{
  throw std::logic_error{"rhs(): type mismatch"};
}

// @brief イベントプライマリの取得
const PtExpr*
CptStmt::primary() const
{
  throw std::logic_error{"primary(): type mismatch"};
}

// @brief 条件が成り立たなかったとき実行されるステートメントの取得
const PtStmt*
CptStmt::else_body() const
{
  throw std::logic_error{"else_body(): type mismatch"};
}

// @brief case item のリストの取得
const PtCaseItem*
CptStmt::caseitem_top() const
{
  throw std::logic_error{"caseitem_top(): type mismatch"};
}

// @brief 初期化代入文の取得
const PtStmt*
CptStmt::init_stmt() const
{
  throw std::logic_error{"init_stmt(): type mismatch"};
}

// @brief 繰り返し代入文の取得
const PtStmt*
CptStmt::next_stmt() const
{
  throw std::logic_error{"next_stmt(): type mismatch"};
  return nullptr;
}

// @brief 宣言ヘッダのリストの取得
const PtDeclHead*
CptStmt::declhead_top() const
{
  throw std::logic_error{"declhead_top(): type mismatch"};
}

// @brief 子供のステートメントのリストの取得
const PtStmt*
CptStmt::stmt_top() const
{
  throw std::logic_error{"stmt_top(): type mismatch"};
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Disable
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Disable::type() const
{
  return AstStmt::Disable;
}

// 末尾の名前を返す．
const char*
CptStmt_Disable::name() const
{
  return mName;
}

// @brief 階層ブランチのリストを返す．
const PtNameBranch*
CptStmt_Disable::namebranch_top() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_DisableH
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
const PtNameBranch*
CptStmt_DisableH::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_EnableBase
//////////////////////////////////////////////////////////////////////

// 末尾の名前を返す．
const char*
CptStmt_EnableBase::name() const
{
  return mName;
}

// @brief 階層ブランチのリストを返す．
const PtNameBranch*
CptStmt_EnableBase::namebranch_top() const
{
  return nullptr;
}

// @brief 引数のリストの取得
const PtExpr*
CptStmt_EnableBase::arg_top() const
{
  return mArgTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Enable
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Enable::type() const
{
  return AstStmt::Enable;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_EnableH
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
const PtNameBranch*
CptStmt_EnableH::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_SysEnable
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_SysEnable::type() const
{
  return AstStmt::SysEnable;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Event
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Event::type() const
{
  return AstStmt::Event;
}

// イベントプライマリを返す．
const PtExpr*
CptStmt_Event::primary() const
{
  return mPrimary;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Null
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Null::type() const
{
  return AstStmt::Null;
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
  void* p = mAlloc.get_memory(sizeof(CptStmt_Disable));
  return new (p) CptStmt_Disable(file_region, name);
}

PtStmt*
PtFactory::new_Disable(
  const FileRegion& file_region,
  const PtHierName& hname
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_DisableH));
  return new (p) CptStmt_DisableH(file_region, hname);
}

// enable 文を生成する．
PtStmt*
PtFactory::new_Enable(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* arg_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Enable));
  return new (p) CptStmt_Enable(file_region, name, arg_top);
}

PtStmt*
PtFactory::new_Enable(
  const FileRegion& file_region,
  const PtHierName& hname,
  const PtExpr* arg_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_EnableH));
  return new (p) CptStmt_EnableH(file_region, hname, arg_top);

}

// system task enable 文を生成する．
PtStmt*
PtFactory::new_SysEnable(
  const FileRegion& file_region,
  const char* task_name,
  const PtExpr* arg_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_SysEnable));
  return new (p) CptStmt_SysEnable(file_region, task_name, arg_top);
}

// event 文を生成する．
PtStmt*
PtFactory::new_EventStmt(
  const FileRegion& file_region,
  const PtExpr* event
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Event));
  return new (p) CptStmt_Event(file_region, event);
}

// null 文を生成する．
PtStmt*
PtFactory::new_NullStmt(
  const FileRegion& file_region
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Null));
  return new (p) CptStmt_Null(file_region);
}

END_NAMESPACE_YM_VERILOG
