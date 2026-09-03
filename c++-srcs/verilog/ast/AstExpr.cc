
/// @file AstExpr.cc
/// @brief AstExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstExpr.h"
#include "ym/vl/AstNameBranch.h"
#include "ym/vl/AstPart.h"
#include "ym/vl/BitVector.h"
#include "parser/PtExpr.h"
#include "parser/JsonUtils.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstExpr
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

std::string
decompile_impl(
  const AstExpr& expr,
  int ppri
);

// @brief 演算子の場合の decompile() 処理
std::string
decompile_opr(
  const AstExpr& expr,
  int ppri
)
{
  // 各演算子を表す文字列のテーブル
  static const char* sym_table[] = {
    "",               // dummy
    "-",              // vpiMinusOp
    "+",              // vpiPlusOp
    "!",              // vpiNotOp
    "~",              // vpiBitNeg
    "&",              // vpiUnaryAndOp
    "~&",             // vpiUnaryNandOp
    "|",              // vpiUnaryOrOp
    "~|",             // vpiUnaryNorOp
    "^",              // vpiUnaryXorOp
    "~^",             // vpiUnaryXNorOp
    "-",              // vpiSubOp
    "/",              // vpiDivOp
    "%",              // vpiModOp
    "==",             // vpiEqOp
    "!=",             // vpiNeqOp
    "===",            // vpiCaseEqOp
    "!==",            // vpiCaseNeqOp
    ">",              // vpiGtOp
    ">=",             // vpiGeOp
    "<",              // vpiLtOp
    "<=",             // vpiLeOp
    "<<",             // vpiLShiftOp
    ">>",             // vpiRShiftOp
    "+",              // vpiAddOp
    "*",              // vpiMultOp
    "&&",             // vpiLogAndOp
    "||",             // vpiLogOrOp
    "&",              // vpiBitAndOp
    "|",              // vpiBitOrOp
    "^",              // vpiBitXorOp
    "~^",             // vpiBitXNorOp
    "?:",             // vpiConditionOp(dummy)
    "{}",             // vpiConcatOp(dummy)
    "{{}}",           // vpiMultiConcatOp(dummy)
    "or",             // vpiEventOrOp(dummy)
    ";",              // vpiNullOp(dummy)
    ",",              // vpiListOp(dummy)
    ":",              // vpiMinTypMaxOp(dummy)
    "posedge ",       // vpiPosedgeOp
    "negedge ",       // vpiNegedgeOp
    "<<<",            // vpiArithLShiftOp
    ">>>",            // vpiArithRShiftOp
    "**"              // vpiPowerOp
  };

  // 優先順位のテーブル
  static int pri_table[] = {
    0,                // dummy
    13,               // vpiMinusOp
    13,               // vpiPlusOp
    13,               // vpiNotOp
    13,               // vpiBitNegOp
    13,               // vpiUnaryAndOp
    13,               // vpiUnaryNandOp
    13,               // vpiUnaryOrOp
    13,               // vpiUnaryNorOp
    13,               // vpiUnaryXorOp
    13,               // vpiUnaryXNorOp
    10,               // vpiSubOp
    11,               // vpiDivOp
    11,               // vpiModOp
    7,                // vpiEqOp
    7,                // vpiNeqOp
    7,                // vpiCaseEqOp
    7,                // vpiCaseNeqOp
    8,                // vpiGtOp
    8,                // vpiGeOp
    8,                // vpiLtOp
    8,                // vpiLeOp
    9,                // vpiLShiftOp
    9,                // vpiRShiftOp
    10,               // vpiAddOp
    11,               // vpiMultOp
    3,                // vpiLogAndOp
    2,                // vpiLogOrOp
    6,                // vpiBitAndOp
    4,                // vpiBitOrOp
    5,                // vpiBitXorOp
    5,                // vpiBitXNorOp
    1,                // vpiConditionOp
    0,                // vpiConcatOp
    0,                // vpiMultiConcatOp
    0,                // vpiEventOrOp
    0,                // vpiNullOp
    0,                // vpiListOp
    0,                // vpiMinTypMaxOp
    0,                // vpiPosedgeOp
    0,                // vpiNegedgeOp
    9,                // vpiArithLShiftOp
    9,                // vpiArithRShiftOp
    12                // vpiPowerOp
  };

  std::string ans;

  auto optype = expr.op_type();
  // parent_optype の優先順位が自分の優先順位よりも高ければ括弧が必要
  bool need_par = false;
  int pri = pri_table[static_cast<int>(optype)];
  if ( ppri > pri ) {
    need_par = true;
    ans += "(";
  }

  switch ( optype ) {
    // 空
  case VpiOpType::Null:
    ans += decompile_impl(expr.operand0(), 0);
    break;

    // 単項演算子
  case VpiOpType::Minus:
  case VpiOpType::Not:
  case VpiOpType::BitNeg:
  case VpiOpType::Plus:
  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryXNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    ans += sym_table[static_cast<int>(optype)] +
      decompile_impl(expr.operand0(), pri);
    break;

    // 二項演算子
  case VpiOpType::Add:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift:
  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
  case VpiOpType::Div:
  case VpiOpType::Eq:
  case VpiOpType::Ge:
  case VpiOpType::Gt:
  case VpiOpType::LShift:
  case VpiOpType::Le:
  case VpiOpType::LogAnd:
  case VpiOpType::LogOr:
  case VpiOpType::Lt:
  case VpiOpType::Mod:
  case VpiOpType::Mult:
  case VpiOpType::Neq:
  case VpiOpType::Power:
  case VpiOpType::RShift:
  case VpiOpType::Sub:
    ans += decompile_impl(expr.operand0(), pri) +
      sym_table[static_cast<int>(optype)] +
      decompile_impl(expr.operand1(), pri);
    break;

    // 三項演算子
  case VpiOpType::Condition:
    ans += decompile_impl(expr.operand0(), pri) + "?" +
      decompile_impl(expr.operand1(), pri) + ":" +
      decompile_impl(expr.operand2(), pri);
    break;

  case VpiOpType::MinTypMax:
    ans += decompile_impl(expr.operand0(), pri) + ":" +
      decompile_impl(expr.operand1(), pri) + ":" +
      decompile_impl(expr.operand2(), pri);
    break;

  case VpiOpType::Concat:
    {
      ans += "{";
      const char* delim = "";
      for ( auto expr1: expr.operand_list() ) {
	ans += delim + expr1.decompile();
	delim = ",";
      }
      ans += "}";
    }
    break;

  case VpiOpType::MultiConcat:
    {
      ans = "{";
      ans += expr.rep().decompile() + "{";
      const char* delim = "";
      for ( auto expr1: expr.operand_list() ) {
	ans += delim + expr1.decompile();
	delim = ",";
      }
      ans += "}}";
    }
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  if ( need_par ) {
    ans += ")";
  }
  return ans;
}

// 定数用の処理
std::string
decompile_const(
  const AstExpr& expr
)
{
  if ( expr.const_type() == VpiConstType::Real ) {
    std::ostringstream buf;
    buf << expr.const_real();
    return buf.str();
  }
  if ( expr.const_type() == VpiConstType::String ) {
    return expr.const_str();
  }
  std::ostringstream buf;
  if ( expr.const_size() > 0 ) {
    buf << expr.const_size() << "'";
  }
  switch ( expr.const_type() ) {
  case VpiConstType::Int:
    if ( expr.const_str() == nullptr ) {
      buf << expr.const_bitvect();
      return buf.str();
    }
    break;

  case VpiConstType::SignedBinary:
    buf << "s";
  case VpiConstType::Binary:
    buf << "b";
    break;

  case VpiConstType::SignedOct:
    buf << "s";
  case VpiConstType::Oct:
    buf << "b";
    break;

  case VpiConstType::SignedDec:
    buf << "s";
  case VpiConstType::Dec:
    buf << "d";
    break;

  case VpiConstType::SignedHex:
    buf << "s";
  case VpiConstType::Hex:
    buf << "h";
    break;
  default: // Real と String は処理済み
    break;
  }
  buf << expr.const_str();
  return buf.str();
}

// @brief decompile() の下請け関数
std::string
decompile_impl(
  const AstExpr& expr,
  int ppri
)
{
  switch ( expr.type() ) {
  case AstExpr::Opr:
    return decompile_opr(expr, ppri);

  case AstExpr::Const:
    return decompile_const(expr);

  case AstExpr::FuncCall:
  case AstExpr::SysFuncCall:
    {
      auto ans = expr.decompile_name();
      ans += "(";
      const char* comma = "";
      for ( auto expr1: expr.arg_list() ) {
	ans += comma + expr1.decompile();
	comma = ", ";
      }
      ans += ")";
      return ans;
    }

  case AstExpr::Primary:
    {
      auto ans = expr.decompile_name();
      for ( auto index: expr.index_list() ) {
	ans += "[" + index.decompile() + "]";
      }
      auto part = expr.part();
      if ( part.is_valid() ) {
	const char* delim = nullptr;
	switch ( part.mode() ) {
	case VpiRangeMode::Const: delim = ":"; break;
	case VpiRangeMode::Plus:  delim = "+:"; break;
	case VpiRangeMode::Minus: delim = "-:"; break;
	case VpiRangeMode::No:    ASSERT_NOT_REACHED;
	}
	ans += "[" + part.left().decompile() + delim +
	  part.right().decompile() + "]";
      }
      return ans;
    }

  default:
    ASSERT_NOT_REACHED;
  }

  return std::string();
}

END_NONAMESPACE

// @brief 式の型の取得
AstExpr::Type
AstExpr::type() const
{
  _check_ptr();
  return mPtr->type();
}

// @brief インデックスとして使える式のチェック
bool
AstExpr::is_index_expr() const
{
  _check_ptr();
  return mPtr->is_index_expr();
}

// @brief インデックスの値の取得
int
AstExpr::index_value() const
{
  _check_ptr();
  return mPtr->index_value();
}

// @brief 階層名を持っている時 true を返す．
bool
AstExpr::has_hierarchical_name() const
{
  return namebranch_list().size() > 0;
}

// @brief 式を表す文字列表現の取得
std::string
AstExpr::decompile() const
{
  return decompile_impl(*this, 0);
}

// @brief 演算子の種類の取得
VpiOpType
AstExpr::op_type() const
{
  _check_ptr();
  return mPtr->op_type();
}

// @brief オペランド数を返す．
SizeType
AstExpr::operand_num() const
{
  _check_ptr();
  return mPtr->operand_num();
}

// @brief 0番目のオペランドの取得
AstExpr
AstExpr::operand0() const
{
  _check_ptr();
  return AstExpr(mPtr->operand0());
}

// @brief 1番目のオペランドの取得
AstExpr
AstExpr::operand1() const
{
  _check_ptr();
  return AstExpr(mPtr->operand1());
}

// @brief 2番目のオペランドの取得
AstExpr
AstExpr::operand2() const
{
  _check_ptr();
  return AstExpr(mPtr->operand2());
}

// @brief オペランドのリストの取得
AstExprList
AstExpr::operand_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->operand_top()));
}

