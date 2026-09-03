#ifndef CPTOPR3_H
#define CPTOPR3_H

/// @file CptOpr3.h
/// @brief CptOpr3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr3 CptOpr3.h "CptOpr3.h"
/// @brief 三項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr3 :
  public CptOpr
{
public:

  /// @brief コンストラクタ
  CptOpr3(
    const PtExpr* opr1,
    const PtExpr* opr2,
    const PtExpr* opr3
  ) : mOpr{opr1, opr2, opr3}
  {
    if ( opr1 == nullptr ) {
      throw std::logic_error{"opr1 == nullptr"};
    }
    if ( opr2 == nullptr ) {
      throw std::logic_error{"opr2 == nullptr"};
    }
    if ( opr3 == nullptr ) {
      throw std::logic_error{"opr3 == nullptr"};
    }
  }

  // デストラクタ
  ~CptOpr3() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル位置を返す．
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

  /// @brief 2番目のオペランドの取得
  const PtExpr*
  operand2() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  const PtExpr* mOpr[3];

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR3_H
