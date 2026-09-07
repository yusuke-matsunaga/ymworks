
/// @file ElbProxy.cc
/// @brief ElbProxy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ElbProxy.h"
#include "ElbEnv.h"
#include "ElbError.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "ExprEval.h"
#include "AttrGen.h"

#include "elaborator/ElbExpr.h"
#include "parser/AstDumper.h"

#include "ym/vl/AstExpr.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
ElbProxy::ElbProxy(
  Elaborator& elab,
  ElbMgr& elb_mgr
) : mElaborator{elab},
    mMgr{elb_mgr}
{
}

// @brief デストラクタ
ElbProxy::~ElbProxy()
{
}

// @brief 初期化を行う．
void
ElbProxy::init(
  ModuleGen* module_gen,
  DeclGen* decl_gen,
  ItemGen* item_gen,
  StmtGen* stmt_gen,
  ExprGen* expr_gen,
  ExprEval* expr_eval,
  AttrGen* attr_gen
)
{
  mModuleGen = module_gen;
  mDeclGen = decl_gen;
  mItemGen = item_gen;
  mStmtGen = stmt_gen;
  mExprGen = expr_gen;
  mExprEval = expr_eval;
  mAttrGen = attr_gen;
}

// @brief 対応する宣言に範囲があり，IO宣言に範囲がない記述を認めるか
bool
ElbProxy::allow_empty_io_range()
{
  return mElaborator.mAllowEmptyIORange;
}

// @brief module の中身のうちスコープに関係するインスタンス化を行う．
void
ElbProxy::phase1_module_item(
  ElbModule* module,
  const AstModule& ast_module,
  const std::vector<ElbParamCon>& param_con_list
)
{
  mModuleGen->phase1_module_item(module, ast_module, param_con_list);
}

// @brief parameter と genvar を実体化する．
void
ElbProxy::phase1_decl(
  const VlScope* parent,
  const AstDeclHeadList& ast_head_list,
  bool force_to_local
)
{
  mDeclGen->phase1_decl(parent, ast_head_list, force_to_local);
}

// @brief IO宣言要素を実体化する．
void
ElbProxy::instantiate_iodecl(
  ElbModule* module,
  const AstIOHeadList& ast_head_list
)
{
  mDeclGen->instantiate_iodecl(module, ast_head_list);
}

// @brief IO宣言要素を実体化する．
void
ElbProxy::instantiate_iodecl(
  ElbTaskFunc* taskfunc,
  const AstIOHeadList& ast_head_list
)
{
  mDeclGen->instantiate_iodecl(taskfunc, ast_head_list);
}

// @brief 宣言要素のリストをインスタンス化する．
void
ElbProxy::instantiate_decl(
  const VlScope* parent,
  const AstDeclHeadList& ast_head_list
)
{
  mDeclGen->instantiate_decl(parent, ast_head_list);
}

// @brief スコープに関係する要素を実体化する．
void
ElbProxy::phase1_items(
  const VlScope* parent,
  const AstItemList& ast_item_list
)
{
  mItemGen->phase1_items(parent, ast_item_list);
}

// @brief constant function の生成を行う．
const VlTaskFunc*
ElbProxy::instantiate_constant_function(
  const VlScope* parent,
  const AstItem& ast_function
)
{
  return mItemGen->instantiate_constant_function(parent, ast_function);
}

// @brief スコープに関係するステートメントの実体化を行う．
void
ElbProxy::phase1_stmt(
  const VlScope* parent,
  const AstStmt& ast_stmt,
  bool cf
)
{
  mStmtGen->phase1_stmt(parent, ast_stmt, cf);
}

// @brief ステートメントの実体化を行う．
const VlStmt*
ElbProxy::instantiate_stmt(
  const VlScope* parent,
  const VlProcess* process,
  const ElbEnv& env,
  const AstStmt& ast_stmt
)
{
  return mStmtGen->instantiate_stmt(parent, process, env, ast_stmt);
}

// @brief AstExpr から ElbiExpr を生成する
ElbExpr*
ElbProxy::instantiate_expr(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  return mExprGen->instantiate_expr(parent, env, ast_expr);
}

// @brief AstExpr から定数式の ElbExpr を生成する
ElbExpr*
ElbProxy::instantiate_constant_expr(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprGen->instantiate_constant_expr(parent, ast_expr);
}

// @brief AstExpr からイベント式の ElbiExpr を生成する
ElbExpr*
ElbProxy::instantiate_event_expr(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  return mExprGen->instantiate_event_expr(parent, env, ast_expr);
}

// @brief AstExpr からシステム関数の引数を生成する．
ElbExpr*
ElbProxy::instantiate_arg(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  return mExprGen->instantiate_arg(parent, env, ast_expr);
}

// @brief AstExpr から左辺式を生成する
ElbExpr*
ElbProxy::instantiate_lhs(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr
)
{
  return mExprGen->instantiate_lhs(parent, env, ast_expr);
}

// @brief AstExpr から右辺式を生成する
ElbExpr*
ElbProxy::instantiate_rhs(
  const VlScope* parent,
  const ElbEnv& env,
  const AstExpr& ast_expr,
  ElbExpr* lhs
)
{
  auto expr = mExprGen->instantiate_expr(parent, env, ast_expr);
  if ( expr ) {
    // lhs の型を expr に設定する．
    expr->set_reqsize(lhs->value_type());
  }
  return expr;
}

