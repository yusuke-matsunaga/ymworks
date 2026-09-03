
/// @file CptConst.cc
/// @brief CptConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptConst.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// PtConstant のベース実装クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptConstant::file_region() const
{
  return mFileRegion;
}

// クラスの型を返す．
AstExpr::Type
CptConstant::type() const
{
  return AstExpr::Const;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数の基底クラス
//////////////////////////////////////////////////////////////////////

// 階層名の添字として使える式の時に true を返す．
bool
CptIntConstant::is_index_expr() const
{
  return true;
}

// @brief インデックスの値の取得
int
CptIntConstant::index_value() const
{
  return const_bitvect().to_int();
}

// 整数型の定数のサイズの取得
SizeType
CptIntConstant::const_size() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズ/基数の指定なし)
//////////////////////////////////////////////////////////////////////

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant1::const_type() const
{
  return VpiConstType::Int;
}

// 整数型の値の取得
BitVector
CptIntConstant1::const_bitvect() const
{
  return BitVector(mValue);
}

// 文字列型の値の取得
const char*
CptIntConstant1::const_str() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(基数のみ指定あり)
//////////////////////////////////////////////////////////////////////

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant2::const_type() const
{
  return mConstType;
}

// 整数型の値の取得
BitVector
CptIntConstant2::const_bitvect() const
{
  switch ( mConstType ) {
  case VpiConstType::Dec:
  case VpiConstType::Int:
    return BitVector(0, false, 10, mValue);
  case VpiConstType::SignedDec:
    return BitVector(0, true,  10, mValue);
  case VpiConstType::Binary:
    return BitVector(0, false,  2, mValue);
  case VpiConstType::SignedBinary:
    return BitVector(0, true,   2, mValue);
  case VpiConstType::Oct:
    return BitVector(0, false,  8, mValue);
  case VpiConstType::SignedOct:
    return BitVector(0, true,   8, mValue);
  case VpiConstType::Hex:
    return BitVector(0, false, 16, mValue);
  case VpiConstType::SignedHex:
    return BitVector(0, true,  16, mValue);
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}

// 文字列型の値の取得
const char*
CptIntConstant2::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 整数型の定数(サイズと基数の指定あり)
//////////////////////////////////////////////////////////////////////

// 定数の種類を表す型(vpiIntConst, vpiBinaryConst など) を返す．
VpiConstType
CptIntConstant3::const_type() const
{
  return mConstType;
}

// 整数型の定数のサイズの取得
SizeType
CptIntConstant3::const_size() const
{
  return mSize;
}

// 整数型の値の取得
BitVector
CptIntConstant3::const_bitvect() const
{
  switch ( mConstType ) {
  case VpiConstType::Dec:
  case VpiConstType::Int:
    return BitVector(mSize, false, 10, mValue);
  case VpiConstType::SignedDec:
    return BitVector(mSize, true,  10, mValue);
  case VpiConstType::Binary:
    return BitVector(mSize, false,  2, mValue);
  case VpiConstType::SignedBinary:
    return BitVector(mSize, true,   2, mValue);
  case VpiConstType::Oct:
    return BitVector(mSize, false,  8, mValue);
  case VpiConstType::SignedOct:
    return BitVector(mSize, true,   8, mValue);
  case VpiConstType::Hex:
    return BitVector(mSize, false, 16, mValue);
  case VpiConstType::SignedHex:
    return BitVector(mSize, true,  16, mValue);
  default: break;
  }
  throw std::logic_error{"Should not be reached"};
}

// 文字列型の値の取得
const char*
CptIntConstant3::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 実数型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// 定数の種類を表す型(vpiRealConst) を返す．
VpiConstType
CptRealConstant::const_type() const
{
  return VpiConstType::Real;
}

// 実数型の値の取得
double
CptRealConstant::const_real() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// 文字列型の定数(中身は PtConstant)
//////////////////////////////////////////////////////////////////////

// 定数の種類を表す型(vpiStringConst) を返す．
VpiConstType
CptStringConstant::const_type() const
{
  return VpiConstType::String;
}

// 値を表示用の文字列の形で得る．
const char*
CptStringConstant::const_str() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  SizeType value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant1));
  return new (p) CptIntConstant1(file_region, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant2));
  return new (p) CptIntConstant2(file_region, VpiConstType::Int, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  VpiConstType const_type,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant2));
  return new (p) CptIntConstant2(file_region, const_type, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_IntConst(
  const FileRegion& file_region,
  SizeType size,
  VpiConstType const_type,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptIntConstant3));
  return new (p) CptIntConstant3(file_region, size, const_type, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_RealConst(
  const FileRegion& file_region,
  double value
)
{
  auto p = mAlloc.get_memory(sizeof(CptRealConstant));
  return new (p) CptRealConstant(file_region, value);
}

// 定数を生成する．
PtExpr*
PtFactory::new_StringConst(
  const FileRegion& file_region,
  const char* value
)
{
  auto p = mAlloc.get_memory(sizeof(CptStringConstant));
  return new (p) CptStringConstant(file_region, value);
}

END_NAMESPACE_YM_VERILOG
