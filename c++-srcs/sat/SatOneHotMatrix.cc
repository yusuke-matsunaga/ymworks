
/// @file SatOneHotMatrix.cc
/// @brief SatOneHotMatrix の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/SatOneHotMatrix.h"
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// クラス SatOneHotMatrix
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SatOneHotMatrix::SatOneHotMatrix(
  SatSolver& solver,
  SizeType elem_size,
  SizeType sel_size
) : mElemSize{elem_size},
    mSelSize{sel_size},
    mVarArray(mElemSize * mSelSize)
{
  for ( SizeType i = 0; i < mVarArray.size(); ++ i ) {
    mVarArray[i] = solver.new_variable(true);
  }
}

// @brief SATの解から値を得る．
SizeType
SatOneHotMatrix::val(
  const SatModel& model,
  SizeType sel_val
) const
{
  _check_sel_range(sel_val, "sel_val");

  for ( SizeType i = 0; i < elem_size(); ++ i ) {
    auto lit = _var(sel_val, i);
    if ( model[lit] == SatBool3::True ) {
      return i;
    }
  }
  throw std::logic_error{"model broken"};
}


//////////////////////////////////////////////////////////////////////
// クラス SatSolver
//////////////////////////////////////////////////////////////////////

// @brief one-hot 符号化した変数を返す．
SatOneHotMatrix
SatSolver::new_onehot_matrix(
  SizeType elem_size,
  SizeType sel_size
)
{
  if ( elem_size < sel_size ) {
    throw std::invalid_argument{"elem_size < sel_size"};
  }
  if ( sel_size < 1 ) {
    throw std::invalid_argument{"sel_size < 1"};
  }

  SatOneHotMatrix matrix(*this, elem_size, sel_size);
  for ( SizeType sel = 0; sel < sel_size; ++ sel ) {
    std::vector<SatLiteral> tmp_lits(elem_size);
    for ( SizeType i = 0; i < elem_size; ++ i ) {
      tmp_lits[i] = matrix._var(sel, i);
    }
    add_exact_one(tmp_lits);
  }
  for ( SizeType elem = 0; elem < elem_size; ++ elem ) {
    if ( sel_size > 1 ) {
      std::vector<SatLiteral> tmp_lits(sel_size);
      for ( SizeType i = 0; i < sel_size; ++ i ) {
	tmp_lits[i] = matrix._var(i, elem);
      }
      add_at_most_one(tmp_lits);
    }
  }
  return matrix;
}

END_NAMESPACE_YM_SAT