// @brief AstExpr(primary) から named_event を生成する．
ElbExpr*
ElbProxy::instantiate_namedevent(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprGen->instantiate_namedevent(parent, ast_expr);
}

// @brief AstDelay から ElbExpr を生成する．
const VlDelay*
ElbProxy::instantiate_delay(
  const VlScope* parent,
  const AstDelay& ast_delay
)
{
  return mExprGen->instantiate_delay(parent, ast_delay);
}

// @brief AstOrderedCon から ElbExpr を生成する．
const VlDelay*
ElbProxy::instantiate_delay(
  const VlScope* parent,
  const AstItem& ast_head
)
{
  return mExprGen->instantiate_delay(parent, ast_head);
}

// @brief 定数式の値を評価する．
VlValue
ElbProxy::evaluate_expr(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprEval->evaluate_expr(parent, ast_expr);
}

// @brief 定数式を評価し int 値を返す．
int
ElbProxy::evaluate_int(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprEval->evaluate_int(parent, ast_expr);
}

// @brief 定数式ならばを評価し int 値を返す．
int
ElbProxy::evaluate_int_if_const(
  const VlScope* parent,
  const AstExpr& ast_expr,
  bool& is_const
)
{
  return mExprEval->evaluate_int_if_const(parent, ast_expr, is_const);
}

// @brief 定数式を評価しスカラー値を返す．
VlScalarVal
ElbProxy::evaluate_scalar(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprEval->evaluate_scalar(parent, ast_expr);
}

// @brief 定数式を評価し bool 値を返す．
bool
ElbProxy::evaluate_bool(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprEval->evaluate_bool(parent, ast_expr);
}

// @brief 定数式を評価しビットベクタ値を返す．
BitVector
ElbProxy::evaluate_bitvector(
  const VlScope* parent,
  const AstExpr& ast_expr
)
{
  return mExprEval->evaluate_bitvector(parent, ast_expr);
}

// @brief 範囲を表す式を評価する．
RangeVal
ElbProxy::evaluate_range(
  const VlScope* parent,
  const AstRange& ast_range
)
{
  return mExprEval->evaluate_range(parent, ast_range);
}

// @brief 範囲を表す式を評価する．
RangeVal
ElbProxy::evaluate_range(
  const VlScope* parent,
  const AstPart& ast_part
)
{
  return mExprEval->evaluate_range(parent, ast_part);
}

// @brief 構文木要素に対応する属性リストを返す．
const std::vector<const VlAttribute*>&
ElbProxy::attribute_list(
  const AstBase& ast_obj
)
{
  return mAttrGen->attribute_list(ast_obj);
}

// @brief 構文木要素に対応する属性リストを返す．
std::vector<const VlAttribute*>
ElbProxy::attribute_list(
  const AstBase& ast_obj1,
  const AstBase& ast_obj2
)
{
  auto ans = mAttrGen->attribute_list(ast_obj1);
  const auto& ans1 = mAttrGen->attribute_list(ast_obj2);
  ans.insert(ans.end(), ans1.begin(), ans1.end());
  return ans;
}

// @brief 同名のオブジェクトが定義されていないか調べる．
void
ElbProxy::check_name(
  const VlScope* parent,
  const char* name,
  const FileRegion& file_region
)
{
  if ( name == nullptr ) {
    return;
  }
  auto prev_obj = find_obj(parent, name);
  if ( prev_obj != nullptr ) {
    error_dup_name(__FILE__, __LINE__,
		   file_region,
		   name,
		   prev_obj->file_region());
  }
}

// @brief 重複した名前を持つ．
void
ElbProxy::error_dup_name(
  const char* file_name,
  int line,
  const FileRegion& loc,
  const char* name,
  const FileRegion& prev_loc
)
{
  std::ostringstream buf;
  buf << "\"" << name
      << "\": redefined. previous location is "
      << prev_loc;
  throw ElbError(file_name, line,
		 loc,
		 "ELAB_DUP_NAME",
		 buf.str());
}

// @brief 対象の要素が見つからない．
void
ElbProxy::error_not_found(
  const char* file_name,
  int line,
  const FileRegion& file_region,
  const char* name
)
{
  std::ostringstream buf;
  buf << "\"" << name
      << "\": Not found";
  throw ElbError(file_name, line,
		 file_region,
		 "ELAB_NOT_FOUND",
		 buf.str());
}

// @brief エラーメッセージを出力する．
void
ElbProxy::put_error(
  const ElbError& error
)
{
  MsgMgr::put_msg(error.file(), error.line(),
		  error.file_region(),
		  MsgType::Error,
		  error.label().c_str(),
		  error.message());
}

// @brief 警告メッセージを出力する．
void
ElbProxy::put_warning(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)

{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Warning,
		  label,
		  msg);
}

// @brief 情報メッセージを出力する．
void
ElbProxy::put_info(
  const char* file,
  int line,
  const FileRegion& loc,
  const char* label,
  const std::string& msg
)
{
  MsgMgr::put_msg(file, line,
		  loc,
		  MsgType::Info,
		  label,
		  msg);
}

END_NAMESPACE_YM_VERILOG
