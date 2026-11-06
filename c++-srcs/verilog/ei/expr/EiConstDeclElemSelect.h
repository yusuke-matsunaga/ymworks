#ifndef EICONSTDECLELEMSELECT_H
#define EICONSTDECLELEMSELECT_H

/// @file EiConstDeclElemSelect.h
/// @brief EiConstDeclElemSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiConstDeclElemPrimary.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiConstDeclElemSelect EiConstDeclElemSelect.h "EiConstDeclElemSelect.h"
/// @brief DeclArray の固定インデックス要素の部分選択を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiConstDeclElemSelect :
  public EiConstDeclElemPrimary
{
public:

  /// @brief コンストラクタ
  EiConstDeclElemSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    const VlDeclArray* obj,  ///< [in] 本体のオブジェクト
    SizeType array_offset,   ///< [in] 配列位置のオフセット
    SizeType width           ///< [in] 選択するビット幅
  ) : EiConstDeclElemPrimary(ast_expr, obj, array_offset),
      mWidth{width}
  {
  }


  /// @brief デストラクタ
  ~EiConstDeclElemSelect() = default;


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

#endif // EICONSTDECLELEMSELECT_H
