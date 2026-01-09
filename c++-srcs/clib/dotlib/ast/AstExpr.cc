
/// @file AstExpr.cc
/// @brief AstExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/AstExpr.h"
#include "ym/MsgMgr.h"
#include "AstExpr_int.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス AstExpr
//////////////////////////////////////////////////////////////////////

// @brief ブール値を作る．
AstExprPtr
AstExpr::new_bool(
  bool value,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstBoolExpr(loc, value)};
}

// @brief 数値を作る．
AstExprPtr
AstExpr::new_float(
  double value,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstFloatExpr(loc, value)};
}

// @brief 文字列を作る．
AstExprPtr
AstExpr::new_string(
  const ShString& value,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstStrExpr(loc, value)};
}

// @brief VDDシンボルを作る．
AstExprPtr
AstExpr::new_vdd(
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstSymbolExpr(loc, AstExpr::VDD)};
}

/// @brief VSSシンボルを作る．
AstExprPtr
AstExpr::new_vss(
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstSymbolExpr(loc, AstExpr::VSS)};
}

// @brief VCCシンボルを作る．
AstExprPtr
AstExpr::new_vcc(
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstSymbolExpr(loc, AstExpr::VCC)};
}

// @brief not 演算子を作る．
AstExprPtr
AstExpr::new_not(
  AstExprPtr&& opr1,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstNot>{new AstNot(loc, std::move(opr1))};
}

// @brief plus 演算子を作る．
AstExprPtr
AstExpr::new_plus(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::Plus,
					     std::move(opr1),
					     std::move(opr2))};
}

/// @brief minus 演算子を作る．
AstExprPtr
AstExpr::new_minus(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::Minus,
					     std::move(opr1),
					     std::move(opr2))};
}

// @brief mult 演算子を作る．
AstExprPtr
AstExpr::new_mult(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::Mult,
					     std::move(opr1),
					     std::move(opr2))};
}

// @brief div 演算子を作る．
AstExprPtr
AstExpr::new_div(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::Div,
					     std::move(opr1),
					     std::move(opr2))};
}

// @brief and 演算子を作る．
AstExprPtr
AstExpr::new_and(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::And,
					     std::move(opr1),
					     std::move(opr2))};
}

// @brief or 演算子を作る．
AstExprPtr
AstExpr::new_or(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::Or,
					     std::move(opr1),
					     std::move(opr2))};
}

// @brief xor 演算子を作る．
AstExprPtr
AstExpr::new_xor(
  AstExprPtr&& opr1,
  AstExprPtr&& opr2,
  const FileRegion& loc
)
{
  return std::unique_ptr<AstExpr>{new AstOpr(AstExpr::Xor,
					     std::move(opr1),
					     std::move(opr2))};
}

// @brief 無効なリファレンスを返す．
const AstExpr&
AstExpr::null_ref()
{
  static AstNullExpr dummy;
  return dummy;
}

// @brief コンストラクタ
// @param[in] loc 位置情報
AstExpr::AstExpr(
  const FileRegion& loc
) : mLoc{loc}
{
}

// @brief 二項演算子型(Plus, Minus, Mult, Div)の時に true を返す．
bool
AstExpr::is_opr() const
{
  return false;
}

// @brief ブール値を返す．
//
// Bool の時のみ意味を持つ．
bool
AstExpr::bool_value() const
{
  ASSERT_NOT_REACHED;

  return false;
}

// @brief 浮動小数点値を返す．
//
// Float の時のみ意味を持つ．
double
AstExpr::float_value() const
{
  ASSERT_NOT_REACHED;

  return 0.0;
}

// @brief 文字列シンボルを返す．
//
// Str の時のみ意味を持つ．
ShString
AstExpr::string_value() const
{
  ASSERT_NOT_REACHED;

  return ShString();
}

// @brief 第一オペランドを返す．
//
// Not, Plus, Minus, Mult, Div の時のみ意味を持つ．
const AstExpr&
AstExpr::opr1() const
{
  ASSERT_NOT_REACHED;

  return null_ref();
}

// @brief 第二オペランドを返す．
//
// Plus, Minus, Mult, Div の時のみ意味を持つ．
const AstExpr&
AstExpr::opr2() const
{
  ASSERT_NOT_REACHED;

  return null_ref();
}


//////////////////////////////////////////////////////////////////////
// クラス AstBoolExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstBoolExpr::AstBoolExpr(
  const FileRegion& loc,
  bool val
) : AstExpr{loc},
    mValue(val)
{
}

// @brief 型を返す．
AstExpr::Type
AstBoolExpr::type() const
{
  return Type::Bool;
}

// @brief ブール値を返す．
//
// Bool の時のみ意味を持つ．
bool
AstBoolExpr::bool_value() const
{
  return mValue;
}

// @brief Expr を作る．
Expr
AstBoolExpr::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  if ( mValue ) {
    return Expr::zero();
  }
  else {
    return Expr::one();
  }
}

// @brief 内容を表す文字列を返す．
std::string
AstBoolExpr::decompile() const
{
  if ( mValue ) {
    return "1";
  }
  else {
    return "0";
  }
}


//////////////////////////////////////////////////////////////////////
// クラス AstFlotExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstFloatExpr::AstFloatExpr(
  const FileRegion& loc,
  double val
) : AstExpr{loc},
    mValue{val}
{
}

// @brief 型を返す．
AstExpr::Type
AstFloatExpr::type() const
{
  return Type::Float;
}

