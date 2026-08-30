#ifndef CPTOPR_ARITHLSHIFT_H
#define CPTOPR_ARITHLSHIFT_H

/// @file CptOpr_ArithLShift.h
/// @brief CptOpr_ArithLShift のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_ArithLShift CptOpr_ArithLShift.h "CptOpr_ArithLShift.h"
/// @brief ArithLShift 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_ArithLShift :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_ArithLShift(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_ArithLShift() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_ARITHLSHIFT_H
