#ifndef CPTOPR_BITXNOR_H
#define CPTOPR_BITXNOR_H

/// @file CptOpr_BitXNor.h
/// @brief CptOpr_BitXNor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_BitXNor CptOpr_BitXNor.h "CptOpr_BitXNor.h"
/// @brief BitXNor 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_BitXNor :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_BitXNor(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_BitXNor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_BITXNOR_H
