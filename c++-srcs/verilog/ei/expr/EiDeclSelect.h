#ifndef EIDECLSELECT_H
#define EIDECLSELECT_H

/// @file EiDeclSelect.h
/// @brief EiDeclSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiDeclPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiDeclSelect EiDeclSelect.h "EiDeclSelect.h"
/// @brief Decl の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiDeclSelect :
  public EiDeclPrimary
{
public:

  /// @brief コンストラクタ
  EiDeclSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDecl* obj,       ///< [in] 本体のオブジェクト
    SizeType width           ///< [in] 選択するビット幅
  ) : EiDeclPrimary(ast_expr, obj),
      mWidth{width}
  {
  }

  /// @brief デストラクタ
  ~EiDeclSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するビット/部分選択を持つ時 true を返す．
  ///
  /// - expr_type() が PRIMARY かつ primary_type() が DECL,
  ///   CONST_DECL_ARRAY, VAR_DECL_ARRAY の場合に意味を持つ．
  bool
  has_bit_select() const override;

  /// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
  bool
  has_const_bit_select() const override;

  /// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_plus_bit_select() const override;

  /// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  bool
  has_minus_bit_select() const override;

  /// @brief 宣言要素に対する部分選択のビット幅を返す．
  ///
  /// - has_bit_select() == true の時のみ意味を持つ．
  /// - ビット選択の場合は 1 を返す．
  SizeType
  select_width() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 選択するビット幅
  SizeType mWidth;

};

END_NAMESPACE_YM_VERILOG

#endif // EIDECLSELECT_H