// @brief multi-concat の繰り返し数
AstExpr
AstExpr::rep() const
{
  _check_ptr();
  return AstExpr(mPtr->rep());
}

// @brief 引数リストの取得
AstExprList
AstExpr::arg_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->arg_top()));
}

// @brief 定数インデックスのチェック
bool
AstExpr::is_const_index() const
{
  _check_ptr();
  return mPtr->is_const_index();
}

// @brief インデックスリストの取得
AstExprList
AstExpr::index_list() const
{
  _check_ptr();
  return AstExprList(AstExpr(mPtr->index_top()));
}

// @brief 範囲指定を表す構文木を返す．
AstPart
AstExpr::part() const
{
  _check_ptr();
  return AstPart(mPtr->part());
}

// @brief simple primary のチェック
bool
AstExpr::is_simple() const
{
  _check_ptr();
  return mPtr->is_simple();
}

// @brief 定数の種類の取得
VpiConstType
AstExpr::const_type() const
{
  _check_ptr();
  return mPtr->const_type();
}

// @brief 整数型の定数のサイズの取得
SizeType
AstExpr::const_size() const
{
  _check_ptr();
  return mPtr->const_size();
}

// @brief ビットベクタ型の値の取得
BitVector
AstExpr::const_bitvect() const
{
  _check_ptr();
  return mPtr->const_bitvect();
}