// @brief 浮動小数点値を返す．
//
// Float の時のみ意味を持つ．
double
AstFloatExpr::float_value() const
{
  return mValue;
}

// @brief Expr を作る．
Expr
AstFloatExpr::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  ASSERT_NOT_REACHED;
  return Expr::invalid();
}

// @brief 内容を表す文字列を返す．
std::string
AstFloatExpr::decompile() const
{
  std::ostringstream buf;
  buf << mValue;
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス AstStrExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstStrExpr::AstStrExpr(
  const FileRegion& loc,
  const ShString& val
) : AstExpr{loc},
    mValue{val}
{
}

// @brief 型を返す．
AstExpr::Type
AstStrExpr::type() const
{
  return Type::Str;
}

// @brief 文字列値を返す．
//
// Str の時のみ意味を持つ．
ShString
AstStrExpr::string_value() const
{
  return mValue;
}

// @brief Expr を作る．
Expr
AstStrExpr::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  if ( pin_map.count(mValue) == 0 ) {
    std::ostringstream buf;
    buf << mValue << ": No such pin-name.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc(),
		    MsgType::Error,
		    "DOTLIB_PARSER",
		    buf.str());
    return Expr::invalid();
  }

  SizeType id = pin_map.at(mValue);
  return Expr::positive_literal(id);
}

// @brief 内容を表す文字列を返す．
std::string
AstStrExpr::decompile() const
{
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス AstSymbolExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstSymbolExpr::AstSymbolExpr(
  const FileRegion& loc,
  Type type
) : AstExpr{loc},
    mType{type}
{
}

// @brief 型を返す．
AstExpr::Type
AstSymbolExpr::type() const
{
  return mType;
}

// @brief Expr を作る．
Expr
AstSymbolExpr::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  ASSERT_NOT_REACHED;
  return Expr::invalid();
}

// @brief 内容を表す文字列を返す．
std::string
AstSymbolExpr::decompile() const
{
  switch ( mType ) {
  case VDD: return "VDD";
  case VSS: return "VSS";
  case VCC: return "VCC";
  default: ASSERT_NOT_REACHED; break;
  }
  return std::string();
}


//////////////////////////////////////////////////////////////////////
// クラス AstNot
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstNot::AstNot(
  const FileRegion& loc,
  AstExprPtr&& opr
) : AstExpr{loc},
    mOpr1{std::move(opr)}
{
}

// @brief 型を得る．
AstExpr::Type
AstNot::type() const
{
  return Type::Not;
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const AstExpr&
AstNot::opr1() const
{
  return *mOpr1;
}

// @brief Expr を作る．
Expr
AstNot::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  Expr expr1 = opr1().to_expr(pin_map);
  return ~expr1;
}

// @brief 内容を表す文字列を返す．
std::string
AstNot::decompile() const
{
  std::ostringstream buf;
  buf << "~" << opr1().decompile();
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス AstOpr
//////////////////////////////////////////////////////////////////////

// @brief 表すコンストラクタ
AstOpr::AstOpr(
  Type type,
  AstExprPtr&& opr1,
  AstExprPtr&& opr2
) : AstExpr{FileRegion(opr1->loc(), opr2->loc())},
    mType{type},
    mOpr1{std::move(opr1)},
    mOpr2{std::move(opr2)}
{
}

// @brief 型を得る．
AstExpr::Type
AstOpr::type() const
{
  return mType;
}

// @brief 演算子型(Plus, Minsu, Mult, Div)の時に true を返す．
bool
AstOpr::is_opr() const
{
  return true;
}

// @brief 第一オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const AstExpr&
AstOpr::opr1() const
{
  return *mOpr1;
}

// @brief 第二オペランドを返す．
// @note type() が演算子の型の時のみ意味を持つ．
const AstExpr&
AstOpr::opr2() const
{
  return *mOpr2;
}

// @brief Expr を作る．
Expr
AstOpr::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  Expr expr1 = opr1().to_expr(pin_map);
  Expr expr2 = opr2().to_expr(pin_map);
  switch ( type() ) {
  case Type::And: return expr1 & expr2;
  case Type::Or:  return expr1 | expr2;
  case Type::Xor: return expr1 ^ expr2;
  default: break;
  }
  ASSERT_NOT_REACHED;
  return Expr();
}

// @brief 内容を表す文字列を返す．
std::string
AstOpr::decompile() const
{
  std::ostringstream buf;
  buf << "( ";
  buf << opr1().decompile();
  switch ( type() ) {
  case Type::Plus:  buf << " + "; break;
  case Type::Minus: buf << " - "; break;
  case Type::Mult:  buf << " * "; break;
  case Type::Div:   buf << " / "; break;
  case Type::And:   buf << " & "; break;
  case Type::Or:    buf << " | "; break;
  case Type::Xor:   buf << " ^ "; break;
  default:ASSERT_NOT_REACHED; break;
  }
  buf << opr2().decompile();
  buf << " )";
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス AtNullExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstNullExpr::AstNullExpr(
) : AstExpr{FileRegion{}}
{
}

// @brief 型を返す．
AstExpr::Type
AstNullExpr::type() const
{
  return Type::Null;
}

// @brief Expr を作る．
Expr
AstNullExpr::to_expr(
  const std::unordered_map<ShString, SizeType>& pin_map
) const
{
  return Expr::invalid();
}

// @brief 内容を表す文字列を返す．
std::string
AstNullExpr::decompile() const
{
  return "---";
}

END_NAMESPACE_YM_DOTLIB
