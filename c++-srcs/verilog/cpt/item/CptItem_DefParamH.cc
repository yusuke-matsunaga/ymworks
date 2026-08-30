
/// @file CptItem_DefParamH.cc
/// @brief CptItem_DefParamH の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_DefParamH.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptItem_DefParamH
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_DefParamH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptItem_DefParamH::type() const
{
  return DefParam;
}

// @brief defparam リストの取得
AstDefParamList
CptItem_DefParamH::defparam_list() const
{
  return AstDefParamList(mTop);
}


//////////////////////////////////////////////////////////////////////
// クラス CptDefparam
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptDefParam::file_region() const
{
  return FileRegion{mTopLoc, mExpr->file_region().end_loc()};
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptDefParam::namebranch_list() const
{
  return AstNameBranchList();
}

// 末尾の名前を返す．
const char*
CptDefParam::name() const
{
  return mName;
}

// 値を返す．
const AstExpr*
CptDefParam::expr() const
{
  return mExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス CptDefparam2
//////////////////////////////////////////////////////////////////////

// @brief 階層ブランチのリストを返す．
AstNameBranchList
CptDefParam2::namebranch_list() const
{
  return AstNameBranchList(mNbTop);
}


//////////////////////////////////////////////////////////////////////
// PtFactory
//////////////////////////////////////////////////////////////////////

// defparam 文のヘッダを生成する．
PtItem*
PtFactory::new_DefParamH(
  const FileRegion& file_region,
  PtDefParam* elem_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_DefParamH));
  return new (p) CptItem_DefParamH(file_region, elem_top);
}

// defparam 文の要素を生成する．
PtDefParam*
PtFactory::new_DefParam(
  const FileRegion& file_region,
  const char* name,
  const AstExpr* value
)
{
  void* p = mAlloc.get_memory(sizeof(CptDefParam));
  return new (p) CptDefParam(file_region, name, value);
}

PtDefParam*
PtFactory::new_DefParam(
  const FileRegion& file_region,
  const PtHierName& hname,
  const AstExpr* value
)
{
  void* p = mAlloc.get_memory(sizeof(CptDefParam2));
  return new (p) CptDefParam2(file_region,
			      hname.nb_list.top, hname.tail_name,
			      value);
}

END_NAMESPACE_YM_VERILOG
