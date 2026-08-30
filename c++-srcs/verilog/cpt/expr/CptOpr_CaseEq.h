#ifndef CPTOPR_CASEEQ_H
#define CPTOPR_CASEEQ_H

/// @file CptOpr_CaseEq.h
/// @brief CptOpr_CaseEq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_CaseEq CptOpr_CaseEq.h "CptOpr_CaseEq.h"
/// @brief CaseEq 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_CaseEq :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_CaseEq(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_CaseEq() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_CASEEQ_H
