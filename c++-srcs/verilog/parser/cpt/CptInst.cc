
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptInst.h"
#include "parser/PtMisc.h"
#include "parser/PtFactory.h"


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
  return mInstList.size();
}

// @brief module/UDP/gate instance リストの取得
const AstInst*
CptGateH::inst(
  SizeType index
) const
{
  return mInstList[index];
}

// @brief module/UDP/gate instance リストの取得
AstInstVec
CptGateH::inst_list() const
{
  return mInstList.to_vector();
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
  return mInstList.size();
}

// @brief module/UDP/gate instance リストの取得
const AstInst*
CptMuH::inst(
  SizeType index
) const
{
  return mInstList[index];
}

// @brief module/UDP/gate instance リストの取得
AstInstVec
CptMuH::inst_list() const
{
  return mInstList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当ての要素数の取得
SizeType
CptMuHP::paramassign_num() const
{
  return mParamAssignList.size();
}

// @brief パラメータ割り当ての取得
const AstConnection*
CptMuHP::paramassign(
  SizeType index
) const
{
  return mParamAssignList[index];
}

// @brief パラメータ割り当てのリストの取得
AstConnectionVec
CptMuHP::paramassign_list() const
{
  return mParamAssignList.to_vector();
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
  return mPortList.size();
}

// @brief ポート接続の取得
const AstConnection*
CptInst::port(
  SizeType index
) const
{
  return mPortList[index];
}

// @brief ポートリストの取得
AstConnectionVec
CptInst::port_list() const
{
  return mPortList.to_vector();
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
  const AstInstVec& inst_list
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptGateH));
      return new (p) CptGateH(file_region, type,
			      PtInstArray(mAlloc, inst_list));
    }
    void* p = mAlloc.get_memory(sizeof(CptGateHD));
    return new (p) CptGateHD(file_region, type, delay,
			     PtInstArray(mAlloc, inst_list));

  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptGateHS));
    return new (p) CptGateHS(file_region, type, strength,
			     PtInstArray(mAlloc, inst_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptGateHSD));
  return new (p) CptGateHSD(file_region, type, strength, delay,
			    PtInstArray(mAlloc, inst_list));
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  const AstStrength* strength,
  const AstDelay* delay,
  const AstInstVec& inst_list
)
{
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptMuH));
      return new (p) CptMuH(file_region, def_name,
			    PtInstArray(mAlloc, inst_list));

    }
    void* p = mAlloc.get_memory(sizeof(CptMuHD));
    return new (p) CptMuHD(file_region, def_name,
			   delay,
			   PtInstArray(mAlloc, inst_list));
  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptMuHS));
    return new (p) CptMuHS(file_region, def_name,
			   strength,
			   PtInstArray(mAlloc, inst_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptMuHSD));
  return new (p) CptMuHSD(file_region, def_name,
			  strength, delay,
			  PtInstArray(mAlloc, inst_list));
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  PtConnectionList* con_list,
  const AstInstVec& inst_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptMuHP));
  return new (p) CptMuHP(file_region, def_name,
			 con_list->to_array(mAlloc),
			 PtInstArray(mAlloc, inst_list));
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  PtConnectionList* con_list
)
{
  if ( name == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInst));
    return new (p) CptInst(file_region,
			   con_list->to_array(mAlloc));
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    return new (p) CptInstN(file_region, name,
			    con_list->to_array(mAlloc));
  }
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range,
			  con_list->to_array(mAlloc));
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1
)
{
  auto con1 = new_OrderedCon(expr1);
  PtConnectionArray con_list(mAlloc, AstConnectionVec{con1});
  if ( name == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInst));
    return new (p) CptInst(file_region, std::move(con_list));
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    return new (p) CptInstN(file_region, name, std::move(con_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, std::move(con_list));
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  PtConnectionArray con_list(mAlloc, AstConnectionVec{con1, con2});
  if ( name == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInst));
    return new (p) CptInst(file_region, std::move(con_list));
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    return new (p) CptInstN(file_region, name, std::move(con_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, std::move(con_list));
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  auto con3 = new_OrderedCon(expr3);
  PtConnectionArray con_list(mAlloc, AstConnectionVec{con1, con2, con3});
  if ( name == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInst));
    return new (p) CptInst(file_region, std::move(con_list));
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    return new (p) CptInstN(file_region, name, std::move(con_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, std::move(con_list));
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3,
  const AstExpr* expr4
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  auto con3 = new_OrderedCon(expr3);
  auto con4 = new_OrderedCon(expr4);
  PtConnectionArray con_list(mAlloc, AstConnectionVec{con1, con2, con3, con4});
  if ( name == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInst));
    return new (p) CptInst(file_region, std::move(con_list));
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptInstN));
    return new (p) CptInstN(file_region, name, std::move(con_list));
  }
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, std::move(con_list));
}

END_NAMESPACE_YM_VERILOG
