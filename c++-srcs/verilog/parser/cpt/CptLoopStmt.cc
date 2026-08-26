
/// @file CptLoopStmt.cc
/// @brief CptLoopStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptLoopStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

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
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

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

END_NAMESPACE_YM_VERILOG
