#ifndef EICOMPAREOP_H
#define EICOMPAREOP_H

/// @file EiCompareOp.h
/// @brief EiExpr の比較演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiBinaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiCompareOp EiCompareOp.h "EiCompareOp.h"
/// @brief 比較演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiCompareOp :
  public EiBinaryOp
{
public:

  /// @brief コンストラクタ
  EiCompareOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,           ///< [in] オペランド1
    ElbExpr* opr2            ///< [in] オペランド2
  ) : EiBinaryOp(ast_expr, opr1, opr2)
  {
    // 比較演算は大きい方の型を用いる．
    auto type1 = opr1->value_type();
    auto type2 = opr2->value_type();

    mOprType = calc_type(type1, type2);

    opr1->set_reqsize(mOprType);
    opr2->set_reqsize(mOprType);
  }

  /// @brief デストラクタ
  ~EiCompareOp() = default;


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランドの型
  VlValueType mOprType;

};

END_NAMESPACE_YM_VERILOG

#endif // EICOMPAREOP_H
