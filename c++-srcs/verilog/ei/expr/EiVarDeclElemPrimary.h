#ifndef EIVARDECLELEMPRIMARY_H
#define EIVARDECLELEMPRIMARY_H

/// @file EiDeclElemPrimary.h
/// @brief EiDeclElemPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclElemPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemPrimary EiDeclElemPrimary.h "EiDeclElemPrimary.h"
/// @brief 可変インデックスの配列要素のプライマリ式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemPrimary :
  public EiDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemPrimary(
    const AstExpr& ast_expr,                     ///< [in] パース木の定義要素
    const VlDeclArray* obj,                      ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list ///< [in] インデックスのリスト
  ) : EiDeclElemPrimary(ast_expr, obj),
      mIndexList{index_list}
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemPrimary() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
  ///
  /// - primary_type() が CONST_DECL_ELEM, VAR_DECL_ELEM の場合のみ意味を持つ．
  std::vector<const VlExpr*>
  declarray_index_list() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスのリスト
  std::vector<const VlExpr*> mIndexList;

};

END_NAMESPACE_YM_VERILOG

#endif // EIVARDECLELEMPRIMARY_H
