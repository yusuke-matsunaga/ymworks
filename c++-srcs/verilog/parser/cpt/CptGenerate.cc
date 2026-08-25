
/// @file CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptGenerate.h"
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
CptGenBase::file_region() const
{
  return mFileRegion;
}

// @brief 宣言ヘッダリストの取得
AstDeclHeadList
CptGenBase::declhead_list() const
{
  return mBody.declhead_list();
}

// @brief item リストの取得
AstItemList
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

// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
AstDeclHeadList
CptGenIf::then_declhead_list() const
{
  return mThenBody.declhead_list();
}

// @brief 条件が成り立った時に生成される要素リストの取得
AstItemList
CptGenIf::then_item_list() const
{
  return mThenBody.item_list();
}

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadList
CptGenIf::else_declhead_list() const
{
  return AstDeclHeadList();
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemList
CptGenIf::else_item_list() const
{
  return AstItemList();
}


//////////////////////////////////////////////////////////////////////
// ELSE 付き gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
AstDeclHeadList
CptGenIfElse::else_declhead_list() const
{
  return mElseBody.declhead_list();
}

// @brief 条件が成り立たなかった時に生成されるitemリストの取得
AstItemList
CptGenIfElse::else_item_list() const
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

// @brief case item リストの取得
AstGenCaseItemList
CptGenCase::caseitem_list() const
{
  return AstGenCaseItemList(mCaseItemTop);
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
  PtDeclHead* declhead_top,
  PtItem* item_top
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenerate));
  return new (p) CptGenerate(file_region,
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
  void* p = mAlloc.get_memory(sizeof(CptGenBlock));
  return new (p) CptGenBlock(file_region,
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
  void* p = mAlloc.get_memory(sizeof(CptGenBlockN));
  return new (p) CptGenBlockN(file_region, name,
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
  void* p = mAlloc.get_memory(sizeof(CptGenIf));
  return new (p) CptGenIf(file_region, cond,
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
  void* p = mAlloc.get_memory(sizeof(CptGenIfElse));
  return new (p) CptGenIfElse(file_region, cond,
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
  void* p = mAlloc.get_memory(sizeof(CptGenCase));
  return new (p) CptGenCase(file_region, expr, item_top);
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
  void* p = mAlloc.get_memory(sizeof(CptGenFor));
  return new (p) CptGenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   declhead_top,
			   item_top);
}

END_NAMESPACE_YM_VERILOG
