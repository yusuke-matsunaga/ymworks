#ifndef EIIFSTMT_H
#define EIIFSTMT_H

/// @file EiIfStmt.h
/// @brief EiIfStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.32 While, repeat, wait
// IEEE Std 1364-2001 26.6.33 For
// IEEE Std 1364-2001 26.6.34 Forever
// IEEE Std 1364-2001 26.6.35 If, if-else
// IEEE Std 1364-2001 26.6.36 Case

#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiIfStmt EiIfStmt.h "ei/EiIfStmt.h"
/// @brief if statement を表すクラス
/// IEEE Std 1364-2001 26.6.35 If, if-else
//////////////////////////////////////////////////////////////////////
class EiIfStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiIfStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  ) : EiStmtBase(parent, process, ast_stmt),
      mCondition{cond},
      mBodyStmt{stmt}
  {
  }

  /// @brief デストラクタ
  ~EiIfStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式を返す．
  const VlExpr*
  expr() const override;

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const VlExpr* mCondition;

  // 本体
  const VlStmt* mBodyStmt;

};


//////////////////////////////////////////////////////////////////////
/// @class EiIfElseStmt EiIfStmt.h "ei/EiIfStmt.h"
/// @brief if else statement を表すクラス
/// IEEE Std 1364-2001 26.6.35 If, if-else
//////////////////////////////////////////////////////////////////////
class EiIfElseStmt :
  public EiIfStmt
{
public:

  /// @brief コンストラクタ
  EiIfElseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt,	      ///< [in] 本体のステートメント
    const VlStmt* else_stmt   ///< [in] else節のステートメント
  ) : EiIfStmt(parent, process, ast_stmt, cond, stmt),
      mElseStmt{else_stmt}
  {
  }

  /// @brief デストラクタ
  ~EiIfElseStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief else ステートメントを返す．
  const VlStmt*
  else_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // else ステートメント
  const VlStmt* mElseStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIIFSTMT_H
