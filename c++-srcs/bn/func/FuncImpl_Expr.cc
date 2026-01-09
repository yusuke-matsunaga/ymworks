
/// @file FuncImpl_Expr.cc
/// @brief FuncImpl_Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "FuncImpl_Expr.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief 論理式型のインスタンスを作る．
FuncImpl*
FuncImpl::new_expr(
  const Expr& expr
)
{
  return new FuncImpl_Expr(expr);
}


//////////////////////////////////////////////////////////////////////
// クラス FuncImpl_Expr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncImpl_Expr::FuncImpl_Expr(
  const Expr& expr
) : mExpr{expr}
{
}

// @brief 関数の種類を返す．
BnFunc::Type
FuncImpl_Expr::type() const
{
  return BnFunc::EXPR;
}

// @brief 論理式型の時 true を返す．
bool
FuncImpl_Expr::is_expr() const
{
  return true;
}

// @brief 入力数を返す．
SizeType
FuncImpl_Expr::input_num() const
{
  return mExpr.input_size();
}

// @brief 論理式を返す．
Expr
FuncImpl_Expr::expr() const
{
  return mExpr;
}

// @brief コピーを作る．
std::unique_ptr<FuncImpl>
FuncImpl_Expr::copy(
  BddMgr& bdd_mgr
) const
{
  return std::unique_ptr<FuncImpl>{new FuncImpl_Expr{*this}};
}

// @brief ハッシュ用のユニークな文字列を返す．
std::string
FuncImpl_Expr::signature() const
{
  std::ostringstream buf;
  buf << "e" << mExpr.rep_string();
  return buf.str();
}

// @brief 内容を出力する．
void
FuncImpl_Expr::print(
  std::ostream& s
) const
{
  s << "Expr" << std::endl
    << mExpr.rep_string() << std::endl;
}

END_NAMESPACE_YM_BN
