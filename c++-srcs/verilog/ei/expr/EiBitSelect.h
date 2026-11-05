#ifndef EIBITSELECT_H
#define EIBITSELECT_H

/// @file EiBitSelect.h
/// @brief EiBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief ビット選択式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiBitSelect :
  public EiExprBase
{
protected:

  /// @brief コンストラクタ
  EiBitSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* base_expr       ///< [in] 対象の式
  ) : EiExprBase(ast_expr),
    mBaseExpr{base_expr}
  {
  }

  /// @brief デストラクタ
  ~EiBitSelect() = default;


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

  /// @brief 式のビット幅を返す．
  ///
  /// value_type().size() を同じ
  SizeType
  bit_size() const override;

  /// @brief 式のタイプを返す．
  VlExprType
  expr_type() const override;

  /// @brief ビット/部分選択の時に true を返す．
  bool
  is_select() const override;

  /// @brief 宣言要素に対する部分選択のビット幅を返す．
  ///
  /// - is_select() == true または has_bit_select() == true の時のみ意味を持つ．
  /// - ビット選択の場合は 1 を返す．
  SizeType
  select_width() const override;

  /// @brief 親の式を返す．
  const VlExpr*
  parent_expr() const override;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  void
  _set_reqsize(
    const VlValueType& type ///< [in] 要求される式の型
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の式
  ElbExpr* mBaseExpr;

};


//////////////////////////////////////////////////////////////////////
/// @class EiConstBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 固定ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiConstBitSelect :
  public EiBitSelect
{
public:

  /// @brief コンストラクタ
  EiConstBitSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* base_expr,      ///< [in] 対象の式
    SizeType bit_offset      ///< [in] ビット位置のオフセット
  ) : EiBitSelect(ast_expr, base_expr),
      mOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiConstBitSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
  bool
  has_const_bit_select() const override;

  /// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
  ///
  /// - has_const_bit_select() == true の場合のみ意味を持つ．
  SizeType
  bit_offset() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オフセット
  SizeType mOffset;

};


//////////////////////////////////////////////////////////////////////
/// @class EiVarBitSelect EiBitSelect.h "EiBitSelect.h"
/// @brief 可変ビット選択式を表すクラス
//////////////////////////////////////////////////////////////////////
class EiVarBitSelect :
  public EiBitSelect
{
public:

  /// @brief コンストラクタ
  EiVarBitSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* base_expr,      ///< [in] 対象の式
    ElbExpr* index_expr      ///< [in] ビット選択式
  ) : EiBitSelect(ast_expr, base_expr),
    mIndexExpr{index_expr}
  {
  }

  /// @brief デストラクタ
  ~EiVarBitSelect() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
  ///
  /// - has_bit_select() == true の場合のみ意味を持つ．
  /// - 可変ビット選択の場合も true を返す．
  bool
  has_plus_bit_select() const override;

  /// @brief インデックス式を返す．
  const VlExpr*
  bit_index() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックス式
  ElbExpr* mIndexExpr;

};

END_NAMESPACE_YM_VERILOG

#endif // EIBITSELECT_H
