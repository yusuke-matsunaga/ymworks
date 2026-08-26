
/// @file CptBlockStmt.cc
/// @brief CptBlockStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptBlockStmt.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStmtBlock
//////////////////////////////////////////////////////////////////////

// @brief 子供のステートメントのリストの取得
AstStmtList
CptStmtBlock::stmt_list() const
{
  return AstStmtList(mStmtTop);
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

// @brief 宣言ヘッダのリストの取得
AstDeclHeadList
CptStmtBlockN::declhead_list() const
{
  return AstDeclHeadList(mDeclHeadTop);
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

// par block を生成する．
PtStmt*
PtFactory::new_ParBlock(
  const FileRegion& file_region,
  PtStmt* stmt_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptParBlock));
  return new (p) CptParBlock(file_region, stmt_top);
}

// par block を生成する．
PtStmt*
PtFactory::new_NamedParBlock(
  const FileRegion& file_region,
  const char* name,
  PtDeclHead* declhead_top,
  PtStmt* stmt_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptParBlockN));
  return new (p) CptParBlockN(file_region, name,
			      declhead_top, stmt_top);
}

// seq block を生成する．
PtStmt*
PtFactory::new_SeqBlock(
  const FileRegion& file_region,
  PtStmt* stmt_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptSeqBlock));
  return new (p) CptSeqBlock(file_region, stmt_top);
}

// seq block を生成する．
PtStmt*
PtFactory::new_NamedSeqBlock(
  const FileRegion& file_region,
  const char* name,
  PtDeclHead* declhead_top,
  PtStmt* stmt_top

)
{
  void* p = mAlloc.get_memory(sizeof(CptSeqBlockN));
  return new (p) CptSeqBlockN(file_region, name,
			      declhead_top, stmt_top);
}

END_NAMESPACE_YM_VERILOG
