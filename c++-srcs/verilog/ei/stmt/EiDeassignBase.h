#ifndef EIDEASSIGNBASE_H
#define EIDEASSIGNBASE_H

/// @file EiDeassignBase.h
/// @brief 代入文を表すクラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release

#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbDeassignBase EiDeassignBase.h "ei/EiDeassignBase.h"
/// @brief deassign/release の基底クラス
//////////////////////////////////////////////////////////////////////
class EiDeassignBase :
  public EiStmtBase
{
protected:

  /// @brief コンストラクタ
  EiDeassignBase(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs	      ///< [in] 左辺の式
  ) : EiStmtBase(parent, process, ast_stmt),
      mLhs{lhs}
  {
  }

  /// @brief デストラクタ
  ~EiDeassignBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺を返す．
  const VlExpr*
  lhs() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺
  const VlExpr* mLhs;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDEASSIGNBASE_H
