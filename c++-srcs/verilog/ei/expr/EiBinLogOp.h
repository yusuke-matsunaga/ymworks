#ifndef EIBINLOGOP_H
#define EIBINLOGOP_H

/// @file EiBinLogOp.h
/// @brief EiExpr の2項論理演算子の派生クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiBinaryOp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBinLogOp EiBinLogOp.h "EiBinLogOp.h"
/// @brief 2項論理演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class EiBinLogOp :
  public EiBinaryOp
{
public:

  /// @brief コンストラクタ
  EiBinLogOp(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* opr1,           ///< [in] オペランド1
    ElbExpr* opr2            ///< [in] オペランド2
  ) : EiBinaryOp(ast_expr, opr1, opr2)
  {
    // 論理演算の場合はオペランドも1ビットスカラーのはずだが
    // 仕様書には max(L(i), L(j)) なんて書いてある．
    // これは == などと一緒に表記しているからだと思う．
    // 実際には self determined で計算しておいて
    // 評価時に true/false/X に分類する．
    opr1->set_selfsize();
    opr2->set_selfsize();
  }

  /// @brief デストラクタ
  ~EiBinLogOp() = default;


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

#endif // EIBINLOGOP_H
