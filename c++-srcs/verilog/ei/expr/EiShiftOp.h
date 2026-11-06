#ifndef EISHIFTOP_H
#define EISHIFTOP_H

/// @file EiShiftOp.h
/// @brief EiExpr のシフト演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiBinaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiShiftOp EiShiftOp.h "EiShiftOp.h"
/// @brief シフト演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiShiftOp :
  public EiBinaryOp
{
public:

  /// @brief コンストラクタ
  EiShiftOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,	     ///< [in] オペランド1
    ElbExpr* opr2	     ///< [in] オペランド2
  ) : EiBinaryOp(ast_expr, opr1, opr2)
  {
    // シフト演算子は第1オペランドの型とサイズをそのまま引き継ぐ
    mType = opr1->value_type();
    if ( mType.is_real_type() ) {
      throw std::logic_error{"mType.is_real_type()"};
    }
    if ( opr2->value_type().is_real_type() ) {
      throw std::logic_error{"opr2->value_type().is_real_type()"};
    }

    // 第2オペランドのサイズは self determined
    opr2->set_selfsize();
  }

  /// @brief デストラクタ
  ~EiShiftOp() = default;


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

#endif // EISHIFTOP_H
