#ifndef EICASEITEM_H
#define EICASEITEM_H

/// @file EiCaseItem.h
/// @brief EiCaseItem のヘッダファイル
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

#include "ym/vl/VlStmt.h"
#include "ym/vl/AstCaseItem.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCaseItem EiCaseItem.h "ei/EiCaseItem.h"
/// @brief case item を表すクラス
/// IEEE Std 1364-2001 26.6.36 Case
//////////////////////////////////////////////////////////////////////
class EiCaseItem :
  public VlCaseItem
{
public:

  /// @brief コンストラクタ
  EiCaseItem(
    const AstCaseItem& ast_item,             ///< [in] パース木の caseitem 要素
    const std::vector<ElbExpr*>& label_list, ///< [in] ラベルを表す式のリスト
    const VlStmt* body                       ///< [in] 本体のステートメント
  ) : mAstCaseItem{ast_item},
      mExprList{label_list},
      mBodyStmt{body}
  {
  }

  /// @brief デストラクタ
  ~EiCaseItem() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;

  /// @brief ファイル位置を返す．
  FileRegion
  file_region() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlCaseItem に固有の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件式のリストの要素数を返す．
  SizeType
  expr_num() const override;

  /// @brief 条件式を返す．
  const VlExpr*
  expr(
    SizeType pos ///< [in] 位置番号
  ) const override;

  /// @brief 条件式のリストを返す．
  std::vector<const VlExpr*>
  expr_list() const override;

  /// @brief 本体のステートメントを返す．
  const VlStmt*
  body_stmt() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の caseitem 定義
  AstCaseItem mAstCaseItem;

  // ラベルのリスト
  std::vector<ElbExpr*> mExprList;

  // 本文
  const VlStmt* mBodyStmt;

};

END_NAMESPACE_YM_VERILOG

#endif // EICASEITEM_H
