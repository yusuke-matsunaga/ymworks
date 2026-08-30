
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_GateH.h"
#include "alloc/Alloc.h"
#include "parser/PtMisc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// クラス CptItem_GateH
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_GateH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptItem_GateH::type() const
{
  return GateInst;
}

// @brief strength の取得
const AstStrength*
CptItem_GateH::strength() const
{
  return nullptr;
}

// @brief delay の取得
const AstDelay*
CptItem_GateH::delay() const
{
  return nullptr;
}

// プリミティブタイプ(vpiAndPrim など)を返す．
VpiPrimType
CptItem_GateH::prim_type() const
{
  return mPrimType;
}

// @brief module/UDP/gate instance リストの取得
AstInstList
CptItem_GateH::inst_list() const
{
  return AstInstList(mInstTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_GateHS
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptItem_GateHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_GateHD
//////////////////////////////////////////////////////////////////////

// delay を返す．
const AstDelay*
CptItem_GateHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_GateHSD
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptItem_GateHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const AstDelay*
CptItem_GateHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// gate instance 文のヘッダを生成する．
PtItem*
PtFactory::new_GateH(
  const FileRegion& file_region,
  VpiPrimType type,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GateH));
  return new (p) CptItem_GateH(file_region, type, inst_top);
}

// gate instance 文のヘッダを生成する．
PtItem*
PtFactory::new_GateH(
  const FileRegion& file_region,
  VpiPrimType type,
  const AstDelay* delay,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GateHD));
  return new (p) CptItem_GateHD(file_region, type, delay, inst_top);
}

// gate instance 文のヘッダを生成する．
PtItem*
PtFactory::new_GateH(
  const FileRegion& file_region,
  VpiPrimType type,
  const AstStrength* strength,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GateHS));
  return new (p) CptItem_GateHS(file_region, type, strength, inst_top);
}

// gate instance 文のヘッダを生成する．
PtItem*
PtFactory::new_GateH(
  const FileRegion& file_region,
  VpiPrimType type,
  const AstStrength* strength,
  const AstDelay* delay,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GateHSD));
  return new (p) CptItem_GateHSD(file_region, type, strength, delay, inst_top);
}

END_NAMESPACE_YM_VERILOG
