
/// @file ExprGen_funccall.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ExprGen.h"
#include "ElbEnv.h"
#include "ErrorGen.h"

#include "ym/vl/AstModule.h"
#include "ym/vl/AstItem.h"
#include "ym/vl/AstExpr.h"
#include "ym/vl/AstMisc.h"

#include "ym/vl/VlModule.h"
#include "ym/vl/VlIODecl.h"
#include "ym/vl/VlDecl.h"
#include "ym/vl/VlTaskFunc.h"
#include "ym/vl/VlStmt.h"

#include "elaborator/ElbExpr.h"
#include "elaborator/ElbUserSystf.h"


BEGIN_NONAMESPACE

const int debug = 0;
#define dout std::cout

END_NONAMESPACE


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

void
put_value_type(
  std::ostream& s,
  const VlValueType& type
)
{
  if ( type.is_int_type() ) {
    s << "integer type";
  }
  else if ( type.is_real_type() ) {
    s << "real type";
  }
  else if ( type.is_time_type() ) {
    s << "time type";
  }
  else {
    s << "bitvector type: ";
    if ( type.is_signed() ) {
      s << "signed";
    }
    else {
      s << "unsigned";
    }
    s << " , ";
    if ( type.is_sized() ) {
      s << type.size() << " bits";
    }
    else {
      s << "unsized";
    }
  }
}

END_NONAMESPACE

// @brief AstFuncCall から ElbExpr を生成する．
ElbExpr*
ExprGen::instantiate_funccall(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr* ast_expr
)
{
  const VlTaskFunc* child_func{nullptr};
  if ( env.is_constant() ) {
    // 定数関数を探し出す．
    if ( ast_expr->namebranch_list().size() > 0 ) {
      // 階層名は受け付けない．
      ErrorGen::hname_in_ce(__FILE__, __LINE__, ast_expr);
    }

    // 関数名
    auto name = ast_expr->name();

    // 関数本体を探し出す．
    // constant function はモジュール直下にしかあり得ない
    // <- generated scope 内の関数は constant function ではない．
    auto module = parent->parent_module();
    auto ast_func = find_funcdef(module, name);
    if ( !ast_func ) {
      // 関数が見つからなかった．
      ErrorGen::no_such_function(__FILE__, __LINE__, ast_expr);
    }

    if ( ast_func->is_in_use() ) {
      // 自分自身を呼び出している．
      ErrorGen::uses_itself(__FILE__, __LINE__, ast_expr);
    }

    child_func = find_constant_function(module, name);
    if ( child_func == nullptr ) {
      ast_func->set_in_use();
      // なかったので作る．
      child_func = instantiate_constant_function(parent, ast_func);
      ast_func->clear_in_use();
    }
    if ( !child_func ) {
      // 定数関数ではなかった．
      ErrorGen::not_a_constant_function(__FILE__, __LINE__, ast_expr);
    }
  }
  else {
    // 関数本体を探し出す．
    auto handle = mgr().find_obj_up(parent, ast_expr, nullptr);
    if ( handle == nullptr ) {
      // 関数が見つからなかった．
      ErrorGen::no_such_function(__FILE__, __LINE__, ast_expr);
    }
    if ( handle->type() != VpiObjType::Function ) {
      // 関数ではなかった．
      ErrorGen::not_a_function(__FILE__, __LINE__, ast_expr);
    }
    child_func = handle->taskfunc();
    if ( child_func == nullptr ) {
      throw std::logic_error{"child_func == nullptr"};
    }
  }

  // 引数の生成
  SizeType n{ast_expr->operand_num()};
  if ( n != child_func->io_num() ) {
    // 引数の数が合わなかった．
    ErrorGen::n_of_arguments_mismatch(__FILE__, __LINE__, ast_expr);
  }

  std::vector<ElbExpr*> arg_list;
  arg_list.reserve(n);
  for ( auto ast_expr1: ast_expr->operand_list() ) {
    auto expr1 = instantiate_expr(parent, env, ast_expr1);
    auto io_decl = child_func->io(arg_list.size());
    auto decl = io_decl->decl();
    if ( decl->value_type() != expr1->value_type() ) {
      if ( debug ) {
	dout << "decl->value_type() = ";
	put_value_type(dout, decl->value_type());
	dout << std::endl
	     << "expr1->value_type() = ";
	put_value_type(dout, expr1->value_type());
	dout << std::endl;
      }
      ErrorGen::illegal_argument_type(__FILE__, __LINE__, ast_expr);
    }
    arg_list.push_back(expr1);
  }

  // function call の生成
  auto expr = mgr().new_FuncCall(ast_expr, child_func, arg_list);

  // attribute instance の生成
  auto attr_list = attribute_list(ast_expr);
  mgr().reg_attr(expr, attr_list);

  return expr;
}

// AstSysFuncCall から引数を生成する．
ElbExpr*
ExprGen::instantiate_sysfunccall(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr* ast_expr
)
{
  auto name = ast_expr->name();

  // system function を探し出す．
  auto user_systf = mgr().find_user_systf(name);
  if ( user_systf == nullptr ) {
    ErrorGen::no_such_sysfunction(__FILE__, __LINE__, ast_expr);
  }

  // 引数の数のチェック
  auto n = ast_expr->operand_num();
  if ( !user_systf->check_n_of_args(n) ) {
    ErrorGen::n_of_arguments_mismatch(__FILE__, __LINE__, ast_expr);
  }

  // 引数の生成
  std::vector<ElbExpr*> arg_list;
  arg_list.reserve(n);
  for ( auto ast_expr1: ast_expr->operand_list() ) {
    ElbExpr* arg = nullptr;
    if ( ast_expr ) {
      arg = instantiate_arg(parent, env, ast_expr1);
    }
    else {
      // 関数呼び出しと異なり，空の引数がありうる．
      ;
    }
    if ( !user_systf->check_argument(arg_list.size(), arg) ) {
      ErrorGen::illegal_argument_type(__FILE__, __LINE__, ast_expr);
    }
    arg_list.push_back(arg);
  }

  // system function call の生成
  return mgr().new_SysFuncCall(ast_expr, user_systf, arg_list);
}

END_NAMESPACE_YM_VERILOG
