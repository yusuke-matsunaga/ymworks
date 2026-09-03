#ifndef CPTOPR_GT_H
#define CPTOPR_GT_H

/// @file CptOpr_Gt.h
/// @brief CptOpr_Gt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Gt CptOpr_Gt.h "CptOpr_Gt.h"
/// @brief Gt 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Gt :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Gt(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Gt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_GT_H
