
/// @file CptPrimary.cc
/// @brief CptPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptPrimary.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryBase
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptPrimaryBase::type() const
{
  return AstExpr::Primary;
}

// 末尾の名前を取り出す．
const char*
CptPrimaryBase::name() const
{
  return mName;
}

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptPrimaryBase::namebranch_top() const
{
  return nullptr;
}

// @brief 定数インデックスのチェック
bool
CptPrimaryBase::is_const_index() const
{
  return false;
}

// @brief インデックスリストの先頭取得
const PtExpr*
CptPrimaryBase::index_top() const
{
  return nullptr;
}

// @brief 範囲指定を表す構文木を返す．
const PtPart*
CptPrimaryBase::part() const
{
  return nullptr;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryBase::is_simple() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimary
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPrimary::file_region() const
{
  return mFileRegion;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryI
//////////////////////////////////////////////////////////////////////

// @brief インデックスリストの先頭の取得
const PtExpr*
CptPrimaryI::index_top() const
{
  return mIndexTop;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryI::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryCI
//////////////////////////////////////////////////////////////////////

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCI::is_const_index() const
{
  // この関数こそがこのクラスの存在意義
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryR
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPrimaryR::file_region() const
{
  return mFileRegion;
}

// 範囲指定の取得
const PtPart*
CptPrimaryR::part() const
{
  return mPart;
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryR::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryCR
//////////////////////////////////////////////////////////////////////

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryCR::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryIR
//////////////////////////////////////////////////////////////////////

// 範囲指定を取り出す．
const PtPart*
CptPrimaryIR::part() const
{
  return mPart;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryH
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPrimaryH::file_region() const
{
  return mFileRegion;
}

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptPrimaryH::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHI
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptPrimaryHI::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHCI
//////////////////////////////////////////////////////////////////////

// インデックスもしくは範囲が定数にならなければならないとき true を返す．
bool
CptPrimaryHCI::is_const_index() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHR
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptPrimaryHR::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHIR
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストの先頭を返す．
const PtNameBranch*
CptPrimaryHIR::namebranch_top() const
{
  return mNbTop;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimary));
  return new (p) CptPrimary(file_region, name);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* index_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryI));
  return new (p) CptPrimaryI(file_region, name, index_top);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const PtPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryR));
  return new (p) CptPrimaryR(file_region, name, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* index_top,
  const PtPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryIR));
  return new (p) CptPrimaryIR(file_region, name,
			      index_top, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const PtHierName& hname
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryH));
  return new (p) CptPrimaryH(file_region, hname);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const PtHierName& hname,
  const PtExpr* index_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHI));
  return new (p) CptPrimaryHI(file_region, hname, index_top);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const PtHierName& hname,
  const PtPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHR));
  return new (p) CptPrimaryHR(file_region, hname, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const PtHierName& hname,
  const PtExpr* index_top,
  const PtPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHIR));
  return new (p) CptPrimaryHIR(file_region, hname,
			       index_top, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const PtExpr* index_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryCI));
  return new (p) CptPrimaryCI(file_region, name, index_top);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const PtPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryCR));
  return new (p) CptPrimaryCR(file_region, name, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const PtHierName& hname,
  const PtExpr* index_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHCI));
  return new (p) CptPrimaryHCI(file_region, hname, index_top);
}

END_NAMESPACE_YM_VERILOG
