#ifndef EIDECLELEMPRIMARY_H
#define EIDECLELEMPRIMARY_H

/// @file EiDeclElemPrimary.h
/// @brief EiDeclElemPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimaryExprBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclElemPrimary EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclElemPrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiDeclElemPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj   ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase(ast_expr),
    mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiDeclElemPrimary() = default;


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

  /// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
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

#endif // EIDECLELEMPRIMARY_H
