#ifndef CPTOPR_UNARYAND_H
#define CPTOPR_UNARYAND_H

/// @file CptOpr_UnaryAnd.h
/// @brief CptOpr_UnaryANd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptOpr1.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr_UnaryAnd CptOpr_UnaryAnd.h "CptOpr_UnaryAnd.h"
/// @brief UnaryAnd 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class CptOpr_UnaryAnd :
  public CptOpr1
{
public:

  /// @brief コンストラクタ
  CptOpr_UnaryAnd(
    const FileRegion& file_region,
    const AstExpr* opr
  ) : CptOpr1(file_region, opr)
  {
  }

  // デストラクタ
  ~CptOpr_UnaryAnd() {}


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算子のトークン番号を得る．
  VpiOpType
  op_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_UNARYAND_H
