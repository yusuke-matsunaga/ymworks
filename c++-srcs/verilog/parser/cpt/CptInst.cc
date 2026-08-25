
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptInst.h"
#include "alloc/Alloc.h"
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

// @brief module/UDP/gate instance リストの取得
AstInstList
CptGateH::inst_list() const
{
  return AstInstList(mInstTop);
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

// @brief module/UDP/gate instance リストの取得
AstInstList
CptMuH::inst_list() const
{
  return AstInstList(mInstTop);
}


//////////////////////////////////////////////////////////////////////
// パラメータ割り当てつきの CptMuH
//////////////////////////////////////////////////////////////////////

// @brief パラメータ割り当てのリストの取得
AstConnectionList
CptMuHP::paramassign_list() const
{
  return AstConnectionList(mParamAssignTop);
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

// @brief ポートリストの取得
AstConnectionList
CptInst::port_list() const
{
  return AstConnectionList(mPortTop);
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
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptGateH));
  return new (p) CptGateH(file_region, type, inst_top);
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
  void* p = mAlloc.get_memory(sizeof(CptGateHD));
  return new (p) CptGateHD(file_region, type, delay, inst_top);
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
  void* p = mAlloc.get_memory(sizeof(CptGateHS));
  return new (p) CptGateHS(file_region, type, strength, inst_top);
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
  void* p = mAlloc.get_memory(sizeof(CptGateHSD));
  return new (p) CptGateHSD(file_region, type, strength, delay, inst_top);
}

// module instance/UDP instance 文のヘッダを生成する．
PtItem*
PtFactory::new_MuH(
  const FileRegion& file_region,
  const char* def_name,
  PtInst* inst_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptMuH));
  return new (p) CptMuH(file_region, def_name, inst_top);
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
  void* p = mAlloc.get_memory(sizeof(CptMuHP));
  return new (p) CptMuHP(file_region, def_name,
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
  void* p = mAlloc.get_memory(sizeof(CptMuHD));
  return new (p) CptMuHD(file_region, def_name, delay,
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
  void* p = mAlloc.get_memory(sizeof(CptMuHS));
  return new (p) CptMuHS(file_region, def_name, strength,
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
  void* p = mAlloc.get_memory(sizeof(CptMuHSD));
  return new (p) CptMuHSD(file_region, def_name,
			  strength, delay,
			  inst_top);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  PtConnection* con_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptInst));
  return new (p) CptInst(file_region, con_top);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  PtConnection* con_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptInstN));
  return new (p) CptInstN(file_region, name, con_top);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstRange* range,
  PtConnection* con_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, con_top);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const AstExpr* expr1
)
{
  auto con1 = new_OrderedCon(expr1);
  void* p = mAlloc.get_memory(sizeof(CptInst));
  return new (p) CptInst(file_region, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* expr1
)
{
  auto con1 = new_OrderedCon(expr1);
  void* p = mAlloc.get_memory(sizeof(CptInstN));
  return new (p) CptInstN(file_region, name, con1);
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
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  con1->set_link(con2);
   void* p = mAlloc.get_memory(sizeof(CptInst));
   return new (p) CptInst(file_region, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* expr1,
  const AstExpr* expr2
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  con1->set_link(con2);
  void* p = mAlloc.get_memory(sizeof(CptInstN));
  return new (p) CptInstN(file_region, name, con1);
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
  con1->set_link(con2);
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  auto con3 = new_OrderedCon(expr3);
  con1->set_link(con2);
  con2->set_link(con3);
  void* p = mAlloc.get_memory(sizeof(CptInst));
  return new (p) CptInst(file_region, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* expr1,
  const AstExpr* expr2,
  const AstExpr* expr3
)
{
  auto con1 = new_OrderedCon(expr1);
  auto con2 = new_OrderedCon(expr2);
  auto con3 = new_OrderedCon(expr3);
  con1->set_link(con2);
  con2->set_link(con3);
  void* p = mAlloc.get_memory(sizeof(CptInstN));
  return new (p) CptInstN(file_region, name, con1);
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
  con1->set_link(con2);
  con2->set_link(con3);
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
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
  con1->set_link(con2);
  con2->set_link(con3);
  con3->set_link(con4);
  void* p = mAlloc.get_memory(sizeof(CptInst));
  return new (p) CptInst(file_region, con1);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const char* name,
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
  con1->set_link(con2);
  con2->set_link(con3);
  con3->set_link(con4);
  void* p = mAlloc.get_memory(sizeof(CptInstN));
  return new (p) CptInstN(file_region, name, con1);
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
  con1->set_link(con2);
  con2->set_link(con3);
  con3->set_link(con4);
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, con1);
}

END_NAMESPACE_YM_VERILOG
