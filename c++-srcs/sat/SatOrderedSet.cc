
/// @file SatOrderedSet.cc
/// @brief SatOrderedSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatOrderedSet.h"
#include "ym/SatSolver.h"
#include "ym/SatModel.h"


BEGIN_NAMESPACE_YM_SAT

// @brief SATの解から値を得る．
int
SatOrderedSet::val(
  const SatModel& model
) const
{
  auto n = mVarArray.size();
  if ( n > 1 ) {
    for ( SizeType i = 0; i < n; ++ i ) {
      auto lit = mVarArray[i];
      if ( model[lit] == SatBool3::True ) {
	return i + mMin;
      }
    }
    ASSERT_NOT_REACHED;
    return -1;
  }
  return mMin;
}


//////////////////////////////////////////////////////////////////////
// クラス SatSolver
//////////////////////////////////////////////////////////////////////

// @brief 順序符号化した変数を返す．
SatOrderedSet
SatSolver::new_ordered_set(
  int min_val,
  int max_val
)
{
  if ( min_val > max_val ) {
    throw std::invalid_argument{"min_val > max_val"};
  }

  SizeType n1 = max_val - min_val;
  SizeType n = n1 + 1;

  SatOrderedSet var;
  var.mMin = min_val;
  var.mMax = max_val;

  // 2種類の変数ベクタを作る．
  // mPriVarArray は順序符号化
  if ( n1 > 0 ) {
    var.mPriVarArray.resize(n1);
    for ( SizeType i = 0; i < n1; ++ i ) {
      var.mPriVarArray[i] = new_variable(true);
      // 順序制約を作る．
      if ( i > 0 ) {
	auto lit1 = var.mPriVarArray[i - 1];
	auto lit2 = var.mPriVarArray[i + 0];
	add_clause( lit1, ~lit2);
      }
    }
  }

  // mVarArray は one-hot 符号化を行う．
  var.mVarArray.resize(n);

  if ( n1 > 0 ) {
    var.mVarArray[0] = ~var.mPriVarArray[0];
    for ( int i = 1; i < n1; ++ i ) {
      var.mVarArray[i] = new_variable(true);
    }
    var.mVarArray[n1] = var.mPriVarArray[n1 - 1];
  }
  else {
    auto lit = new_variable(true);
    var.mVarArray[0] = lit;
    add_clause(lit);
  }

  // 順序制約から one-hot へ符号化を行う．
  for ( SizeType i = 1; i < n1; ++ i ) {
    auto lit1 = var.mPriVarArray[i - 1];
    auto lit2 = var.mPriVarArray[i + 0];
    auto lit = var.mVarArray[i];
    add_clause(~lit,  lit1       );
    add_clause(~lit,        ~lit2);
    add_clause( lit, ~lit1,  lit2);
  }

  return var;
}

// @brief a < b という制約を作る．
void
SatSolver::add_lt(
  const SatOrderedSet& a,
  int b
)
{
  if ( b > a.max() ) {
    // 常に成り立つ．
    return;
  }

  auto lit = a.pri_var(b);
  add_clause(~lit);
}

// @brief a <= b という制約を作る．
void
SatSolver::add_le(
  const SatOrderedSet& a,
  int b
)
{
  add_lt(a, b + 1);
}

// @brief a > b という制約を作る．
void
SatSolver::add_gt(
  const SatOrderedSet& a,
  int b
)
{
  add_ge(a, b + 1);
}

// @brief a >= b という制約を作る．
void
SatSolver::add_ge(
  const SatOrderedSet& a,
  int b
)
{
  if ( b <= a.min() ) {
    // 常に成り立つ．
    return;
  }

  auto lit = a.pri_var(b);
  add_clause(lit);
}

// @brief a == b という条件を追加する．
void
SatSolver::add_eq(
  const SatOrderedSet& a,
  const SatOrderedSet& b
)
{
  if ( a.max() < b.min() || a.min() > b.max() ) {
    // 絶対に成り立たない．
    add_clause(std::vector<SatLiteral>{});
    return;
  }
  // a, b の共通区間
  auto lval = std::max(a.min(), b.min());
  auto uval = std::min(a.max(), b.max());
  for ( SizeType v = a.min() + 1; v <= lval; ++ v ) {
    auto a_var = a.pri_var(v);
    add_clause(a_var);
  }
  for ( SizeType v = b.min() + 1; v <= lval; ++ v ) {
    auto b_var = b.pri_var(v);
    add_clause(b_var);
  }
  for ( SizeType v = lval + 1; v <= uval; ++ v ) {
    auto a_var = a.pri_var(v);
    auto b_var = b.pri_var(v);
    add_clause( a_var, ~b_var);
    add_clause(~a_var,  b_var);
  }
  for ( SizeType v = uval + 1; v <= a.max(); ++ v ) {
    auto a_var = a.pri_var(v);
    add_clause(~a_var);
  }
  for ( SizeType v = uval + 1; v <= b.max(); ++ v ) {
    auto b_var = b.pri_var(v);
    add_clause(~b_var);
  }
}

