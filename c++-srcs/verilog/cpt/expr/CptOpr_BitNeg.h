#ifndef CPTOPR_BITNEG_H
#define CPTOPR_BITNEG_H

/// @file CptOpr_BitNeg.h
/// @brief CptOpr_BitNeg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_BitNeg CptOpr_BitNeg.h "CptOpr_BitNeg.h"
/// @brief BitNeg 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_BitNeg :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_BitNeg(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_BitNeg() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_BITNEG_H
