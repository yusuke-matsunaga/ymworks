#ifndef CPTOPR_RSHIFT_H
#define CPTOPR_RSHIFT_H

/// @file CptOpr_RShift.h
/// @brief CptOpr_Rshift のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_RShift CptOpr_RShift.h "CptOpr_RShift.h"
/// @brief RShift 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_RShift :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_RShift(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_RShift() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_RSHIFT_H
