#ifndef CPTOPR_UNARYOR_H
#define CPTOPR_UNARYOR_H

/// @file CptOpr_UnaryOr.h
/// @brief CptOpr_UnaryOr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_UnaryOr CptOpr_UnaryOr.h "CptOpr_UnaryOr.h"
/// @brief UnaryOr 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_UnaryOr :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_UnaryOr(
    const FileRegion& file_region,
    const PtExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_UnaryOr() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_UNARYOR_H
