#ifndef EICASESTMT_H
#define EICASESTMT_H

/// @file EiCondStmt.h
/// @brief EiCondStmt のヘッダファイル
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
//#include "ym/vl/AstCaseItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCaseStmt EiCaseStmt.h "ei/EiCaseStmt.h"
/// @brief case statement を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class EiCaseStmt :
  public EiStmtBase
{
public:

  /// @brief コンストラクタ
  EiCaseStmt(
    const VlScope* parent,        ///< [in] 親のスコープ
    const VlProcess* process,     ///< [in] 親のプロセス (or nullptr)
    const AstStmt& ast_stmt,      ///< [in] パース木のステートメント定義
    const VlExpr* expr,           ///< [in] 条件式
    const std::vector<const VlCaseItem*>& item_list ///< [in] caseitem のリスト
  ) : EiStmtBase(parent, process, ast_stmt),
      mCondition{expr},
      mCaseItemList{item_list}
  {
  }

  /// @brief デストラクタ
  ~EiCaseStmt() = default;


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

  /// @brief case type を返す．
  VpiCaseType
  case_type() const override;

  /// @brief 条件式を返す．
  const VlExpr*
  expr() const override;

  /// @brief case item のリストの要素数を返す．
  SizeType
  caseitem_num() const override;

  /// @brief case item を返す．
  const VlCaseItem*
  caseitem(
    SizeType pos ///< [in] 位置番号
  ) const override;

  /// @brief case item のリストを返す．
  std::vector<const VlCaseItem*>
  caseitem_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  const VlExpr* mCondition;

  // case item のリスト
  std::vector<const VlCaseItem*> mCaseItemList;

};

END_NAMESPACE_YM_VERILOG

#endif // EICASESTMT_H
