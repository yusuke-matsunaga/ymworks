#ifndef EINULLSTMT_H
#define EINULLSTMT_H

/// @file EiNullStmt.h
/// @brief EiNullStmt のヘッダファイル
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
/// @class EiNullStmt ElbNullStmt.h "ElbNullStmt.h"
/// @brief null statement を表すクラス
//////////////////////////////////////////////////////////////////////
class EiNullStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiNullStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const AstStmt& ast_stmt   ///< [in]	パース木のステートメント定義
  ) : EiStmtBase(parent, process, ast_stmt)
  {
  }

  /// @brief デストラクタ
  ~EiNullStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EINULLSTMT_H
