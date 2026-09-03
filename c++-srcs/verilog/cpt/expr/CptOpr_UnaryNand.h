#ifndef CPTOPR_UNARYNAND_H
#define CPTOPR_UNARYNAND_H

/// @file CptOpr_UnaryNand.h
/// @brief CptOpr_UnaryNand のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_UnaryNand CptOpr_UnaryNand.h "CptOpr_UnaryNand.h"
/// @brief UnaryNand 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_UnaryNand :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_UnaryNand(
    const FileRegion& file_region,
    const PtExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_UnaryNand() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_UNARYNAND_H
