#ifndef EIVARDECLELEMMINUSSELECT_H
#define EIVARDECLELEMMINUSSELECT_H

/// @file EiDeclElemPrimary.h
/// @brief EiDeclElemPrimary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiVarDeclElemVarSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiVarDeclElemMinusSelect EiVarDeclElemMinusSelect.h "EiVarDeclElemMinusSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarDeclElemMinusSelect :
  public EiVarDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiVarDeclElemMinusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    const std::vector<const VlExpr*>& index_list, ///< [in] インデックスのリスト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiVarDeclElemVarSelect(ast_expr, obj, index_list,
			     bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiVarDeclElemMinusSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するビット/部分選択の種類を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  VlSelectType
  select_type() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIVARDECLELEMMINUSSELECT_H
