#ifndef EIPARTSELECT_H
#define EIPARTSELECT_H

/// @file EiPartSelect.h
/// @brief EiPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiPartSelectPart EiPartSelect.h "EiPartSelect.h"
/// @brief パート選択式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class EiPartSelect :
  public EiExprBase
{
public:

  /// @brief コンストラクタ
  EiPartSelect(
    const AstExpr& ast_expr, ///< [in] パース木の定義要素
    ElbExpr* parent_expr,    ///< [in] 対象の式
    SizeType select_width,   ///< [in] 選択するビット幅
    SizeType bit_offset      ///< [in] ビット位置のオフセット
  ) : EiExprBase(ast_expr),
      mParentExpr{parent_expr},
      mWidth{select_width},
      mOffset{bit_offset}
  {
  }

  /// @brief デストラクタ
  ~EiPartSelect() = default;


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

  /// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
  SizeType
  bit_offset() const override;

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
  ElbExpr* mParentExpr;

  // 選択するビット幅
  SizeType mWidth;

  // ビット位置のオフセット
  SizeType mOffset;

};

END_NAMESPACE_YM_VERILOG

#endif // EIPARTSELECT_H
