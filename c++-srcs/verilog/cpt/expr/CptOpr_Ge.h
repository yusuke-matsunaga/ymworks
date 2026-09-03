#ifndef CPTOPR_GE_H
#define CPTOPR_GE_H

/// @file CptOpr_Ge.h
/// @brief CptOpr_Ge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Ge CptOpr_Ge.h "CptOpr_Ge.h"
/// @brief Ge 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Ge :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Ge(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Ge() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_GE_H
