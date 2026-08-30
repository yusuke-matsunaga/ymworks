#ifndef CPTOPR_LE_H
#define CPTOPR_LE_H

/// @file CptOpr_Le.h
/// @brief CptOpr_Le のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Le CptOpr_Le.h "CptOpr_le.h"
/// @brief Le 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Le :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Le(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Le() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_LE_H
