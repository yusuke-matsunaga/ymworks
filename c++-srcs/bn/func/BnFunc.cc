
/// @file BnFunc.cc
/// @brief BnFunc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnFunc.h"
#include "ym/BnModel.h"
#include "ym/Expr.h"
#include "ym/SopCover.h"
#include "ym/Bdd.h"
#include "FuncImpl.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

// @brief 内容を指定したコンストラクタ
BnFunc::BnFunc(
  const std::shared_ptr<ModelImpl>& model,
  SizeType id
) : BnBase(model),
    mId{id}
{
  if ( is_invalid() ) {
    mId = BAD_ID;
  }
}

// @brief デストラクタ
BnFunc::~BnFunc()
{
}

// @brief 種類を返す．
BnFunc::Type
BnFunc::type() const
{
  return _func_impl().type();
}

// @brief 種類を表す文字列を返す．
std::string
BnFunc::type_str() const
{
  switch ( type() ) {
  case NONE:      return "NONE";
  case PRIMITIVE: return "PRIMITIVE";
  case COVER:     return "COVER";
  case EXPR:      return "EXPR";
  case TVFUNC:    return "TVFUNC";
  case BDD:       return "BDD";
  }
}

// @brief プリミティブ型の時 true を返す．
bool
BnFunc::is_primitive() const
{
  return _func_impl().is_primitive();
}

// @brief カバー型の時 true を返す．
bool
BnFunc::is_cover() const
{
  return _func_impl().is_cover();
}

// @brief 論理式型の時 true を返す．
bool
BnFunc::is_expr() const
{
  return _func_impl().is_expr();
}

// @brief 真理値表型の時 true を返す．
bool
BnFunc::is_tvfunc() const
{
  return _func_impl().is_tvfunc();
}

// @brief BDD型の時 true を返す．
bool
BnFunc::is_bdd() const
{
  return _func_impl().is_bdd();
}

// @brief 入力数を返す．
SizeType
BnFunc::input_num() const
{
  return _func_impl().input_num();
}

// @brief プリミティブ型を返す．
PrimType
BnFunc::primitive_type() const
{
  return _func_impl().primitive_type();
}

// @brief 入力カバーを返す．
const SopCover&
BnFunc::input_cover() const
{
  return _func_impl().input_cover();
}

// @brief 出力の反転属性を返す．
bool
BnFunc::output_inv() const
{
  return _func_impl().output_inv();
}

// @brief 論理式を返す．
Expr
BnFunc::expr() const
{
  return _func_impl().expr();
}

// @brief 真理値表を返す．
const TvFunc&
BnFunc::tvfunc() const
{
  return _func_impl().tvfunc();
}

// @brief BDDを返す．
Bdd
BnFunc::bdd() const
{
  return _func_impl().bdd();
}

// @brief 内容を出力する．
void
BnFunc::print(
  std::ostream& s
) const
{
  _func_impl().print(s);
}

// @brief 実装を取り出す．
const FuncImpl&
BnFunc::_func_impl() const
{
  if ( !is_valid() ) {
    throw std::invalid_argument{"BnFunc: invalid data"};
  }
  return _model_impl().func_impl(mId);
}

END_NAMESPACE_YM_BN
