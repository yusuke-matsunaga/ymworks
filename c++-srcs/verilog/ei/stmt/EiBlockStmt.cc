
/// @file EiBlockStmt.cc
/// @brief EiBlockStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "EiBlockStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief begin ブロックを生成する．
const VlStmt*
EiFactory::new_Begin(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiBegin(parent, process, ast_stmt, stmt_list);
  return stmt;
}

// @brief fork ブロックを生成する．
const VlStmt*
EiFactory::new_Fork(
  const VlScope* parent,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiFork(parent, process, ast_stmt, stmt_list);
  return stmt;
}

// @brief 名前付き begin ブロックを生成する．
const VlStmt*
EiFactory::new_NamedBegin(
  const VlScope* block,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiNamedBegin(block, process, ast_stmt, stmt_list);
  return stmt;
}

// @brief 名前付き fork ブロックを生成する．
const VlStmt*
EiFactory::new_NamedFork(
  const VlScope* block,
  const VlProcess* process,
  const AstStmt& ast_stmt,
  const std::vector<const VlStmt*>& stmt_list
)
{
  auto stmt = new EiNamedFork(block, process, ast_stmt, stmt_list);
  return stmt;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief 中身のステートメントのリストの要素数を返す．
SizeType
EiBlockStmt::child_stmt_num() const
{
  return mStmtList.size();
}

// @brief 子供のステートメントの取得
const VlStmt*
EiBlockStmt::child_stmt(
  SizeType pos
) const
{
  if ( pos >= child_stmt_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mStmtList[pos];
}

// @brief 子供のステートメントのリストの取得
std::vector<const VlStmt*>
EiBlockStmt::child_stmt_list() const
{
  return mStmtList;
}


//////////////////////////////////////////////////////////////////////
// クラス EiBegin
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiBegin::type() const
{
  return VpiObjType::Begin;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFork
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiFork::type() const
{
  return VpiObjType::Fork;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief 対応するスコープを返す．
const VlScope*
EiNamedBlockStmt::scope() const
{
  return mScope;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedBegin
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiNamedBegin::type() const
{
  return VpiObjType::NamedBegin;
}


//////////////////////////////////////////////////////////////////////
// クラス EiNamedFork
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiNamedFork::type() const
{
  return VpiObjType::NamedFork;
}

END_NAMESPACE_YM_VERILOG
