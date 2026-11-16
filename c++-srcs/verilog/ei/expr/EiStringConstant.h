#ifndef EISTRINGCONSTANT_H
#define EISTRINGCONSTANT_H

/// @file EiStringConstant.h
/// @brief EiStringConstant のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstant.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiStringConst EiStringConstant.h "EiStringConstant.h"
/// @brief 文字列型の定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiStringConst :
  public EiConstant
{
public:

  /// @brief コンストラクタ
  EiStringConst(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const std::string& value ///< [in] 値
  ) : EiConstant(ast_expr),
      mValue(value)
  {
  }

  /// @brief デストラクタ
  ~EiStringConst() = default;


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
  BitVector mValue;

};

END_NAMESPACE_YM_VERILOG

#endif // EISTRINGCONSTANT_H
