#ifndef EIBLKASSIGNMENT_H
#define EIBLKASSIGNMENT_H

/// @file EiBlkAssignment.h
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

#include "EiNbAssignment.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBlkAssignment EiBlkAssignment.h "ei/EiBlkAssignment.h"
/// @brief blocking assignment を表すクラス
/// IEEE Std 1364-2001 26.6.28 Assignment
//////////////////////////////////////////////////////////////////////
class EiBlkAssignment :
  public EiNbAssignment
{
public:

  /// @brief コンストラクタ
  EiBlkAssignment(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlExpr* lhs,	      ///< [in] 左辺の式
    const VlExpr* rhs,	      ///< [in] 右辺の式
    const VlControl* control  ///< [in] コントロール
  ) : EiNbAssignment(parent, process, ast_stmt, lhs, rhs, control)
  {
  }

  /// @brief デストラクタ
  ~EiBlkAssignment() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入のブロッキング/ノンブロッキングの区別の取得
  /// @note このクラスでは true を返す．
  bool
  is_blocking() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBLKASSIGNMENT_H
