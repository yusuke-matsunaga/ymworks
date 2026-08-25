
/// @file CptProcess.cc
/// @brief CptProcess の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptProcess.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptProcess
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptProcess::file_region() const
{
  return FileRegion{mTopLoc, mBody->file_region().end_loc()};
}

// 本体のステートメントを返す．
const AstStmt*
CptProcess::body() const
{
  return mBody;
}


//////////////////////////////////////////////////////////////////////
// クラス CptInitial
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptInitial::type() const
{
  return Initial;
}


//////////////////////////////////////////////////////////////////////
// クラス CptAlways
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptAlways::type() const
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
  void* p = mAlloc.get_memory(sizeof(CptInitial));
  return new (p) CptInitial(file_region, body);
}

// always 文を生成する．
PtItem*
PtFactory::new_Always(
  const FileRegion& file_region,
  const AstStmt* body
)
{
  void* p = mAlloc.get_memory(sizeof(CptAlways));
  return new (p) CptAlways(file_region, body);
}

END_NAMESPACE_YM_VERILOG
