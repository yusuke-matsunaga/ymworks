#ifndef EIWAITSTMT_H
#define EIWAITSTMT_H

/// @file EiWaitStmt.h
/// @brief EiWaitStmt のヘッダファイル
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

#include "EiLoopStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiWaitStmt EiWaitStmt.h "ei/EiWaitStmt.h"
/// @brief wait 文を表すクラス
/// IEEE Std 1364-2001 26.6.32 While, repeat, wait
//////////////////////////////////////////////////////////////////////
class EiWaitStmt :
  public EiLoopStmt
{
public:

  /// @brief コンストラクタ
  EiWaitStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  ) : EiLoopStmt(parent, process, ast_stmt, cond, stmt)
  {
  }

  /// @brief デストラクタ
  ~EiWaitStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIWAITSTMT_H
