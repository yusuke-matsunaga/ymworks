#ifndef CPTOPR_BITAND_H
#define CPTOPR_BITAND_H

/// @file CptOpr_BitAnd.h
/// @brief CptOpr_BitAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr2.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_BitAnd CptOpr_BitAnd.h "CptOpr_BitAnd.h"
/// @brief BitAnd 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_BitAnd :
  public CptOpr2
{
public:

  /// @brief コンストラクタ
  CptOpr_BitAnd(
    const AstExpr* opr1,
    const AstExpr* opr2
  ) : CptOpr2(opr1, opr2)
  {
  }

  // デストラクタ
  ~CptOpr_BitAnd() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_BITAND_H
