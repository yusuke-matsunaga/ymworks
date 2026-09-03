
/// @file CptStmt_Block.cc
/// @brief CptStmt_Block の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt_Block.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptStmt_Block
//////////////////////////////////////////////////////////////////////

// @brief 子供のステートメントのリストの取得
const PtStmt*
CptStmt_Block::stmt_top() const
{
  return mStmtTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_BlockN
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptStmt_BlockN::name() const
{
  return mName;
}

// @brief 宣言ヘッダのリストの取得
const PtDeclHead*
CptStmt_BlockN::declhead_top() const
{
  return mDeclHeadTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_ParBlock
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_ParBlock::type() const
{
  return AstStmt::ParBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_ParBlockN
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_ParBlockN::type() const
{
  return AstStmt::NamedParBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_SeqBlock
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_SeqBlock::type() const
{
  return AstStmt::SeqBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStmt_SeqBlockN
//////////////////////////////////////////////////////////////////////

// クラスの型を返す仮想関数
AstStmt::Type
CptStmt_SeqBlockN::type() const
{
  return AstStmt::NamedSeqBlock;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// par block を生成する．
PtStmt*
PtFactory::new_ParBlock(
  const FileRegion& file_region,
  const PtStmt* stmt_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_ParBlock));
  return new (p) CptStmt_ParBlock(file_region, stmt_top);
}

// par block を生成する．
PtStmt*
PtFactory::new_NamedParBlock(
  const FileRegion& file_region,
  const char* name,
  const PtDeclHead* declhead_top,
  const PtStmt* stmt_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_ParBlockN));
  return new (p) CptStmt_ParBlockN(file_region, name,
			      declhead_top, stmt_top);
}

// seq block を生成する．
PtStmt*
PtFactory::new_SeqBlock(
  const FileRegion& file_region,
  const PtStmt* stmt_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_SeqBlock));
  return new (p) CptStmt_SeqBlock(file_region, stmt_top);
}

// seq block を生成する．
PtStmt*
PtFactory::new_NamedSeqBlock(
  const FileRegion& file_region,
  const char* name,
  const PtDeclHead* declhead_top,
  const PtStmt* stmt_top

)
{
  void* p = mAlloc.get_memory(sizeof(CptStmt_SeqBlockN));
  return new (p) CptStmt_SeqBlockN(file_region, name,
				   declhead_top, stmt_top);
}

END_NAMESPACE_YM_VERILOG
