#ifndef CPTOPR_UNARYXOR_H
#define CPTOPR_UNARYXOR_H

/// @file CptOpr_UnaryXor.h
/// @brief CptOpr_UnaryXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_UnaryXor CptOpr_UnaryXor.h "CptOpr_UnaryXor.h"
/// @brief UnaryXor 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_UnaryXor :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_UnaryXor(
    const FileRegion& file_region,
    const PtExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_UnaryXor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_UNARYXOR_H
