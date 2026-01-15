
/// @file FuncMgr.cc
/// @brief FuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "FuncMgr.h"


BEGIN_NAMESPACE_YM_BN

// @brief コピーコンストラクタもどき
FuncMgr::FuncMgr(
  const FuncMgr& src
)
{
  for ( auto& src_func: src.mFuncArray ) {
    SizeType id = mFuncArray.size();
    mFuncArray.push_back(src_func->copy(mBddMgr));
    auto func = mFuncArray.back().get();
    mFuncMap.emplace(func, id);
  }
}

// @brief クリアする．
void
FuncMgr::clear()
{
  mFuncArray.clear();
  mFuncMap.clear();
}

// @brief プリミティブ型を登録する．
const FuncImpl*
FuncMgr::reg_primitive(
  const ModelImpl* model,
  SizeType input_num,
  PrimType primitive_type
)
{
  return reg_func(
    [&](SizeType id)
    {
      return FuncImpl::new_primitive(model, id, input_num, primitive_type);
    });
}

// @brief カバー型を登録する．
const FuncImpl*
FuncMgr::reg_cover(
  const ModelImpl* model,
  const SopCover& input_cover,
  bool output_inv
)
{
  return reg_func(
    [&](SizeType id)
    {
      return FuncImpl::new_cover(model, id, input_cover, output_inv);
    });
}

// @brief 論理式型を登録する．
const FuncImpl*
FuncMgr::reg_expr(
  const ModelImpl* model,
  const Expr& expr
)
{
  return reg_func(
    [&](SizeType id)
    {
      return FuncImpl::new_expr(model, id, expr);
    });
}

// @brief 真理値表型を登録する．
const FuncImpl*
FuncMgr::reg_tvfunc(
  const ModelImpl* model,
  const TvFunc& tvfunc
)
{
  return reg_func(
    [&](SizeType id)
    {
      return FuncImpl::new_tvfunc(model, id, tvfunc);
    });
}

// @brief BDD型を登録する．
const FuncImpl*
FuncMgr::reg_bdd(
  const ModelImpl* model,
  const Bdd& bdd
)
{
  return reg_func(
    [&](SizeType id)
    {
      return FuncImpl::new_bdd(model, id, mBddMgr.copy(bdd));
    });
}

// @brief 関数情報を登録する．
const FuncImpl*
FuncMgr::reg_func(
  std::function<FuncImpl*(SizeType id)> new_func
)
{
  // 関数情報を生成する．
  auto id = mFuncArray.size();
  auto func = new_func(id);
  // 重複のチェック
  auto p = mFuncMap.find(func);
  if ( p != mFuncMap.end() ) {
    // 既に同じ関数が登録されていた．
    delete func;
    id = p->second;
    return mFuncArray[id].get();
  }
  // 新規に登録する．
  mFuncArray.push_back(std::unique_ptr<FuncImpl>{func});
  mFuncMap.emplace(func, id);
  return func;
}

END_NAMESPACE_YM_BN
