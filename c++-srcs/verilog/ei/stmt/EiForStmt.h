#ifndef EIFORSTMT_H
#define EIFORSTMT_H

/// @file EiForStmt.h
/// @brief EiForStmt のヘッダファイル
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
/// @class EiForStmt EiForStmt.h "ei/EiForStmt.h"
/// @brief for statement を表すクラス
/// IEEE Std 1364-2001 26.6.33 For
//////////////////////////////////////////////////////////////////////
class EiForStmt :
  public EiLoopStmt
{
public:

  /// @brief コンストラクタ
  EiForStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* cond,	      ///< [in] 条件式
    const VlStmt* init_stmt,  ///< [in] 初期化式
    const VlStmt* inc_stmt,   ///< [in] 増加式
    const VlStmt* stmt        ///< [in] 本体のステートメント
  ) : EiLoopStmt(parent, process, ast_stmt, cond, stmt),
      mInitStmt{init_stmt},
      mIncStmt{inc_stmt}
  {
  }

  /// @brief デストラクタ
  ~EiForStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// 初期化文を返す．
  const VlStmt*
  init_stmt() const override;

  /// 繰り返し文を返す．
  const VlStmt*
  inc_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 初期化文
  const VlStmt* mInitStmt;

  // 増加文
  const VlStmt* mIncStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFORSTMT_H
