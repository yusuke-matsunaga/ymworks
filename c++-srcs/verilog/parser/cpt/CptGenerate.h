#ifndef CPTGENERATE_H
#define CPTGENERATE_H

/// @file CptGenerate.h
/// @brief CptGenerate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "CptItem.h"
#include "parser/PtArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief generate 文で生成される要素を持つ本体
///
/// CptGenBase などのメンバとして埋め込まれる．
//////////////////////////////////////////////////////////////////////
class CptGenBody
{
public:

  /// @brief コンストラクタ
  CptGenBody(
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : mDeclHeadList{std::move(declhead_list)},
      mItemList{std::move(item_list)}
  {
  }

  /// @brief デストラクタ
  ~CptGenBody() {}


public:

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const
  {
    return mDeclHeadList.size();
  }

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < declhead_num() )
  ) const
  {
    return mDeclHeadList[index];
  }

  /// @brief 宣言ヘッダのリストの取得
  AstDeclHeadVec
  declhead_list() const
  {
    return mDeclHeadList.to_vector();
  }

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const
  {
    return mItemList.size();
  }

  /// @brief item の取得
  const AstItem*
  item(
    SizeType index ///< [in] インデックス ( 0 <= index < item_num() )
  ) const
  {
    return mItemList[index];
  }

  /// @brief item リストの取得
  AstItemVec
  item_list() const
  {
    return mItemList.to_vector();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言のリスト
  PtDeclHeadArray mDeclHeadList;

  // 要素のリスト
  PtItemArray mItemList;

};


