#ifndef EIRELEASESTMT_H
#define EIRELEASESTMT_H

/// @file EiReleaseStmt.h
/// @brief EiReleaseStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.28 Assignment
// IEEE Std 1364-2001 26.6.29 Delay control
// IEEE Std 1364-2001 26.6.30 Event control
// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release

#include "EiDeassignBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiReleaseStmt EiReleaseStmt.h "ei/EiReleaseStmt.h"
/// @brief release statement を表すクラス
/// IEEE Std 1364-2001 26.6.37 Assign statement, deassign, force, release
//////////////////////////////////////////////////////////////////////
class EiReleaseStmt :
  public EiDeassignBase
{
public:

  /// @brief コンストラクタ
  EiReleaseStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs	      ///< [in] 左辺の式
  ) : EiDeassignBase(parent, process, ast_stmt, lhs)
  {
  }

  /// @brief デストラクタ
  ~EiReleaseStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIRELEASESTMT_H
