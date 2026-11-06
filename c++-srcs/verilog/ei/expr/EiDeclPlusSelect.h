#ifndef EIDECLPLUSSELECT_H
#define EIDECLPLUSSELECT_H

/// @file EiDeclPlusSelect.h
/// @brief EiDeclPlusSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclVarSelect.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclPlusSelect EiDeclPlusSelect.h "EiDeclPlusSelect.h"
/// @brief 可変インデックスを持つ部分選択を表すクラス
//////////////////////////////////////////////////////////////////////
class EiDeclPlusSelect :
  public EiDeclVarSelect
{
public:

  /// @brief コンストラクタ
  EiDeclPlusSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    const VlExpr* bit_index, ///< [in] ビット位置のインデックス式
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclVarSelect(ast_expr, obj, bit_index, width)
  {
  }

  /// @brief デストラクタ
  ~EiDeclPlusSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_plus_bit_select() const override;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLPLUSSELECT_H
