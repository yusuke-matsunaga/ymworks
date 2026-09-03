
/// @file CptParam.cc
/// @brief CptParam の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptParam.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// parameter 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptParamH::type() const
{
  return AstDeclHead::Param;
}


//////////////////////////////////////////////////////////////////////
/// localparam 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptLocalParamH::type() const
{
  return AstDeclHead::LocalParam;
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
const PtRange*
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
  return AstDeclHead::LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 符号付き範囲つき(ビットベクタタイプ)の localparam ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptLocalParamHSV::type() const
{
  return AstDeclHead::LocalParam;
}


//////////////////////////////////////////////////////////////////////
// 組み込み型の parameter ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptParamHT::type() const
{
  return AstDeclHead::Param;
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
  return AstDeclHead::LocalParam;
}


//////////////////////////////////////////////////////////////////////
// specparam 宣言のヘッダのベース実装クラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptSpecParamH::type() const
{
  return AstDeclHead::SpecParam;
}


//////////////////////////////////////////////////////////////////////
// ビットベクタ型の specparam
//////////////////////////////////////////////////////////////////////

// 範囲の取得
const PtRange*
CptSpecParamHV::range() const
{
  return mRange;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// 1ビットのパラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_ParamH(
  const FileRegion& file_region,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptParamH));
  return new (p) CptParamH(file_region, item_top);
}

// 範囲指定型パラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_ParamH(
  const FileRegion& file_region,
  bool sign,
  const PtRange* range,
  const PtDeclItem* item_top
)
{
  if ( sign ) {
    void* p = mAlloc.get_memory(sizeof(CptParamHSV));
    return new (p) CptParamHSV(file_region, range, item_top);
  }
  void* p = mAlloc.get_memory(sizeof(CptParamHV));
  return new (p) CptParamHV(file_region, range, item_top);
}

// 1ビットのローカルパラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_LocalParamH(
  const FileRegion& file_region,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptLocalParamH));
  return new (p) CptLocalParamH(file_region, item_top);
}

// 範囲指定型ローカルパラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_LocalParamH(
  const FileRegion& file_region,
  bool sign,
  const PtRange* range,
  const PtDeclItem* item_top
)
{
  if ( sign ) {
    void* p = mAlloc.get_memory(sizeof(CptLocalParamHSV));
    return new (p) CptLocalParamHSV(file_region, range, item_top);
  }
  void* p = mAlloc.get_memory(sizeof(CptLocalParamHV));
  return new (p) CptLocalParamHV(file_region, range, item_top);
}

// 組み込み型パラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_ParamH(
  const FileRegion& file_region,
  VpiVarType var_type,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptParamHT));
  return new (p) CptParamHT(file_region, var_type, item_top);
}

// 組み込み型ローカルパラメータ宣言のヘッダの生成
PtDeclHead*
PtFactory::new_LocalParamH(
  const FileRegion& file_region,
  VpiVarType var_type,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptLocalParamHT));
  return new (p) CptLocalParamHT(file_region, var_type, item_top);
}

// 1ビットの specparam 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_SpecParamH(
  const FileRegion& file_region,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptSpecParamH));
  return new (p) CptSpecParamH(file_region, item_top);
}

// specparam 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_SpecParamH(
  const FileRegion& file_region,
  const PtRange* range,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptSpecParamHV));
  return new (p) CptSpecParamHV(file_region, range, item_top);
}

END_NAMESPACE_YM_VERILOG
