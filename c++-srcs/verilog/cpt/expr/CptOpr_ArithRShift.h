#ifndef CPTOPR_ARITHRSHIFT_H
#define CPTOPR_ARITHRSHIFT_H

/// @file CptOpr_ArithRShift.h
/// @brief CptOpr_ArithRShift のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_ArithRShift CptOpr_ArithRShift.h "CptOpr_ArithRShift.h"
/// @brief ArithRShift 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_ArithRShift :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_ArithRShift(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_ArithRShift() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_ARITHRSHIFT_H
