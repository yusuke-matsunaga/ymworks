
/// @file CptMisc.cc
/// @brief CptMisc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptMisc.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// ordered_connection/named_connection を表すクラス
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
const AstExpr*
CptConnection::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス NamedCon
//////////////////////////////////////////////////////////////////////

// 名前を取り出す．
const char*
CptNamedCon::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptStrength::file_region() const
{
  return mFileRegion;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength1
//////////////////////////////////////////////////////////////////////

// drive strength0 を返す．
VpiStrength
CptStrength1::drive0() const
{
  return mValue[0];
}

// drive strength1 を返す．
VpiStrength
CptStrength1::drive1() const
{
  return mValue[1];
}

// charge strength を返す．
VpiStrength
CptStrength1::charge() const
{
  return VpiStrength::NoStrength;
}


//////////////////////////////////////////////////////////////////////
// クラス CptStrength2
//////////////////////////////////////////////////////////////////////

// drive strength0 を返す．
VpiStrength
CptStrength2::drive0() const
{
  return VpiStrength::NoStrength;
}

// drive strength1 を返す．
VpiStrength
CptStrength2::drive1() const
{
  return VpiStrength::NoStrength;
}

// charge strength を返す．
VpiStrength
CptStrength2::charge() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay1
//////////////////////////////////////////////////////////////////////

// ファイル位置を取出す．
FileRegion
CptDelay1::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value0() const
{
  return mValue;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value1() const
{
  return nullptr;
}

// 値を取り出す．
const AstExpr*
CptDelay1::value2() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay2
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptDelay2::value1() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDelay3
//////////////////////////////////////////////////////////////////////

// 値を取り出す．
const AstExpr*
CptDelay3::value2() const
{
  return mValue;
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
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptOrderedCon));
  return new (p) CptOrderedCon(expr->file_region(), expr);
}

// 順序つき結合子を生成する．
PtConnection*
PtFactory::new_OrderedCon(
  const FileRegion& file_region,
  const AstExpr* expr
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
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptNamedCon));
  return new (p) CptNamedCon(file_region, name, expr);
}

// drive strength を生成する．
PtStrength*
PtFactory::new_Strength(
  const FileRegion& file_region,
  VpiStrength value1,
  VpiStrength value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptStrength1));
  return new (p) CptStrength1(file_region, value1, value2);
}

// charge strength を生成する．
PtStrength*
PtFactory::new_Strength(
  const FileRegion& file_region,
  VpiStrength value1
)
{
  void* p = mAlloc.get_memory(sizeof(CptStrength2));
  return new (p) CptStrength2(file_region, value1);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay1));
  return new (p) CptDelay1(file_region, value1);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay2));
  return new (p) CptDelay2(file_region, value1, value2);
}

// delay 値を生成する．
PtDelay*
PtFactory::new_Delay(
  const FileRegion& file_region,
  const AstExpr* value1,
  const AstExpr* value2,
  const AstExpr* value3
)
{
  void* p = mAlloc.get_memory(sizeof(CptDelay3));
  return new (p) CptDelay3(file_region, value1, value2, value3);
}

END_NAMESPACE_YM_VERILOG
