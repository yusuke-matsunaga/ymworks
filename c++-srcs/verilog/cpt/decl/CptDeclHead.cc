
/// @file CptDeclHead.cc
/// @brief CptDeclHead の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptDeclHead.h"
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
const PtRange*
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
const PtStrength*
CptDeclHead::strength() const
{
  return nullptr;
}

// @brief delay の取得
const PtDelay*
CptDeclHead::delay() const
{
  return nullptr;
}

// @brief 要素のリストの先頭を返す．
const PtDeclItem*
CptDeclHead::item_top() const
{
  return mItemTop;
}


//////////////////////////////////////////////////////////////////////
// 変数宣言 ヘッダのクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptVarH::type() const
{
  return AstDeclHead::Var;
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
  return AstDeclHead::Genvar;
}


//////////////////////////////////////////////////////////////////////
// event 宣言のヘッダを表すクラス
//////////////////////////////////////////////////////////////////////

// 宣言要素の型の取得
AstDeclHead::Type
CptEventH::type() const
{
  return AstDeclHead::Event;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

// event 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_EventH(
  const FileRegion& file_region,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptEventH));
  return new (p) CptEventH(file_region, item_top);
}

// genvar 宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_GenvarH(
  const FileRegion& file_region,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenvarH));
  return new (p) CptGenvarH(file_region, item_top);
}

// 変数宣言のヘッダを生成する．
PtDeclHead*
PtFactory::new_VarH(
  const FileRegion& file_region,
  VpiVarType var_type,
  const PtDeclItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptVarH));
  return new (p) CptVarH(file_region, var_type, item_top);
}

END_NAMESPACE_YM_VERILOG
