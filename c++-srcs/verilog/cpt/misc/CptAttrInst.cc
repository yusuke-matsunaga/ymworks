
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
const PtAttrSpec*
CptAttrInst::attrspec_top() const
{
  return mAttrSpecTop;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// attribute instance を生成する．
PtAttrInst*
PtFactory::new_AttrInst(
  const FileRegion& file_region,
  const PtAttrSpec* as_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptAttrInst));
  return new (p) CptAttrInst(file_region, as_top);
}

END_NAMESPACE_YM_VERILOG
