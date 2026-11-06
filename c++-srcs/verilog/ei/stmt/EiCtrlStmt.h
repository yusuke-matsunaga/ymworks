#ifndef EICTRLSTMT_H
#define EICTRLSTMT_H

/// @file EiCtrlStmt.h
/// @brief EiCtrlStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
// IEEE Std 1364-2001 26.6.38 Disable

#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCtrlStmt EiCtrlStmt.h "EiCtrlStmt.h"
/// @brief delay / event control 付きの statement を表すクラス
/// IEEE Std 1364-2001 26.6.29 Delay control
/// IEEE Std 1364-2001 26.6.30 Event control
//////////////////////////////////////////////////////////////////////
class EiCtrlStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiCtrlStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in]	パース木のステートメント定義
    const VlControl* control, ///< [in]	コントロール
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) : EiStmtBase(parent, process, ast_stmt),
      mControl{control},
      mBodyStmt{stmt}
  {
  }

  /// @brief デストラクタ
  ~EiCtrlStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コントロールを返す．
  const VlControl*
  control() const override;

  /// @brief 本文のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コントロール
  const VlControl* mControl;

  // 本文
  const VlStmt* mBodyStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EICTRLSTMT_H
