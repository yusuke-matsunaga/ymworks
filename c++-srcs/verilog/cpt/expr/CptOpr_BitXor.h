#ifndef CPTOPR_BITXOR_H
#define CPTOPR_BITXOR_H

/// @file CptOpr_BitXor.h
/// @brief CptOpr_BitXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_BitXor CptOpr_BitXor.h "CptOpr_BitXor.h"
/// @brief BitXor 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_BitXor :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_BitXor(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_BitXor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_BITXOR_H
