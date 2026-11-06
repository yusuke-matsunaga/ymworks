#ifndef EINOTOP_H
#define EINOTOP_H

/// @file EiNotOp.h
/// @brief EiNotOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiUnaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiNotOp EiNotOp.h "EiNotOp.h"
/// @note 論理否定演算子
//////////////////////////////////////////////////////////////////////
class EiNotOp :
  public EiUnaryOp
{
public:

  /// @brief コンストラクタ
  EiNotOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1            ///< [in] オペランド
  );

  /// @brief デストラクタ
  ~EiNotOp();


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

#endif // EINOTOP_H
