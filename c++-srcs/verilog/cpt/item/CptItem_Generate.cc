
/// @file CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem_Generate.h"
#include "alloc/Alloc.h"
#include "parser/PtDeclHead.h"
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
const PtDeclHead*
CptItem_GenBase::declhead_top() const
{
  return mBody.declhead_top();
}

// @brief item リストの取得
const PtItem*
CptItem_GenBase::item_top() const
{
  return mBody.item_top();
}


//////////////////////////////////////////////////////////////////////
// generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_Generate::type() const
{
  return AstItem::Generate;
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_GenBlock::type() const
{
  return AstItem::GenBlock;
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
  return AstItem::GenIf;
}

// 条件式を返す．
const PtExpr*
CptItem_GenIf::cond_expr() const
{
  return mCond;
}

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
const PtDeclHead*
CptItem_GenIf::then_declhead_top() const
{
  return mThenBody.declhead_top();
}

// @brief 条件が成り立った時に生成される要素リストの取得
const PtItem*
CptItem_GenIf::then_item_top() const
{
  return mThenBody.item_top();
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
const PtDeclHead*
CptItem_GenIf::else_declhead_top() const
{
  return nullptr;
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
const PtItem*
CptItem_GenIf::else_item_top() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// ELSE 付き gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
const PtDeclHead*
CptItem_GenIfElse::else_declhead_top() const
{
  return mElseBody.declhead_top();
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
const PtItem*
CptItem_GenIfElse::else_item_top() const
{
  return mElseBody.item_top();
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
const PtExpr*
CptGenCaseItem::label_top() const
{
  return mLabelTop;
}

// @brief 宣言リストの取得
const PtDeclHead*
CptGenCaseItem::declhead_top() const
{
  return mBody.declhead_top();
}

// @brief item リストの取得
const PtItem*
CptGenCaseItem::item_top() const
{
  return mBody.item_top();
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
  return AstItem::GenCase;
}

// 比較式を返す．
const PtExpr*
CptItem_GenCase::cond_expr() const
{
  return mExpr;
}

// @brief case item リストの取得
const PtGenCaseItem*
CptItem_GenCase::caseitem_top() const
{
  return mCaseItemTop;
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// 型を返す．
AstItem::Type
CptItem_GenFor::type() const
{
  return AstItem::GenFor;
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
const PtExpr*
CptItem_GenFor::init_expr() const
{
  return mInitExpr;
}

// 繰り返し条件を返す．
const PtExpr*
CptItem_GenFor::cond_expr() const
{
  return mCond;
}

// 増加文の右辺を返す．
const PtExpr*
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
  const PtDeclHead* declhead_top,
  const PtItem* item_top
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
  const PtDeclHead* declhead_top,
  const PtItem* item_top
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
  const PtDeclHead* declhead_top,
  const PtItem* item_top
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
  const PtExpr* cond,
  const PtDeclHead* declhead_top,
  const PtItem* item_top
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
  const PtExpr* cond,
  const PtDeclHead* then_declhead_top,
  const PtItem* then_item_top,
  const PtDeclHead* else_declhead_top,
  const PtItem* else_item_top
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
  const PtExpr* expr,
  const PtGenCaseItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenCase));
  return new (p) CptItem_GenCase(file_region, expr, item_top);
}

// generate case の要素を生成する．
PtGenCaseItem*
PtFactory::new_GenCaseItem(
  const FileRegion& file_region,
  const PtExpr* label_top,
  const PtDeclHead* declhead_top,
  const PtItem* item_top
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
  const PtExpr* init_expr,
  const PtExpr* cond,
  const PtExpr* next_expr,
  const char* block_name,
  const PtDeclHead* declhead_top,
  const PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptItem_GenFor));
  return new (p) CptItem_GenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   declhead_top,
			   item_top);
}

END_NAMESPACE_YM_VERILOG
