
/// @file CptGenerate.cc
/// @brief CptGenerate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptGenerate.h"
#include "alloc/Alloc.h"
#include "parser/PtFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// generate 文で生成される要素を持つ本体
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBody::CptGenBody(
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : mDeclHeadArray{std::move(declhead_array)},
    mItemArray{std::move(item_array)}
{
}

// デストラクタ
CptGenBody::~CptGenBody()
{
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBase::CptGenBase(
  const FileRegion& file_region,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : mFileRegion{file_region},
    mBody{std::move(declhead_array), std::move(item_array)}
{
}

// デストラクタ
CptGenBase::~CptGenBase()
{
}

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
  SizeType pos
) const
{
  return mBody.declhead(pos);
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
  SizeType pos
) const
{
  return mBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenerate::CptGenerate(
  const FileRegion& file_region,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : CptGenBase(file_region, std::move(declhead_array), std::move(item_array))
{
}

// デストラクタ
CptGenerate::~CptGenerate()
{
}

// 型を返す．
AstItem::Type
CptGenerate::type() const
{
  return Generate;
}


//////////////////////////////////////////////////////////////////////
// generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBlock::CptGenBlock(
  const FileRegion& file_region,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : CptGenBase(file_region, std::move(declhead_array), std::move(item_array))
{
}

// デストラクタ
CptGenBlock::~CptGenBlock()
{
}

// 型を返す．
AstItem::Type
CptGenBlock::type() const
{
  return GenBlock;
}


//////////////////////////////////////////////////////////////////////
// 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenBlockN::CptGenBlockN(
  const FileRegion& file_region,
  const char* name,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : CptGenBlock(file_region, std::move(declhead_array), std::move(item_array)),
    mName{name}
{
}

// デストラクタ
CptGenBlockN::~CptGenBlockN()
{
}

// 名前を返す．
const char*
CptGenBlockN::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenIf::CptGenIf(
  const FileRegion& file_region,
  const AstExpr* cond,
  PtDeclHeadArray&& then_declhead_array,
  PtItemArray&& then_item_array,
  PtDeclHeadArray&& else_declhead_array,
  PtItemArray&& else_item_array
) : mFileRegion{file_region},
    mCond{cond},
    mThenBody(std::move(then_declhead_array), std::move(then_item_array)),
    mElseBody(std::move(else_declhead_array), std::move(else_item_array))
{
}

// デストラクタ
CptGenIf::~CptGenIf()
{
}

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
CptGenIf::expr() const
{
  return mCond;
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
SizeType
CptGenIf::then_declhead_num() const
{
  return mThenBody.declhead_num();
}

// @brief 条件が成り立ったときに生成される宣言ヘッダの取得
const AstDeclHead*
CptGenIf::then_declhead(
  SizeType pos
) const
{
  return mThenBody.declhead(pos);
}

// @brief 条件が成り立ったときに生成される item リストの要素数の取得
SizeType
CptGenIf::then_item_num() const
{
  return mThenBody.item_num();
}

// @brief 条件が成り立ったときに生成される item の取得
const AstItem*
CptGenIf::then_item(
  SizeType pos
) const
{
  return mThenBody.item(pos);
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの要素数の取得
SizeType
CptGenIf::else_declhead_num() const
{
  return mElseBody.declhead_num();
}

// @brief 条件が成り立たなかったときに生成される宣言ヘッダの取得
const AstDeclHead*
CptGenIf::else_declhead(
  SizeType pos
) const
{
  return mElseBody.declhead(pos);
}

// @brief 条件が成り立たなかったときに生成される item リストの要素数の取得
SizeType
CptGenIf::else_item_num() const
{
  return mElseBody.item_num();
}

// @brief 条件が成り立たなかったときに生成される item の取得
const AstItem*
CptGenIf::else_item(
  SizeType pos
) const
{
  return mElseBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// caseitem を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenCaseItem::CptGenCaseItem(
  const FileRegion& file_region,
  PtExprArray&& label_array,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : mFileRegion{file_region},
    mLabelArray{std::move(label_array)},
    mBody(std::move(declhead_array), std::move(item_array))
{
}

// デストラクタ
CptGenCaseItem::~CptGenCaseItem()
{
}

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
  return mLabelArray.size();
}

// @brief ラベルの取得
const AstExpr*
CptGenCaseItem::label(
  SizeType pos
) const
{
  return mLabelArray[pos];
}

// @brief 宣言ヘッダの要素数の取得
SizeType
CptGenCaseItem::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptGenCaseItem::declhead(
  SizeType pos
) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
CptGenCaseItem::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const AstItem*
CptGenCaseItem::item(
  SizeType pos
) const
{
  return mBody.item(pos);
}


//////////////////////////////////////////////////////////////////////
// case 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenCase::CptGenCase(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtGenCaseItemArray&& item_array
) : mFileRegion{file_region},
    mExpr{expr},
    mCaseItemArray{std::move(item_array)}
{
}

// デストラクタ
CptGenCase::~CptGenCase()
{
}

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
CptGenCase::expr() const
{
  return mExpr;
}

// @brief case item のリストの要素数の取得
SizeType
CptGenCase::caseitem_num() const
{
  return mCaseItemArray.size();
}

// @brief case item の取得
const AstGenCaseItem*
CptGenCase::caseitem(
  SizeType pos
) const
{
  return mCaseItemArray[pos];
}


//////////////////////////////////////////////////////////////////////
// gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CptGenFor::CptGenFor(
  const FileRegion& file_region,
  const char* loop_var,
  const AstExpr* init_expr,
  const AstExpr* cond,
  const AstExpr* next_expr,
  const char* block_name,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
) : mFileRegion{file_region},
    mName{block_name},
    mLoopVar{loop_var},
    mInitExpr{init_expr},
    mCond{cond},
    mNextExpr{next_expr},
    mBody(std::move(declhead_array), std::move(item_array))
{
}

// デストラクタ
CptGenFor::~CptGenFor()
{
}

// ファイル位置を返す．
FileRegion
CptGenFor::file_region() const
{
  return mFileRegion;
}

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

// @brief 宣言ヘッダの要素数の取得
SizeType
CptGenFor::declhead_num() const
{
  return mBody.declhead_num();
}

// @brief 宣言ヘッダの取得
const AstDeclHead*
CptGenFor::declhead(
  SizeType pos
) const
{
  return mBody.declhead(pos);
}

// @brief item リストの要素数の取得
SizeType
CptGenFor::item_num() const
{
  return mBody.item_num();
}

// @brief item の取得
const AstItem*
CptGenFor::item(
  SizeType pos
) const
{
  return mBody.item(pos);
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
CptGenFor::expr() const
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
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenerate));
  return new (p) CptGenerate(file_region,
			     std::move(declhead_array),
			     std::move(item_array));
}

// generate block 文を生成する．
PtItem*
PtFactory::new_GenBlock(
  const FileRegion& file_region,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenBlock));
  return new (p) CptGenBlock(file_region,
			     std::move(declhead_array),
			     std::move(item_array));
}

