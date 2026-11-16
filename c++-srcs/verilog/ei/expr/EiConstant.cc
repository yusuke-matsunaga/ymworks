
/// @file EiConstant.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "elaborator/ElbMgr.h"
#include "EiConstant.h"
#include "EiIntConstant.h"
#include "EiBitVectorConstant.h"
#include "EiRealConstant.h"
#include "EiStringConstant.h"
#include "EiParamConstant.h"
#include "ym/vl/VlValue.h"
#include "ym/vl/AstExpr.h"
#include "elaborator/ElbParameter.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 定数式を生成する．
ElbExpr*
ElbMgr::new_Constant(
  const AstExpr& ast_expr
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      auto const_type = ast_expr.const_type();
      auto size = ast_expr.const_size();
      auto is_signed = false;
      int base = 0;
      switch ( const_type ) {
      case VpiConstType::Int:
	if ( ast_expr.const_str() == nullptr ) {
	  auto val = ast_expr.const_bitvect();
	  return new EiBitVectorConst(ast_expr, VpiConstType::Int, val);
	}
	break;

      case VpiConstType::SignedBinary:
	is_signed = true;
      case VpiConstType::Binary:
	base = 2;
	break;

      case VpiConstType::SignedOct:
	is_signed = true;
      case VpiConstType::Oct:
	base = 8;
	break;

      case VpiConstType::SignedDec:
	is_signed = true;
      case VpiConstType::Dec:
	base = 10;
	break;

      case VpiConstType::SignedHex:
	is_signed = true;
      case VpiConstType::Hex:
	base = 16;
	break;

      case VpiConstType::Real:
	return new EiRealConst(ast_expr, ast_expr.const_real());

      case VpiConstType::String:
	return new EiStringConst(ast_expr, ast_expr.const_str());

      default:
	throw std::logic_error{"Should not be reached"};
      }

      // ここに来たということはビットベクタ型
      return new EiBitVectorConst(ast_expr, const_type,
				  BitVector(size, is_signed, base,
					    ast_expr.const_str()));
    });
}

// @brief genvar 起因の定数式を生成する．
ElbExpr*
ElbMgr::new_GenvarConstant(
  const AstExpr& ast_primary,
  int val
)
{
  return new_expr(
    [&]() -> ElbExpr* {
      return new EiIntConst(ast_primary, val);
    });
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstant
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiConstant::type() const
{
  return VpiObjType::Constant;
}

// @brief 式のタイプを返す．
VlExprType
EiConstant::expr_type() const
{
  return VlExprType::CONST;
}

// @brief 定数の時 true を返す．
/// @note このクラスは常に true を返す．
bool
EiConstant::is_const() const
{
  return true;
}

// @brief 要求される式の型を計算してセットする．
void
EiConstant::_set_reqsize(
  const VlValueType& type
)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiIntConst
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiIntConst::value_type() const
{
  return VlValueType::int_type();
}

// @brief 定数の型を返す．
VpiConstType
EiIntConst::constant_type() const
{
  return VpiConstType::Int;
}

// @brief 定数値を返す．
VlValue
EiIntConst::constant_value() const
{
  return VlValue(mValue);
}


//////////////////////////////////////////////////////////////////////
// クラス EiBitVectorConst
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiBitVectorConst::value_type() const
{
  auto size = mValue.size();
  auto sign = ( static_cast<int>(mConstType) & 8 ) == 8;
  return VlValueType(sign, true, size);
}

// @brief 定数の型を返す．
VpiConstType
EiBitVectorConst::constant_type() const
{
  return mConstType;
}

// @brief 定数値を返す．
VlValue
EiBitVectorConst::constant_value() const
{
  return VlValue(mValue);
}


//////////////////////////////////////////////////////////////////////
// クラス EiRealConst
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiRealConst::value_type() const
{
  return VlValueType::real_type();
}

// @brief 定数の型を返す．
VpiConstType
EiRealConst::constant_type() const
{
  return VpiConstType::Real;
}

// @brief 定数値を返す．
VlValue
EiRealConst::constant_value() const
{
  return VlValue(mValue);
}


//////////////////////////////////////////////////////////////////////
// クラス EiStringConst
//////////////////////////////////////////////////////////////////////

// @brief 式のタイプを返す．
VlValueType
EiStringConst::value_type() const
{
  auto size = mValue.size();
  return VlValueType(false, true, size);
}

// @brief 定数の型を返す．
VpiConstType
EiStringConst::constant_type() const
{
  return VpiConstType::String;
}

// @brief 定数値を返す．
VlValue
EiStringConst::constant_value() const
{
  return VlValue(mValue);
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamConstant
//////////////////////////////////////////////////////////////////////

// @brief 型の取得
VpiObjType
EiParamConstant::type() const
{
  return mObj->type();
}

// @brief 式のタイプを返す．
VlValueType
EiParamConstant::value_type() const
{
  return mObj->value_type();
}

// @brief 定数値を返す．
VlValue
EiParamConstant::constant_value() const
{
  return mObj->value();
}

END_NAMESPACE_YM_VERILOG
