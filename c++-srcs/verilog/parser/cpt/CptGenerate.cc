
/// @file CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptGenerate.h"
#include "alloc/Alloc.h"
#include "parser/PtDecl.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptGenBase::file_region() const
{
  return mFileRegion;
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptGenBase::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptGenBase::declhead(
  SizeType index
) const
{
  return mBody.declhead(index);
}

// @brief 宣言ヘッダリストの取得
AstDeclHeadVec
CptGenBase::declhead_list() const
{
  return mBody.declhead_list();
}

// @brief item リストの要素数の取得
SizeType
CptGenBase::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const AstItem*
CptGenBase::item(
  SizeType index
) const
{
  return mBody.item(index);
}

// @brief item リストの取得
AstItemVec
CptGenBase::item_list() const
{
  return mBody.item_list();
}


//////////////////////////////////////////////////////////////////////
// generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptGenerate::type() const
{
  return Generate;
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptGenBlock::type() const
{
  return GenBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////

// 名前を返す．
const char*
CptGenBlockN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptGenIf::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptGenIf::type() const
{
  return GenIf;
}

// 条件式を返す．
const AstExpr*
CptGenIf::cond_expr() const
{
  return mCond;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
SizeType
CptGenIf::then_declhead_num() const
{
  return mThenBody.declhead_num();
}

// @brief 条件が成り立った時に生成される宣言ヘッダの取得
const AstDeclHead*
CptGenIf::then_declhead(
  SizeType index
) const
{
  return mThenBody.declhead(index);
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
AstDeclHeadVec
CptGenIf::then_declhead_list() const
{
  return mThenBody.declhead_list();
}

// @brief 条件が成り立ったときに生成される要素数の取得
SizeType
CptGenIf::then_item_num() const
{
  return mThenBody.item_num();
}

// @brief 条件が成り立った時に生成される要素の取得
const AstItem*
CptGenIf::then_item(
  SizeType index
) const
{
  return mThenBody.item(index);
}

// @brief 条件が成り立った時に生成される要素リストの取得
AstItemVec
CptGenIf::then_item_list() const
{
  return mThenBody.item_list();
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
SizeType
CptGenIf::else_declhead_num() const
{
  return mElseBody.declhead_num();
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダの取得
const AstDeclHead*
CptGenIf::else_declhead(
  SizeType index
) const
{
  return mElseBody.declhead(index);
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadVec
CptGenIf::else_declhead_list() const
{
  return mElseBody.declhead_list();
}

// @brief 条件が成り立たなかったときに生成される要素数の取得
SizeType
CptGenIf::else_item_num() const
{
  return mElseBody.item_num();
}

// @brief 条件が成り立たなかった時に生成される要素の取得
const AstItem*
CptGenIf::else_item(
  SizeType index
) const
{
  return mElseBody.item(index);
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemVec
CptGenIf::else_item_list() const
{
  return mElseBody.item_list();
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptGenCaseItem::file_region() const
{
  return mFileRegion;
}

// @brief ラベルの要素数の取得
SizeType
CptGenCaseItem::label_num() const
{
  return mLabelList.size();
}

// @brief ラベルの取得
const AstExpr*
CptGenCaseItem::label(
  SizeType index
) const
{
  return mLabelList[index];
}

// @brief ラベルリストの取得
AstExprVec
CptGenCaseItem::label_list() const
{
  return mLabelList.to_vector();
}

// @brief 宣言の要素数の取得
SizeType
CptGenCaseItem::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言の取得
const AstDeclHead*
CptGenCaseItem::declhead(
  SizeType index
) const
{
  return mBody.declhead(index);
}

// @brief 宣言リストの取得
AstDeclHeadVec
CptGenCaseItem::declhead_list() const
{
  return mBody.declhead_list();
}

// @brief item の要素数の取得
SizeType
CptGenCaseItem::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const AstItem*
CptGenCaseItem::item(
  SizeType index
) const
{
  return mBody.item(index);
}

// @brief item リストの取得
AstItemVec
CptGenCaseItem::item_list() const
{
  return mBody.item_list();
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptGenCase::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptGenCase::type() const
{
  return GenCase;
}

// 比較式を返す．
const AstExpr*
CptGenCase::cond_expr() const
{
  return mExpr;
}

// @brief case item のリストの要素数の取得
SizeType
CptGenCase::caseitem_num() const
{
  return mCaseItemList.size();
}

// @brief case item の取得
const AstGenCaseItem*
CptGenCase::caseitem(
  SizeType index
) const
{
  return mCaseItemList[index];
}

// @brief case item リストの取得
AstGenCaseItemVec
CptGenCase::caseitem_list() const
{
  return mCaseItemList.to_vector();
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptGenFor::type() const
{
  return GenFor;
}

// 名前を返す．
const char*
CptGenFor::name() const
{
  return mName;
}

// 繰り返し制御用の変数名を返す．
const char*
CptGenFor::loop_var() const
{
  return mLoopVar;
}

// 初期化文の右辺を返す．
const AstExpr*
CptGenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const AstExpr*
CptGenFor::cond_expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const AstExpr*
CptGenFor::next_expr() const
{
  return mNextExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス PtFactory
//////////////////////////////////////////////////////////////////////

// generate 文を生成する．
PtItem*
PtFactory::new_Generate(
  const FileRegion& file_region,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstItemVec& item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenerate));
  return new (p) CptGenerate(file_region,
			     PtDeclHeadArray(mAlloc, declhead_list),
			     PtItemArray(mAlloc, item_list));
}

// generate block 文を生成する．
PtItem*
PtFactory::new_GenBlock(
  const FileRegion& file_region,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstItemVec& item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenBlock));
  return new (p) CptGenBlock(file_region,
			     PtDeclHeadArray(mAlloc, declhead_list),
			     PtItemArray(mAlloc, item_list));
}

// generate block 文を生成する．
PtItem*
PtFactory::new_GenBlock(
  const FileRegion& file_region,
  const char* name,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstItemVec& item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenBlockN));
  return new (p) CptGenBlockN(file_region, name,
			      PtDeclHeadArray(mAlloc, declhead_list),
			      PtItemArray(mAlloc, item_list));

}

// generate if 文を生成する．
PtItem*
PtFactory::new_GenIf(
  const FileRegion& file_region,
  const AstExpr* cond,
  const std::vector<PtDeclHead*>& then_declhead_list,
  const AstItemVec& then_item_list,
  const std::vector<PtDeclHead*>& else_declhead_list,
  const AstItemVec& else_item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenIf));
  return new (p) CptGenIf(file_region, cond,
			  PtDeclHeadArray(mAlloc, then_declhead_list),
			  PtItemArray(mAlloc, then_item_list),
			  PtDeclHeadArray(mAlloc, else_declhead_list),
			  PtItemArray(mAlloc, else_item_list));
}

// generate case 文を生成する．
PtItem*
PtFactory::new_GenCase(
  const FileRegion& file_region,
  const AstExpr* expr,
  const AstGenCaseItemVec& item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenCase));
  return new (p) CptGenCase(file_region, expr,
			    PtGenCaseItemArray(mAlloc, item_list));
}

// generate case の要素を生成する．
PtGenCaseItem*
PtFactory::new_GenCaseItem(
  const FileRegion& file_region,
  PtExprList* label_list,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstItemVec& item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenCaseItem));
  return new (p) CptGenCaseItem(file_region,
				label_list->to_array(mAlloc),
				PtDeclHeadArray(mAlloc, declhead_list),
				PtItemArray(mAlloc, item_list));
}

// generate for 文を生成する．
PtItem*
PtFactory::new_GenFor(
  const FileRegion& file_region,
  const char* loop_var,
  const AstExpr* init_expr,
  const AstExpr* cond,
  const AstExpr* next_expr,
  const char* block_name,
  const std::vector<PtDeclHead*>& declhead_list,
  const AstItemVec& item_list
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenFor));
  return new (p) CptGenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   PtDeclHeadArray(mAlloc, declhead_list),
			   PtItemArray(mAlloc, item_list));
}

END_NAMESPACE_YM_VERILOG
