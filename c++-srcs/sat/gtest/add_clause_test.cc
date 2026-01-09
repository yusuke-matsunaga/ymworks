
/// @file AddClauseTest.cc
/// @brief AddClauseTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "SatTestFixture.h"
#include "ym/SatCondBlock.h"


BEGIN_NAMESPACE_YM

TEST_P(SatTestFixture, add_clause0)
{
  auto vals = std::vector<int>{
    // lit1 ans
    //   0    1
    //   1    1
    1,
    1
  };

  check(1, vals);
}

TEST_P(SatTestFixture, add_clause1_1)
{
  auto lit1 = mVarList[0];

  mSolver.add_clause(lit1);

  auto vals = std::vector<int>{
    // lit1 ans
    //   0    0
    //   1    1
    0,
    1
  };

  check(1, vals);
}

TEST_P(SatTestFixture, add_clause1_2)
{
  auto lit1 = mVarList[0];

  mSolver.add_clause(~lit1);

  auto vals = std::vector<int>{
    // lit1 ans
    //   0    1
    //   1    0
    1,
    0
  };

  check(1, vals);
}

TEST_P(SatTestFixture, add_clause2_1)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];

  mSolver.add_clause(lit1, lit2);

  auto vals = std::vector<int>{
    // lit2 lit1 ans
    //   0    0    0
    //   0    1    1
    //   1    0    1
    //   1    1    1
    0,
    1,
    1,
    1
  };

  check(2, vals);
}

TEST_P(SatTestFixture, add_clause2_2)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];

  mSolver.add_clause(~lit1, lit2);

  auto vals = std::vector<int>{
    // lit2 lit1 ans
    //   0    0    1
    //   0    1    0
    //   1    0    1
    //   1    1    1
    1,
    0,
    1,
    1
  };

  check(2, vals);
}

TEST_P(SatTestFixture, add_clause3_1)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];

  mSolver.add_clause(lit1, lit2, lit3);

  auto vals = std::vector<int>{
    // lit3 lit2 lit1 ans
    //   0    0    0    0
    //   0    0    1    1
    //   0    1    0    1
    //   0    1    1    1
    //   1    0    0    1
    //   1    0    1    1
    //   1    1    0    1
    //   1    1    1    1
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1
  };

  check(3, vals);
}

TEST_P(SatTestFixture, add_clause3_2)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];

  mSolver.add_clause(~lit1, lit2, ~lit3);

  auto vals = std::vector<int>{
    // lit3 lit2 lit1 ans
    //   0    0    0    1
    //   0    0    1    1
    //   0    1    0    1
    //   0    1    1    1
    //   1    0    0    1
    //   1    0    1    0
    //   1    1    0    1
    //   1    1    1    1
    1,
    1,
    1,
    1,
    1,
    0,
    1,
    1
  };

  check(3, vals);
}

TEST_P(SatTestFixture, add_clause4_1)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];
  auto lit4 = mVarList[3];

  mSolver.add_clause(lit1, lit2, lit3, lit4);

  auto vals = std::vector<int>{
    // lit4 lit3 lit2 lit1 ans
    //   0    0    0    0    0
    //   0    0    0    1    1
    //   0    0    1    0    1
    //   0    0    1    1    1
    //   0    1    0    0    1
    //   0    1    0    1    1
    //   0    1    1    0    1
    //   0    1    1    1    1
    //   1    0    0    0    1
    //   1    0    0    1    1
    //   1    0    1    0    1
    //   1    0    1    1    1
    //   1    1    0    0    1
    //   1    1    0    1    1
    //   1    1    1    0    1
    //   1    1    1    1    1
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
  };

  check(4, vals);
}

TEST_P(SatTestFixture, add_clause5_1)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];
  auto lit4 = mVarList[3];
  auto lit5 = mVarList[4];

  mSolver.add_clause(lit1, lit2, lit3, lit4, lit5);

  auto vals = std::vector<int>{
    // lit5 lit4 lit3 lit2 lit1  ans
    //    0    0    0    0    0    0
    //    0    0    0    0    1    1
    //    0    0    0    1    0    1
    //    0    0    0    1    1    1
    //    0    0    1    0    0    1
    //    0    0    1    0    1    1
    //    0    0    1    1    0    1
    //    0    0    1    1    1    1
    //    0    1    0    0    0    1
    //    0    1    0    0    1    1
    //    0    1    0    1    0    1
    //    0    1    0    1    1    1
    //    0    1    1    0    0    1
    //    0    1    1    0    1    1
    //    0    1    1    1    0    1
    //    0    1    1    1    1    1
    //    1    0    0    0    0    1
    //    1    0    0    0    1    1
    //    1    0    0    1    0    1
    //    1    0    0    1    1    1
    //    1    0    1    0    0    1
    //    1    0    1    0    1    1
    //    1    0    1    1    0    1
    //    1    0    1    1    1    1
    //    1    1    0    0    0    1
    //    1    1    0    0    1    1
    //    1    1    0    1    0    1
    //    1    1    0    1    1    1
    //    1    1    1    0    0    1
    //    1    1    1    0    1    1
    //    1    1    1    1    0    1
    //    1    1    1    1    1    1
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  check(5, vals);
}

