
/// @file EiBitSelect.cc
/// @brief EiBitSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "EiBitSelect.h"
#include "elaborator/ElbDecl.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 固定ビット選択式を生成する．
ElbExpr*
EiFactory::new_BitSelect(
  const AstExpr& ast_expr,
  ElbExpr* base_expr,
  SizeType bit_offset
)
{
  return new EiConstBitSelect(ast_expr, base_expr, bit_offset);
}

// @brief 可変ビット選択式を生成する．
ElbExpr*
EiFactory::new_BitSelect(
  const AstExpr& ast_expr,
  ElbExpr* base_expr,
  ElbExpr* index_expr
)
{
  return new EiVarBitSelect(ast_expr, base_expr, index_expr);
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitSelect
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiBitSelect::type() const
{
  return VpiObjType::BitSelect;
}

// @brief 式のタイプを返す．
VlValueType
EiBitSelect::value_type() const
{
  return VlValueType(false, true, 1);
}

// @brief 式のビット幅を返す．
SizeType
EiBitSelect::bit_size() const
{
  return 1;
}

// @brief 式のタイプを返す．
VlExprType
EiBitSelect::expr_type() const
{
  return VlExprType::SELECT;
}

// @brief ビット/部分指定の時に true を返す．
bool
EiBitSelect::is_select() const
{
  return true;
}

// @brief 宣言要素に対する部分選択のビット幅を返す．
SizeType
EiBitSelect::select_width() const
{
  return 1;
}

// @brief 親の式を返す．
const VlExpr*
EiBitSelect::parent_expr() const
{
  return mBaseExpr;
}

// @brief 要求される式の型を計算してセットする．
void
EiBitSelect::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstBitSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiConstBitSelect::has_const_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
SizeType
EiConstBitSelect::bit_offset() const
{
  return mOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarBitSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiVarBitSelect::has_plus_bit_select() const
{
  return true;
}

// @brief インデックス式を返す．
const VlExpr*
EiVarBitSelect::bit_index() const
{
  return mIndexExpr;
}

END_NAMESPACE_YM_VERILOG