// @brief a != b という条件を追加する．
void
SatSolver::add_ne(
  const SatOrderedSet& a,
  const SatOrderedSet& b
)
{
  if ( a.max() < b.min() || a.min() > b.max() ) {
    // 常に成り立つ．
    return;
  }
  // a, b の共通区間
  auto lval = std::max(a.min(), b.min());
  auto uval = std::min(a.max(), b.max());
  std::vector<SatLiteral> tmp_lits;
  tmp_lits.reserve(uval - lval + 3);
  if ( a.min() < lval ) {
    auto a_var = a.pri_var(lval);
    tmp_lits.push_back(~a_var);
  }
  if ( b.min() < lval ) {
    auto b_var = b.pri_var(lval);
    tmp_lits.push_back(~b_var);
  }
  for ( SizeType v = lval + 1; v <= uval; ++ v ) {
    auto a_var = a.pri_var(v);
    auto b_var = b.pri_var(v);
    auto d_var = new_variable();
    add_xorgate(d_var, a_var, b_var);
    tmp_lits.push_back(d_var);
  }
  if ( a.max() > uval ) {
    auto a_var = a.pri_var(uval + 1);
    tmp_lits.push_back(a_var);
  }
  if ( b.max() > uval ) {
    auto b_var = b.pri_var(uval + 1);
    tmp_lits.push_back(b_var);
  }
  add_clause(tmp_lits);
}

// @brief a < b という条件を追加する．
void
SatSolver::add_lt(
  const SatOrderedSet& a,
  const SatOrderedSet& b
)
{
  if ( a.max() < b.min() ) {
    // 常に成り立つ．
    return;
  }
  if ( a.min() >= b.max() ) {
    // 絶対に成り立たない．
    add_clause(std::vector<SatLiteral>{});
    return;
  }
  // a, b の共通区間
  auto lval = std::max(a.min(), b.min());
  auto uval = std::min(a.max(), b.max());
  std::vector<SatLiteral> tmp_lits;
  auto n = uval - lval;
  tmp_lits.reserve(n + 2);
  for ( int v = b.min() + 1; v <= lval; ++ v ) {
    auto b_var = b.pri_var(v);
    add_clause(b_var);
  }
  if ( a.min() < lval ) {
    auto a_var = a.pri_var(lval);
    tmp_lits.push_back(~a_var);
  }
  for ( int v = lval + 1; v <= uval; ++ v ) {
    auto a_var = a.pri_var(v);
    auto b_var = b.pri_var(v);
    add_clause(~a_var, b_var);
    auto d_var = new_variable();
    add_clause(~a_var, ~d_var);
    add_clause( b_var, ~d_var);
    add_clause(a_var, ~b_var, d_var);
    tmp_lits.push_back(d_var);
  }
  for ( int v = uval + 1; v <= a.max(); ++ v ) {
    auto a_var = a.pri_var(v);
    add_clause(~a_var);
  }
  if ( b.max() > uval ) {
    auto b_var = b.pri_var(uval + 1);
    tmp_lits.push_back(b_var);
  }
  add_clause(tmp_lits);
}

// @brief a <= b という条件を追加する．
void
SatSolver::add_le(
  const SatOrderedSet& a,
  const SatOrderedSet& b
)
{
  if ( a.max() <= b.min() ) {
    // 常に成り立つ．
    return;
  }
  if ( a.min() > b.max() ) {
    // 絶対に成り立たない．
    add_clause(std::vector<SatLiteral>{});
    return;
  }
  // a, b の共通区間
  auto lval = std::max(a.min(), b.min());
  auto uval = std::min(a.max(), b.max());
  for ( int v = b.min() + 1; v <= lval; ++ v ) {
    auto b_var = b.pri_var(v);
    add_clause(b_var);
  }
  for ( int v = lval + 1; v <= uval; ++ v ) {
    auto a_var = a.pri_var(v);
    auto b_var = b.pri_var(v);
    add_clause(~a_var, b_var);
  }
  for ( int v = uval + 1; v <= a.max(); ++ v ) {
    auto a_var = a.pri_var(v);
    add_clause(~a_var);
  }
}

// @brief この変数の値が uval 以下か lval 以上という制約を作る．
void
SatSolver::add_dropoff(
  const SatOrderedSet& a,
  int uval,
  int lval
)
{
  uval = std::min(uval, a.max());
  lval = std::max(lval, a.min());

  if ( uval >= lval ) {
    // 常に成り立つ．
    return;
  }
  // この時点で uval == mMax, lval == mMin はない．

  auto lit1 = a.pri_var(uval + 1);
  auto lit2 = a.pri_var(lval);
  add_clause(~lit1, lit2);
}

