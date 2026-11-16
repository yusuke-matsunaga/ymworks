#ifndef EIFORCESTMT_H
#define EIFORCESTMT_H

/// @file EiForceStmt.h
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

#include "EiAssignBase.h"
#include "elaborator/ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiForceStmt EiForceStmt.h "ei/EiForceStmt.h"
/// @brief force statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiForceStmt :
  public EiAssignBase
{
public:

  /// @brief コンストラクタ
  EiForceStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs,	      ///< [in] 左辺の式
    const VlExpr* rhs	      ///< [in] 右辺の式
  ) : EiAssignBase(parent, process, ast_stmt, lhs, rhs)
  {
  }

  /// @brief デストラクタ
  ~EiForceStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFORCESTMT_H