// @brief 整数型および文字列型の定数の文字列表現の取得
const char*
AstExpr::const_str() const
{
  _check_ptr();
  return mPtr->const_str();
}

// @brief 実数型の値の取得
double
AstExpr::const_real() const
{
  _check_ptr();
  return mPtr->const_real();
}

// @brief 適切な値を持っている時 true を返す．
bool
AstExpr::is_valid() const
{
  return mPtr != nullptr;
}

// @brief ファイル位置の取得
FileRegion
AstExpr::file_region() const
{
  _check_ptr();
  return mPtr->file_region();
}

// @brief 比較用のユニークなキーを返す．
PtrIntType
AstExpr::key() const
{
  return _key();
}

// @brief 名前の取得
const char*
AstExpr::name() const
{
  _check_ptr();
  return mPtr->name();
}

// @brief 階層ブランチのリストを返す．
AstNameBranchList
AstExpr::namebranch_list() const
{
  _check_ptr();
  return AstNameBranchList(AstNameBranch(mPtr->namebranch_top()));
}

// @brief 次の要素を返す．
AstExpr
AstExpr::next() const
{
  _check_ptr();
  return AstExpr(mPtr->link());
}

// @brief json_obj() の下請け関数
void
AstExpr::json_sub(
  JsonValue& jobj
) const
{
  AstHierNamedBase::json_sub(jobj);
  switch ( type() ) {
  case AstExpr::Opr:         json_sub_opr(jobj); break;
  case AstExpr::FuncCall:    json_sub_funccall(jobj); break;
  case AstExpr::SysFuncCall: json_sub_funccall(jobj); break;
  case AstExpr::Primary:     json_sub_primary(jobj); break;
  case AstExpr::Const:       json_sub_const(jobj); break;
  }
}