TEST_P(SatTestFixture, add_clause5n_1)
{
  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];
  auto lit4 = mVarList[3];
  auto lit5 = mVarList[4];

  auto tmp_lits = std::vector<SatLiteral>{
    lit1, lit2, lit3, lit4, lit5
  };

  mSolver.add_clause(tmp_lits);

  auto vals = std::vector<int>{
    // lit5 lit4 lit3 lit2 lit1  ans
    //    0    0    0    0    0    0
    //    0    0    0    0    1    1
    //    0    0    0    1    0    1
    //    0    0    0    1    1    1
    //    0    0    1    0    0    1
    //    0    0    1    0    1    1
    //    0    0    1    1    0    1
    //    0    0    1    1    1    1
    //    0    1    0    0    0    1
    //    0    1    0    0    1    1
    //    0    1    0    1    0    1
    //    0    1    0    1    1    1
    //    0    1    1    0    0    1
    //    0    1    1    0    1    1
    //    0    1    1    1    0    1
    //    0    1    1    1    1    1
    //    1    0    0    0    0    1
    //    1    0    0    0    1    1
    //    1    0    0    1    0    1
    //    1    0    0    1    1    1
    //    1    0    1    0    0    1
    //    1    0    1    0    1    1
    //    1    0    1    1    0    1
    //    1    0    1    1    1    1
    //    1    1    0    0    0    1
    //    1    1    0    0    1    1
    //    1    1    0    1    0    1
    //    1    1    0    1    1    1
    //    1    1    1    0    0    1
    //    1    1    1    0    1    1
    //    1    1    1    1    0    1
    //    1    1    1    1    1    1
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  check(5, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond1_1)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];

  mSolver.add_clause(lit1);

  auto vals = std::vector<int>{
    // lit1 ans
    //   0    0
    //   1    1
    0,
    1
  };

  check_with_cond1(1, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond1_1_ext)
{
  {
    auto clit1 = mCondVarList[0];
    auto _ = mSolver.condblock(clit1);

    auto lit1 = mVarList[0];

    mSolver.add_clause(lit1);

    auto vals = std::vector<int>{
      // lit1 ans
      //   0    0
      //   1    1
      0,
      1
    };

    check_with_cond1(1, vals);
  }

  // ここは条件変数の影響を受けない．
  auto lit2 = mVarList[1];
  mSolver.add_clause(lit2);

  auto vals = std::vector<int>{
    // lit2 lit1 ans
    //    0    0   0
    //    0    1   0
    //    1    0   1
    //    1    1   1
    0,
    0,
    1,
    1
  };
  check(2, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond1_2)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];

  mSolver.add_clause(~lit1);

  auto vals = std::vector<int>{
    // lit1 ans
    //   0    1
    //   1    0
    1,
    0
  };

  check_with_cond1(1, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond2_1)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];

  mSolver.add_clause(lit1, lit2);

  auto vals = std::vector<int>{
    // lit2 lit1 ans
    //   0    0    0
    //   0    1    1
    //   1    0    1
    //   1    1    1
    0,
    1,
    1,
    1
  };

  check_with_cond1(2, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond2_2)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];

  mSolver.add_clause(~lit1, lit2);

  auto vals = std::vector<int>{
    // lit2 lit1 ans
    //   0    0    1
    //   0    1    0
    //   1    0    1
    //   1    1    1
    1,
    0,
    1,
    1
  };

  check_with_cond1(2, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond3_1)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];

  mSolver.add_clause(lit1, lit2, lit3);

  auto vals = std::vector<int>{
    // lit3 lit2 lit1 ans
    //   0    0    0    0
    //   0    0    1    1
    //   0    1    0    1
    //   0    1    1    1
    //   1    0    0    1
    //   1    0    1    1
    //   1    1    0    1
    //   1    1    1    1
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1
  };

  check_with_cond1(3, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond3_2)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];

  mSolver.add_clause(~lit1, lit2, ~lit3);

  auto vals = std::vector<int>{
    // lit3 lit2 lit1 ans
    //   0    0    0    1
    //   0    0    1    1
    //   0    1    0    1
    //   0    1    1    1
    //   1    0    0    1
    //   1    0    1    0
    //   1    1    0    1
    //   1    1    1    1
    1,
    1,
    1,
    1,
    1,
    0,
    1,
    1
  };

  check_with_cond1(3, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond4_1)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];
  auto lit4 = mVarList[3];

  mSolver.add_clause(lit1, lit2, lit3, lit4);

  auto vals = std::vector<int>{
    // lit4 lit3 lit2 lit1 ans
    //   0    0    0    0    0
    //   0    0    0    1    1
    //   0    0    1    0    1
    //   0    0    1    1    1
    //   0    1    0    0    1
    //   0    1    0    1    1
    //   0    1    1    0    1
    //   0    1    1    1    1
    //   1    0    0    0    1
    //   1    0    0    1    1
    //   1    0    1    0    1
    //   1    0    1    1    1
    //   1    1    0    0    1
    //   1    1    0    1    1
    //   1    1    1    0    1
    //   1    1    1    1    1
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
  };

  check_with_cond1(4, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond5_1)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];
  auto lit4 = mVarList[3];
  auto lit5 = mVarList[4];

  mSolver.add_clause(lit1, lit2, lit3, lit4, lit5);

  auto vals = std::vector<int>{
    // lit5 lit4 lit3 lit2 lit1  ans
    //    0    0    0    0    0    0
    //    0    0    0    0    1    1
    //    0    0    0    1    0    1
    //    0    0    0    1    1    1
    //    0    0    1    0    0    1
    //    0    0    1    0    1    1
    //    0    0    1    1    0    1
    //    0    0    1    1    1    1
    //    0    1    0    0    0    1
    //    0    1    0    0    1    1
    //    0    1    0    1    0    1
    //    0    1    0    1    1    1
    //    0    1    1    0    0    1
    //    0    1    1    0    1    1
    //    0    1    1    1    0    1
    //    0    1    1    1    1    1
    //    1    0    0    0    0    1
    //    1    0    0    0    1    1
    //    1    0    0    1    0    1
    //    1    0    0    1    1    1
    //    1    0    1    0    0    1
    //    1    0    1    0    1    1
    //    1    0    1    1    0    1
    //    1    0    1    1    1    1
    //    1    1    0    0    0    1
    //    1    1    0    0    1    1
    //    1    1    0    1    0    1
    //    1    1    0    1    1    1
    //    1    1    1    0    0    1
    //    1    1    1    0    1    1
    //    1    1    1    1    0    1
    //    1    1    1    1    1    1
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  check_with_cond1(5, vals);
}

