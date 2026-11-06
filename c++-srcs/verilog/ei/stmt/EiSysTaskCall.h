#ifndef EISYSTASKCALL_H
#define EISYSTASKCALL_H

/// @file EiSysTaskCall.h
/// @brief EiSysTaskCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

// ステートメントを表すクラス
// IEEE Std 1364-2001 26.6.27 Process, block, statement, event statement
// IEEE Std 1364-2001 26.6.38 Disable

#include "EiTcBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiSysTaskCall ElbSysTaskCall.h "ElbSysTaskCall.h"
/// @brief system task call を表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysTaskCall :
  public EiTcBase
{
public:

  /// @brief コンストラクタ
  EiSysTaskCall(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,               ///< [in] パース木のステートメント定義
    const VlUserSystf* user_systf,         ///< [in] システムタスク
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数を納める配列
  ) : EiTcBase(parent, process, ast_stmt, arg_array),
      mUserSystf{user_systf}
  {
  }

  /// @brief デストラクタ
  ~EiSysTaskCall() = default;


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

  /// @brief user systf クラスへのポインタを返す．
  const VlUserSystf*
  user_systf() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // user systf へのポインタ
  const VlUserSystf* mUserSystf;

};

END_NAMESPACE_YM_VERILOG

#endif // EISYSTASKCALL_H
