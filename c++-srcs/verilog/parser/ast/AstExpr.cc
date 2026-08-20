
/// @file AstExpr.cc
/// @brief AstExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/vl/AstExpr.h"
#include "ym/vl/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス AstExpr
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

std::string
decompile_impl(
  const AstExpr* expr,
  int ppri
);

// @brief 演算子の場合の decompile() 処理
std::string
decompile_opr(
  const AstExpr* expr,
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

  auto optype = expr->op_type();
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
    ans += decompile_impl(expr->operand0(), 0);
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
      decompile_impl(expr->operand0(), pri);
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
    ans += decompile_impl(expr->operand0(), pri) +
      sym_table[static_cast<int>(optype)] +
      decompile_impl(expr->operand1(), pri);
    break;

    // 三項演算子
  case VpiOpType::Condition:
    ans += decompile_impl(expr->operand0(), pri) + "?" +
      decompile_impl(expr->operand1(), pri) + ":" +
      decompile_impl(expr->operand2(), pri);
    break;

  case VpiOpType::MinTypMax:
    ans += decompile_impl(expr->operand0(), pri) + ":" +
      decompile_impl(expr->operand1(), pri) + ":" +
      decompile_impl(expr->operand2(), pri);
    break;

  case VpiOpType::Concat:
    {
      ans += "{";
      const char* delim = "";
      for ( auto expr1: expr->operand_list() ) {
	ans += delim + expr1->decompile();
	delim = ",";
      }
      ans += "}";
    }
    break;

  case VpiOpType::MultiConcat:
    {
      ans = "{";
      ans += expr->rep()->decompile() + "{";
      const char* delim = "";
      for ( auto expr1: expr->operand_list() ) {
	ans += delim + expr1->decompile();
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
  const AstExpr* expr
)
{
  if ( expr->const_type() == VpiConstType::Real ) {
    std::ostringstream buf;
    buf << expr->const_real();
    return buf.str();
  }
  if ( expr->const_type() == VpiConstType::String ) {
    return expr->const_str();
  }
  std::ostringstream buf;
  if ( expr->const_size() > 0 ) {
    buf << expr->const_size() << "'";
  }
  switch ( expr->const_type() ) {
  case VpiConstType::Int:
    if ( expr->const_str() == nullptr ) {
      buf << expr->const_bitvect();
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
  buf << expr->const_str();
  return buf.str();
}

// @brief decompile() の下請け関数
std::string
decompile_impl(
  const AstExpr* expr,
  int ppri
)
{
  switch ( expr->type() ) {
  case AstExpr::Opr:
    return decompile_opr(expr, ppri);

  case AstExpr::Const:
    return decompile_const(expr);

  case AstExpr::FuncCall:
  case AstExpr::SysFuncCall:
    {
      auto ans = expr->decompile_name();
      ans += "(";
      const char* comma = "";
      for ( auto expr1: expr->operand_list() ) {
	ans += comma + expr1->decompile();
	comma = ", ";
      }
      ans += ")";
      return ans;
    }

  case AstExpr::Primary:
    {
      auto ans = expr->decompile_name();
      auto n = expr->index_num();
      for ( auto index: expr->index_list() ) {
	ans += "[" + index->decompile() + "]";
      }
      if ( expr->part() != nullptr ) {
	auto part = expr->part();
	const char* delim = nullptr;
	switch ( part->mode() ) {
	case VpiRangeMode::Const: delim = ":"; break;
	case VpiRangeMode::Plus:  delim = "+:"; break;
	case VpiRangeMode::Minus: delim = "-:"; break;
	case VpiRangeMode::No:    ASSERT_NOT_REACHED;
	}
	ans += "[" + part->left()->decompile() + delim +
	  part->right()->decompile() + "]";
      }
      return ans;
    }

  default:
    ASSERT_NOT_REACHED;
  }

  return std::string();
}

END_NONAMESPACE


// @brief 式を表す文字列表現の取得
std::string
AstExpr::decompile() const
{
  return decompile_impl(this, 0);
}

END_NAMESPACE_YM_VERILOG
