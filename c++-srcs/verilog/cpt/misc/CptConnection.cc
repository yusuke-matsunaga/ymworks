
/// @file CptConnection.cc
/// @brief CptConnection の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptConnection.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptConnection
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptConnection::file_region() const
{
  return mFileRegion;
}

// @brief 名前の取得
const char*
CptConnection::name() const
{
  return nullptr;
}

// 式を取出す
const PtExpr*
CptConnection::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptNamedCon
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptNamedCon::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// 順序つき結合子を生成する．
PtConnection*
PtFactory::new_OrderedCon()
{
  void* p = mAlloc.get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon({}, nullptr);
}

// 順序つき結合子を生成する．
PtConnection*
PtFactory::new_OrderedCon(
  const PtExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(expr->file_region(), expr);
}

// 順序つき結合子を生成する．
PtConnection*
PtFactory::new_OrderedCon(
  const FileRegion& file_region,
  const PtExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(file_region, expr);
}

// 名前つき結合子を生成する．
PtConnection*
PtFactory::new_NamedCon(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptNamedCon));
  return new (p) CptNamedCon(file_region, name, nullptr);
}

// 名前つき結合子を生成する．
PtConnection*
PtFactory::new_NamedCon(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptNamedCon));
  return new (p) CptNamedCon(file_region, name, expr);
}

END_NAMESPACE_YM_VERILOG
