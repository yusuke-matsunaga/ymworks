#ifndef CPTOPR_NOT_H
#define CPTOPR_NOT_H

/// @file CptOpr_UnaryNot.h
/// @brief CptOpr_UnaryNot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Not CptOpr_Not.h "CptOpr_Not.h"
/// @brief Not 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Not :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_Not(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_Not() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_NOT_H
