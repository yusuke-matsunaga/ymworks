#ifndef EIDECLMINUSSELECT_H
#define EIDECLMINUSSELECT_H

/// @file EiDeclMinusSelect.h
/// @brief EiDeclMinusSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclVarSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclMinusSelect EiDeclMinusSelect.h "EiDeclMinusSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclMinusSelect :
  public EiDeclVarSelect
{
public:

  /// @brief コンストラクタ
  EiDeclMinusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclVarSelect(ast_expr, obj, bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiDeclMinusSelect() = default;


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

#endif // EIDECLMINUSSELECT_H
