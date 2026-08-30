#ifndef CPTSTMT_COND_H
#define CPTSTMT_COND_H

/// @file CptStmt_Cond.h
/// @brief CptStmt_Cond のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptStmt_If CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_If :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptStmt_If(
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
  ~CptStmt_If() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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
/// @class CptStmt_IfElse CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief else 節付きの if 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_IfElse :
  public CptStmt_If
{
public:

  /// @brief コンストラクタ
  CptStmt_IfElse(
    const FileRegion& file_region,
    const AstExpr* expr,
    const AstStmt* then_body,
    const AstStmt* else_body
  ) : CptStmt_If(file_region, expr, then_body),
      mElse{else_body}
  {
    if ( expr == nullptr ) {
      throw std::logic_error{"expr = nullptr"};
    }
  }

  /// @brief デストラクタ
  ~CptStmt_IfElse() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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
/// @class CptStmt_Case CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief case 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_Case :
  public CptStmt
{
public:

  /// @brief コンストラクタ
  CptStmt_Case(
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
  ~CptStmt_Case() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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
/// @class CptStmt_CaseX CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief casex 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_CaseX :
  public CptStmt_Case
{
public:

  /// @brief コンストラクタ
  CptStmt_CaseX(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItem* caseitem_top
  ) : CptStmt_Case(file_region, expr, caseitem_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_CaseX() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラスの型を返す仮想関数
  Type
  type() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class CptStmt_CaseZ CptStmt_Cond.h "CptStmt_Cond.h"
/// @brief casez 文 を表すクラス
//////////////////////////////////////////////////////////////////////
class CptStmt_CaseZ :
  public CptStmt_Case
{
public:

  /// @brief コンストラクタ
  CptStmt_CaseZ(
    const FileRegion& file_region,
    const AstExpr* expr,
    PtCaseItem* caseitem_top
  ) : CptStmt_Case(file_region, expr, caseitem_top)
  {
  }

  /// @brief デストラクタ
  ~CptStmt_CaseZ() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstStmt の派生クラスのための仮想関数
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

#endif // CPTSTMT_COND_H
