#ifndef CPTOPR_LSHIFT_H
#define CPTOPR_LSHIFT_H

/// @file CptOpr_LShift.h
/// @brief CptOpr_LShift のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_LShift CptOpr_LShift.h "CptOpr_LShft.h"
/// @brief LShift 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_LShift :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_LShift(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_LShift() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_LSHIFT_H
