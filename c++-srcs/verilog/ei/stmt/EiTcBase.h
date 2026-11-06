#ifndef EITCBASE_H
#define EITCBASE_H

/// @file EiTcBase.h
/// @brief EiTcBase のヘッダファイル
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
/// @class EiTcBase ElbTcBase.h "ElbTcBase.h"
/// @brief task call / system task call の基底クラス
//////////////////////////////////////////////////////////////////////
class EiTcBase :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiTcBase(
    const VlScope* parent,                 ///< [in] 親のスコープ
    const VlProcess* process,              ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,               ///< [in] パース木のステートメント定義
    const std::vector<ElbExpr*>& arg_array ///< [in] 引数を納める配列
  ) : EiStmtBase(parent, process, ast_stmt),
      mArgumentList{arg_array}
  {
  }

  /// @brief デストラクタ
  ~EiTcBase() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  SizeType
  argument_num() const override;

  /// @brief 引数の取得
  const VlExpr*
  argument(
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < argument_num() )
  ) const override;

  /// @brief 引数のリストの取得
  std::vector<const VlExpr*>
  argument_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の設定
  void
  set_argument(
    SizeType pos, ///< [in] 位置番号
    ElbExpr* arg  ///< [in] 設定する引数
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数のリスト
  std::vector<ElbExpr*> mArgumentList;

};

END_NAMESPACE_YM_VERILOG

#endif // EITCBASE_H
