#ifndef EITASKCALL_H
#define EITASKCALL_H

/// @file EiTaskCall.h
/// @brief EiTaskCall のヘッダファイル
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
/// @class EiTaskCall ElbTaskCall.h "ElbTaskCall.h"
/// @brief task call を表すクラス
//////////////////////////////////////////////////////////////////////
class EiTaskCall :
  public EiTcBase
{
public:

  /// @brief コンストラクタ
  EiTaskCall(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,               ///< [in] パース木のステートメント定義
    const VlTaskFunc* task,                ///< [in] 対象のタスク
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数を納める配列
  ) : EiTcBase(parent, process, ast_stmt, arg_array),
      mTask{task}
  {
  }

  /// @brief デストラクタ
  ~EiTaskCall() = default;


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

  /// @brief task の実体を返す．
  const VlTaskFunc*
  task() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タスク
  const VlTaskFunc* mTask;

};

END_NAMESPACE_YM_VERILOG

#endif // EITASKCALL_H
