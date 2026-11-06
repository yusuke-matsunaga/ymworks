#ifndef EIEVENTSTMT_H
#define EIEVENTSTMT_H

/// @file EiEventStmt.h
/// @brief EiEventStmt のヘッダファイル
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
/// @class EiEventStmt ElbEventStmt.h "ElbEventStmt.h"
/// @brief event statement を表すクラス
/// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
//////////////////////////////////////////////////////////////////////
class EiEventStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiEventStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in]	パース木のステートメント定義
    ElbExpr* named_event      ///< [in]	対象のイベント
  ) : EiStmtBase(parent, process, ast_stmt),
      mEvent{named_event}
  {
  }

  /// @brief デストラクタ
  ~EiEventStmt() = default;


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

  /// @brief named event を返す．
  const VlExpr*
  named_event() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // イベントを表す式
  ElbExpr* mEvent;

};

END_NAMESPACE_YM_VERILOG

#endif // EIEVENTSTMT_H
