
/// @file Exact.cc
/// @brief Exact の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "Exact.h"
#include "mincov/McMatrix.h"
#include "mincov/LbCalc.h"
#include "mincov/Selector.h"
#include "ym/JsonValue.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM_MINCOV

BEGIN_NONAMESPACE

static
int solve_id = 0;

// 2つの行列が等しいかをチェックする関数
// 等しくなければ例外を送出する．
void
verify_block(
  McMatrix& a,
  McMatrix& b
)
{
  ASSERT_COND( a.row_size() == b.row_size() );
  ASSERT_COND( a.col_size() == b.col_size() );
  ASSERT_COND( a.row_head_list() == b.row_head_list() );
  ASSERT_COND( a.col_head_list() == b.col_head_list() );

  for ( int row_pos: Range(a.row_size()) ) {
    ASSERT_COND( a.row_list(row_pos) == b.row_list(row_pos) );
  }

  for ( int col_pos: Range(a.col_size()) ) {
    ASSERT_COND( a.col_list(col_pos) == b.col_list(col_pos) );
  }
}

std::unique_ptr<LbCalc>
new_LbCalc(
  const JsonValue& opt_obj
)
{
  JsonValue lb_opt;
  if ( opt_obj.has_key("lower_bound") ) {
    lb_opt = opt_obj.at("lower_bound");
  }
  return LbCalc::new_obj(lb_opt);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Exact
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Exact::Exact(
  McMatrix& matrix,
  const JsonValue& opt_obj
) : Solver{matrix, opt_obj},
    mLbCalc{new_LbCalc(opt_obj)},
    mSelector{new_Selector(opt_obj)},
    mDoPartition{opt_obj.get_bool_elem("partition", false)}
{
}

// @brief デストラクタ
Exact::~Exact()
{
}

// @brief 最小被覆問題を解く．
SizeType
Exact::solve(
  std::vector<SizeType>& solution
)
{
  solve_id = 0;

  mBest = INT_MAX;
  mCurSolution.clear();
  bool stat = _solve(0, 0);
  ASSERT_COND( stat );

  solution = mBestSolution;

  if ( debug() ) {
    std::cout << "Total branch: " << solve_id
	      << std::endl;
  }

  return mBest;
}

// @brief 解を求める再帰関数
bool
Exact::_solve(
  int lb,
  int depth
)
{
  int cur_id = solve_id;
  ++ solve_id;

  std::vector<SizeType> dummy;
  matrix().reduce_loop(mCurSolution, dummy);

  auto tmp_cost = matrix().cost(mCurSolution);
  auto tmp_lb = mLbCalc->calc(matrix()) + tmp_cost;
  if ( lb < tmp_lb ) {
    lb = tmp_lb;
  }

  bool cur_debug = debug();
  if ( depth > debug_depth() ) {
    cur_debug = false;
  }

  if ( cur_debug ) {
    auto nr = matrix().active_row_num();
    auto nc = matrix().active_col_num();
    std::cout << "[" << depth << "] " << nr << "x" << nc
	      << " sel=" << tmp_cost << " bnd=" << mBest
	      << " lb=" << lb
	      << std::endl;
  }

  if ( lb >= mBest ) {
    // 下界を用いた枝刈り
    if ( cur_debug ) {
      std::cout << " bounded"
		<< std::endl;
    }
    return false;
  }

  if ( matrix().active_row_num() == 0 ) {
    // 自明な解
    mBest = tmp_cost;
    mBestSolution = mCurSolution;
    if ( cur_debug ) {
      std::cout << " best"
		<< std::endl;
    }
    return true;
  }

#if 0
  McBlock block1(mBlock.matrix());
  if ( mDoPartition && mBlock.partition(block1) ) {
    // ブロック分割を行う．
    Exact solver1(block1, mLbCalcList, mSelector);
    Exact solver2(block2, mLbCalcList, mSelector);
    if ( cur_debug ) {
      std::cout << std::endl
		<< "BLOCK PARTITION"
		<< std::endl
		<< "Matrix#1"
		<< std::endl;
      solver1.block().print(std::cout);
      std::cout << "Matrix#2"
		<< std::endl;
      solver2.block().print(std::cout);
    }

    block1.save();
    block2.save();
    int cost_so_far = mBlock.cost(mCurSolution);
    int lb_rest = 0;
    for ( auto lb_calc_p: mLbCalcList ) {
      int tmp_lb = (*lb_calc_p)(solver1.block());
      if ( lb_rest < tmp_lb ) {
	lb_rest = tmp_lb;
      }
    }
    solver1.mBest = mBest - cost_so_far - lb_rest;
    solver1.mCurSolution.clear();
    bool stat1 = solver1._solve(0, depth + 1);
    if ( stat1 ) {
      mCurSolution.insert(mCurSolution.end(),
			  solver1.mBestSolution.begin(), solver1.mBestSolution.end());
      cost_so_far += solver1.mBest;
      solver2.mBest = mBest - cost_so_far;
      solver2.mCurSolution.clear();
      stat1 = solver2._solve(0, depth + 1);
      if ( stat1 ) {
	mCurSolution.insert(mCurSolution.end(),
			    solver2.mBestSolution.begin(), solver2.mBestSolution.end());
	cost_so_far += solver2.mBest;
      }
    }
    block1.restore();
    block2.restore();
    mBlock.merge(block1, block2);

    if ( stat1 ) {
      ASSERT_COND( mBlock.verify(mCurSolution) );
      if ( mBest > cost_so_far ) {
	mBest = cost_so_far;
	mBestSolution = mCurSolution;
	return true;
      }
    }
    return false;
  }
#endif

  // 次の分岐のための列をとってくる．
  auto col = mSelector->select(matrix());

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(matrix());
  std::vector<int> orig_solution(mCurSolution);
#endif

  auto cur_n = mCurSolution.size();
  matrix().save();

  // その列を選択したときの最良解を求める．
  matrix().select_col(col);
  mCurSolution.push_back(col);

  if ( cur_debug ) {
    std::cout << "[" << depth << "]A select column#" << col
	      << std::endl;
  }

  auto stat1 = _solve(lb, depth + 1);

  matrix().restore();
  auto c = mCurSolution.size() - cur_n;
  for ( SizeType i = 0; i < c; ++ i ) {
    mCurSolution.pop_back();
  }

#if defined(VERIFYY_MINCOV)
  verify_matrix(orig_matrix, matrix());
  ASSERT_COND( orig_solution == mCurSlution );
#endif

  // 今得た最良解が下界と等しかったら探索を続ける必要はない．
  if ( lb >= mBest ) {
    if ( cur_debug ) {
      std::cout << "[" << depth << "]C bounded"
		<< std::endl;
    }
    return true;
  }

  // その列を選択しなかったときの最良解を求める．
  matrix().delete_col(col);

  if ( cur_debug ) {
    std::cout << "[" << depth << "]B deselect column#" << col
	      << std::endl;
  }

  auto stat2 = _solve(lb, depth + 1);

  return stat1 || stat2;
}

END_NAMESPACE_YM_MINCOV
