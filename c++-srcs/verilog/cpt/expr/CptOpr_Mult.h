#ifndef CPTOPR_MULT_H
#define CPTOPR_MULT_H

/// @file CptOpr_Mult.h
/// @brief CptOpr_Mult のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Mult CptOpr_Mult.h "CptOpr_Mult.h"
/// @brief Mult 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Mult :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Mult(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Mult() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_MULT_H
