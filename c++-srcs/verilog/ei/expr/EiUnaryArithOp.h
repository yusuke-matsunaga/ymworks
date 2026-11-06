#ifndef EIUNARYARITHOP_H
#define EIUNARYARITHOP_H

/// @file EiUnaryArithOp.h
/// @brief EiUnaryArithOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiUnaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiUnaryArithOp EiUnaryOp.h "EiUnaryOp.h"
/// @brief 単項算術演算の派生クラス
//////////////////////////////////////////////////////////////////////
class EiUnaryArithOp :
  public EiUnaryOp
{
public:

  /// @brief コンストラクタ
  EiUnaryArithOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1 	     ///< [in] オペランド
  );

  /// @brief デストラクタ
  ~EiUnaryArithOp();


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIUNARYARITHOP_H
