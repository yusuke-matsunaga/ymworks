#ifndef CPTOPR_EQ_H
#define CPTOPR_EQ_H

/// @file CptOpr_Eq.h
/// @brief CptOpr_Eq のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Eq CptOpr_Eq.h "CptOpr_Eq.h"
/// @brief Eq 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Eq :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Eq(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Eq() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_EQ_H
