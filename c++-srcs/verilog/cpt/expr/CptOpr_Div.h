#ifndef CPTOPR_DIV_H
#define CPTOPR_DIV_H

/// @file CptOpr_Div.h
/// @brief CptOpr_Div のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Div CptOpr_Div.h "CptOpr_Div.h"
/// @brief Div 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Div :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Div(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Div() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_DIV_H
