#ifndef CPTOPR_CONDITION_H
#define CPTOPR_CONDITION_H

/// @file CptOpr_Condition.h
/// @brief CptOpr_Condition のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr3.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Condition CptOpr_Condition.h "CptOpr_Condition.h"
/// @brief Condition 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Condition :
  public CptOpr3
{
public:

  /// @brief コンストラクタ
  CptOpr_Condition(
    const PtExpr* opr1,
    const PtExpr* opr2,
    const PtExpr* opr3
  ) : CptOpr3(opr1, opr2, opr3)
  {
  }

  // デストラクタ
  ~CptOpr_Condition() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_CONDITION_H
