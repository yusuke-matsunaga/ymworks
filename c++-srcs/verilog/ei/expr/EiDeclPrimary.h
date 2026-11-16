#ifndef EIDECLPRIMARY_H
#define EIDECLPRIMARY_H

/// @file EiDeclPrimary.h
/// @brief EiDeclPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiPrimaryExprBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclPrimary EiDeclPrimary.h "EiDeclPrimary.h"
/// @brief VlDecl を持つプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPrimary :
  public EiPrimaryExprBase
{
public:

  /// @brief コンストラクタ
  EiDeclPrimary(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj        ///< [in] 本体のオブジェクト
  ) : EiPrimaryExprBase(ast_expr),
      mObj{obj}
  {
  }

  /// @brief デストラクタ
  ~EiDeclPrimary() = default;


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
  const VlDecl*
  decl_obj() const override;

  /// @brief 宣言要素の番号を返す．
  SizeType
  decl_id() const override;

  /// @brief 宣言要素に対するビット/部分選択の種類を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  VlSelectType
  select_type() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の宣言要素
  const VlDecl* mObj;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLPRIMARY_H
