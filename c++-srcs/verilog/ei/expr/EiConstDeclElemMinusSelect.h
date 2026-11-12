#ifndef EICONSTDECLELEMMINUSSELECT_H
#define EICONSTDECLELEMMINUSSELECT_H

/// @file EiConstDeclElemVarSelect.h
/// @brief EiConstDeclElemVarSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstDeclElemVarSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemMinusSelect EiConstDeclElemMinusSelect.h "EiConstDeclElemMinusSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemMinusSelect :
  public EiConstDeclElemVarSelect
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemMinusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemVarSelect(ast_expr, obj, array_offset,
			       bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiConstDeclElemMinusSelect() = default;


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

#endif // EICONSTDECLELEMMINUSSELECT_H