//////////////////////////////////////////////////////////////////////
/// @brief generate ブロックの基底クラス
//////////////////////////////////////////////////////////////////////
class CptGenBase :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenBase(
    const FileRegion& file_region,
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : mFileRegion{file_region},
      mBody(std::move(declhead_list),
	    std::move(item_list))
  {
  }

  /// @brief デストラクタ
  ~CptGenBase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < declhead_num() )
  ) const override;

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadVec
  declhead_list() const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const AstItem*
  item(
    SizeType index ///< [in] インデックス ( 0 <= index < item_num() )
  ) const override;

  /// @brief item リストの取得
  AstItemVec
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
class CptGenerate :
  public CptGenBase
{
public:

  /// @brief コンストラクタ
  CptGenerate(
    const FileRegion& file_region,
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : CptGenBase(file_region,
		 std::move(declhead_list),
		 std::move(item_list))
  {
  }

  /// @brief デストラクタ
  ~CptGenerate() {}


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
class CptGenBlock :
  public CptGenBase
{
public:

  /// @brief コンストラクタ
  CptGenBlock(
    const FileRegion& file_region,
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : CptGenBase(file_region,
		 std::move(declhead_list),
		 std::move(item_list))
  {
  }

  /// @brief デストラクタ
  ~CptGenBlock() { }


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
class CptGenBlockN :
  public CptGenBlock
{
public:

  /// @brief コンストラクタ
  CptGenBlockN(
    const FileRegion& file_region,
    const char* name,
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : CptGenBlock(file_region,
		  std::move(declhead_list),
		  std::move(item_list)),
      mName{name}
  {
  }

  /// @brief デストラクタ
  ~CptGenBlockN() {}


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
class CptGenIf :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenIf(
    const FileRegion& file_region,
    const AstExpr* cond,
    PtDeclHeadArray&& then_declhead_list,
    PtItemArray&& then_item_list
  ) : mFileRegion{file_region},
      mCond{cond},
      mThenBody(std::move(then_declhead_list),
		std::move(then_item_list))
  {
  }

  /// @brief デストラクタ
  ~CptGenIf() {}


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

  /// @brief 条件が成り立ったときに生成される宣言ヘッダの要素数の取得
  SizeType
  then_declhead_num() const override;

  /// @brief 条件が成り立った時に生成される宣言ヘッダの取得
  const AstDeclHead*
  then_declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < then_declhead_num() )
  ) const override;

  /// @brief 条件が成り立った時に生成される宣言ヘッダリストの取得
  AstDeclHeadVec
  then_declhead_list() const override;

  /// @brief 条件が成り立ったときに生成される要素数の取得
  SizeType
  then_item_num() const override;

  /// @brief 条件が成り立った時に生成される要素の取得
  const AstItem*
  then_item(
    SizeType index ///< [in] インデックス ( 0 <= index < then_item_num() )
  ) const override;

  /// @brief 条件が成り立った時に生成される要素リストの取得
  AstItemVec
  then_item_list() const override;

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
  SizeType
  else_declhead_num() const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダの取得
  const AstDeclHead*
  else_declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < else_declhead_num() )
  ) const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  AstDeclHeadVec
  else_declhead_list() const override;

  /// @brief 条件が成り立たなかったときに生成される要素数の取得
  SizeType
  else_item_num() const override;

  /// @brief 条件が成り立たなかった時に生成される要素の取得
  const AstItem*
  else_item(
    SizeType index ///< [in] インデックス ( 0 <= index < else_item_num() )
  ) const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  AstItemVec
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
class CptGenIfElse :
  public CptGenIf
{
public:

  /// @brief コンストラクタ
  CptGenIfElse(
    const FileRegion& file_region,
    const AstExpr* cond,
    PtDeclHeadArray&& then_declhead_list,
    PtItemArray&& then_item_list,
    PtDeclHeadArray&& else_declhead_list,
    PtItemArray&& else_item_list
  ) : CptGenIf(file_region, cond,
	       std::move(then_declhead_list),
	       std::move(then_item_list)),
      mElseBody(std::move(else_declhead_list),
		std::move(else_item_list))
  {
  }

  /// @brief デストラクタ
  ~CptGenIfElse() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstItem の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件が成り立たなかったときに生成される宣言ヘッダ配列の要素数の取得
  SizeType
  else_declhead_num() const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダの取得
  const AstDeclHead*
  else_declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < else_declhead_num() )
  ) const override;

  /// @brief 条件が成り立たなかった時に生成される宣言ヘッダリストの取得
  AstDeclHeadVec
  else_declhead_list() const override;

  /// @brief 条件が成り立たなかったときに生成される要素数の取得
  SizeType
  else_item_num() const override;

  /// @brief 条件が成り立たなかった時に生成される要素の取得
  const AstItem*
  else_item(
    SizeType index ///< [in] インデックス ( 0 <= index < else_item_num() )
  ) const override;

  /// @brief 条件が成り立たなかった時に生成されるitemリストの取得
  AstItemVec
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
class CptGenCase :
  public CptItem
{
public:

  /// @brief コンストラクタ
  CptGenCase(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtGenCaseItemArray&& item_list
  ) : mFileRegion{file_region},
      mExpr{expr},
      mCaseItemList{std::move(item_list)}
  {
  }

  /// @brief デストラクタ
  ~CptGenCase() {}


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

  /// @brief case item のリストの要素数の取得
  SizeType
  caseitem_num() const override;

  /// @brief case item の取得
  const AstGenCaseItem*
  caseitem(
    SizeType index ///< [in] インデックス ( 0 <= index < caseitem_num() )
  ) const override;

  /// @brief case item リストの取得
  AstGenCaseItemVec
  caseitem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // 比較式
  const AstExpr* mExpr;

  // case item のリスト
  PtGenCaseItemArray mCaseItemList;

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
    const AstExprList* label_list,
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : mFileRegion{file_region},
      mLabelList{std::move(label_list)},
      mBody(std::move(declhead_list),
	    std::move(item_list))
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

  /// @brief ラベルの要素数の取得
  SizeType
  label_num() const override;

  /// @brief ラベルの取得
  const AstExpr*
  label(
    SizeType index ///< [in] インデックス ( 0 <= index < label_num() )
  ) const override;

  /// @brief ラベルリストの取得
  AstExprVec
  label_list() const override;

  /// @brief 宣言ヘッダの要素数の取得
  SizeType
  declhead_num() const override;

  /// @brief 宣言ヘッダの取得
  const AstDeclHead*
  declhead(
    SizeType index ///< [in] インデックス ( 0 <= index < declhead_num() )
  ) const override;

  /// @brief 宣言ヘッダリストの取得
  AstDeclHeadVec
  declhead_list() const override;

  /// @brief item リストの要素数の取得
  SizeType
  item_num() const override;

  /// @brief item の取得
  const AstItem*
  item(
    SizeType index ///< [in] インデックス ( 0 <= index < item_num() )
  ) const override;

  /// @brief item リストの取得
  AstItemVec
  item_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルのリスト
  const AstExprList* mLabelList;

  // 生成される本体
  CptGenBody mBody;

};


//////////////////////////////////////////////////////////////////////
/// @brief gen_for 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptGenFor :
  public CptGenBase
{
public:

  /// @brief コンストラクタ
  CptGenFor(
    const FileRegion& file_region,
    const char* loop_var,
    const AstExpr* init_expr,
    const AstExpr* cond,
    const AstExpr* next_expr,
    const char* block_name,
    PtDeclHeadArray&& declhead_list,
    PtItemArray&& item_list
  ) : CptGenBase(file_region,
		 std::move(declhead_list),
		 std::move(item_list)),
      mName{block_name},
      mLoopVar{loop_var},
      mInitExpr{init_expr},
      mCond{cond}
  {
  }

  /// @brief デストラクタ
  ~CptGenFor() {}


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

#endif // CPTITEM_H
