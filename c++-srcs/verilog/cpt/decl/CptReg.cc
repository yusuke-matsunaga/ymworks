
/// @file CptReg.cc
/// @brief CptReg の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptReg.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptRegH::type() const
{
  return AstDeclHead::Reg;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
bool
CptRegHV::is_signed() const
{
  return false;
}

// 範囲の取得
const PtRange*
CptRegHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
bool
CptRegHSV::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// reg 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_RegH(
  const FileRegion& file_region,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptRegH));
  return new (p) CptRegH(file_region, item_top);
}

// reg 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_RegH(
  const FileRegion& file_region,
  bool sign,
  const PtRange* range,
  const PtDeclItem* item_top
)
{
  if ( sign ) {
    void* p = mAlloc.get_memory(sizeof(CptRegHSV));
    return new (p) CptRegHSV(file_region, range, item_top);
  }
  void* p = mAlloc.get_memory(sizeof(CptRegHV));
  return new (p) CptRegHV(file_region, range, item_top);
}

END_NAMESPACE_YM_VERILOG
