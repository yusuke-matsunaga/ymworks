#ifndef CPTOPR_ADD_H
#define CPTOPR_ADD_H

/// @file CptOpr_Add.h
/// @brief CptOpr_Add のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Add CptOpr_Add.h "CptOpr_Add.h"
/// @brief Add 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Add :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Add(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Add() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_ADD_H
