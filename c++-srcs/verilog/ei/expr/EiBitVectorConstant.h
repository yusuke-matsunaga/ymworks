#ifndef EIBITVECTORCONSTANT_H
#define EIBITVECTORCONSTANT_H

/// @file EiBitVectorConstant.h
/// @brief EiBitVectorConstant のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstant.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBitVectorConst EiBitVectorConstant.h "EiBitVectorConstant.h"
/// @brief ビットベクタ型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiBitVectorConst :
  public EiConstant
{
public:

  /// @brief コンストラクタ
  EiBitVectorConst(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    VpiConstType const_type, ///< [in] 定数型
    const BitVector& value   ///< [in] 値
  ) : EiConstant(ast_expr),
      mConstType{const_type},
      mValue{value}
  {
  }

  /// @brief デストラクタ
  ~EiBitVectorConst() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数の型を返す．
  VpiConstType
  constant_type() const override;

  /// @brief 定数値を返す．
  VlValue
  constant_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数の型
  VpiConstType mConstType;

  // 値
  BitVector mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBITVECTORCONSTANT_H
