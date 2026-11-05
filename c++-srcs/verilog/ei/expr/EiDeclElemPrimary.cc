
/// @file EiDeclElemPrimary.cc
/// @brief EiDeclElemPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "EiDeclElemPrimary.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス EiFactory
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する(配列要素版)．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  const std::vector<const VlExpr*>& index_list
)
{
  return new EiVarDeclElemPrimary(ast_expr, obj, index_list);
}

// @brief プライマリ式を生成する(固定インデックスの配列要素版)．
ElbExpr*
EiFactory::new_Primary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  SizeType offset
)
{
  return new EiConstDeclElemPrimary(ast_expr, obj, offset);
}

// @brief 固定部分選択に対応するプライマリ式を生成する(配列要素版)．
ElbExpr*
EiFactory::new_ConstSelectPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  const std::vector<const VlExpr*>& array_index_list,
  SizeType bit_offset,
  SizeType width
)
{
  return new EiVarDeclElemConstSelect(ast_expr, obj, array_index_list,
				      bit_offset, width);
}

// @brief 固定部分選択に対応するプライマリ式を生成する(固定インデックスの配列要素版)．
ElbExpr*
EiFactory::new_ConstSelectPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  SizeType array_offset,
  SizeType bit_offset,
  SizeType width
)
{
  return new EiConstDeclElemConstSelect(ast_expr, obj, array_offset,
					bit_offset, width);
}

// @brief プラス可変部分選択に対応するプライマリ式を生成する(配列要素版)．
ElbExpr*
EiFactory::new_PlusSelectPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  const std::vector<const VlExpr*>& array_index_list,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new EiVarDeclElemPlusSelect(ast_expr, obj, array_index_list,
				     bit_index, width);
}

// @brief マイナス可変部分選択に対応するプライマリ式を生成する(配列要素版)．
ElbExpr*
EiFactory::new_MinusSelectPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  const std::vector<const VlExpr*>& array_index_list,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new EiVarDeclElemMinusSelect(ast_expr, obj, array_index_list,
				      bit_index, width);
}

// @brief プラス可変部分選択に対応するプライマリ式を生成する(固定インデックスの配列要素版)．
ElbExpr*
EiFactory::new_PlusSelectPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  SizeType array_offset,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new EiConstDeclElemPlusSelect(ast_expr, obj, array_offset,
				       bit_index, width);
}

// @brief マイナス可変部分選択に対応するプライマリ式を生成する(固定インデックスの配列要素版)．
ElbExpr*
EiFactory::new_MinusSelectPrimary(
  const AstExpr& ast_expr,
  const VlDeclArray* obj,
  SizeType array_offset,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new EiConstDeclElemMinusSelect(ast_expr, obj, array_offset,
					bit_index, width);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiDeclElemPrimary::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiDeclElemPrimary::value_type() const
{
  return mObj->value_type();
}

// @brief 宣言要素もしくは配列型宣言要素への参照を返す．
const VlDeclBase*
EiDeclElemPrimary::decl_base() const
{
  return mObj;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDeclArray*
EiDeclElemPrimary::declarray_obj() const
{
  return mObj;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief 配列型宣言要素への参照のオフセットを返す．
SizeType
EiConstDeclElemPrimary::declarray_offset() const
{
  return mOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemSelect::has_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemSelect::has_const_bit_select() const
{
  return false;
}

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemSelect::has_plus_bit_select() const
{
  return false;
}

// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemSelect::has_minus_bit_select() const
{
  return false;
}

// @brief 宣言要素に対する部分選択のビット幅を返す．
SizeType
EiConstDeclElemSelect::select_width() const
{
  return mWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemConstSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemConstSelect::has_const_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
SizeType
EiConstDeclElemConstSelect::bit_offset() const
{
  return mBitOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemVarSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
const VlExpr*
EiConstDeclElemVarSelect::bit_index() const
{
  return mBitIndex;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemPlusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemPlusSelect::has_plus_bit_select() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemMinusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
bool
EiConstDeclElemMinusSelect::has_minus_bit_select() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
std::vector<const VlExpr*>
EiVarDeclElemPrimary::declarray_index_list() const
{
  return mIndexList;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemSelect::has_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemSelect::has_const_bit_select() const
{
  return false;
}

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemSelect::has_plus_bit_select() const
{
  return false;
}

// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemSelect::has_minus_bit_select() const
{
  return false;
}

// @brief 宣言要素に対する部分選択のビット幅を返す．
SizeType
EiVarDeclElemSelect::select_width() const
{
  return mWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemConstSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する固定ビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemConstSelect::has_const_bit_select() const
{
  return true;
}

// @brief 宣言要素に対する固定ビット/部分選択のオフセットを返す．
SizeType
EiVarDeclElemConstSelect::bit_offset() const
{
  return mBitOffset;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemVarSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対する可変ビット/部分選択のインデックス式を返す．
const VlExpr*
EiVarDeclElemVarSelect::bit_index() const
{
  return mBitIndex;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemPlusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するプラス可変ビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemPlusSelect::has_plus_bit_select() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemMinusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するマイナス可変ビット/部分選択を持つ時 true を返す．
bool
EiVarDeclElemMinusSelect::has_minus_bit_select() const
{
  return true;
}

END_NAMESPACE_YM_VERILOG
