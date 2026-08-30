
/// @file CptItem_Process.cc
/// @brief CptItem_Process の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_Process.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptItem_Process
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_Process::file_region() const
{
  return FileRegion{mTopLoc, mBody->file_region().end_loc()};
}

// 本体のステートメントを返す．
const AstStmt*
CptItem_Process::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_Initial
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_Initial::type() const
{
  return Initial;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_Always
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_Always::type() const
{
  return Always;
}


//////////////////////////////////////////////////////////////////////
// PtFactory
//////////////////////////////////////////////////////////////////////

// initial 文を生成する．
PtItem*
PtFactory::new_Initial(
  const FileRegion& file_region,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_Initial));
  return new (p) CptItem_Initial(file_region, body);
}

// always 文を生成する．
PtItem*
PtFactory::new_Always(
  const FileRegion& file_region,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_Always));
  return new (p) CptItem_Always(file_region, body);
}

END_NAMESPACE_YM_VERILOG
