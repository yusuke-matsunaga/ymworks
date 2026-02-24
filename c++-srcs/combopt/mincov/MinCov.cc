
/// @file MinCov.cc
/// @brief MinCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MinCov.h"
#include "ym/JsonValue.h"
#include "mincov/McMatrix.h"
#include "Solver.h"
#include "ym/Range.h"

BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス MinCov
//////////////////////////////////////////////////////////////////////

// @brief 最小被覆問題を解く．
SizeType
MinCov::solve(
  std::vector<SizeType>& solution,
  const JsonValue& option
) const
{
  McMatrix matrix(*this);
  auto solver = Solver::new_obj(matrix, option);
  SizeType cost = solver->solve(solution);

  { // 結果が正しいか検証しておく．
    std::vector<SizeType> uncov_row_list;
    bool stat = verify_solution(solution, uncov_row_list);
    if ( !stat ) {
      for ( auto row: uncov_row_list ) {
	std::cout << "Error: Row#" << row << " is not covered"
		  << std::endl;
      }
    }
  }

  return cost;
}

// @brief mElemList をチェックする．
void
MinCov::sanity_check() const
{
  std::vector<bool> row_mark(row_size(), false);
  for ( auto& elem: mElemList ) {
    SizeType row_pos = elem.row_pos;
    row_mark[row_pos] = true;
  }
  for ( auto row_pos: Range(row_size()) ) {
    if ( !row_mark[row_pos] ) {
      std::cout << "Warning: Row#" << row_pos << " has no elem"
		<< std::endl;
    }
  }
}

// @brief 解を検証する．
bool
MinCov::verify_solution(
  const std::vector<SizeType>& solution,
  std::vector<SizeType>& uncov_row_list
) const
{
  McMatrix matrix(*this);
  std::vector<bool> row_mark(row_size(), false);
  for ( auto col: solution ) {
    for ( auto row: matrix.col_list(col) ) {
      row_mark[row] = true;
    }
  }
  uncov_row_list.clear();
  for ( auto row: Range(row_size()) ) {
    if ( matrix.row_elem_num(row) > 0 && !row_mark[row] ) {
      uncov_row_list.push_back(row);
    }
  }
  return uncov_row_list.empty();
}

END_NAMESPACE_YM_MINCOV
