
/// @file ExprGen_lhs.cc
/// @brief ExprGen の実装ファイル(左辺式)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ym/vl/BitVector.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"
#include "elaborator/ElbExpr.h"
#include "elaborator/ElbPrimary.h"
#include "elaborator/ElbError.h"
#include "ElbEnv.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprGen
//////////////////////////////////////////////////////////////////////

// @brief AstExpr から左辺式を生成する
const VlExpr*
ExprGen::instantiate_lhs(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  std::vector<const VlExpr*> lhs_list;
  instantiate_lhs_sub(parent, env, ast_expr, lhs_list);
  if ( lhs_list.size() == 1 ) {
    return lhs_list.front();
  }
  auto expr = elb_mgr().new_Lhs(lhs_list);
  return expr;
}

// @brief instantiate_lhs() の下請け関数
void
ExprGen::instantiate_lhs_sub(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr,
  std::vector<const VlExpr*>& expr_list
)
{
  switch ( ast_expr.type() ) {
  case AstExpr::Opr:
    // 左辺では concatination しか適当でない．
    if ( ast_expr.op_type() == VpiOpType::Concat ) {
      auto ast_opr_list = ast_expr.operand_list().to_vector();
      auto opr_size = ast_opr_list.size();
      for ( SizeType i = 0; i < opr_size; ++ i ) {
	// 現れた順は上位ビットからなので位置が逆になる．
	SizeType pos = opr_size - i - 1;
	auto ast_expr1 = ast_opr_list[pos];
	instantiate_lhs_sub(parent, env, ast_expr1, expr_list);
      }
      return;
    }
    // それ以外の演算子はエラー
    log_mgr().error_illegal_operator_in_lhs(__FILE__, __LINE__, ast_expr);
    break;

  case AstExpr::Primary:
    instantiate_lhs_primary(parent, env, ast_expr, expr_list);
    return;

  case AstExpr::Const:
    log_mgr().error_illegal_constant_in_lhs(__FILE__, __LINE__, ast_expr);

  case AstExpr::FuncCall:
    log_mgr().error_illegal_funccall_in_lhs(__FILE__, __LINE__, ast_expr);

  case AstExpr::SysFuncCall:
    log_mgr().error_illegal_sysfunccall_in_lhs(__FILE__, __LINE__, ast_expr);

  default:
    break;
  }

  throw std::logic_error{"ExprGen::instantiate_lhs_sub(): wrong ast_expr.type()"};
}

// @brief 左辺のプライマリ式の生成を行う．
void
ExprGen::instantiate_lhs_primary(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr,
  std::vector<const VlExpr*>& expr_list
)
{
  // 名前に対応したオブジェクトのハンドルを取り出す．
  auto handle = find_handle(parent, env, ast_expr);

  // 添字には constant/constant function 以外の情報は引き継がない
  ElbEnv index_env;
  if ( ast_expr.is_const_index() ) {
    index_env = ElbConstantEnv();
  }
  else if ( env.inside_constant_function() ) {
    index_env = ElbConstantFunctionEnv(env.constant_function());
  }

  // 宣言要素の参照を求める．
  // ビット指定式が含まれていたら ast_bit に入れる．
  AstExpr ast_bit;
  auto decl_ref = instantiate_decl_ref(handle, parent, index_env,
				       ast_expr, ast_bit);

  // 宣言要素のタイプをチェックする．
  check_decl(env, ast_expr, decl_ref, ast_bit.is_valid());

  auto var_index = !env.is_net_lhs();
  auto sel = instantiate_selecter(parent, ast_expr, decl_ref.declbase(),
				  index_env, ast_bit, var_index);
  auto expr = elb_mgr().new_Primary(ast_expr, decl_ref, sel);
  expr_list.push_back(expr);
}

// @brief AstExpr から継続的代入文の左辺式を生成する
const VlExpr*
ExprGen::instantiate_net_lhs(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  ElbNetLhsEnv env;
  return instantiate_lhs(parent, env, ast_expr);
}

END_NAMESPACE_YM_VERILOG
