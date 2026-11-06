#ifndef EIFOREVERSTMT_H
#define EIFOREVERSTMT_H

/// @file EiForeverStmt.h
/// @brief EiForeverStmt のヘッダファイル
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

#include "EiStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiForeverStmt EiForeverStmt.h "ei/EiForeverStmt.h"
/// @brief forever statement を表すクラス
/// IEEE Std 1364-2001 26.6.34 Forever
//////////////////////////////////////////////////////////////////////
class EiForeverStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiForeverStmt(
    const VlScope* parent,    ///< [in] 親のスコープ
    const VlProcess* process, ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,  ///< [in] パース木のステートメント定義
    const VlStmt* stmt	      ///< [in] 本体のステートメント
  ) : EiStmtBase(parent, process, ast_stmt),
      mBodyStmt{stmt}
  {
  }

  /// @brief デストラクタ
  ~EiForeverStmt() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlStmt に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const VlStmt* mBodyStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EIFOREVERSTMT_H
