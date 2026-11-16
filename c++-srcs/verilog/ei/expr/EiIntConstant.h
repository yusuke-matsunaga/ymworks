#ifndef EIINTCONSTANT_H
#define EIINTCONSTANT_H

/// @file EiIntConstant.h
/// @brief EiIntConstant のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstant.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiIntConst EiIntConstant.h "EiIntConstant.h"
/// @brief 整数型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiIntConst :
  public EiConstant
{
public:

  /// @brief コンストラクタ
  EiIntConst(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    std::int32_t value       ///< [in] 値
  ) : EiConstant(ast_expr),
      mValue{value}
  {
  }

  /// @brief デストラクタ
  ~EiIntConst() = default;


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

  // 値
  std::int32_t mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // EIINTCONSTANT_H
