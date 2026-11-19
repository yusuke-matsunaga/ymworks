
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
#include "elaborator/ElbPrimary.h"
#include "ym/vl/VlDeclArray.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbMgr
//////////////////////////////////////////////////////////////////////

// @brief プライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const ElbPrimary& primary
)
{
  // ElbDeclRef と ElbSelecter の2次元マトリックスになっている．
  auto& ast_expr = primary.ast_expr();
  auto& decl_ref = primary.decl_ref();
  auto& selecter = primary.selecter();

  return new_Primary(ast_expr, decl_ref, selecter);
}

// @brief プライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref
)
{
  auto width = decl_ref.declbase()->bit_size();
  return new_Primary(ast_expr, decl_ref,
		     ElbSelecter::new_NONE(width));
}

// @brief プライマリ式を生成する．
ElbExpr*
ElbMgr::new_Primary(
  const AstExpr& ast_expr,
  const ElbDeclRef& decl_ref,
  const ElbSelecter& selecter
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      if ( decl_ref.is_decl() ) {
	auto decl = decl_ref.decl();
	switch ( selecter.type() ) {
	case VlSelectType::NONE:
	  return new EiDeclPrimary(ast_expr, decl);
	case VlSelectType::CONST:
	  return new EiDeclConstSelect(ast_expr, decl,
				       selecter.bit_offset(),
				       selecter.select_width());
	case VlSelectType::PLUS:
	  return new EiDeclPlusSelect(ast_expr, decl,
				      selecter.bit_index_expr(),
				      selecter.select_width());
	case VlSelectType::MINUS:
	  return new EiDeclMinusSelect(ast_expr, decl,
				       selecter.bit_index_expr(),
				       selecter.select_width());
	}
      }
      if ( decl_ref.is_declarray() ) {
	auto declarray = decl_ref.declarray();
	if ( decl_ref.is_const_elem() ) {
	  auto array_offset = decl_ref.offset();
	  switch ( selecter.type() ) {
	  case VlSelectType::NONE:
	    return new EiConstDeclElemPrimary(ast_expr, declarray,
					      array_offset);
	  case VlSelectType::CONST:
	    return new EiConstDeclElemConstSelect(ast_expr, declarray,
						  array_offset,
						  selecter.bit_offset(),
						  selecter.select_width());
	  case VlSelectType::PLUS:
	    return new EiConstDeclElemPlusSelect(ast_expr, declarray,
						 array_offset,
						 selecter.bit_index_expr(),
						 selecter.select_width());
	  case VlSelectType::MINUS:
	    return new EiConstDeclElemMinusSelect(ast_expr, declarray,
						  array_offset,
						  selecter.bit_index_expr(),
						  selecter.select_width());
	  }
	}
	else {
	  auto index_list = decl_ref.index_list();
	  switch ( selecter.type() ) {
	  case VlSelectType::NONE:
	    return new EiVarDeclElemPrimary(ast_expr, declarray,
					    index_list);
	  case VlSelectType::CONST:
	    return new EiVarDeclElemConstSelect(ast_expr, declarray,
						index_list,
						selecter.bit_offset(),
						selecter.select_width());
	  case VlSelectType::PLUS:
	    return new EiVarDeclElemPlusSelect(ast_expr, declarray,
					       index_list,
					       selecter.bit_index_expr(),
					       selecter.select_width());
	  case VlSelectType::MINUS:
	    return new EiVarDeclElemMinusSelect(ast_expr, declarray,
						index_list,
						selecter.bit_index_expr(),
						selecter.select_width());
	  }
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

// @brief プライマリの種類を返す．
VlPrimaryType
EiDeclPrimary::primary_type() const
{
  return VlPrimaryType::DECL;
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

// @brief 宣言要素の番号を返す．
SizeType
EiDeclPrimary::decl_id() const
{
  return mObj->decl_id();
}

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiDeclPrimary::select_type() const
{
  return VlSelectType::NONE;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclSelect
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiDeclSelect::value_type() const
{
  return VlValueType(false, true, mWidth);
}

// @brief 式のビット幅を返す．
SizeType
EiDeclSelect::bit_size() const
{
  return mWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclConstSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiDeclConstSelect::select_type() const
{
  return VlSelectType::CONST;
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


// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiDeclPlusSelect::select_type() const
{
  return VlSelectType::PLUS;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclMinusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiDeclMinusSelect::select_type() const
{
  return VlSelectType::MINUS;
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

// @brief プライマリの種類を返す．
VlPrimaryType
EiConstDeclElemPrimary::primary_type() const
{
  return VlPrimaryType::CONST_DECL_ELEM;
}

// @brief 配列型宣言要素への参照のオフセットを返す．
SizeType
EiConstDeclElemPrimary::declarray_offset() const
{
  return mOffset;
}

// @brief 宣言要素の番号を返す．
SizeType
EiConstDeclElemPrimary::decl_id() const
{
  return declarray_obj()->decl_id(mOffset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemSelect
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiConstDeclElemSelect::value_type() const
{
  return VlValueType(false, true, mWidth);
}

// @brief 式のビット幅を返す．
SizeType
EiConstDeclElemSelect::bit_size() const
{
  return mWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemConstSelect
//////////////////////////////////////////////////////////////////////

/// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiConstDeclElemConstSelect::select_type() const
{
  return VlSelectType::CONST;
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

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiConstDeclElemPlusSelect::select_type() const
{
  return VlSelectType::PLUS;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstDeclElemMinusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiConstDeclElemMinusSelect::select_type() const
{
  return VlSelectType::MINUS;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemPrimary
//////////////////////////////////////////////////////////////////////

// @brief プライマリの種類を返す．
VlPrimaryType
EiVarDeclElemPrimary::primary_type() const
{
  return VlPrimaryType::VAR_DECL_ELEM;
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスリストを返す．
std::vector<const VlExpr*>
EiVarDeclElemPrimary::declarray_index_list() const
{
  return mIndexList;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemSelect
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiVarDeclElemSelect::value_type() const
{
  return VlValueType(false, true, mWidth);
}

// @brief 式のビット幅を返す．
SizeType
EiVarDeclElemSelect::bit_size() const
{
  return mWidth;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemConstSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiVarDeclElemConstSelect::select_type() const
{
  return VlSelectType::CONST;
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

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiVarDeclElemPlusSelect::select_type() const
{
  return VlSelectType::PLUS;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarDeclElemMinusSelect
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素に対するビット/部分選択の種類を返す．
VlSelectType
EiVarDeclElemMinusSelect::select_type() const
{
  return VlSelectType::MINUS;
}

END_NAMESPACE_YM_VERILOG
