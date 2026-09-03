#ifndef CPTOPR_CASENEQ_H
#define CPTOPR_CASENEQ_H

/// @file CptOpr_CaseNeq.h
/// @brief CptOpr_CaseNeq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_CaseNeq CptOpr_Neq.h "CptOpr_Neq.h"
/// @brief CaseNeq 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_CaseNeq :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_CaseNeq(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_CaseNeq() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_CASENEQ_H
