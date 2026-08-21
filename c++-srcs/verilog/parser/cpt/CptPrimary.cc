
/// @file CptPrimary.cc
/// @brief CptPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
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
// クラス CptPrimaryI
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptPrimaryI::file_region() const
{
  return mFileRegion;
}

// @brief インデックスリストのサイズの取得
SizeType
CptPrimaryI::index_num() const
{
  return mIndexList->size();
}

// @brief インデックスの取得
const AstExpr*
CptPrimaryI::index(
  SizeType i
) const
{
  return mIndexList->expr(i);
}

// @brief インデックスリストの取得
AstExprVec
CptPrimaryI::index_list() const
{
  return mIndexList->to_vector();
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

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryH::namebranch_num() const
{
  return mNbList.size();
}

// @brief 階層ブランチを返す．
const AstNameBranch*
CptPrimaryH::namebranch(
  SizeType index
) const
{
  return mNbList[index];
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptPrimaryH::namebranch_list() const
{
  return mNbList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHI
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHI::namebranch_num() const
{
  return mNbList.size();
}

// @brief 階層ブランチを返す．
const AstNameBranch*
CptPrimaryHI::namebranch(
  SizeType index
) const
{
  return mNbList[index];
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptPrimaryHI::namebranch_list() const
{
  return mNbList.to_vector();
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

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHR::namebranch_num() const
{
  return mNbList.size();
}

// @brief 階層ブランチを返す．
const AstNameBranch*
CptPrimaryHR::namebranch(
  SizeType index
) const
{
  return mNbList[index];
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptPrimaryHR::namebranch_list() const
{
  return mNbList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptPrimaryHIR
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチの要素数の取得
SizeType
CptPrimaryHIR::namebranch_num() const
{
  return mNbList.size();
}

// @brief 階層ブランチを返す．
const AstNameBranch*
CptPrimaryHIR::namebranch(
  SizeType index
) const
{
  return mNbList[index];
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptPrimaryHIR::namebranch_list() const
{
  return mNbList.to_vector();
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
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryI));
  return new (p) CptPrimaryI(file_region, name, index_list);
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
  PtHierName* hname
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryH));
  return new (p) CptPrimaryH(file_region,
			     hname->nb_list()->to_array(mAlloc),
			     hname->tail_name());
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname,
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHI));
  return new (p) CptPrimaryHI(file_region,
			      hname->nb_list()->to_array(mAlloc),
			      hname->tail_name(),
			      index_list);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHR));
  return new (p) CptPrimaryHR(file_region,
			      hname->nb_list()->to_array(mAlloc),
			      hname->tail_name(),
			      part);
}

// primary を生成する．
PtExpr*
PtFactory::new_Primary(
  const FileRegion& file_region,
  PtHierName* hname,
  const AstExprList* index_list,
  const AstPart* part
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHIR));
  return new (p) CptPrimaryHIR(file_region,
			       hname->nb_list()->to_array(mAlloc),
			       hname->tail_name(),
			       index_list, part);
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
  return new (p) CptPrimaryCI(file_region, name,
			      index_list);
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
  PtHierName* hname,
  const AstExprList* index_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptPrimaryHCI));
  return new (p) CptPrimaryHCI(file_region,
			       hname->nb_list()->to_array(mAlloc),
			       hname->tail_name(),
			       index_list);
}

END_NAMESPACE_YM_VERILOG
