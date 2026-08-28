#ifndef CPTCONDSTMT_H
#define CPTCONDSTMT_H

/// @file CptStmt.h
/// @brief CptStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @brief if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIf :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptIf(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* then_body
  ) : CptStmt(file_region),
      mCond{expr},
      mThen{then_body}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptIf() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 条件式を返す．
  const AstExpr*
  expr() const override;

  /// @brief 成り立ったとき実行されるステートメント
  const AstStmt*
  body() const override;

  /// @brief 成り立たなかったとき実行されるステートメント
  const AstStmt*
  else_body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const AstExpr* mCond;

  // 成り立ったとき実行されるステートメント
  const AstStmt* mThen;

};


//////////////////////////////////////////////////////////////////////
/// @brief else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptIfElse :
  public CptIf
{
public:

  /// @brief コンストラクタ
  CptIfElse(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* then_body,
    const AstStmt* else_body
  ) : CptIf(file_region, expr, then_body),
      mElse{else_body}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptIfElse() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 成り立たなかったとき実行されるステートメント
  const AstStmt*
  else_body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 成り立たなかったとき実行されるステートメント
  const AstStmt* mElse;

};


//////////////////////////////////////////////////////////////////////
/// @brief case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCase :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptCase(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItem* caseitem_top
  ) : CptStmt(file_region),
      mExpr{expr},
      mCaseItemTop{caseitem_top}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptCase() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

  /// @brief 比較される式を返す．
  const AstExpr*
  expr() const override;

  /// @brief case item のリストの取得
  ///
  /// - type() != Case|CaseX|CaseZ の時 std::logic_error 例外を送出する．
  AstCaseItemList
  caseitem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 比較される式
  const AstExpr* mExpr;

  // case item の先頭
  PtCaseItem* mCaseItemTop;

};


//////////////////////////////////////////////////////////////////////
/// @brief casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseX :
  public CptCase
{
public:

  /// @brief コンストラクタ
  CptCaseX(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItem* caseitem_top
  ) : CptCase(file_region, expr, caseitem_top)
  {
  }

  /// @brief デストラクタ
  ~CptCaseX() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseZ :
  public CptCase
{
public:

  /// @brief コンストラクタ
  CptCaseZ(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItem* caseitem_top
  ) : CptCase(file_region, expr, caseitem_top)
  {
  }

  /// @brief デストラクタ
  ~CptCaseZ() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @brief caseitemを表すクラス
//////////////////////////////////////////////////////////////////////
class CptCaseItem :
  public PtCaseItem
{
public:

  /// @brief コンストラクタ
  CptCaseItem(
    const FileRegion& file_region,
    PtExpr* label_top,
    const AstStmt* body
  ) : mFileRegion{file_region},
      mLabelTop{label_top},
      mBody{body}
  {
    if ( body == nullptr ) {
      throw std::logic_error{"body = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptCaseItem() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtCaseItem の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief ラベルリストの取得
  AstExprList
  label_list() const override;

  /// @brief 本体のステートメント得る．
  const AstStmt*
  body() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mFileRegion;

  // ラベルの先頭
  const AstExpr* mLabelTop;

  // ラベルが一致したときに実行されるステートメント
  const AstStmt* mBody;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTCONDSTMT_H
