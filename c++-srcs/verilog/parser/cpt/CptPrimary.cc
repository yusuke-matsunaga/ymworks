
/// @file CptPrimary.cc
/// @brief CptPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptPrimary.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryBase
//////////////////////////////////////////////////////////////////////

// クラスの型を返す．
AstExpr::Type
CptPrimaryBase::type() const
{
  return Primary;
}

// 末尾の名前を取り出す．
const char*
CptPrimaryBase::name() const
{
  return mName;
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
// クラス CptPrimaryI1
//////////////////////////////////////////////////////////////////////

// @brief インデックスリストのサイズの取得
SizeType
CptPrimaryI1::index_num() const
{
  return 1;
}

// @brief インデックスの取得
const AstExpr*
CptPrimaryI1::index(
  SizeType i
) const
{
  if ( i > 0 ) {
    throw std::out_of_range{"index(i): i is out of range"};
  }
  return mIndex;
}

// @brief インデックスリストの取得
AstExprVec
CptPrimaryI1::index_list() const
{
  return {mIndex};
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryI1::is_simple() const
{
  return false;
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryI2
//////////////////////////////////////////////////////////////////////

// @brief インデックスリストのサイズの取得
SizeType
CptPrimaryI2::index_num() const
{
  return mIndexList->size();
}

// @brief インデックスの取得
const AstExpr*
CptPrimaryI2::index(
  SizeType i
) const
{
  return mIndexList->expr(i);
}

// @brief インデックスリストの取得
AstExprVec
CptPrimaryI2::index_list() const
{
  return mIndexList->to_vector();
}

// index_list も range も持たないとき true を返す．
bool
CptPrimaryI2::is_simple() const
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
const AstPart*
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
const AstPart*
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

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptPrimaryH::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHI
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptPrimaryHI::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
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

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptPrimaryHR::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHIR
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptPrimaryHIR::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
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
  const AstExpr* index
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryI1));
  return new (p) CptPrimaryI1(file_region, name, index);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryI2));
  return new (p) CptPrimaryI2(file_region, name, index_list);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const char* name,
  const AstPart* part
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
  const AstExprList* index_list,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryIR));
  return new (p) CptPrimaryIR(file_region, name,
			      index_list, part);
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
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHI));
  return new (p) CptPrimaryHI(file_region, hname, index_list);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  const PtHierName& hname,
  const AstPart* part
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
  const AstExprList* index_list,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHIR));
  return new (p) CptPrimaryHIR(file_region, hname,
			       index_list, part);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* index
)
{
  auto index_list = new_ExprList({index});
  void* p = mAlloc.get_memory(sizeof(CptPrimaryCI));
  return new (p) CptPrimaryCI(file_region, name, index_list);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryCI));
  return new (p) CptPrimaryCI(file_region, name, index_list);
}

// primary を生成する．
PtExpr*
PtFactory::new_CPrimary(
  const FileRegion& file_region,
  const char* name,
  const AstPart* part
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
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHCI));
  return new (p) CptPrimaryHCI(file_region, hname, index_list);
}

END_NAMESPACE_YM_VERILOG
