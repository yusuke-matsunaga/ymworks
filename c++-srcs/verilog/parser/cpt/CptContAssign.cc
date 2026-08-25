
/// @file CptContAssign.cc
/// @brief CptContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptContAssign.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptContAssignH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptContAssignH::type() const
{
  return ContAssign;
}

// @brief strength の取得
const AstStrength*
CptContAssignH::strength() const
{
  return nullptr;
}

// @brief delay の取得
const AstDelay*
CptContAssignH::delay() const
{
  return nullptr;
}

// @brief continuous assign リストの取得
AstContAssignList
CptContAssignH::contassign_list() const
{
  return AstContAssignList(mTop);
}


//////////////////////////////////////////////////////////////////////
// strength つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptContAssignHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// delay を返す．
const AstDelay*
CptContAssignHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの continuous assign 文のヘッダ
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptContAssignHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const AstDelay*
CptContAssignHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// continuous assign文 のベース実装クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を取り出す．
FileRegion
CptContAssign::file_region() const
{
  return FileRegion{lhs()->file_region(), rhs()->file_region()};
}

// 左辺式を取り出す．
const AstExpr*
CptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式を取り出す．
const AstExpr*
CptContAssign::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// PtFactory
//////////////////////////////////////////////////////////////////////

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptContAssignH));
  return new (p) CptContAssignH(file_region, elem_top);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const AstDelay* delay,
  PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptContAssignHD));
  return new (p) CptContAssignHD(file_region, delay, elem_top);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const AstStrength* strength,
  PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptContAssignHS));
  return new (p) CptContAssignHS(file_region, strength, elem_top);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const AstStrength* strength,
  const AstDelay* delay,
  PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptContAssignHSD));
  return new (p) CptContAssignHSD(file_region, strength, delay, elem_top);
}

// continuous assign 文の要素を生成する．
PtContAssign*
PtFactory::new_ContAssign(
  const FileRegion& file_region,
  const AstExpr* lhs,
  const AstExpr* rhs
)
{
  // 実は file_region は不要
  void* p = mAlloc.get_memory(sizeof(CptContAssign));
  return new (p) CptContAssign(lhs, rhs);
}

END_NAMESPACE_YM_VERILOG
