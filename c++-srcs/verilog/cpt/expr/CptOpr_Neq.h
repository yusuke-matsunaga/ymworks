#ifndef CPTOPR_NEQ_H
#define CPTOPR_NEQ_H

/// @file CptOpr_Neq.h
/// @brief CptOpr_Neq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Neq CptOpr_Neq.h "CptOpr_Neq.h"
/// @brief Neq 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Neq :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Neq(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Neq() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_NEQ_H
