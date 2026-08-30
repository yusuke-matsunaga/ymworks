#ifndef CPTOPR_UNARYNOR_H
#define CPTOPR_UNARYNOR_H

/// @file CptOpr_UnaryNor.h
/// @brief CptOpr_UnaryNor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_UnaryNor CptOpr_UnaryNor.h "CptOpr_UnaryNor.h"
/// @brief UnaryNor 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_UnaryNor :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_UnaryNor(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_UnaryNor() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_UNARYNOR_H
