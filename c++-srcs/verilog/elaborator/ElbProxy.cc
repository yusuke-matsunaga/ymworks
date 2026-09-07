
/// @file ElbProxy.cc
/// @brief ElbProxy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ElbProxy.h"
#include "ElbEnv.h"
#include "ElbError.h"
#include "ElbParamCon.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "ExprEval.h"
#include "AttrGen.h"

#include "elaborator/ElbModule.h"
#include "elaborator/ElbExpr.h"
#include "ym/vl/AstExpr.h"


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

// @brief パラメータポートの割り当て数が多すぎる．
void
ElbProxy::error_too_many_params(
  const char* file,
  int line,
  const std::vector<ElbParamCon>& param_con_list
)
{
  auto last = param_con_list.back();
  throw ElbError(file, line,
		 last.mAstCon.file_region(),
		 "ELAB_TOO_MANY_PARAMS",
		 "Too many parameters");
}

// @brief パラメータポートに現れるパラメータが存在しない．
void
ElbProxy::error_param_not_found(
  const char* file,
  int line,
  const AstConnection& ast_con
)
{
  std::ostringstream buf;
  buf << "\""
      << ast_con.name()
      << "\": No such parameter.";
  throw ElbError(file, line,
		 ast_con.file_region(),
		 "ELAB_PARAM_NOT_FOUND",
		 buf.str());
}

// @brief ポートに配列が使われている．
void
ElbProxy::error_array_in_port_connection(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  throw ElbError(file, line,
		 file_region,
		 "ELAB_ARRAY_IN_PORT_CONNECTION",
		 "Array shall not be connected to a module port.");
}

// @brief ポートに使われている要素が宣言要素でなかった．
void
ElbProxy::error_illegal_port(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  throw ElbError(file, line,
		 file_region,
		 "ELAB_ILLEGA_PORT",
		 "Illegal type for port connection.");
}

// @brief 重複した名前を持つ．
void
ElbProxy::error_dup_name(
  const char* file,
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
  throw ElbError(file, line,
		 loc,
		 "ELAB_DUP_NAME",
		 buf.str());
}

// @brief 対象の要素が見つからない．
void
ElbProxy::error_not_found(
  const char* file,
  int line,
  const FileRegion& file_region,
  const char* name
)
{
  std::ostringstream buf;
  buf << "\"" << name
      << "\": Not found";
  throw ElbError(file, line,
		 file_region,
		 "ELAB_NOT_FOUND",
		 buf.str());
}

// @brief IO 宣言に aux_type と宣言が重複している．
void
ElbProxy::error_duplicate_type(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  const ObjHandle* handle
)
{
}

// @brief IO 宣言に配列型の要素が現れている．
void
ElbProxy::error_array_in_io_decl(
  const char* file,
  int line,
  const AstIOItem& ast_item
)
{
}

// @brief IO 宣言に不適切な型の要素が現れている．
void
ElbProxy::error_illegal_io_decl(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  const ObjHandle* handle,
  bool is_module
)
{
}

// @brief IO宣言と要素宣言の範囲が異なる．
void
ElbProxy::error_conflict_io_range(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  ElbDecl* decl
)
{
}

// @brief 暗黙のネット宣言は許されていない．
void
ElbProxy::error_no_impnet(
  const char* file,
  int line,
  const AstIOItem& ast_item
)
{
}

// @brief 暗黙のネット宣言は初期値を持てない．
void
ElbProxy::error_impnet_with_init(
  const char* file,
  int line,
  const AstIOItem& ast_item
)
{
}

// @brief 添字が範囲外
void
ElbProxy::warning_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  put_warning(file, line,
	      file_region,
	      "ELAB_INDEX_OUT_OF_RANGE",
	      "Index is out of range.");
}

// @brief 左の範囲が範囲外
void
ElbProxy::warning_left_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  put_warning(file, line,
	      file_region,
	      "ELAB_LEFT_INDEX_OUT_OF_RANGE",
	      "Left index is out of range.");
}

// @brief 右の範囲が範囲外
void
ElbProxy::warning_right_index_out_of_range(
  const char* file,
  int line,
  const FileRegion& file_region
)
{
  put_warning(file, line,
	      file_region,
	      "ELAB_RIGHT_INDEX_OUT_OF_RANGE",
	      "Right index is out of range.");
}

// @brief モジュール配列のインスタンス生成
void
ElbProxy::info_module_array(
  const char* file,
  int line,
  const AstItem& ast_head,
  const AstInst& ast_inst,
  const RangeVal& range
)
{
  std::ostringstream buf;
  buf << "Instantiating module array \"" << ast_inst.name() << "\" of \""
      << ast_head.name() << "\" [" << range.left << " : " << range.right << "].";
  put_info(file, line,
	   ast_inst.file_region(),
	   "ELAB_MODULE_ARRAY_INSTANTIATE",
	   buf.str());
}

// @brief モジュールのインスタンス生成
void
ElbProxy::info_module(
  const char* file,
  int line,
  ElbModule* module
)
{
  std::ostringstream buf;
  buf << "\"" << module->full_name() << "\" has been created.";
  put_info(file, line,
	   module->file_region(),
	   "ELAB_MODULE_INSTANTIATE",
	   buf.str());
}

// @brief IO宣言のインスタンス生成
void
ElbProxy::info_iodecl(
  const char* file,
  int line,
  const AstIOItem& ast_item,
  const VlScope* scope
)
{
}

// @brief パラメータのインスタンス生成
void
ElbProxy::info_param(
  const char* file,
  int line,
  const VlDecl* decl
)
{
}

// @brief ネット配列のインスタンス生成
void
ElbProxy::info_net_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
}

// @brief ネットのインスタンス生成
void
ElbProxy::info_net(
  const char* file,
  int line,
  const VlDecl* decl
)
{
}

// @brief Reg配列のインスタンス生成
void
ElbProxy::info_reg_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
}

// @brief Regのインスタンス生成
void
ElbProxy::info_reg(
  const char* file,
  int line,
  const VlDecl* decl
)
{
}

// @brief Var配列のインスタンス生成
void
ElbProxy::info_var_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
}

// @brief Varのインスタンス生成
void
ElbProxy::info_var(
  const char* file,
  int line,
  const VlDecl* decl
)
{
}

// @brief イベント配列のインスタンス生成
void
ElbProxy::info_event_array(
  const char* file,
  int line,
  const VlDeclArray* decl_array
)
{
}

// @brief イベントのインスタンス生成
void
ElbProxy::info_event(
  const char* file,
  int line,
  const VlDecl* decl
)
{
}

// @brief genvarのインスタンス生成
void
ElbProxy::info_genvar(
  const char* file,
  int line,
  ElbGenvar* genvar
)
{
}

// @brief エラーメッセージを出力する．
void
ElbProxy::put_error(
  const ElbError& error
)
{
  mElaborator.put_error(error);
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
  mElaborator.put_warning(file, line, loc, label, msg);
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
  mElaborator.put_info(file, line, loc, label, msg);
}

END_NAMESPACE_YM_VERILOG