// @brief a == b という条件を追加する．
void
SatSolver::add_eq(
  const std::vector<SatOrderedSet>& a,
  const std::vector<SatOrderedSet>& b
)
{
  if ( a.size() != b.size() ) {
    throw std::invalid_argument{"a.size() != b.size()"};
  }
  auto n = a.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    add_eq(a[i], b[i]);
  }
}

// @brief a != b という条件を追加する．
void
SatSolver::add_ne(
  const std::vector<SatOrderedSet>& a,
  const std::vector<SatOrderedSet>& b
)
{
  if ( a.size() != b.size() ) {
    throw std::invalid_argument{"a.size() != b.size()"};
  }
  auto n = a.size();
  std::vector<SatLiteral> tmp_lits(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    auto d_var = new_variable(true);
    tmp_lits[i] = d_var;
    {
      auto _ = condblock(d_var);
      add_ne(a[i], b[i]);
    }
    {
      auto _ = condblock(~d_var);
      add_eq(a[i], b[i]);
    }
  }
  add_clause(tmp_lits);
}

// @brief a < b という条件を追加する．
void
SatSolver::add_lt(
  const std::vector<SatOrderedSet>& a,
  const std::vector<SatOrderedSet>& b
)
{
  if ( a.size() != b.size() ) {
    throw std::invalid_argument{"a.size() != b.size()"};
  }
  auto n = a.size();
  if ( n == 0 ) {
    return;
  }
  if ( n == 1 ) {
    add_lt(a[0], b[0]);
    return;
  }
  auto c1_var = new_variable(true);
  {
    auto _ = condblock(c1_var);
    add_lt(a[0], b[0]);
  }
  auto c2_var = new_variable(true);
  auto d_var = _add_lt(a, b, 1);
  {
    auto _ = condblock(c2_var);
    add_eq(a[0], b[0]);
    add_clause(d_var);
  }
  add_clause(c1_var, c2_var);
}

// @brief add_lt() の下請け関数
SatLiteral
SatSolver::_add_lt(
  const std::vector<SatOrderedSet>& a,
  const std::vector<SatOrderedSet>& b,
  SizeType pos
)
{
  auto c_var = new_variable(true);
  if ( pos == a.size() - 1 ) {
    // 最後の要素
    auto _ = condblock(c_var);
    add_lt(a[pos], b[pos]);
  }
  else {
    auto c1_var = new_variable(true);
    {
      auto _ = condblock(c1_var);
      add_lt(a[pos], b[pos]);
    }
    auto c2_var = new_variable(true);
    auto d_var = _add_lt(a, b, pos + 1);
    {
      auto _ = condblock(c2_var);
      add_eq(a[pos], b[pos]);
      add_clause(d_var);
    }
    add_clause(~c_var,  c1_var,  c2_var);
  }
  return c_var;
}

// @brief a <= b という条件を追加する．
void
SatSolver::add_le(
  const std::vector<SatOrderedSet>& a,
  const std::vector<SatOrderedSet>& b
)
{
  if ( a.size() != b.size() ) {
    throw std::invalid_argument{"a.size() != b.size()"};
  }
  auto n = a.size();
  if ( n == 0 ) {
    return;
  }
  if ( n == 1 ) {
    add_le(a[0], b[0]);
    return;
  }
  auto c1_var = new_variable(true);
  {
    auto _ = condblock(c1_var);
    add_lt(a[0], b[0]);
  }
  auto c2_var = new_variable(true);
  auto d_var = _add_le(a, b, 1);
  {
    auto _ = condblock(c2_var);
    add_eq(a[0], b[0]);
    add_clause(d_var);
  }
  add_clause(c1_var, c2_var);
}

// @brief add_le() の下請け関数
SatLiteral
SatSolver::_add_le(
  const std::vector<SatOrderedSet>& a,
  const std::vector<SatOrderedSet>& b,
  SizeType pos
)
{
  auto c_var = new_variable(true);
  if ( pos == a.size() - 1 ) {
    // 最後の要素
    auto _ = condblock(c_var);
    add_le(a[pos], b[pos]);
  }
  else {
    auto c1_var = new_variable(true);
    {
      auto _ = condblock(c1_var);
      add_lt(a[pos], b[pos]);
    }
    auto c2_var = new_variable(true);
    auto d_var = _add_le(a, b, pos + 1);
    {
      auto _ = condblock(c2_var);
      add_eq(a[pos], b[pos]);
      add_clause(d_var);
    }
    add_clause(~c_var,  c1_var,  c2_var);
  }
  return c_var;
}

END_NAMESPACE_YM_SAT
