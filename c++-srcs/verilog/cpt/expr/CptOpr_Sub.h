#ifndef CPTOPR_SUB_H
#define CPTOPR_SUB_H

/// @file CptOpr_Sub.h
/// @brief CptOpr_Sub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Sub CptOpr_Sub.h "CptOpr_Sub.h"
/// @brief Sub 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Sub :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_Sub(
    const PtExpr* opr1,
    const PtExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_Sub() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_SUB_H
