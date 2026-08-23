
/// @file CptAttrInst.cc
/// @brief CptAttrInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptAttrInst.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptAttrInstList
//////////////////////////////////////////////////////////////////////

// @brief 要素数を返す．
SizeType
CptAttrInstList::attrinst_num() const
{
  return mList.size();
}

// @brief 要素を返す．
const AstAttrInst*
CptAttrInstList::attrinst(
  SizeType index
) const
{
  return mList[index];
}

// @brief 要素のリストを返す．
AstAttrInstVec
CptAttrInstList::attrinst_list() const
{
  return mList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス CptAttrInst
//////////////////////////////////////////////////////////////////////

// @brief ファイル位置を返す．
FileRegion
CptAttrInst::file_region() const
{
  return mFileRegion;
}

// @brief 要素数の取得
SizeType
CptAttrInst::attrspec_num() const
{
  return mAttrSpecList.size();
}

// @brief 要素の取得
const AstAttrSpec*
CptAttrInst::attrspec(
  SizeType index
) const
{
  return mAttrSpecList[index];
}

// @brief 要素のリストの取得
AstAttrSpecVec
CptAttrInst::attrspec_list() const
{
  return mAttrSpecList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// クラス AttrSpec
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptAttrSpec::file_region() const
{
  return mFileRegion;
}

// 名前を取り出す．
const char*
CptAttrSpec::name() const
{
  return mName;
}

// 式を取り出す．nullptr の場合もある．
const AstExpr*
CptAttrSpec::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// @brief AttrInstList の生成
PtAttrInstList*
PtFactory::new_AttrInstList(
  const AstAttrInstVec& ai_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrInstList));
  return new (p) CptAttrInstList(PtAttrInstArray(mAlloc, ai_list));
}

// attribute instance を生成する．
PtAttrInst*
PtFactory::new_AttrInst(
  const FileRegion& file_region,
  const AstAttrSpecVec& as_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(file_region,
			     PtAttrSpecArray(mAlloc, as_list));
}

// attribute spec を生成する．
PtAttrSpec*
PtFactory::new_AttrSpec(
  const FileRegion& file_region,
  const char* name
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrSpec));
  return new (p) CptAttrSpec(file_region, name, nullptr);
}

// attribute spec を生成する．
PtAttrSpec*
PtFactory::new_AttrSpec(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* expr
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrSpec));
  return new (p) CptAttrSpec(file_region, name, expr);
}

END_NAMESPACE_YM_VERILOG
