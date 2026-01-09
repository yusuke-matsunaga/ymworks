
/// @file AigMgr_expr.cc
/// @brief AigMgr::from_expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "ym/Expr.h"
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgr
//////////////////////////////////////////////////////////////////////

// @brief Expr から変換する．
AigLiteral
AigMgr::from_expr(
  const Expr& expr
)
{
  auto nmax = expr.input_size();
  auto input_list = _input_list(nmax);
  return from_expr(expr, input_list);
}

// @brief Expr から変換する．
AigLiteral
AigMgr::from_expr(
  const Expr& expr,
  const std::vector<AigLiteral>& input_list
)
{
  if ( expr.is_zero() ) {
    return AigLiteral::zero();
  }
  if ( expr.is_one() ) {
    return AigLiteral::one();
  }
  if ( expr.is_literal() ) {
    auto lit = expr.literal();
    auto varid = lit.varid();
    if ( input_list.size() <= varid ) {
      throw std::invalid_argument{"input_list is too small"};
    }
    auto e = input_list[varid];
    return e * lit.is_negative();
  }
  std::vector<AigLiteral> edge_list;
  edge_list.reserve(expr.operand_num());
  for ( auto operand: expr.operand_list() ) {
    auto edge = from_expr(operand, input_list);
    edge_list.push_back(edge);
  }
  if ( expr.is_and() ) {
    return new_and(edge_list);
  }
  if ( expr.is_or() ) {
    return new_or(edge_list);
  }
  if ( expr.is_xor() ) {
    return new_xor(edge_list);
  }
  ASSERT_NOT_REACHED;
  return AigLiteral::zero();
}

// @brief 複数の Expr から変換する．
std::vector<AigLiteral>
AigMgr::from_expr_list(
  const std::vector<Expr>& expr_list
)
{
  SizeType nmax = 0;
  for ( auto& expr: expr_list ) {
    nmax = std::max(nmax, expr.input_size());
  }
  auto input_list = _input_list(nmax);
  return from_expr_list(expr_list, input_list);
}

// @brief 複数の Expr から変換する．
std::vector<AigLiteral>
AigMgr::from_expr_list(
  const std::vector<Expr>& expr_list,
  const std::vector<AigLiteral>& input_list
)
{
  std::vector<AigLiteral> ans_list;
  ans_list.reserve(expr_list.size());
  for ( auto& expr: expr_list ) {
    auto lit = from_expr(expr, input_list);
    ans_list.push_back(lit);
  }
  return ans_list;
}

END_NAMESPACE_YM_AIG
