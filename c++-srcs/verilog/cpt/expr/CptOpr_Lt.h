#ifndef CPTOPR_LT_H
#define CPTOPR_LT_H

/// @file CptOpr_Lt.h
/// @brief CptOpr_Lt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Lt CptOpr_Lt.h "CptOpr_Lt.h"
/// @brief Lt 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Lt :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Lt(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Lt() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_LT_H
