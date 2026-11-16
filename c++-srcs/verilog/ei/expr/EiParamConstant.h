#ifndef EIPARAMCONSTANT_H
#define EIPARAMCONSTANT_H

/// @file EiParamConstant.h
/// @brief EiParamConstant のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstant.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiParamConstant EiParamConstant.h "EiParamConstant.h"
/// @brief パラメータの定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiParamConstant :
  public EiConstant
{
public:

  /// @brief コンストラクタ
  EiParamConstant(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbParameter* obj        ///< [in] 本体のオブジェクト
  ) : EiConstant(ast_expr),
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiParamConstant() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  VpiObjType
  type() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  VlValueType
  value_type() const override;

  /// @brief 定数値を返す．
  VlValue
  constant_value() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  ElbParameter* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPARAMCONSTANT_H
