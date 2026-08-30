
/// @file CptDelay.cc
/// @brief CptDelay の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptDelay.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptDelay1
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptDelay1::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value0() const
{
  return mValue;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value1() const
{
  return nullptr;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value2() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay2
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptDelay2::value1() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay3
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptDelay3::value2() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay1));
  return new (p) CptDelay1(file_region, value1);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay2));
  return new (p) CptDelay2(file_region, value1, value2);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay3));
  return new (p) CptDelay3(file_region, value1, value2, value3);
}

END_NAMESPACE_YM_VERILOG
