
/// @file ElbEnv.cc
/// @brief ElbEnv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ElbEnv.h"
#include "ym/vl/VlTaskFunc.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
ElbEnv::ElbEnv(
) : mFlags{0U},
    mCf{nullptr}
{
}

// @brief コピーコンストラクタ
ElbEnv::ElbEnv(
  const ElbEnv& src
) : mFlags{src.mFlags},
    mCf{src.mCf}
{
}

// @brief 代入演算子
const ElbEnv&
ElbEnv::operator=(
  const ElbEnv& src
)
{
  mFlags = src.mFlags;
  mCf = src.mCf;
  return *this;
}

// @brief 定数式を要求する．
void
ElbEnv::set_constant()
{
  mFlags[CONSTANT] = true;
}

// @brief function を設定する．
void
ElbEnv::set_function()
{
  mFlags[FUNCTION] = true;
}

// @brief 親の constant function を設定する．
void
ElbEnv::set_constant_function(
  const VlScope* function
)
{
  mFlags[CONSTFUNC] = true;
  mFlags[FUNCTION] = true;
  mCf = function;
}

// @brief system task/system function の引数の印をつける．
void
ElbEnv::set_system_tf_arg()
{
  mFlags[ARG] = true;
}

// @brief イベント式の印をつける．
void
ElbEnv::set_event_expr()
{
  mFlags[EVENT] = true;
}

// @brief net 型の左辺式の印をつける．
void
ElbEnv::set_net_lhs()
{
  mFlags[NET] = true;
  mFlags[LHS] = true;
}

// @brief reg/var 型の左辺式の印をつける．
void
ElbEnv::set_var_lhs()
{
  mFlags[VAR] = true;
  mFlags[LHS] = true;
}

// @brief pca 代入文の左辺式の印をつける．
void
ElbEnv::set_pca_lhs()
{
  mFlags[PCA] = true;
  mFlags[LHS] = true;
}

// @brief force 代入文の左辺式の印をつける．
void
ElbEnv::set_force_lhs()
{
  mFlags[FORCE] = true;
  mFlags[LHS] = true;
}

// @brief ビットベクタ型の印を付ける．
void
ElbEnv::set_bitvect()
{
  mFlags[BITVECT] = true;
}

// @brief 定数式が要求されている時に true を返す．
bool
ElbEnv::is_constant() const
{
  return mFlags[CONSTANT];
}

// @brief constant function 内の生成の時に親の function を返す．
const VlScope*
ElbEnv::constant_function() const
{
  return mCf;
}

// @brief function 内の生成時に true を返す．
bool
ElbEnv::inside_function() const
{
  return mFlags[FUNCTION];
}

// @brief constant function 内の生成の時に true を返す．
bool
ElbEnv::inside_constant_function() const
{
  return mFlags[CONSTFUNC];
}

// @brief system task/system function の引数の時に true を返す．
bool
ElbEnv::is_system_tf_arg() const
{
  return mFlags[ARG];
}

// @brief イベント式の時に true を返す．
bool
ElbEnv::is_event_expr() const
{
  return mFlags[EVENT];
}

// @brief 左辺式の時に true を返す．
bool
ElbEnv::is_lhs() const
{
  return mFlags[LHS];
}

// @brief net 型の左辺式の時に true を返す．
bool
ElbEnv::is_net_lhs() const
{
  return mFlags[NET];
}

// @brief reg/var 型の左辺式の時に true を返す．
bool
ElbEnv::is_var_lhs() const
{
  return mFlags[VAR];
}

// @brief pca 代入文の左辺式の時に true を返す．
bool
ElbEnv::is_pca_lhs() const
{
  return mFlags[PCA];
}

// @brief force 代入文の左辺式の時に true を返す．
bool
ElbEnv::is_force_lhs() const
{
  return mFlags[FORCE];
}

// @brief ビットベクタ型の時に true を返す．
bool
ElbEnv::is_bitvect() const
{
  return mFlags[BITVECT];
}

// @brief 階層付き識別子に関するチェック
void
ElbEnv::check_hname(
  const AstExpr& ast_expr,
  LogMgr& log_mgr
) const
{
  if ( is_constant() ) {
    // 定数式
    log_mgr.error_hname_in_ce(__FILE__, __LINE__, ast_expr);
  }
  if ( inside_constant_function() ) {
    // 定数関数内の式
    log_mgr.error_hname_in_cf(__FILE__, __LINE__, ast_expr);
  }
  if ( is_net_lhs() ) {
    // ネット型の左辺式
    //log_mgr.error_hname_in_net_assign(__FILE__, __LINE__, ast_expr);
#warning "TODO: エラー処理"
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ElbConstantEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbConstantEnv::ElbConstantEnv()
{
  set_constant();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbConstantFunctionEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbConstantFunctionEnv::ElbConstantFunctionEnv(
  const VlScope* func
)
{
  set_constant_function(func);
}


//////////////////////////////////////////////////////////////////////
// クラス ElbTfEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbTfEnv::ElbTfEnv(
  const VlTaskFunc* taskfunc
)
{
  if ( taskfunc->type() == VpiObjType::Function ) {
    set_function();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス ElbSystemTfArgEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbSystemTfArgEnv::ElbSystemTfArgEnv(
  const ElbEnv& env
) : ElbEnv{env}
{
  set_system_tf_arg();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbEventExprEnv ElbEnv.h "ElbEnv.h"
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbEventExprEnv::ElbEventExprEnv(
  const ElbEnv& env
) : ElbEnv{env}
{
  set_event_expr();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbNetLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbNetLhsEnv::ElbNetLhsEnv()
{
  set_net_lhs();
  set_bitvect();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbVarLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbVarLhsEnv::ElbVarLhsEnv(
  const ElbEnv& env
) : ElbEnv{env}
{
  set_var_lhs();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbPcaLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbPcaLhsEnv::ElbPcaLhsEnv()
{
  set_pca_lhs();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbForceLhsEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbForceLhsEnv::ElbForceLhsEnv()
{
  set_force_lhs();
}


//////////////////////////////////////////////////////////////////////
// クラス ElbBitvectEnv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbBitvectEnv::ElbBitvectEnv()
{
  set_bitvect();
}

END_NAMESPACE_YM_VERILOG
