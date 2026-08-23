
/// @file CptPart.cc
/// @brief CptPart の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptPart.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptPart
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPart::file_region() const
{
  return mFileRegion;
}

// @brief 1番目の式を取り出す．
const AstExpr*
CptPart::left() const
{
  return mLeft;
}

// @brief 2番めの式を取り出す．
const AstExpr*
CptPart::right() const
{
  return mRight;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPartC
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
CptPartC::mode() const
{
  return VpiRangeMode::Const;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPartP
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
CptPartP::mode() const
{
  return VpiRangeMode::Plus;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPartM
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定のモードを返す．
VpiRangeMode
CptPartM::mode() const
{
  return VpiRangeMode::Minus;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// @brief 範囲指定の生成
PtPart*
PtFactory::new_PartConst(
  const FileRegion& fr,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto p = mAlloc.get_memory(sizeof(CptPartC));
  return new (p) CptPartC(fr, expr1, expr2);
}

// @brief 範囲指定の生成
PtPart*
PtFactory::new_PartPlus(
  const FileRegion& fr,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto p = mAlloc.get_memory(sizeof(CptPartP));
  return new (p) CptPartP(fr, expr1, expr2);
}

// @brief 範囲指定の生成
PtPart*
PtFactory::new_PartMinus(
  const FileRegion& fr,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto p = mAlloc.get_memory(sizeof(CptPartM));
  return new (p) CptPartM(fr, expr1, expr2);
}

END_NAMESPACE_YM_VERILOG