// @brief Opr 用の json_sub()
void
AstExpr::json_sub_opr(
  JsonValue& jobj
) const
{
  std::ostringstream buf;
  buf << op_type();
  jobj.add("op_type", JsonValue(buf.str()));
  auto opnum = operand_num();
  if ( opnum > 0 ) {
    jobj.add("operand0", operand0().json_obj());
  }
  if ( opnum > 1 ) {
    jobj.add("operand1", operand1().json_obj());
  }
  if ( opnum > 2 ) {
    jobj.add("operand2", operand2().json_obj());
  }
  if ( opnum == 0 ) {
    jobj.add("operand_list", operand_list().json_obj());
  }
  if ( op_type() == VpiOpType::MultiConcat ) {
    jobj.add("rep", rep().json_obj());
  }
}

// @brief FuncCall 用の json_sub()
void
AstExpr::json_sub_funccall(
  JsonValue& jobj
) const
{
  jobj.add("arg_list", arg_list().json_obj());
}

// @brief Primary 用の json_sub()
void
AstExpr::json_sub_primary(
  JsonValue& jobj
) const
{
  jobj.add("is_const_index", JsonValue(is_const_index()));
  if ( !index_list().empty() ) {
    jobj.add("index_list", index_list().json_obj());
  }
  if ( part().is_valid() ) {
    jobj.add("part", part().json_obj());
  }
}

// @brief Const 用の json_sub()
void
AstExpr::json_sub_const(
  JsonValue& jobj
) const
{
  const char* type_str;
  switch ( const_type() ) {
  case VpiConstType::Dec:           type_str = "Dec"; break;
  case VpiConstType::Real:          type_str = "Real"; break;
  case VpiConstType::Binary:        type_str = "Binary"; break;
  case VpiConstType::Oct:           type_str = "Oct"; break;
  case VpiConstType::Hex:           type_str = "Hex"; break;
  case VpiConstType::String:        type_str = "String"; break;
  case VpiConstType::Int:           type_str = "Int"; break;
  case VpiConstType::SignedDec:     type_str = "SignedDec"; break;
  case VpiConstType::SignedBinary:  type_str = "SignedBinary"; break;
  case VpiConstType::SignedOct:     type_str = "SignedOct"; break;
  case VpiConstType::SignedHex:     type_str = "SignedHex"; break;
  }
  jobj.add("const_type", JsonValue(type_str));
  jobj.add("const_size", JsonValue(static_cast<int>(const_size())));
  JsonUtils::add(jobj, "const_str", const_str());
  if ( const_type() == VpiConstType::Real ) {
    jobj.add("const_real", const_real());
  }
}

END_NAMESPACE_YM_VERILOG
