
/// @file EiDeclPrimary.cc
/// @brief EiDeclPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "EiDeclPrimary.h"
#include "ym/vl/VlDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  const VlDecl* obj
)
{
  return new EiDeclPrimary(ast_expr, obj);
}

// @brief 固定部分選択に対応するプライマリ式を生成する．
ElbExpr*
EiFactory::new_ConstSelectPrimary(
  const AstExpr& ast_expr,
  const VlDecl* obj,
  SizeType bit_offset,
  SizeType width
)
{
  return new EiDeclConstSelect(ast_expr, obj, bit_offset, width);
}

// @brief プラス可変部分選択に対応するプライマリ式を生成する．
ElbExpr*
EiFactory::new_PlusSelectPrimary(
  const AstExpr& ast_expr,
  const VlDecl* obj,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new EiDeclMinusSelect(ast_expr, obj, bit_index, width);
}

// @brief マイナス可変部分選択に対応するプライマリ式を生成する．
ElbExpr*
EiFactory::new_MinusSelectPrimary(
  const AstExpr& ast_expr,
  const VlDecl* obj,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new EiDeclMinusSelect(ast_expr, obj, bit_index, width);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclPrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiDeclPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiDeclPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiDeclPrimary::decl_obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclSelect
//////////////////////////////////////////////////////////////////////

/// @brief 宣言要素に対するビット/部分選択を持つ時 true を返す．
bool
EiDeclSelect::has_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiDeclSelect::has_const_bit_select() const
{
  return false;
}

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiDeclSelect::has_plus_bit_select() const
{
  return false;
}

// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
bool
EiDeclSelect::has_minus_bit_select() const
{
  return false;
}

// @brief 宣言要素に対する部分選択のビット幅を返す．
SizeType
EiDeclSelect::select_width() const
{
  return mWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclConstSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiDeclConstSelect::has_const_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
SizeType
EiDeclConstSelect::bit_offset() const
{
  return mBitOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
const VlExpr*
EiDeclVarSelect::bit_index() const
{
  return mBitIndex;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclPlusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiDeclPlusSelect::has_plus_bit_select() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclMinusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
bool
EiDeclMinusSelect::has_minus_bit_select() const
{
  return true;
}

END_NAMESPACE_YM_VERILOG
