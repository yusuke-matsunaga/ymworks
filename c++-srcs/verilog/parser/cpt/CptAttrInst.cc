
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

// attribute instance を生成する．
PtAttrInst*
PtFactory::new_AttrInst(
  const FileRegion& file_region,
  PtAttrSpecList* as_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(file_region,
			     as_list->to_array(mAlloc));
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
