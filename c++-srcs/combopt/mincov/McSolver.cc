
/// @file McSolver.cc
/// @brief McSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McSolver.h"
#include "McMatrix.h"
#include "LbMAX.h"
#include "LbMIS1.h"
#include "LbMIS2.h"
#include "LbCS.h"
#include "SelSimple.h"
#include "SelCS.h"

#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_MINCOV

bool McSolver_debug = false;

//////////////////////////////////////////////////////////////////////
// クラス McSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
McSolver::McSolver()
{
  auto calc1 = new LbCS();
  auto calc2 = new LbMIS1();
  auto calc3 = new LbMAX();
  calc3->add_calc(calc1);
  calc3->add_calc(calc2);
  mLbCalc = calc3;

  mSelector = new SelSimple();
  //mSelector = new SelCS();
  mMatrix = nullptr;
  mCostArray = nullptr;
}

// @brief デストラクタ
McSolver::~McSolver()
{
  delete mLbCalc;
  delete mSelector;
  delete mMatrix;
  delete mCostArray;
}

// @brief 問題のサイズを設定する．
void
McSolver::set_size(
  int row_size,
  int col_size
)
{
  delete mMatrix;
  delete mCostArray;
  mCostArray = new int[col_size];
  for ( int i = 0; i < col_size; ++ i ) {
    mCostArray[i] = 1;
  }
  mMatrix = new McMatrix(row_size, col_size, mCostArray);
}

// @brief 列のコストを設定する
void
McSolver::set_col_cost(
  int col_pos,
  int cost
)
{
  mCostArray[col_pos] = cost;
}

// @brief 要素を追加する．
void
McSolver::insert_elem(
  int row_pos,
  int col_pos
)
{
  mMatrix->insert_elem(row_pos, col_pos);
}

// @brief 最小被覆問題を解く．
int
McSolver::exact(
  std::vector<int>& solution
)
{
  McSolverImpl impl(*mMatrix, *mLbCalc, *mSelector);

  int cost = impl.exact(solution);

  return cost;
}

// @brief ヒューリスティックで最小被覆問題を解く．
int
McSolver::heuristic(
  const std::string& algorithm,
  std::vector<int>& solution
)
{
  McMatrix cur_matrix(*mMatrix);

  solution.clear();
  cur_matrix.reduce(solution);

  if ( cur_matrix.row_num() > 0 ) {

    if ( algorithm == "greedy" ) {
      greedy(cur_matrix, solution);
    }
    else if ( algorithm == "random" ) {
      random(cur_matrix, solution);
    }
    else if ( algorithm == "MCT" ) {
      ;
    }
    else {
      // デフォルトフォールバックは greedy
      greedy(cur_matrix, solution);
    }
  }

  ASSERT_COND( mMatrix->verify(solution) );

  int cost = mMatrix->cost(solution);

  return cost;
}

// @brief greedy アルゴリズムで解を求める．
void
McSolver::greedy(
  const McMatrix& matrix,
  std::vector<int>& solution
)
{
  if ( McSolver_debug ) {
    std::cout << "McSolver::greedy() start"
	      << std::endl;
  }

#if 0
  for ( int i = 0; i < 10000; ++ i ) {
  McMatrix cur_matrix(matrix);
  vector<int> solution;
  while ( cur_matrix.row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    int col = (*mSelector)(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    solution.push_back(col);

    if ( McSolver_debug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }

    cur_matrix.reduce(solution);
  }
  }
#endif
  McMatrix cur_matrix(matrix);

  while ( cur_matrix.row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    int col = (*mSelector)(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    solution.push_back(col);

    if ( McSolver_debug ) {
      std::cout << "Col#" << col << " is selected heuristically"
		<< std::endl;
    }

    cur_matrix.reduce(solution);
  }
}

// @brief naive な random アルゴリズムで解を求める．
void
McSolver::random(
  const McMatrix& matrix,
  std::vector<int>& solution
)
{
  if ( McSolver_debug ) {
    std::cout << "McSolver::random() start"
	      << std::endl;
  }

  RandGen rg;

  int count_limit = 1000;

  bool first = true;
  int best_cost = 0;
  std::vector<int> best_solution;
  for ( int count = 0; count < count_limit; ++ count ) {
    McMatrix cur_matrix(matrix);
    std::vector<int> cur_solution;

    while ( cur_matrix.row_num() > 0 ) {
      // ランダムに選ぶ
      int col = 0; // 未完
      auto rh = cur_matrix.row_front();
      int n = rh->num();
      ASSERT_COND( n > 0 );
      int idx = rg.int32() % n;
      for ( auto cell = rh->front();
	    !rh->is_end(cell);
	    cell = cell->row_next(), -- idx ) {
	if ( idx == 0 ) {
	  col = cell->col_pos();
	  break;
	}
      }

      // その列を選択する
      cur_matrix.select_col(col);
      cur_solution.push_back(col);

      cur_matrix.reduce(cur_solution);
    }
    int cur_cost = matrix.cost(cur_solution);
    if ( first || best_cost > cur_cost ) {
      first = false;
      best_cost = cur_cost;
      best_solution = cur_solution;
      {
	int base_cost = matrix.cost(solution);
	std::cout << "best so far = " << best_cost + base_cost
		  << " ( " << best_cost << " + " << base_cost << " )"
		  << std::endl;
      }
    }
  }

  for ( auto col: best_solution ) {
    solution.push_back(col);
  }
}

// @brief 内部の行列の内容を出力する．
void
McSolver::print_matrix(
  std::ostream& s
)
{
  mMatrix->print(s);
}

END_NAMESPACE_YM_MINCOV
