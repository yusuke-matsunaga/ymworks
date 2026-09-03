
/// @file CptRange.cc
/// @brief CptRange の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptRange.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// @class CptRange CptDecl.h "CptDecl.h"
// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptRange::file_region() const
{
  return mFileRegion;
}

// @brief 範囲の MSB を取り出す．
const PtExpr*
CptRange::left() const
{
  return mMsb;
}

// @brief 範囲の LSB を取り出す．
const PtExpr*
CptRange::right() const
{
  return mLsb;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

PtRange*
PtFactory::new_Range(
  const FileRegion& fr,
  const PtExpr* msb,
  const PtExpr* lsb
)
{
  void* p = mAlloc.get_memory(sizeof(CptRange));
  return new (p) CptRange(fr, msb, lsb);
}

END_NAMESPACE_YM_VERILOG
