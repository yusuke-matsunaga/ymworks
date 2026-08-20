
/// @file ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ElbEnv.h"
#include "ErrorGen.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"
#include "elaborator/ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief AstOpr から ElbExpr を生成する．
ElbExpr*
ExprGen::instantiate_opr(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr* ast_expr
)
{
  auto op_type = ast_expr->op_type();
  SizeType opr_size{ast_expr->operand_num()};

  ElbExpr* opr0{nullptr};
  ElbExpr* opr1{nullptr};
  ElbExpr* opr2{nullptr};
  ElbExpr* expr{nullptr};
  bool real_check{false};
  switch ( op_type ) {
  case VpiOpType::Posedge:
  case VpiOpType::Negedge:
    ErrorGen::illegal_edge_descriptor(__FILE__, __LINE__, ast_expr);
    break;

  case VpiOpType::BitNeg:
  case VpiOpType::UnaryAnd:
  case VpiOpType::UnaryNand:
  case VpiOpType::UnaryOr:
  case VpiOpType::UnaryNor:
  case VpiOpType::UnaryXor:
  case VpiOpType::UnaryXNor:
    real_check = true;
    // わざと次の case に続く．
  case VpiOpType::Plus:
  case VpiOpType::Minus:
  case VpiOpType::Not:
    opr0 = instantiate_expr(parent, env, ast_expr->operand0());
    if ( real_check && opr0->value_type().is_real_type() ) {
      ErrorGen::illegal_real_type(__FILE__, __LINE__, ast_expr->operand0());
    }
    return mgr().new_UnaryOp(ast_expr, op_type, opr0);

  case VpiOpType::BitAnd:
  case VpiOpType::BitOr:
  case VpiOpType::BitXNor:
  case VpiOpType::BitXor:
  case VpiOpType::LShift:
  case VpiOpType::RShift:
  case VpiOpType::ArithLShift:
  case VpiOpType::ArithRShift:
    real_check = true;
    // わざと次の case に続く．
  case VpiOpType::Add:
  case VpiOpType::Sub:
  case VpiOpType::Mult:
  case VpiOpType::Div:
  case VpiOpType::Mod:
  case VpiOpType::Power:
  case VpiOpType::LogAnd:
  case VpiOpType::LogOr:
  case VpiOpType::CaseEq:
  case VpiOpType::CaseNeq:
  case VpiOpType::Eq:
  case VpiOpType::Neq:
  case VpiOpType::Ge:
  case VpiOpType::Gt:
  case VpiOpType::Le:
  case VpiOpType::Lt:
    opr0 = instantiate_expr(parent, env, ast_expr->operand0());
    opr1 = instantiate_expr(parent, env, ast_expr->operand1());
    if ( real_check ) {
      if ( opr0->value_type().is_real_type() ) {
	ErrorGen::illegal_real_type(__FILE__, __LINE__, ast_expr->operand0());
      }
      if ( opr1->value_type().is_real_type() ) {
	ErrorGen::illegal_real_type(__FILE__, __LINE__, ast_expr->operand1());
      }
    }
    expr = mgr().new_BinaryOp(ast_expr, op_type, opr0, opr1);
    break;

  case VpiOpType::Condition:
  case VpiOpType::MinTypMax:
    opr0 = instantiate_expr(parent, env, ast_expr->operand0());
    opr1 = instantiate_expr(parent, env, ast_expr->operand1());
    opr2 = instantiate_expr(parent, env, ast_expr->operand2());
    expr = mgr().new_TernaryOp(ast_expr, op_type, opr0, opr1, opr2);
    break;

  case VpiOpType::Concat:
    {
      std::vector<ElbExpr*> opr_list;
      opr_list.reserve(opr_size);
      for ( auto ast_expr1: ast_expr->operand_list() ) {
	auto expr1 = instantiate_expr(parent, env, ast_expr1);
	auto type1 = expr1->value_type();
	if ( type1.is_real_type() ) {
	  ErrorGen::illegal_real_type(__FILE__, __LINE__, ast_expr1);
	}
	opr_list.push_back(expr1);
      }

      expr = mgr().new_ConcatOp(ast_expr, opr_list);
    }
    break;

  case VpiOpType::MultiConcat:
    {
      auto ast_rep = ast_expr->rep();
      auto rep_num = evaluate_int(parent, ast_rep);
      auto rep_expr = instantiate_expr(parent, env, ast_rep);
      std::vector<ElbExpr*> opr_list;
      opr_list.reserve(opr_size);
      for ( auto ast_expr1: ast_expr->operand_list() ) {
	auto expr1 = instantiate_expr(parent, env, ast_expr1);
	auto type1 = expr1->value_type();
	if ( type1.is_real_type() ) {
	  ErrorGen::illegal_real_type(__FILE__, __LINE__, ast_expr1);
	}
	opr_list.push_back(expr1);
      }
      expr = mgr().new_MultiConcatOp(ast_expr, rep_num, rep_expr, opr_list);
    }
    break;

  default:
    throw std::logic_error{"Should not be reached"};
  }

  // attribute instance の生成
  auto attr_list = attribute_list(ast_expr);
  mgr().reg_attr(expr, attr_list);

  return expr;
}

END_NAMESPACE_YM_VERILOG