TEST_P(SatTestFixture, add_clause_with_cond5n_1)
{
  auto clit1 = mCondVarList[0];
  auto _ = mSolver.condblock(clit1);

  auto lit1 = mVarList[0];
  auto lit2 = mVarList[1];
  auto lit3 = mVarList[2];
  auto lit4 = mVarList[3];
  auto lit5 = mVarList[4];

  auto tmp_lits = std::vector<SatLiteral>{
    lit1, lit2, lit3, lit4, lit5
  };
  mSolver.add_clause(tmp_lits);

  auto vals = std::vector<int>{
    // lit5 lit4 lit3 lit2 lit1  ans
    //    0    0    0    0    0    0
    //    0    0    0    0    1    1
    //    0    0    0    1    0    1
    //    0    0    0    1    1    1
    //    0    0    1    0    0    1
    //    0    0    1    0    1    1
    //    0    0    1    1    0    1
    //    0    0    1    1    1    1
    //    0    1    0    0    0    1
    //    0    1    0    0    1    1
    //    0    1    0    1    0    1
    //    0    1    0    1    1    1
    //    0    1    1    0    0    1
    //    0    1    1    0    1    1
    //    0    1    1    1    0    1
    //    0    1    1    1    1    1
    //    1    0    0    0    0    1
    //    1    0    0    0    1    1
    //    1    0    0    1    0    1
    //    1    0    0    1    1    1
    //    1    0    1    0    0    1
    //    1    0    1    0    1    1
    //    1    0    1    1    0    1
    //    1    0    1    1    1    1
    //    1    1    0    0    0    1
    //    1    1    0    0    1    1
    //    1    1    0    1    0    1
    //    1    1    0    1    1    1
    //    1    1    1    0    0    1
    //    1    1    1    0    1    1
    //    1    1    1    1    0    1
    //    1    1    1    1    1    1
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  check_with_cond1(5, vals);
}

INSTANTIATE_TEST_SUITE_P(SatSolverTest,
			 SatTestFixture,
			 ::testing::Values("lingeling", "glueminisat2", "minisat2", "minisat",
					   "ymsat1", "ymsat2", "ymsat1_old"));

END_NAMESPACE_YM
