
/// @file CptItem_ContAssignH.cc
/// @brief CptItem_ContAssignH の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_ContAssignH.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptItem_ContAssignH
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_ContAssignH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptItem_ContAssignH::type() const
{
  return AstItem::ContAssign;
}

// @brief strength の取得
const PtStrength*
CptItem_ContAssignH::strength() const
{
  return nullptr;
}

// @brief delay の取得
const PtDelay*
CptItem_ContAssignH::delay() const
{
  return nullptr;
}

// @brief continuous assign リストの取得
const PtContAssign*
CptItem_ContAssignH::contassign_top() const
{
  return mTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_ContAssignHS
//////////////////////////////////////////////////////////////////////

// strength を返す．
const PtStrength*
CptItem_ContAssignHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_ContAssignHD
//////////////////////////////////////////////////////////////////////

// delay を返す．
const PtDelay*
CptItem_ContAssignHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_ContAssignHSD
//////////////////////////////////////////////////////////////////////

// strength を返す．
const PtStrength*
CptItem_ContAssignHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const PtDelay*
CptItem_ContAssignHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス CptContAssign
//////////////////////////////////////////////////////////////////////

// ファイル位置を取り出す．
FileRegion
CptContAssign::file_region() const
{
  return FileRegion{lhs()->file_region(), rhs()->file_region()};
}

// 左辺式を取り出す．
const PtExpr*
CptContAssign::lhs() const
{
  return mLhs;
}

// 右辺式を取り出す．
const PtExpr*
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
  const PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_ContAssignH));
  return new (p) CptItem_ContAssignH(file_region, elem_top);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const PtDelay* delay,
  const PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_ContAssignHD));
  return new (p) CptItem_ContAssignHD(file_region, delay, elem_top);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const PtStrength* strength,
  const PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_ContAssignHS));
  return new (p) CptItem_ContAssignHS(file_region, strength, elem_top);
}

// continuous assign 文のヘッダを生成する．
PtItem*
PtFactory::new_ContAssignH(
  const FileRegion& file_region,
  const PtStrength* strength,
  const PtDelay* delay,
  const PtContAssign* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_ContAssignHSD));
  return new (p) CptItem_ContAssignHSD(file_region, strength, delay, elem_top);
}

// continuous assign 文の要素を生成する．
PtContAssign*
PtFactory::new_ContAssign(
  const FileRegion& file_region,
  const PtExpr* lhs,
  const PtExpr* rhs
)
{
  // 実は file_region は不要
  void* p = mAlloc.get_memory(sizeof(CptContAssign));
  return new (p) CptContAssign(lhs, rhs);
}

END_NAMESPACE_YM_VERILOG
