
/// @file FuncImpl.cc
/// @brief FuncImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncImpl.h"
#include "ym/Expr.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_YM_BN

//////////////////////////////////////////////////////////////////////
// クラス FuncImpl
//////////////////////////////////////////////////////////////////////

// @brief プリミティブ型の時 true を返す．
bool
FuncImpl::is_primitive() const
{
  return false;
}

// @brief カバー型の時 true を返す．
bool
FuncImpl::is_cover() const
{
  return false;
}

// @brief 論理式型の時 true を返す．
bool
FuncImpl::is_expr() const
{
  return false;
}

// @brief 真理値表型の時 true を返す．
bool
FuncImpl::is_tvfunc() const
{
  return false;
}

// @brief BDD型の時 true を返す．
bool
FuncImpl::is_bdd() const
{
  return false;
}

// @brief プリミティブ型を返す．
PrimType
FuncImpl::primitive_type() const
{
  throw std::invalid_argument{"not a Primitive type."};
}

// @brief 入力カバーを返す．
const SopCover&
FuncImpl::input_cover() const
{
  throw std::invalid_argument{"not a Cover type."};
}

// @brief 出力の反転属性を返す．
bool
FuncImpl::output_inv() const
{
  throw std::invalid_argument{"not a Cover type."};
}

// @brief 論理式を返す．
Expr
FuncImpl::expr() const
{
  throw std::invalid_argument{"not an Expr type."};
}

// @brief 真理値表を返す．
const TvFunc&
FuncImpl::tvfunc() const
{
  throw std::invalid_argument{"not a TvFunc type."};
}

// @brief BDDを返す．
Bdd
FuncImpl::bdd() const
{
  throw std::invalid_argument{"not a BDD type."};
}

END_NAMESPACE_YM_BN
