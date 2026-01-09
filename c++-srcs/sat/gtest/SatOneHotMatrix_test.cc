
/// @file SatOneHotMatrix_test.cc
/// @brief SatOneHotMatrix_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SatSolver.h"
#include "ym/PermGen.h"


BEGIN_NAMESPACE_YM

class SatOneHotMatrixTest :
public ::testing::TestWithParam<std::string>
{
public:

  /// @brief コンストラクタ
  SatOneHotMatrixTest() = default;

  /// @brief デストラクタ
  ~SatOneHotMatrixTest() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 単体テスト
  void
  check(
    SizeType elem_size,
    SizeType sel_size
  );

  // SATソルバ
  SatSolver mSolver;

};

// @brief 単体テスト
void
SatOneHotMatrixTest::check(
  SizeType elem_size,
  SizeType sel_size
)
{
  auto var = mSolver.new_onehot_matrix(elem_size, sel_size);
  ASSERT_EQ( SatBool3::True, mSolver.solve() );
  for ( PermGen pg(elem_size, sel_size); !pg.is_end(); ++ pg ) {
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(sel_size);
    for ( SizeType i = 0; i < sel_size; ++ i ) {
      auto lit = var.var(i, pg(i));
      assumptions.push_back(lit);
    }
    auto ans = mSolver.solve(assumptions);
    ASSERT_EQ( SatBool3::True, ans );
    auto& model = mSolver.model();
    for ( SizeType sel1 = 0; sel1 < sel_size; ++ sel1 ) {
      for ( SizeType elem1 = 0; elem1 < elem_size; ++ elem1 ) {
	auto lit1 = var.var(sel1, elem1);
	auto exp_ans = elem1 == pg(sel1) ? SatBool3::True : SatBool3::False;
	std::ostringstream buf;
	buf << ", sel1 = " << sel1
	    << ", elem1 = " << elem1;
	EXPECT_EQ( exp_ans, model[lit1] ) << buf.str();
      }
    }
    for ( SizeType sel = 0; sel < sel_size; ++ sel ) {
      auto elem = var.val(model, sel);
      EXPECT_EQ( pg(sel), elem );
    }
  }
}

TEST_P(SatOneHotMatrixTest, basic2_1)
{
  check(2, 1);
}

TEST_P(SatOneHotMatrixTest, basic2_2)
{
  check(2, 2);
}

TEST_P(SatOneHotMatrixTest, basic3_1)
{
  check(3, 1);
}

TEST_P(SatOneHotMatrixTest, basic3_2)
{
  check(3, 2);
}

TEST_P(SatOneHotMatrixTest, basic3_3)
{
  check(3, 3);
}

TEST_P(SatOneHotMatrixTest, basic5_1)
{
  check(5, 1);
}

TEST_P(SatOneHotMatrixTest, basic5_3)
{
  check(5, 3);
}

TEST_P(SatOneHotMatrixTest, basic5_5)
{
  check(5, 5);
}

TEST_P(SatOneHotMatrixTest, basic10_1)
{
  check(10, 1);
}

TEST_P(SatOneHotMatrixTest, basic10_3)
{
  check(10, 3);
}

TEST_P(SatOneHotMatrixTest, basic10_5)
{
  check(10, 5);
}

INSTANTIATE_TEST_SUITE_P(SatSolverTest,
			 SatOneHotMatrixTest,
			 ::testing::Values("lingeling", "glueminisat2",
					   "minisat2", "minisat",
					   "ymsat1", "ymsat2", "ymsat1_old"));

END_NAMESPACE_YM
