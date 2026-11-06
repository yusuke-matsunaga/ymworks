#ifndef EIBINARITHOP_H
#define EIBINARITHOP_H

/// @file EiBinArithOp.h
/// @brief EiExpr の2項算術演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiBinaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBinArithOp EiBinArithOp.h "EiBinArithOp.h"
/// @brief 2項算術演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinArithOp :
  public EiBinaryOp
{
public:

  /// @brief コンストラクタ
  EiBinArithOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,	     ///< [in] オペランド1
    ElbExpr* opr2	     ///< [in] オペランド2
  ) : EiBinaryOp(ast_expr, opr1, opr2)
  {
    // オペランドのサイズの大きい方で決まる．
    auto type1 = opr1->value_type();
    auto type2 = opr2->value_type();

    mType = calc_type(type1, type2);
  }

  /// @brief デストラクタ
  ~EiBinArithOp() = default;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の型
  VlValueType mType;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBINARITHOP_H
