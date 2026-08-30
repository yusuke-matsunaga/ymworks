#ifndef CPTOPR_PLUS_H
#define CPTOPR_PLUS_H

/// @file CptOpr_Plus.h
/// @brief CptOpr_Plus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_Plus CptOpr_Plus.h "CptOpr_Plus.h"
/// @brief Plus 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_Plus :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_Plus(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_Plus() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_PLUS_H
