
/// @file CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_Generate.h"
#include "alloc/Alloc.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_GenBase::file_region() const
{
  return mFileRegion;
}

// @brief 宣言ヘッダリストの取得
AstDeclHeadList
CptItem_GenBase::declhead_list() const
{
  return mBody.declhead_list();
}

// @brief item リストの取得
AstItemList
CptItem_GenBase::item_list() const
{
  return mBody.item_list();
}


//////////////////////////////////////////////////////////////////////
// generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_Generate::type() const
{
  return Generate;
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_GenBlock::type() const
{
  return GenBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////

// 名前を返す．
const char*
CptItem_GenBlockN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_GenIf::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptItem_GenIf::type() const
{
  return GenIf;
}

// 条件式を返す．
const AstExpr*
CptItem_GenIf::cond_expr() const
{
  return mCond;
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
AstDeclHeadList
CptItem_GenIf::then_declhead_list() const
{
  return mThenBody.declhead_list();
}

// @brief 条件が成り立った時に生成される要素リストの取得
AstItemList
CptItem_GenIf::then_item_list() const
{
  return mThenBody.item_list();
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadList
CptItem_GenIf::else_declhead_list() const
{
  return AstDeclHeadList();
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemList
CptItem_GenIf::else_item_list() const
{
  return AstItemList();
}


//////////////////////////////////////////////////////////////////////
// ELSE 付き gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadList
CptItem_GenIfElse::else_declhead_list() const
{
  return mElseBody.declhead_list();
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemList
CptItem_GenIfElse::else_item_list() const
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

// @brief ラベルリストの取得
AstExprList
CptGenCaseItem::label_list() const
{
  return AstExprList(mLabelTop);
}

// @brief 宣言リストの取得
AstDeclHeadList
CptGenCaseItem::declhead_list() const
{
  return mBody.declhead_list();
}

// @brief item リストの取得
AstItemList
CptGenCaseItem::item_list() const
{
  return mBody.item_list();
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// ファイル位置を返す．
FileRegion
CptItem_GenCase::file_region() const
{
  return mFileRegion;
}

// 型を返す．
AstItem::Type
CptItem_GenCase::type() const
{
  return GenCase;
}

// 比較式を返す．
const AstExpr*
CptItem_GenCase::cond_expr() const
{
  return mExpr;
}

// @brief case item リストの取得
AstGenCaseItemList
CptItem_GenCase::caseitem_list() const
{
  return AstGenCaseItemList(mCaseItemTop);
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_GenFor::type() const
{
  return GenFor;
}

// 名前を返す．
const char*
CptItem_GenFor::name() const
{
  return mName;
}

// 繰り返し制御用の変数名を返す．
const char*
CptItem_GenFor::loop_var() const
{
  return mLoopVar;
}

// 初期化文の右辺を返す．
const AstExpr*
CptItem_GenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const AstExpr*
CptItem_GenFor::cond_expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const AstExpr*
CptItem_GenFor::next_expr() const
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
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_Generate));
  return new (p) CptItem_Generate(file_region,
			     declhead_top,
			     item_top);
}

// generate block 文を生成する．
PtItem*
PtFactory::new_GenBlock(
  const FileRegion& file_region,
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenBlock));
  return new (p) CptItem_GenBlock(file_region,
			     declhead_top,
			     item_top);
}

// generate block 文を生成する．
PtItem*
PtFactory::new_GenBlock(
  const FileRegion& file_region,
  const char* name,
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenBlockN));
  return new (p) CptItem_GenBlockN(file_region, name,
			      declhead_top,
			      item_top);
}

// generate if 文を生成する．
PtItem*
PtFactory::new_GenIf(
  const FileRegion& file_region,
  const AstExpr* cond,
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenIf));
  return new (p) CptItem_GenIf(file_region, cond,
			  declhead_top,
			  item_top);
}

// generate if 文を生成する．
PtItem*
PtFactory::new_GenIfElse(
  const FileRegion& file_region,
  const AstExpr* cond,
  PtDeclHead* then_declhead_top,
  PtItem* then_item_top,
  PtDeclHead* else_declhead_top,
  PtItem* else_item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenIfElse));
  return new (p) CptItem_GenIfElse(file_region, cond,
			      then_declhead_top,
			      then_item_top,
			      else_declhead_top,
			      else_item_top);
}

// generate case 文を生成する．
PtItem*
PtFactory::new_GenCase(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtGenCaseItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenCase));
  return new (p) CptItem_GenCase(file_region, expr, item_top);
}

// generate case の要素を生成する．
PtGenCaseItem*
PtFactory::new_GenCaseItem(
  const FileRegion& file_region,
  PtExpr* label_top,
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenCaseItem));
  return new (p) CptGenCaseItem(file_region,
				label_top,
				declhead_top,
				item_top);
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
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenFor));
  return new (p) CptItem_GenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   declhead_top,
			   item_top);
}

END_NAMESPACE_YM_VERILOG
