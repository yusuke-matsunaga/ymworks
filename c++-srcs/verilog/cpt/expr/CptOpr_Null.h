#ifndef CPTOPR_NULL_H
#define CPTOPR_NULL_H

/// @file CptOpr_Null.h
/// @brief CptOpr_Null のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Null CptOpr_Null.h "CptOpr_Null.h"
/// @brief Null 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Null :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_Null(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_Null() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_NULL_H
