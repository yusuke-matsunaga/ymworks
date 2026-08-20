
/// @file CptContAssign.cc
/// @brief CptContAssign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptContAssign.h"
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

// @brief continuous assign の要素数の取得
SizeType
CptContAssignH::contassign_num() const
{
  return mList.size();
}

// @brief continuous assign の取得
const AstContAssign*
CptContAssignH::contassign(
  SizeType index
) const
{
  return mList[index];
}

// @brief continuous assign リストの取得
AstContAssignVec
CptContAssignH::contassign_list() const
{
  return mList.to_vector();
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
  const AstStrength* strength,
  const AstDelay* delay,
  const AstContAssignVec& elem_list
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptContAssignH));
      return new (p) CptContAssignH(file_region,
				    PtContAssignArray(mAlloc, elem_list));
    }
    void* p = mAlloc.get_memory(sizeof(CptContAssignHD));
    return new (p) CptContAssignHD(file_region, delay,
				   PtContAssignArray(mAlloc, elem_list));
  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptContAssignHS));
    return new (p) CptContAssignHS(file_region, strength,
				   PtContAssignArray(mAlloc, elem_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptContAssignHSD));
  return new (p) CptContAssignHSD(file_region, strength, delay,
				  PtContAssignArray(mAlloc, elem_list));
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
