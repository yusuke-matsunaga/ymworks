
/// @file Tabu.cc
/// @brief Tabu の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "Tabu.h"
#include "McMatrix.h"
#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Tabu
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Tabu::Tabu(
  McMatrix& matrix,
  const JsonValue& opt_obj
) : Solver(maxtirx, opt_obj),
{
}

// @brief tabu search アルゴリズムで解を求める．
SizeType
Tabu::solve(
  std::vector<SizeType>& solution
)
{
  if ( debug() ) {
    std::cout << "Tabu::solve() start"
	      << std::endl;
  }

  // 作業用に行列のコピーを作る．
  auto cur_matrix = matrix();

  // 初期解を Greedy で作る．
  std::vector<SizeType> cur_solution;
  while ( cur_matrix.row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    int col = selector(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    cur_solution.push_back(col);

    if ( debug() ) {
      std::cout << "Col#" << col << " is selected heuristically"
		<< std::endl;
    }

    // 行列を縮約する．
    cur_matrix.reduce(cur_solution);
  }
  // それを最良値としておく．
  auto best_solution = cur_solution;
  best_cost = matrix.cost(best_solution);

  for ( int count = 0; count < limit; ++ count ) {
    // 削除する列を選ぶ．
    int col = choose(cur_matrix, solution);
  }

  return matrix.cost(solution);
}

END_NAMESPACE_YM_MINCOV
