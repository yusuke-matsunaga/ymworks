#ifndef CPTITEM_GENERATE_H
#define CPTITEM_GENERATE_H

/// @file CptItem_Generate.h
/// @brief CptItem_Generate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"


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
    const AstDeclHead* declhead_top,
    const AstItem* item_top
  ) : mDeclHeadTop{declhead_top},
      mItemTop{item_top}
  {
  }

  /// @brief デストラクタ
  ~CptGenBody() {}


public:

  /// @brief 宣言ヘッダのリストの取得
  AstDeclHeadList
  declhead_list() const
  {
    return AstDeclHeadList(mDeclHeadTop);
  }

  /// @brief item リストの取得
  AstItemList
  item_list() const
  {
    return AstItemList(mItemTop);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言の先頭
  const AstDeclHead* mDeclHeadTop;

  // 要素の先頭
  const AstItem* mItemTop;

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
    const AstDeclHead* declhead_top,
    const AstItem* item_top
  ) : mFileRegion{file_region},
      mBody(declhead_top, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadList
  declhead_list() const override;

  /// @brief item リストの取得
  AstItemList
  item_list() const override;


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
    const AstDeclHead* declhead_top,
    const AstItem* item_top
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
  Type
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
    const AstDeclHead* declhead_top,
    const AstItem* item_top
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
  Type
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
    const AstDeclHead* declhead_top,
    const AstItem* item_top
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
    const AstExpr* cond,
    const AstDeclHead* then_declhead_top,
    const AstItem* then_item_top
  ) : mFileRegion{file_region},
      mCond{cond},
      mThenBody(then_declhead_top, then_item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenIf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;

  /// @brief 条件式を返す．
  const AstExpr*
  cond_expr() const override;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  then_declhead_list() const override;

  /// @brief 条件が成り立った時に生成される要素リストの取得
  AstItemList
  then_item_list() const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  else_declhead_list() const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  AstItemList
  else_item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 条件式
  const AstExpr* mCond;

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
    const AstExpr* cond,
    const AstDeclHead* then_declhead_top,
    const AstItem* then_item_top,
    const AstDeclHead* else_declhead_top,
    const AstItem* else_item_top
  ) : CptItem_GenIf(file_region, cond, then_declhead_top, then_item_top),
      mElseBody(else_declhead_top, else_item_top)
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenIfElse() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  AstDeclHeadList
  else_declhead_list() const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  AstItemList
  else_item_list() const override;


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
    const AstExpr* expr,
    PtGenCaseItem* item_top
  ) : mFileRegion{file_region},
      mExpr{expr},
      mCaseItemTop{item_top}
  {
  }

  /// @brief デストラクタ
  ~CptItem_GenCase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 型を返す．
  Type
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // AstGenCase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 比較式を返す．
  const AstExpr*
  cond_expr() const override;

  /// @brief case item リストの取得
  AstGenCaseItemList
  caseitem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 比較式
  const AstExpr* mExpr;

  // case item の先頭
  PtGenCaseItem* mCaseItemTop;

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
    const AstExpr* label_top,
    const AstDeclHead* declhead_top,
    const AstItem* item_top
  ) : mFileRegion{file_region},
      mLabelTop{label_top},
      mBody(declhead_top, item_top)
  {
  }

  /// @brief デストラクタ
  ~CptGenCaseItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstGenCaseItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルリストの取得
  AstExprList
  label_list() const override;

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadList
  declhead_list() const override;

  /// @brief item リストの取得
  AstItemList
  item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの先頭
  const AstExpr* mLabelTop;

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
    const AstExpr* init_expr,
    const AstExpr* cond,
    const AstExpr* next_expr,
    const char* block_name,
    const AstDeclHead* declhead_top,
    const AstItem* item_top
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
  Type
  type() const override;

  /// @brief 名前を返す．
  const char*
  name() const override;

  /// @brief 繰り返し制御用の変数名を返す．
  const char*
  loop_var() const override;

  /// @brief 初期化文の右辺を返す．
  const AstExpr*
  init_expr() const override;

  /// @brief 繰り返し条件を返す．
  const AstExpr*
  cond_expr() const override;

  /// @brief 増加文の右辺を返す．
  const AstExpr*
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
  const AstExpr* mInitExpr;

  // 繰り返し条件
  const AstExpr* mCond;

  // 増加文の右辺
  const AstExpr* mNextExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTITEM_GENERATE_H
