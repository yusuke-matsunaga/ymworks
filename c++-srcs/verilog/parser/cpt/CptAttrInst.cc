
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

// @brief 要素のリストの取得
AstAttrSpecList
CptAttrInst::attrspec_list() const
{
  return AstAttrSpecList(mAttrSpecTop);
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
  PtAttrSpec* as_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(file_region, as_top);
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
