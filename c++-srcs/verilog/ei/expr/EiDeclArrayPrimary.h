#ifndef EIDECLARRAYPRIMARY_H
#define EIDECLARRAYPRIMARY_H

/// @file EiDeclArrayPrimary.h
/// @brief EiDeclArrayPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclArrayPrimary EiDeclArrayPrimary.h "EiDeclArrayPrimary.h"
/// @brief プライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclArrayPrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiDeclArrayPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj   ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase(ast_expr),
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiDeclArrayPrimary() = default;


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

  /// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
  const VlDeclBase*
  decl_base() const override;

  /// @brief 宣言要素の配列への参照の場合，対象のオブジェクトを返す．
  const VlDeclArray*
  declarray_obj() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDeclArray* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLARRAYPRIMARY_H
