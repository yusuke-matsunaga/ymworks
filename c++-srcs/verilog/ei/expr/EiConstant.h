#ifndef EICONSTANT_H
#define EICONSTANT_H

/// @file EiConstant.h
/// @brief EiConstant のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstant EiConstant.h "EiConstant.h"
/// @brief 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstant :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  EiConstant(
    const AstExpr& ast_expr ///< [in] パース木の定義要素
  ) : EiExprBase(ast_expr)
  {
  }

  /// @brief デストラクタ
  ~EiConstant() = default;


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
  VlExprType
  expr_type() const override;

  /// @brief 定数の時 true を返す．
  bool
  is_const() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // EiExpr の設定用の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;

};

END_NAMESPACE_YM_VERILOG

#endif // EICONSTANT_H
