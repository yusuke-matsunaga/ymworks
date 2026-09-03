#ifndef CPTITEM_GENERATE_H
#define CPTITEM_GENERATE_H

/// @file CptItem_Generate.h
/// @brief CptItem_Generate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtGenCaseItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief generate 文で生成される要素を持つ本体
///
/// CptItem_GenBase などのメンバとして埋め込まれる．
//////////////////////////////////////////////////////////////////////
class CptGenBody
{
public:

  /// @brief コンストラクタ
  CptGenBody(
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : mDeclHeadTop{declhead_top},
      mItemTop{item_top}
  {
  }

  /// @brief デストラクタ
  ~CptGenBody() {}


public:

  /// @brief 宣言ヘッダのリストの先頭の取得
  const PtDeclHead*
  declhead_top() const
  {
    return mDeclHeadTop;
  }

  /// @brief item リストの先頭の取得
  const PtItem*
  item_top() const
  {
    return mItemTop;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の先頭
  const PtDeclHead* mDeclHeadTop;

  // 要素の先頭
  const PtItem* mItemTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class CptItem_GenBase :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_GenBase(
    const FileRegion& file_region,
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : mFileRegion{file_region},
      mBody(declhead_top, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 宣言ヘッダリストの先頭の取得
  const PtDeclHead*
  declhead_top() const override;

  /// @brief item リストの先頭の取得
  const PtItem*
  item_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックを表すノード
//////////////////////////////////////////////////////////////////////
class CptItem_Generate :
  public CptItem_GenBase
{
public:

  /// @brief コンストラクタ
  CptItem_Generate(
    const FileRegion& file_region,
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : CptItem_GenBase(file_region, declhead_top, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_Generate() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの中のブロック
//////////////////////////////////////////////////////////////////////
class CptItem_GenBlock :
  public CptItem_GenBase
{
public:

  /// @brief コンストラクタ
  CptItem_GenBlock(
    const FileRegion& file_region,
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : CptItem_GenBase(file_region, declhead_top, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenBlock() { }


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief 名前つきの GenBlock
//////////////////////////////////////////////////////////////////////
class CptItem_GenBlockN :
  public CptItem_GenBlock
{
public:

  /// @brief コンストラクタ
  CptItem_GenBlockN(
    const FileRegion& file_region,
    const char* name,
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : CptItem_GenBlock(file_region, declhead_top, item_top),
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenBlockN() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenerate の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const char*
  name() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GenIf :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_GenIf(
    const FileRegion& file_region,
    const PtExpr* cond,
    const PtDeclHead* then_declhead_top,
    const PtItem* then_item_top
  ) : mFileRegion{file_region},
      mCond{cond},
      mThenBody(then_declhead_top, then_item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenIf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

  /// @brief 条件式を返す．
  const PtExpr*
  cond_expr() const override;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの先頭の取得
  const PtDeclHead*
  then_declhead_top() const override;

  /// @brief 条件が成り立った時に生成される要素リストの先頭の取得
  const PtItem*
  then_item_top() const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの先頭の取得
  const PtDeclHead*
  else_declhead_top() const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの先頭の取得
  const PtItem*
  else_item_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 条件式
  const PtExpr* mCond;

  // 成り立ったとき生成される本体
  CptGenBody mThenBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief ELSE 付きの gen_if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GenIfElse :
  public CptItem_GenIf
{
public:

  /// @brief コンストラクタ
  CptItem_GenIfElse(
    const FileRegion& file_region,
    const PtExpr* cond,
    const PtDeclHead* then_declhead_top,
    const PtItem* then_item_top,
    const PtDeclHead* else_declhead_top,
    const PtItem* else_item_top
  ) : CptItem_GenIf(file_region, cond, then_declhead_top, then_item_top),
      mElseBody(else_declhead_top, else_item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenIfElse() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの先頭の取得
  const PtDeclHead*
  else_declhead_top() const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの先頭の取得
  const PtItem*
  else_item_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 成り立たなかったとき生成される本体
  CptGenBody mElseBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GenCase :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptItem_GenCase(
    const FileRegion& file_region,
    const PtExpr* expr,
    const PtGenCaseItem* item_top
  ) : mFileRegion{file_region},
      mExpr{expr},
      mCaseItemTop{item_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenCase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  AstItem::Type
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較式を返す．
  const PtExpr*
  cond_expr() const override;

  /// @brief case item リストの先頭の取得
  const PtGenCaseItem*
  caseitem_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 比較式
  const PtExpr* mExpr;

  // case item の先頭
  const PtGenCaseItem* mCaseItemTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief gencaseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenCaseItem :
  public PtGenCaseItem
{
public:

  /// @brief コンストラクタ
  CptGenCaseItem(
    const FileRegion& file_region,
    const PtExpr* label_top,
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : mFileRegion{file_region},
      mLabelTop{label_top},
      mBody(declhead_top, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptGenCaseItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルリストの先頭の取得
  const PtExpr*
  label_top() const override;

  /// @brief 宣言ヘッダリストの先頭の取得
  const PtDeclHead*
  declhead_top() const override;

  /// @brief item リストの先頭の取得
  const PtItem*
  item_top() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの先頭
  const PtExpr* mLabelTop;

  // 生成される本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptItem_GenFor :
  public CptItem_GenBase
{
public:

  /// @brief コンストラクタ
  CptItem_GenFor(
    const FileRegion& file_region,
    const char* loop_var,
    const PtExpr* init_expr,
    const PtExpr* cond,
    const PtExpr* next_expr,
    const char* block_name,
    const PtDeclHead* declhead_top,
    const PtItem* item_top
  ) : CptItem_GenBase(file_region, declhead_top, item_top),
      mName{block_name},
      mLoopVar{loop_var},
      mInitExpr{init_expr},
      mCond{cond},
      mNextExpr{next_expr}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenFor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  AstItem::Type
  type() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 繰り返し制御用の変数名を返す．
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺を返す．
  const PtExpr*
  init_expr() const override;

  /// @brief 繰り返し条件を返す．
  const PtExpr*
  cond_expr() const override;

  /// @brief 増加文の右辺を返す．
  const PtExpr*
  next_expr() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const char* mName;

  // 繰り返し制御用の変数名
  const char* mLoopVar;

  // 初期化文の右辺
  const PtExpr* mInitExpr;

  // 繰り返し条件
  const PtExpr* mCond;

  // 増加文の右辺
  const PtExpr* mNextExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_GENERATE_H
