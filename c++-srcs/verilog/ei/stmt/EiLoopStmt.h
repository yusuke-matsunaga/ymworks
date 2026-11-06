#ifndef EILOOPSTMT_H
#define EILOOPSTMT_H

/// @file EiLoopStmt.h
/// @brief EiLoopStmt のヘッダファイル
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
/// @class EiLoopStmt EiLoopStmt.h "ei/EiLoopStmt.h"
/// @brief while 文/ repeat 文/ wait 文 の基底クラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiLoopStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiLoopStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,       ///< [in] 条件式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) : EiStmtBase(parent, process, ast_stmt),
      mCondition{cond},
      mBodyStmt{stmt}
  {
  }

  /// @brief デストラクタ
  ~EiLoopStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
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

END_NAMESPACE_YM_VERILOG

#endif // EILOOPSTMT_H
