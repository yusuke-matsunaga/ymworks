
/// @file CptInst.cc
/// @brief インスタンス系オブジェクトの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptInst.h"
#include "alloc/Alloc.h"
#include "parser/PtConnection.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptInst
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
const PtRange*
CptInst::range() const
{
  return nullptr;
}

// @brief ポートリストの取得
const PtConnection*
CptInst::port_top() const
{
  return mPortTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptInstN
//////////////////////////////////////////////////////////////////////

// 名前を返す．
const char*
CptInstN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス CptInstR
//////////////////////////////////////////////////////////////////////

// 範囲を取出す．
const PtRange*
CptInstR::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const PtConnection* con_top
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
  const PtConnection* con_top
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
  const PtRange* range,
  const PtConnection* con_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptInstR));
  return new (p) CptInstR(file_region, name, range, con_top);
}

// module instance/UDP instance の要素を生成する．
PtInst*
PtFactory::new_Inst(
  const FileRegion& file_region,
  const PtExpr* expr1
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
  const PtExpr* expr1
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
  const PtRange* range,
  const PtExpr* expr1
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
  const PtExpr* expr1,
  const PtExpr* expr2
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
  const PtExpr* expr1,
  const PtExpr* expr2
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
  const PtRange* range,
  const PtExpr* expr1,
  const PtExpr* expr2
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
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
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
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
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
  const PtRange* range,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3
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
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
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
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
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
  const PtRange* range,
  const PtExpr* expr1,
  const PtExpr* expr2,
  const PtExpr* expr3,
  const PtExpr* expr4
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
