#ifndef CPTOPR_MINTYPEMAX_H
#define CPTOPR_MINTYPEMAX_H

/// @file CptOpr_MinTypMax.h
/// @brief CptOpr_MinTypeMax のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr3.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_MinTypMax CptOpr_MinTypeMax.h "CptOpr_MinTypMax.h"
/// @brief MinTypMax 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_MinTypMax :
  public CptOpr3
{
public:

  /// @brief コンストラクタ
  CptOpr_MinTypMax(
    const AstExpr* opr1,
    const AstExpr* opr2,
    const AstExpr* opr3
  ) : CptOpr3(opr1, opr2, opr3)
  {
  }

  // デストラクタ
  ~CptOpr_MinTypMax() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_MINTYPMAX_H
