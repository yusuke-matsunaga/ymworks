
/// @file McPerturbator.cc
/// @brief McPerturbator の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ym/McPerturbator.h"
#include "mincov/McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス McParturbator
//////////////////////////////////////////////////////////////////////

// @brief 別解を求める．
std::vector<SizeType>
McPerturbator::operator()(
  const std::vector<SizeType>& solution
)
{
  // 対象の行列
  auto matrix = McMatrix(mMinCov);

  // 新しい解
  std::vector<SizeType> new_solution;
  new_solution.reserve(solution.size());

  // solution から必須列を除いた候補リストを作る．
  std::vector<bool> col_mark(mMinCov.col_size(), false);
  for ( auto col: matrix.essential_cols() ) {
    col_mark[col] = true;
    new_solution.push_back(col);
  }
  std::vector<SizeType> cand_list;
  cand_list.reserve(solution.size());
  for ( auto col: solution ) {
    if ( !col_mark[col] ) {
      cand_list.push_back(col);
    }
  }
  auto nc = cand_list.size();
  if ( nc == 0 ) {
    // 別解はない．
    return {};
  }

  auto uid = std::uniform_int_distribution<SizeType>(0, nc - 1);
  auto pos = uid(mRandGen);
  auto col0 = cand_list[pos];
  // col0 を取り除いた解を作る．
  for ( SizeType i = 0; i < nc; ++ i ) {
    if ( i != pos ) {
      auto col = solution[i];
      new_solution.push_back(col);
      matrix.select_col(col);
    }
  }
  // 縮約する．
  matrix.reduce_loop(new_solution);

  // 全ての行をカバーするまでランダムに列を選ぶ．
  // ただし col0 は選ばない．
  while ( matrix.active_row_num() > 0 ) {
    for ( auto row: matrix.row_head_list() ) {
      std::vector<SizeType> col_list;
      for ( auto col: matrix.col_list(row) ) {
	if ( col != col0 ) {
	  col_list.push_back(col);
	}
      }
      auto nc = col_list.size();
      auto uid1 = std::uniform_int_distribution<SizeType>(0, nc - 1);
      auto col_pos = uid1(mRandGen);
      auto col = col_list[col_pos];
      new_solution.push_back(col);
      matrix.select_col(col);
      // 縮約する．
      matrix.reduce_loop(new_solution);
    }
  }
  // new_solution を整列する．
  std::sort(new_solution.begin(), new_solution.end());

  return new_solution;
}

END_NAMESPACE_YM_MINCOV
