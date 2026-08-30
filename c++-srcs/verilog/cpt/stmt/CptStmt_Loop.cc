
/// @file CptStmt_Loop.cc
/// @brief CptStmt_Loop の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt_Loop.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Loop
//////////////////////////////////////////////////////////////////////

// 実行すべき本体を返す．
const AstStmt*
CptStmt_Loop::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Forever
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Forever::type() const
{
  return Forever;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Repeat
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_Repeat::type() const
{
  return Repeat;
}

// 繰り返し式を返す．
const AstExpr*
CptStmt_Repeat::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_While
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_While::type() const
{
  return While;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_For
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_For::type() const
{
  return For;
}

// 初期化代入式を取出す
const AstStmt*
CptStmt_For::init_stmt() const
{
  return mInit;
}

// 繰り返し代入式を取出す
const AstStmt*
CptStmt_For::next_stmt() const
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
  void* p = mAlloc.get_memory(sizeof(CptStmt_Forever));
  return new (p) CptStmt_Forever(file_region, body);
}

// repeat 文を生成する．
PtStmt*
PtFactory::new_Repeat(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_Repeat));
  return new (p) CptStmt_Repeat(file_region, expr, body);
}

// while 文を生成する．
PtStmt*
PtFactory::new_While(
  const FileRegion& file_region,
  const AstExpr* cond,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_While));
  return new (p) CptStmt_While(file_region, cond, body);
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
  void* p = mAlloc.get_memory(sizeof(CptStmt_For));
  return new (p) CptStmt_For(file_region, init, cond, next, body);
}

END_NAMESPACE_YM_VERILOG
