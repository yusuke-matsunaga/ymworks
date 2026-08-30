#ifndef CPTOPR_BITOR_H
#define CPTOPR_BITOR_H

/// @file CptOpr_BitOr.h
/// @brief CptOpr_BitOr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_BitOr CptOpr_BitOr.h "CptOpr_BitOr.h"
/// @brief BitOr 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_BitOr :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_BitOr(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_BitOr() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_BITOR_H
