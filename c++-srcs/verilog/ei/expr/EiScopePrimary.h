#ifndef EISCOPEPRIMARY_H
#define EISCOPEPRIMARY_H

/// @file EiScopePrimary.h
/// @brief EiScopePrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimaryExprBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiScopePrimary EiScopePrimary.h "EiScopePrimary.h"
/// @brief VlNamedObj のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiScopePrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiScopePrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlScope* obj       ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase(ast_expr),
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiScopePrimary() = default;


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

  /// @brief 対象のオブジェクトを返す．
  const VlScope*
  scope_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のスコープ
  const VlScope* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EISCOPEPRIMARY_H
