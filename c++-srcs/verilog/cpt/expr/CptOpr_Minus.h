#ifndef CPTOPR_MINUS_H
#define CPTOPR_MINUS_H

/// @file CptOpr_Minus.h
/// @brief CptOpr_Minus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Minus CptOpr_Minus.h "CptOpr_Minus.h"
/// @brief Minus 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Minus :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_Minus(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_Minus() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_MINUS_H
