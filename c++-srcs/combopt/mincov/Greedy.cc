
/// @file Greedy.cc
/// @brief Greedy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Greedy.h"
#include "mincov/McMatrix.h"
#include "mincov/Selector.h"
#include "ym/JsonValue.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス Greedy
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Greedy::Greedy(
  McMatrix& matrix,
  const JsonValue& opt_obj
) : Solver{matrix, opt_obj},
    mSelector{new_Selector(opt_obj)}
{
}

// @brief greedy アルゴリズムで解を求める．
SizeType
Greedy::solve(
  std::vector<SizeType>& solution
)
{
  if ( debug() ) {
    std::cout << "Greedy::solve() start"
	      << std::endl;
    matrix().print(std::cout);
  }

  while ( matrix().active_row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    auto col = mSelector->select(matrix());
    if ( debug() ) {
      std::cout << " selecting Col#" << col
		<< std::endl;
    }

    // その列を選択する．
    matrix().select_col(col);
    solution.push_back(col);

    if ( debug() ) {
      std::cout << "Col#" << col << " is selected heuristically"
		<< std::endl;
    }

    // 行列を縮約する．
    std::vector<SizeType> dummy;
    matrix().reduce_loop(solution, dummy);

    if ( debug() ) {
      std::cout << "After reduction"
		<< std::endl;
      matrix().print(std::cout);
    }
  }

  return matrix().cost(solution);
}

END_NAMESPACE_YM_MINCOV
