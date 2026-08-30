#ifndef CPTOPR_UNARYXNOR_H
#define CPTOPR_UNARYXNOR_H

/// @file CptOpr_UnaryXNor.h
/// @brief CptOpr_UnaryXNor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_UnaryXNor CptOpr_UnaryXNor.h "CptOpr_UnaryXNor.h"
/// @brief UnaryXNor 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_UnaryXNor :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_UnaryXNor(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_UnaryXNor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_UNARYXNOR_H
