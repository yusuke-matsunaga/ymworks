#ifndef CPTOPR_MOD_H
#define CPTOPR_MOD_H

/// @file CptOpr_Mod.h
/// @brief CptOpr_Mod のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Mod CptOpr_Mode.h "CptOpr_Mod.h"
/// @brief Mod 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Mod :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Mod(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Mod() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_MOD_H
