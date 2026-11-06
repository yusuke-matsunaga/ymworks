#ifndef EIPRIMITIVEPRIMARY_H
#define EIPRIMITIVEPRIMARY_H

/// @file EiPrimitivePrimary.h
/// @brief EiPrimitivePrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPrimitivePrimary EiPrimitivePrimary.h "EiPrimitivePrimary.h"
/// @brief VlPrimitive のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiPrimitivePrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiPrimitivePrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlPrimitive* obj   ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase{ast_expr},
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiPrimitivePrimary() = default;


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
  const VlPrimitive*
  primitive_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlPrimitive* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPRIMITIVEPRIMARY_H
