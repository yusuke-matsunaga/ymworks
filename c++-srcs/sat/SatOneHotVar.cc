
/// @file SatOneHotVar.cc
/// @brief SatOneHotVar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatOneHotVar.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// クラス SatOneHotVar
//////////////////////////////////////////////////////////////////////

// @brief SATの解から値を得る．
SizeType
SatOneHotVar::val(
  const SatModel& model
) const
{
  auto n = mVarArray.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    auto lit = mVarArray[i];
    if ( model[lit] == SatBool3::True ) {
      return i;
    }
  }
  ASSERT_NOT_REACHED;
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス SatSolver
//////////////////////////////////////////////////////////////////////

// @brief one-hot 符号化した変数を返す．
SatOneHotVar
SatSolver::new_onehot_var(
  SizeType size
)
{
  if ( size < 2 ) {
    throw std::invalid_argument{"size < 2"};
  }

  SatOneHotVar var;
  var.mVarArray = std::vector<SatLiteral>(size);
  for ( SizeType i = 0; i < size; ++ i ) {
    var.mVarArray[i] = new_variable(true);
  }
  add_exact_one(var.mVarArray);
  return var;
}

// @brief a == b という条件を追加する．
void
SatSolver::add_eq(
  const SatOneHotVar& a,
  const SatOneHotVar& b
)
{
  if ( a.size() != b.size() ) {
    throw std::invalid_argument{"a.size() != b.size()"};
  }
  for ( SizeType i = 0; i < a.size(); ++ i ) {
    auto a_lit = a.var(i);
    auto b_lit = b.var(i);
    add_clause( a_lit, ~b_lit);
    add_clause(~a_lit,  b_lit);
  }
}

// @brief a != b という条件を追加する．
void
SatSolver::add_ne(
  const SatOneHotVar& a,
  const SatOneHotVar& b
)
{
  if ( a.size() != b.size() ) {
    throw std::invalid_argument{"a.size() != b.size()"};
  }
  std::vector<SatLiteral> tmp_lits;
  tmp_lits.reserve(a.size());
  for ( SizeType i = 0; i < a.size(); ++ i ) {
    auto a_lit = a.var(i);
    auto b_lit = b.var(i);
    auto d_lit = new_variable();
    add_xorgate(d_lit, a_lit, b_lit);
    tmp_lits.push_back(d_lit);
  }
  add_clause(tmp_lits);
}

END_NAMESPACE_YM_SAT
