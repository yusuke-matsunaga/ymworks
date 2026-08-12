
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptInst.h"
#include "parser/PtFactory.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG


//////////////////////////////////////////////////////////////////////
// gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptGateH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptGateH::type() const
{
  return GateInst;
}

// プリミティブタイプ(vpiAndPrim など)を返す．
VpiPrimType
CptGateH::prim_type() const
{
  return mPrimType;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptGateH::inst_num() const
{
  return mInstArray.size();
}

// @brief module/UDP/gate instance の取得
const AstInst*
CptGateH::inst(
  SizeType pos
) const
{
  return mInstArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptGateHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay をもつ gate instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// delay を返す．
const AstDelay*
CptGateHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay をもつ gate instance のヘッダを表すクラス      //
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptGateHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const AstDelay*
CptGateHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptMuH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptMuH::type() const
{
  return MuInst;
}

// 定義名を返す．
const char*
CptMuH::name() const
{
  return mName;
}

// @brief module/UDP/gate instance の要素数の取得
SizeType
CptMuH::inst_num() const
{
  return mInstArray.size();
}

// @brief module/UDP/gate instance の取得
const AstInst*
CptMuH::inst(
  SizeType pos
) const
{
  return mInstArray[pos];
}


//////////////////////////////////////////////////////////////////////
// パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当て数の取得
SizeType
CptMuHP::paramassign_num() const
{
  return mParamAssignArray.size();
}

// @brief パラメータ割り当ての取得
const AstConnection*
CptMuHP::paramassign(
  SizeType pos
) const
{
  return mParamAssignArray[pos];
}


//////////////////////////////////////////////////////////////////////
// strength を持つ MuH
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptMuHS::strength() const
{
  return mStrength;
}


//////////////////////////////////////////////////////////////////////
// delay を持つ MuH
//////////////////////////////////////////////////////////////////////

// delay を返す．
const AstDelay*
CptMuHD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// strength と delay を持つ MuH
//////////////////////////////////////////////////////////////////////

// strength を返す．
const AstStrength*
CptMuHSD::strength() const
{
  return mStrength;
}

// delay を返す．
const AstDelay*
CptMuHSD::delay() const
{
  return mDelay;
}


//////////////////////////////////////////////////////////////////////
// module instance/UDP instance のベース実装クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptInst::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
const char*
CptInst::name() const
{
  return nullptr;
}

// @brief 範囲の取得
const AstRange*
CptInst::range() const
{
  return nullptr;
}

// @brief ポートの要素数の取得
SizeType
CptInst::port_num() const
{
  return mPortArray.size();
}

// @brief ポートの取得
const AstConnection*
CptInst::port(
  SizeType pos
) const
{
  return mPortArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 名前を持つ CptInst
//////////////////////////////////////////////////////////////////////

// 名前を返す．
const char*
CptInstN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// 名前と範囲指定を持つ CptInst
//////////////////////////////////////////////////////////////////////

// 範囲を取出す．
const AstRange*
CptInstR::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// gate instance 文のヘッダを生成する．
PtItem*
PtFactory::new_GateH(
  const FileRegion& file_region,
  VpiPrimType type,
  const AstStrength* strength,
  const AstDelay* delay,
  PtInstArray&& inst_array
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptGateH));
      return new (p) CptGateH(file_region, type,
			      std::move(inst_array));
    }
    void* p = mAlloc.get_memory(sizeof(CptGateHD));
    return new (p) CptGateHD(file_region, type, delay,
			     std::move(inst_array));
  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptGateHS));
    return new (p) CptGateHS(file_region, type, strength,
			     std::move(inst_array));
  }
  void* p = mAlloc.get_memory(sizeof(CptGateHSD));
  return new (p) CptGateHSD(file_region, type, strength, delay,
			    std::move(inst_array));
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const AstStrength* strength,
  const AstDelay* delay,
  PtInstArray&& inst_array
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptMuH));
      return new (p) CptMuH(file_region, def_name,
			    std::move(inst_array));

    }
    void* p = mAlloc.get_memory(sizeof(CptMuHD));
    return new (p) CptMuHD(file_region, def_name, delay,
			   std::move(inst_array));
  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptMuHS));
    return new (p) CptMuHS(file_region, def_name, strength,
			   std::move(inst_array));
  }
  void* p = mAlloc.get_memory(sizeof(CptMuHSD));
  return new (p) CptMuHSD(file_region, def_name, strength, delay,
			  std::move(inst_array));
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  PtConnectionArray&& con_array,
  PtInstArray&& inst_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptMuHP));
  return new (p) CptMuHP(file_region, def_name,
			 std::move(con_array),
			 std::move(inst_array));
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  PtConnectionArray&& con_array
)
{
  if ( name == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInst));
    return new (p) CptInst(file_region,
			   std::move(con_array));
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    return new (p) CptInstN(file_region, name,
			   std::move(con_array));
  }
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range,
			  std::move(con_array));
}

END_NAMESPACE_YM_VERILOG
