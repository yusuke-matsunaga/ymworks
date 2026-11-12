#ifndef EIASSIGNSTMT_H
#define EIASSIGNSTMT_H

/// @file EiAssignStmt.h
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


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiAssignStmt EiAssignStmt.h "ei/EiAssignStmt.h"
/// @brief assign statement を表すクラス
/// assignment とは別物
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiAssignStmt :
  public EiAssignBase
{
public:

  /// @brief コンストラクタ
  EiAssignStmt(
    const VlScope* parent,     ///< [in] 親のスコープ
    const VlProcess* process,  ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,   ///< [in] パース木のステートメント定義
    const VlTerm* lhs,	       ///< [in] 左辺の式
    const VlExpr* rhs	       ///< [in] 右辺の式
  ) : EiAssignBase(parent, process, ast_stmt, lhs, rhs)
  {
  }

  /// @brief デストラクタ
  ~EiAssignStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIASSIGNSTMT_H
