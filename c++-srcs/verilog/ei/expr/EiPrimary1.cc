
/// @file EiDeclElemPrimary.cc
/// @brief EiDeclElemPrimary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiDeclPrimary.h"
#include "EiDeclConstSelect.h"
#include "EiDeclPlusSelect.h"
#include "EiDeclMinusSelect.h"
#include "EiConstDeclElemPrimary.h"
#include "EiConstDeclElemConstSelect.h"
#include "EiConstDeclElemPlusSelect.h"
#include "EiConstDeclElemMinusSelect.h"
#include "EiVarDeclElemPrimary.h"
#include "EiVarDeclElemConstSelect.h"
#include "EiVarDeclElemPlusSelect.h"
#include "EiVarDeclElemMinusSelect.h"
#include "elaborator/ElbDeclRef.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      if ( decl_ref.is_decl() ) {
	auto decl = decl_ref.decl();
	return new EiDeclPrimary(ast_expr, decl);
      }
      if ( decl_ref.is_declarray() ) {
	auto declarray = decl_ref.declarray();
	if ( decl_ref.is_const_index() ) {
	  return new EiConstDeclElemPrimary(ast_expr, declarray,
					    decl_ref.offset());
	}
	else {
	  return new EiVarDeclElemPrimary(ast_expr, declarray,
					  decl_ref.index_list());
	}
      }
      throw std::logic_error{"never reached"};
    });
}

// @brief 固定部分選択に対応するプライマリ式を生成する．
ElbExpr*
ElbMgr::new_ConstSelectPrimary(
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref,
  SizeType bit_offset,
  SizeType width
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      if ( decl_ref.is_decl() ) {
	auto decl = decl_ref.decl();
	return new EiDeclConstSelect(ast_expr, decl, bit_offset, width);
      }
      if ( decl_ref.is_declarray() ) {
	auto declarray = decl_ref.declarray();
	if ( decl_ref.is_const_index() ) {
	  return new EiConstDeclElemConstSelect(ast_expr, declarray,
						decl_ref.offset(),
						bit_offset, width);
	}
	else {
	  return new EiVarDeclElemConstSelect(ast_expr, declarray,
					      decl_ref.index_list(),
					      bit_offset, width);
	}
      }
      throw std::logic_error{"never reached"};
    });
}

// @brief 可変プラス部分選択に対応するプライマリ式を生成する．
ElbExpr*
ElbMgr::new_PlusSelectPrimary(
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      if ( decl_ref.is_decl() ) {
	auto decl = decl_ref.decl();
	return new EiDeclPlusSelect(ast_expr, decl, bit_index, width);
      }
      if ( decl_ref.is_declarray() ) {
	auto declarray = decl_ref.declarray();
	if ( decl_ref.is_const_index() ) {
	  return new EiConstDeclElemPlusSelect(ast_expr, declarray,
					       decl_ref.offset(),
					       bit_index, width);
	}
	else {
	  return new EiVarDeclElemPlusSelect(ast_expr, declarray,
					     decl_ref.index_list(),
					     bit_index, width);
	}
      }
      throw std::logic_error{"never reached"};
    });
}

// @brief 可変マイナス部分選択に対応するプライマリ式を生成する．
ElbExpr*
ElbMgr::new_MinusSelectPrimary(
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref,
  const VlExpr* bit_index,
  SizeType width
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      if ( decl_ref.is_decl() ) {
	auto decl = decl_ref.decl();
	return new EiDeclMinusSelect(ast_expr, decl, bit_index, width);
      }
      if ( decl_ref.is_declarray() ) {
	auto declarray = decl_ref.declarray();
	if ( decl_ref.is_const_index() ) {
	  return new EiConstDeclElemMinusSelect(ast_expr, declarray,
						decl_ref.offset(),
						bit_index, width);
	}
	else {
	  return new EiVarDeclElemMinusSelect(ast_expr, declarray,
					      decl_ref.index_list(),
					      bit_index, width);
	}
      }
      throw std::logic_error{"never reached"};
    });
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
