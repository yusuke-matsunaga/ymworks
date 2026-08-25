
/// @file CptNet.cc
/// @brief CptNet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptNet.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptNetH::type() const
{
  return Net;
}

// net 型の取得
VpiNetType
CptNetH::net_type() const
{
  return static_cast<VpiNetType>((mFlags >> 1) & 15);
}

// 符号の有無の取得
bool
CptNetH::is_signed() const
{
  return static_cast<bool>(mFlags & 1);
}


//////////////////////////////////////////////////////////////////////
// strength つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// strength の取得
const AstStrength*
CptNetHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// delay の取得
const AstDelay*
CptNetHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// strength の取得
const AstStrength*
CptNetHSD::strength() const
{
  return mStrength;
}

// delay の取得
const AstDelay*
CptNetHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の net宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// vectored|scalared 属性の取得
VpiVsType
CptNetHV::vs_type() const
{
  return static_cast<VpiVsType>(mFlags >> 5);
}

// 範囲の取得
const AstRange*
CptNetHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// strength つきの CptNetHV
//////////////////////////////////////////////////////////////////////

// strength の取得
const AstStrength*
CptNetHVS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay つきの CptNetHV
//////////////////////////////////////////////////////////////////////

// delay の取得
const AstDelay*
CptNetHVD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay つきの CptNetHV
//////////////////////////////////////////////////////////////////////

// strength の取得
const AstStrength*
CptNetHVSD::strength() const
{
  return mStrength;
}

// delay の取得
const AstDelay*
CptNetHVSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetH));
  return new (p) CptNetH(file_region, type, false, item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const AstRange* range,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHV));
  return new (p) CptNetHV(file_region, type, vstype,
			  sign, range, item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  const AstDelay* delay,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHD));
  return new (p) CptNetHD(file_region, type,
			  false, delay, item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const AstRange* range,
  const AstDelay* delay,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHVD));
  return new (p) CptNetHVD(file_region, type, vstype,
			   sign, range, delay, item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  const AstStrength* strength,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHS));
  return new (p) CptNetHS(file_region, type, false,
			  strength, item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  const AstStrength* strength,
  VpiVsType vstype,
  bool sign,
  const AstRange* range,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHVS));
  return new (p) CptNetHVS(file_region, type, vstype,
			   sign, range,
			   strength, item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  const AstStrength* strength,
  const AstDelay* delay,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHSD));
  return new (p) CptNetHSD(file_region, type, false,
			   strength, delay,
			   item_top);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  const AstStrength* strength,
  VpiVsType vstype,
  bool sign,
  const AstRange* range,
  const AstDelay* delay,
  PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptNetHVSD));
  return new (p) CptNetHVSD(file_region, type,
			    vstype, sign, range,
			    strength, delay,
			    item_top);
}

END_NAMESPACE_YM_VERILOG
