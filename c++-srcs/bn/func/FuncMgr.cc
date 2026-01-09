
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
SizeType
FuncMgr::reg_primitive(
  SizeType input_num,
  PrimType primitive_type
)
{
  auto func = FuncImpl::new_primitive(input_num, primitive_type);
  return reg_func(func);
}

// @brief カバー型を登録する．
SizeType
FuncMgr::reg_cover(
  const SopCover& input_cover,
  bool output_inv
)
{
  auto func = FuncImpl::new_cover(input_cover, output_inv);
  return reg_func(func);
}

// @brief 論理式型を登録する．
SizeType
FuncMgr::reg_expr(
  const Expr& expr
)
{
  auto func = FuncImpl::new_expr(expr);
  return reg_func(func);
}

// @brief 真理値表型を登録する．
SizeType
FuncMgr::reg_tvfunc(
  const TvFunc& tvfunc
)
{
  auto func = FuncImpl::new_tvfunc(tvfunc);
  return reg_func(func);
}

// @brief BDD型を登録する．
SizeType
FuncMgr::reg_bdd(
  const Bdd& bdd
)
{
  auto func = FuncImpl::new_bdd(mBddMgr.copy(bdd));
  return reg_func(func);
}

// @brief 関数情報を登録する．
SizeType
FuncMgr::reg_func(
  FuncImpl* func
)
{
  auto p = mFuncMap.find(func);
  if ( p != mFuncMap.end() ) {
    // 既に同じ関数が登録されていた．
    delete func;
    return p->second;
  }
  // 新規に登録する．
  auto id = mFuncArray.size();
  mFuncArray.push_back(std::unique_ptr<FuncImpl>{func});
  mFuncMap.emplace(func, id);
  return id;
}

END_NAMESPACE_YM_BN
