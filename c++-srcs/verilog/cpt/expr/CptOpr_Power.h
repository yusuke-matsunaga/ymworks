#ifndef CPTOPR_POWER_H
#define CPTOPR_POWER_H

/// @file CptOpr_Power.h
/// @brief CptOpr_Power のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Power CptOpr_Power.h "CptOpr_Power.h"
/// @brief Power 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Power :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Power(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Power() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_POWER_H