// generate block 文を生成する．
PtItem*
PtFactory::new_GenBlock(
  const FileRegion& file_region,
  const char* name,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenBlockN));
  return new (p) CptGenBlockN(file_region, name,
			      std::move(declhead_array),
			      std::move(item_array));
}

// generate if 文を生成する．
PtItem*
PtFactory::new_GenIf(
  const FileRegion& file_region,
  const AstExpr* cond,
  PtDeclHeadArray&& then_declhead_array,
  PtItemArray&& then_item_array,
  PtDeclHeadArray&& else_declhead_array,
  PtItemArray&& else_item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenIf));
  return new (p) CptGenIf(file_region, cond,
			  std::move(then_declhead_array),
			  std::move(then_item_array),
			  std::move(else_declhead_array),
			  std::move(else_item_array));
}

// generate case 文を生成する．
PtItem*
PtFactory::new_GenCase(
  const FileRegion& file_region,
  const AstExpr* expr,
  PtGenCaseItemArray&& item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenCase));
  return new (p) CptGenCase(file_region, expr,
			    std::move(item_array));
}

// generate case の要素を生成する．
PtGenCaseItem*
PtFactory::new_GenCaseItem(
  const FileRegion& file_region,
  PtExprArray&& label_array,
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenCaseItem));
  return new (p) CptGenCaseItem(file_region,
				std::move(label_array),
				std::move(declhead_array),
				std::move(item_array));
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
  PtDeclHeadArray&& declhead_array,
  PtItemArray&& item_array
)
{
  void* p = mAlloc.get_memory(sizeof(CptGenFor));
  return new (p) CptGenFor(file_region, loop_var, init_expr, cond,
			   next_expr, block_name,
			   std::move(declhead_array),
			   std::move(item_array));
}

END_NAMESPACE_YM_VERILOG
