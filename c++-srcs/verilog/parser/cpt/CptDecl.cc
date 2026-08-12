
/// @file CptDecl.cc
/// @brief CptDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptDecl.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 宣言要素のヘッダの基底クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptDeclHead::file_region() const
{
  return mFileRegion;
}

// @brief 符号の取得
bool
CptDeclHead::is_signed() const
{
  return false;
}

// @brief 範囲の取得
const AstRange*
CptDeclHead::range() const
{
  return nullptr;
}

// @brief データ型の取得
VpiVarType
CptDeclHead::data_type() const
{
  return VpiVarType::None;
}

// @brief net 型の取得
VpiNetType
CptDeclHead::net_type() const
{
  return VpiNetType::None;
}

// @brief vectored|scalared 属性の取得
VpiVsType
CptDeclHead::vs_type() const
{
  return VpiVsType::None;
}

// @brief strength の取得
const AstStrength*
CptDeclHead::strength() const
{
  return nullptr;
}

// @brief delay の取得
const AstDelay*
CptDeclHead::delay() const
{
  return nullptr;
}

// @brief 要素数の取得
SizeType
CptDeclHead::item_num() const
{
  return mItemArray.size();
}

// @brief 要素の取得
const AstDeclItem*
CptDeclHead::item(
  SizeType pos
) const
{
  if ( pos >= item_num() ) {
    throw std::out_of_range{"pos is out of range"};
  }
  return mItemArray[pos];
}

// @brief 要素リストの設定
void
CptDeclHead::set_elem(
  PtDeclItemArray&& elem_array
)
{
  mItemArray = std::move(elem_array);
}


//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptParamH::type() const
{
  return Param;
}


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptLocalParamH::type() const
{
  return LocalParam;
}


//////////////////////////////////////////////////////////////////////
/// 符号なし範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
bool
CptParamHV::is_signed() const
{
  // このクラスは常に false を返す．
  return false;
}

// 範囲の取得
const AstRange*
CptParamHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
/// 符号付き範囲つき(ビットベクタタイプ)の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
bool
CptParamHSV::is_signed() const
{
  // このクラスは常に true を返す．
  return true;
}


//////////////////////////////////////////////////////////////////////
// 符号なし範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptLocalParamHV::type() const
{
  return LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptLocalParamHSV::type() const
{
  return LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptParamHT::type() const
{
  return Param;
}

// @brief 符号の取得
bool
CptParamHT::is_signed() const
{
  switch ( data_type() ) {
  case VpiVarType::Integer:
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return true;
  default:
    return false;
  }
}

// データ型の取得
VpiVarType
CptParamHT::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptLocalParamHT::type() const
{
  return LocalParam;
}


//////////////////////////////////////////////////////////////////////
// reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptRegH::type() const
{
  return Reg;
}


//////////////////////////////////////////////////////////////////////
/// 1ビット符号つき？？？ の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
// このクラスでは true を返す．
bool
CptRegHS::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
bool
CptRegHV::is_signed() const
{
  return false;
}

// 範囲の取得
const AstRange*
CptRegHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// 符号つきビットベクタ型の reg 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 符号の取得
bool
CptRegHSV::is_signed() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// 変数宣言 ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptVarH::type() const
{
  return Var;
}

// @brief 符号の取得
bool
CptVarH::is_signed() const
{
  switch ( data_type() ) {
  case VpiVarType::Integer:
  case VpiVarType::Real:
  case VpiVarType::Realtime:
    return true;
  default:
    return false;
  }
}

// データ型の取得
VpiVarType
CptVarH::data_type() const
{
  return mVarType;
}


//////////////////////////////////////////////////////////////////////
// genvar宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptGenvarH::type() const
{
  return Genvar;
}


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
// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptEventH::type() const
{
  return Event;
}


//////////////////////////////////////////////////////////////////////
// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptSpecParamH::type() const
{
  return SpecParam;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////

// 範囲の取得
const AstRange*
CptSpecParamHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// 名前の取得
const char*
CptDeclItemBase::name() const
{
  return mName;
}

// @brief 範囲リストのサイズの取得
SizeType
CptDeclItemBase::range_num() const
{
  return 0;
}

// @brief 範囲の取得
const AstRange*
CptDeclItemBase::range(
  SizeType pos
) const
{
  throw std::out_of_range{"pos is out of range"};
}

// 初期値の取得
const AstExpr*
CptDeclItemBase::init_value() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptDeclItem::file_region() const
{
  return mLoc;
}


//////////////////////////////////////////////////////////////////////
// 範囲をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptDeclItemR::file_region() const
{
  return mFileRegion;
}

// @brief 範囲リストのサイズの取得
SizeType
CptDeclItemR::range_num() const
{
  return mRangeArray.size();
}

// @brief 範囲の取得
const AstRange*
CptDeclItemR::range(
  SizeType pos
) const
{
  return mRangeArray[pos];
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptDeclItemI::file_region() const
{
  return FileRegion{CptDeclItem::file_region(), mInitValue->file_region()};
}

// 初期値の取得
const AstExpr*
CptDeclItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// @class CptRange CptDecl.h "CptDecl.h"
// @brief 範囲を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptRange::file_region() const
{
  return mFileRegion;
}

// @brief 範囲の MSB を取り出す．
const AstExpr*
CptRange::left() const
{
  return mMsb;
}

// @brief 範囲の LSB を取り出す．
const AstExpr*
CptRange::right() const
{
  return mLsb;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// 範囲指定型パラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_ParamH(
  const FileRegion& file_region,
  bool sign,
  const AstRange* range,
  bool local
)
{
  if ( local ) {
    if ( range == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptLocalParamH));
      return new (p) CptLocalParamH(file_region);
    }
    if ( sign ) {
      void* p = mAlloc.get_memory(sizeof(CptLocalParamHSV));
      return new (p) CptLocalParamHSV(file_region, range);
    }
    void* p = mAlloc.get_memory(sizeof(CptLocalParamHV));
    return new (p) CptLocalParamHV(file_region, range);
  }
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptParamH));
    return new (p) CptParamH(file_region);
  }
  if ( sign ) {
    void* p = mAlloc.get_memory(sizeof(CptParamHSV));
    return new (p) CptParamHSV(file_region, range);
  }
  void* p = mAlloc.get_memory(sizeof(CptParamHV));
  return new (p) CptParamHV(file_region, range);
}

