#ifndef EIDISABLESTMT_H
#define EIDISABLESTMT_H

/// @file EiDisableStmt.h
/// @brief EiDisableStmt のヘッダファイル
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
/// @class EiDisableStmt ElbDisableStmt.h "ElbDisableStmt.h"
/// @brief disable statement を表すクラス
/// IEEE Std 1364-2001 26.6.38 Disable
//////////////////////////////////////////////////////////////////////
class EiDisableStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiDisableStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in]	親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in]	パース木のステートメント定義
    const VlScope* target     ///< [in] 対象のスコープ
  ) : EiStmtBase(parent, process, ast_stmt),
      mExpr{target}
  {
  }

  /// @brief デストラクタ
  ~EiDisableStmt() = default;


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

  /// @brief expr を返す．
  ///
  /// 型が VlScope になっているが実際には
  /// ElbFunction/ElbTask/EiNamedFork/EiNamedBegin
  /// のいずれか
  const VlScope*
  target_scope() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  const VlScope* mExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDISABLESTMT_H
