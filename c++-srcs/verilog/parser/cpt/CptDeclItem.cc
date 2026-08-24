
/// @file CptDeclItem.cc
/// @brief CptDeclItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptDeclItem.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 宣言要素のベース実装クラス
//////////////////////////////////////////////////////////////////////

// 名前の取得
const char*
CptDeclItemBase::name() const
{
  return mName;
}

// @brief 範囲のリストを返す．
const AstRangeList
CptDeclItemBase::range_list() const
{
  return AstRangeList();
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

// @brief 範囲のリストを返す．
const AstRangeList
CptDeclItemR::range_list() const
{
  return AstRangeList(mRangeTop);
}


//////////////////////////////////////////////////////////////////////
// 初期値をもった宣言要素のクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置の取得
FileRegion
CptDeclItemI::file_region() const
{
  return FileRegion(CptDeclItem::file_region(),
		    mInitValue->file_region());
}

// 初期値の取得
const AstExpr*
CptDeclItemI::init_value() const
{
  return mInitValue;
}


//////////////////////////////////////////////////////////////////////
// その他の宣言関係
//////////////////////////////////////////////////////////////////////

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
  PtRange* range_tail
)
{
  auto range_top = range_tail->reverse();
  void* p = mAlloc.get_memory(sizeof(CptDeclItemR));
  return new (p) CptDeclItemR(file_region, name, range_top);
}

END_NAMESPACE_YM_VERILOG
