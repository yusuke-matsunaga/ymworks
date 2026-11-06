#ifndef EIREDUCTIONOP_H
#define EIREDUCTIONOP_H

/// @file EiReductionOp.h
/// @brief EiReductionOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiUnaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiReductionOp EiReductionOp.h "EiReductionOp.h"
/// @note リダクション演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiReductionOp :
  public EiUnaryOp
{
public:

  /// @brief コンストラクタ
  EiReductionOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1            ///< [in] オペランド
  );

  /// @brief デストラクタ
  ~EiReductionOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIREDUCTIONOP_H
