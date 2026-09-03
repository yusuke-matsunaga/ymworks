#ifndef CPTOPR2_H
#define CPTOPR2_H

/// @file CptOpr2.h
/// @brief CptOpr2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr2 CptOpr2.h "CptOpr2.h"
/// @brief 二項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr2 :
  public CptOpr
{
public:

  // コンストラクタ
  CptOpr2(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : mOpr{opr1, opr2}
  {
    if ( opr1 == nullptr ) {
      throw std::logic_error{"opr1 == nullptr"};
    }
    if ( opr2 == nullptr ) {
      throw std::logic_error{"opr2 == nullptr"};
    }
  }

  // デストラクタ
  ~CptOpr2() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // ファイル位置を返す．
  FileRegion
  file_region() const override;

  /// @brief 固定オペランド数の取得
  SizeType
  operand_num() const override;

  /// @brief 0番目のオペランドの取得
  const PtExpr*
  operand0() const override;

  /// @brief 1番目のオペランドの取得
  const PtExpr*
  operand1() const override;

  /// @brief インデックスとして使える式のチェック
  /// @retval true 階層名の添字として使える式
  /// @retval false 使えない式
  bool
  is_index_expr() const override;

  /// @brief インデックスの値の取得
  /// @return 階層名の添字として使える式の時にその値を返す．
  int
  index_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  const PtExpr* mOpr[2];

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_H
