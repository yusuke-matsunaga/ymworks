#ifndef CPTOPR_NEGEDGE_H
#define CPTOPR_NEGEDGE_H

/// @file CptOpr_Negedge.h
/// @brief CptOpr_Negedge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Negedge CptOpr_Negedge.h "CptOpr_Negedge.h"
/// @brief Negedge 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Negedge :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_Negedge(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_Negedge() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_NEGEDGE_H
