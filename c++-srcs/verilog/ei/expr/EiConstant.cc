
/// @file EiConstant.cc
/// @brief EiExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ei/EiFactory.h"
#include "ei/EiConstant.h"
#include "ym/vl/VlValue.h"
#include "ym/vl/AstExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 定数式を生成する．
ElbExpr*
EiFactory::new_Constant(
  const AstExpr& ast_expr
)
{
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
			      BitVector(size, is_signed, base, ast_expr.const_str()));
}

// @brief genvar 起因の定数式を生成する．
ElbExpr*
EiFactory::new_GenvarConstant(
  const AstExpr& ast_primary,
  int val
)
{
  return new EiIntConst(ast_primary, val);
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstant
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiConstant::EiConstant(
  const AstExpr& ast_expr
) : EiExprBase(ast_expr)
{
}

// @brief デストラクタ
EiConstant::~EiConstant()
{
}

// @brief 型の取得
VpiObjType
EiConstant::type() const
{
  return VpiObjType::Constant;
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

// @brief コンストラクタ
EiIntConst::EiIntConst(
  const AstExpr& ast_expr,
  std::int32_t value
) : EiConstant(ast_expr),
    mValue{value}
{
}

// @brief デストラクタ
EiIntConst::~EiIntConst()
{
}

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

// @brief コンストラクタ
EiBitVectorConst::EiBitVectorConst(
  const AstExpr& ast_expr,
  VpiConstType const_type,
  const BitVector& value
) : EiConstant(ast_expr),
    mConstType{const_type},
    mValue{value}
{
}

// @brief デストラクタ
EiBitVectorConst::~EiBitVectorConst()
{
}

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

// @brief コンストラクタ
EiRealConst::EiRealConst(
  const AstExpr& ast_expr,
  double value
) : EiConstant(ast_expr),
    mValue{value}
{
}

// @brief デストラクタ
EiRealConst::~EiRealConst()
{
}

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

// @brief コンストラクタ
EiStringConst::EiStringConst(
  const AstExpr& ast_expr,
  const std::string& value
) : EiConstant(ast_expr),
    mValue(value)
{
}

// @brief デストラクタ
EiStringConst::~EiStringConst()
{
}

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

END_NAMESPACE_YM_VERILOG