// 組み込み型パラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_ParamH(
  const FileRegion& file_region,
  VpiVarType var_type,
  bool local
)
{
  if ( local ) {
    void* p = mAlloc.get_memory(sizeof(CptLocalParamHT));
    return new (p) CptLocalParamHT(file_region, var_type);
  }
  void* p = mAlloc.get_memory(sizeof(CptParamHT));
  return new (p) CptParamHT(file_region, var_type);
}

// specparam 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_SpecParamH(
  const FileRegion& file_region,
  const AstRange* range
)
{
  if ( range == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptSpecParamH));
    return new (p) CptSpecParamH(file_region);
  }
  void* p = mAlloc.get_memory(sizeof(CptSpecParamHV));
  return new (p) CptSpecParamHV(file_region, range);
}

// event 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_EventH(
  const FileRegion& file_region
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventH));
  return new (p) CptEventH(file_region);
}

// genvar 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_GenvarH(
  const FileRegion& file_region
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenvarH));
  return new (p) CptGenvarH(file_region);
}

// 変数宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_VarH(
  const FileRegion& file_region,
  VpiVarType var_type
)
{
  void* p = mAlloc.get_memory(sizeof(CptVarH));
  return new (p) CptVarH(file_region, var_type);
}

// reg 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_RegH(
  const FileRegion& file_region,
  bool sign,
  const AstRange* range
)
{
  if ( range == nullptr ) {
    if ( sign ) {
      void* p = mAlloc.get_memory(sizeof(CptRegHS));
      return new (p) CptRegHS(file_region);
    }
    void* p = mAlloc.get_memory(sizeof(CptRegH));
    return new (p) CptRegH(file_region);
  }
  if ( sign ) {
    void* p = mAlloc.get_memory(sizeof(CptRegHSV));
    return new (p) CptRegHSV(file_region, range);
  }
  void* p = mAlloc.get_memory(sizeof(CptRegHV));
  return new (p) CptRegHV(file_region, range);
}

// net 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_NetH(
  const FileRegion& file_region,
  VpiNetType type,
  VpiVsType vstype,
  bool sign,
  const AstRange* range,
  const AstStrength* strength,
  const AstDelay* delay
)
{
  if ( range == nullptr ) {
    if ( strength == nullptr ) {
      if ( delay == nullptr ) {
	void* p = mAlloc.get_memory(sizeof(CptNetH));
	return new (p) CptNetH(file_region, type, sign);
      }
      void* p = mAlloc.get_memory(sizeof(CptNetHD));
      return new (p) CptNetHD(file_region, type, sign, delay);
    }
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptNetHS));
      return new (p) CptNetHS(file_region, type, sign, strength);
    }
    void* p = mAlloc.get_memory(sizeof(CptNetHSD));
    return new (p) CptNetHSD(file_region, type, sign,
			     strength, delay);
  }
  if ( strength == nullptr ) {
    if ( delay == nullptr ) {
      void* p = mAlloc.get_memory(sizeof(CptNetHV));
      return new (p) CptNetHV(file_region, type, vstype,
			      sign, range);
    }
    void* p = mAlloc.get_memory(sizeof(CptNetHVD));
    return new (p) CptNetHVD(file_region, type, vstype,
			     sign, range, delay);
  }
  if ( delay == nullptr ) {
    void* p = mAlloc.get_memory(sizeof(CptNetHVS));
    return new (p) CptNetHVS(file_region, type, vstype,
			     sign, range, strength);
  }
  void* p = mAlloc.get_memory(sizeof(CptNetHVSD));
  return new (p) CptNetHVSD(file_region, type, vstype,
			    sign, range, strength, delay);
}

// 宣言要素を生成する．
PtDeclItem*
PtFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptDeclItem));
  return new (p) CptDeclItem(file_region, name);
}

PtDeclItem*
PtFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* init_value
)
{
  void* p = mAlloc.get_memory(sizeof(CptDeclItemI));
  return new (p) CptDeclItemI(file_region, name, init_value);
}

PtDeclItem*
PtFactory::new_DeclItem(
  const FileRegion& file_region,
  const char* name,
  PtRangeArray&& range_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptDeclItemR));
  return new (p) CptDeclItemR(file_region, name,
			      std::move(range_array));
}

PtRange*
PtFactory::new_Range(
  const FileRegion& fr,
  const AstExpr* msb,
  const AstExpr* lsb
)
{
  void* p = mAlloc.get_memory(sizeof(CptRange));
  return new (p) CptRange(fr, msb, lsb);
}

END_NAMESPACE_YM_VERILOG
