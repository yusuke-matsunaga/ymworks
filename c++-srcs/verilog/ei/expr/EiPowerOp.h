#ifndef EIPOWEROP_H
#define EIPOWEROP_H

/// @file EiPowerOp.h
/// @brief EiExpr のべき乗演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiBinaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPowerOp EiPowerOp.h "EiPowerOp.h"
/// @brief べき乗算演算子
//////////////////////////////////////////////////////////////////////
class EiPowerOp :
  public EiBinaryOp
{
public:

  /// @brief コンストラクタ
  EiPowerOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,	     ///< [in] オペランド1
    ElbExpr* opr2	     ///< [in] オペランド2
  ) : EiBinaryOp(ast_expr, opr1, opr2)
  {
    // 巾乗演算の場合, どちらかのオペランドが real, integer, signed
    // なら結果は real, どちらも unsigned の時のみ unsigned となる．
    auto type1 = opr1->value_type();
    auto type2 = opr2->value_type();
    mType = calc_type2(type1, type2);

    // ただし opr2 は self-determined
    opr2->set_reqsize(type2);
  }

  /// @brief デストラクタ
  ~EiPowerOp() = default;


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

#endif // EIPOWEROP_H
