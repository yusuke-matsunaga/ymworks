
/// @file CptDefParam.cc
/// @brief CptDefParam の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptDefParam.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス CptDefParamH
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptDefParamH::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptDefParamH::type() const
{
  return DefParam;
}

// @brief defparam の要素数の取得
SizeType
CptDefParamH::defparam_num() const
{
  return mList.size();
}

// @brief defparam の取得
const AstDefParam*
CptDefParamH::defparam(
  SizeType index
) const
{
  return mList[index];
}

// @brief defparam リストの取得
AstDefParamVec
CptDefParamH::defparam_list() const
{
  return mList.to_vector();
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

// @brief 階層ブランチの要素数の取得
SizeType
CptDefParam::namebranch_num() const
{
  return 0;
}

// @brief 先頭の階層ブランチを返す．
const AstNameBranch*
CptDefParam::namebranch_top() const
{
  return nullptr;
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptDefParam::namebranch_list() const
{
  return {};
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

// @brief 階層ブランチの要素数の取得
SizeType
CptDefParam2::namebranch_num() const
{
  return mNbTop->count_num();
}

// @brief 先頭の階層ブランチを返す．
const AstNameBranch*
CptDefParam2::namebranch_top() const
{
  return mNbTop;
}

// @brief 階層ブランチのリストを返す．
AstNameBranchVec
CptDefParam2::namebranch_list() const
{
  return mNbTop->to_vector();
}


//////////////////////////////////////////////////////////////////////
// PtFactory
//////////////////////////////////////////////////////////////////////

// defparam 文のヘッダを生成する．
PtItem*
PtFactory::new_DefParamH(
  const FileRegion& file_region,
  const AstDefParamVec& elem_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptDefParamH));
  return new (p) CptDefParamH(file_region,
			      PtDefParamArray(mAlloc, elem_list));
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
  auto nb_top = hname.nb_top->reverse();
  auto tail_name = hname.tail_name;
  void* p = mAlloc.get_memory(sizeof(CptDefParam2));
  return new (p) CptDefParam2(file_region,
			      nb_top, tail_name,
			      value);
}

END_NAMESPACE_YM_VERILOG
