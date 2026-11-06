
/// @file ExprGen_net_lhs.cc
/// @brief ExprGen の実装ファイル(継続的代入文の左辺)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"
#include "elaborator/ElbDeclRef.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbError.h"
#include "ElbEnv.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprGen
//////////////////////////////////////////////////////////////////////

// @brief AstExpr から継続的代入文の左辺式を生成する
ElbExpr*
ExprGen::instantiate_net_lhs(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  switch ( ast_expr.type() ) {
  case AstExpr::Opr:
    // 左辺では concatination しか適当でない．
    if ( ast_expr.op_type() == VpiOpType::Concat ) {
      auto ast_opr_list = ast_expr.operand_list().to_vector();
      auto opr_size = ast_opr_list.size();
      std::vector<ElbExpr*> opr_list(opr_size);
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	// 現れた順は上位ビットからなので位置が逆になる．
	SizeType pos = opr_size - i - 1;
	auto ast_expr1 = ast_opr_list[pos];
	auto expr1 = instantiate_net_lhs(parent, ast_expr1);
	opr_list[pos] = expr1;
      }
      auto expr = elb_mgr().new_ConcatOp(ast_expr, opr_list);
      expr->set_selfsize();

      // attribute instance の生成
      auto attr_list = attribute_list(ast_expr);
      elb_mgr().reg_attr(expr, attr_list);

      return expr;
    }
    // それ以外の演算子はエラー
    log_mgr().error_illegal_operator_in_lhs(__FILE__, __LINE__, ast_expr);
    break;

  case AstExpr::Primary:
    return instantiate_net_primary(parent, ast_expr);

  case AstExpr::Const:
    log_mgr().error_illegal_constant_in_lhs(__FILE__, __LINE__, ast_expr);

  case AstExpr::FuncCall:
    log_mgr().error_illegal_funccall_in_lhs(__FILE__, __LINE__, ast_expr);

  case AstExpr::SysFuncCall:
    log_mgr().error_illegal_sysfunccall_in_lhs(__FILE__, __LINE__, ast_expr);

  default:
    break;
  }

  throw std::logic_error{"Should not be reached"};
}

// @brief AstPrimary から ElbExpr を生成する．
ElbExpr*
ExprGen::instantiate_net_primary(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  if ( ast_expr.namebranch_list().size() > 0 ) {
    // 階層付き識別子はエラー
    throw std::logic_error{"Net LHS should not have hierarchical name"};
  }

  // 名前に対応したオブジェクトのハンドルを取り出す．
  auto env = ElbEnv();
  auto handle = find_handle(parent, env, ast_expr);

  auto index_env = ElbConstantEnv();
  AstExpr ast_bit;
  auto decl_ref = instantiate_decl_ref(handle, parent, index_env,
				       ast_expr, ast_bit);
  if ( ast_bit.is_valid() ) {
    // ビット選択あり
    return instantiate_bitselect(parent, ast_expr, decl_ref,
				 index_env, ast_bit, false);
  }
  auto ast_part = ast_expr.part();
  if ( ast_part.is_valid() ) {
    // 部分選択あり
    return instantiate_partselect(parent, ast_expr, decl_ref,
				  index_env, ast_part, false);
  }
  // 単純なプライマリ式
  return elb_mgr().new_Primary(ast_expr, decl_ref);
}

END_NAMESPACE_YM_VERILOG
