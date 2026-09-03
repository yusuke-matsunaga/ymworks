#ifndef CPTOPR_H
#define CPTOPR_H

/// @file CptOpr.h
/// @brief CptOpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "CptExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class CptOpr CptOpr.h "CptOpr.h"
/// @brief 演算子のベース実装クラス
//////////////////////////////////////////////////////////////////////
class CptOpr :
  public CptExpr
{
protected:

  // コンストラクタ
  CptOpr() {}

  // デストラクタ
  ~CptOpr() {}


public:
  //////////////////////////////////////////////////////////////////////
  // PtExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  // クラスの型を返す．
  // このクラスの場合は kOpr を返す．
  AstExpr::Type
  type() const override;

  /// @brief 固定オペランド数の取得
  SizeType
  operand_num() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // CPTOPR_H
