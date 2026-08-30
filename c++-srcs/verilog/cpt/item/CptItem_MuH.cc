
/// @file CptItem_MuH.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_MuH.h"
#include "alloc/Alloc.h"
#include "parser/PtMisc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptItem_MuH
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_MuH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptItem_MuH::type() const
{
  return MuInst;
}

// 定義名を返す．
const char*
CptItem_MuH::name() const
{
  return mName;
}

// @brief パラメータ割り当てのリストの取得
AstConnectionList
CptItem_MuH::paramassign_list() const
{
  return AstConnectionList(nullptr);
}

// @brief strength を返す．
const AstStrength*
CptItem_MuH::strength() const
{
  return nullptr;
}

// @brief delay を返す．
const AstDelay*
CptItem_MuH::delay() const
{
  return nullptr;
}

// @brief module/UDP/gate instance リストの取得
AstInstList
CptItem_MuH::inst_list() const
{
  return AstInstList(mInstTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_MuHP
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当てのリストの取得
AstConnectionList
CptItem_MuHP::paramassign_list() const
{
  return AstConnectionList(mParamAssignTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_MuHS
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptItem_MuHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_MuHD
//////////////////////////////////////////////////////////////////////

// delay を返す．
const AstDelay*
CptItem_MuHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス CptItem_MuHSD
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptItem_MuHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const AstDelay*
CptItem_MuHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_MuH));
  return new (p) CptItem_MuH(file_region, def_name, inst_top);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  PtConnection* paramassign_top,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_MuHP));
  return new (p) CptItem_MuHP(file_region, def_name,
			 paramassign_top, inst_top);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const AstDelay* delay,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_MuHD));
  return new (p) CptItem_MuHD(file_region, def_name, delay,
			 inst_top);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const AstStrength* strength,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_MuHS));
  return new (p) CptItem_MuHS(file_region, def_name, strength,
			      inst_top);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const AstStrength* strength,
  const AstDelay* delay,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_MuHSD));
  return new (p) CptItem_MuHSD(file_region, def_name,
			       strength, delay,
			       inst_top);
}

END_NAMESPACE_YM_VERILOG
