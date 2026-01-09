
/// @file SatOneHotVar_test.cc
/// @brief SatOneHotVar_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SatSolver.h"


BEGIN_NAMESPACE_YM

class SatOneHotVarTest :
public ::testing::TestWithParam<std::string>
{
public:

  /// @brief コンストラクタ
  SatOneHotVarTest() = default;

  /// @brief デストラクタ
  ~SatOneHotVarTest() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 単体テスト
  void
  check(
    const SatOneHotVar& var
  );

  /// @brief add_eq() のテスト
  void
  check_add_eq(
    const SatOneHotVar& a_var,
    const SatOneHotVar& b_var
  );

  /// @brief add_nq() のテスト
  void
  check_add_ne(
    const SatOneHotVar& a_var,
    const SatOneHotVar& b_var
  );

  /// @brief テストの共通部分
  void
  check_common(
    const SatOneHotVar& a_var,
    const SatOneHotVar& b_var,
    std::function<bool(int, int)> cmp_func
  );

  // SATソルバ
  SatSolver mSolver;

};

// @brief 単体テスト
void
SatOneHotVarTest::check(
  const SatOneHotVar& var
)
{
  auto& lits = var.vars();

  auto n = lits.size();
  SizeType nexp = (1 << n);
  for ( int bits = 0; bits < nexp; ++ bits ) {
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(n);
    int c = 0;
    int val = -1;
    for ( int i = 0; i < n; ++ i ) {
      auto lit = lits[i];
      if ( bits & (1 << i) ) {
	assumptions.push_back(lit);
	++ c;
	val = i;
      }
      else {
	assumptions.push_back(~lit);
      }
    }
    std::ostringstream buf;
    if ( c == 1 ) {
      buf << "val = " << val;
    }
    else {
      buf << "bits = ";
      for ( int i = 0; i < n; ++ i ) {
	if ( bits & (1 << i) ) {
	  buf << "1";
	}
	else {
	  buf << "0";
	}
      }
    }
    auto exp_ans = (c == 1) ? SatBool3::True : SatBool3::False;
    auto ans = mSolver.solve(assumptions);
    EXPECT_EQ( exp_ans, ans ) << buf.str();
  }
}

// @brief add_eq() のテスト
void
SatOneHotVarTest::check_add_eq(
  const SatOneHotVar& a_var,
  const SatOneHotVar& b_var
)
{
  mSolver.add_eq(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 == val2;
	       });
}

// @brief add_ne() のテスト
void
SatOneHotVarTest::check_add_ne(
  const SatOneHotVar& a_var,
  const SatOneHotVar& b_var
)
{
  mSolver.add_ne(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 != val2;
	       });
}

// @brief テストの共通部分
void
SatOneHotVarTest::check_common(
  const SatOneHotVar& a_var,
  const SatOneHotVar& b_var,
  std::function<bool(int, int)> cmp_func
)
{
  auto& a_lits = a_var.vars();
  auto& b_lits = b_var.vars();

  auto n1 = a_lits.size();
  auto n2 = b_lits.size();
  for ( int val1 = 0; val1 < n1; ++ val1 ) {
    for ( int val2 = 0; val2 < n2; ++ val2 ) {
      std::vector<SatLiteral> assumptions;
      assumptions.reserve(n1 + n2);
      for ( int i = 0; i < n1; ++ i ) {
	auto lit = a_lits[i];
	if ( i == val1 ) {
	  assumptions.push_back(lit);
	}
	else {
	  assumptions.push_back(~lit);
	}
      }
      for ( int i = 0; i < n2; ++ i ) {
	auto lit = b_lits[i];
	if ( i == val2 ) {
	  assumptions.push_back(lit);
	}
	else {
	  assumptions.push_back(~lit);
	}
      }
      std::ostringstream buf;
      buf << "val1 = " << val1
	  << ", val2 = " << val2;
      auto exp_ans = SatBool3::False;
      if ( cmp_func(val1, val2) ) {
	exp_ans = SatBool3::True;
      }
      auto ans = mSolver.solve(assumptions);
      EXPECT_EQ( exp_ans, ans ) << buf.str();
    }
  }
}

TEST_P(SatOneHotVarTest, basic2)
{
  auto var = mSolver.new_onehot_var(2);
  check(var);
}

TEST_P(SatOneHotVarTest, basic3)
{
  auto var = mSolver.new_onehot_var(3);
  check(var);
}

TEST_P(SatOneHotVarTest, basic5)
{
  auto var = mSolver.new_onehot_var(5);
  check(var);
}

TEST_P(SatOneHotVarTest, basic10)
{
  auto var = mSolver.new_onehot_var(10);
  check(var);
}

TEST_P(SatOneHotVarTest, eq2)
{
  auto a_var = mSolver.new_onehot_var(2);
  auto b_var = mSolver.new_onehot_var(2);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, eq3)
{
  auto a_var = mSolver.new_onehot_var(3);
  auto b_var = mSolver.new_onehot_var(3);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, eq5)
{
  auto a_var = mSolver.new_onehot_var(5);
  auto b_var = mSolver.new_onehot_var(5);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, eq10)
{
  auto a_var = mSolver.new_onehot_var(10);
  auto b_var = mSolver.new_onehot_var(10);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, eq20)
{
  auto a_var = mSolver.new_onehot_var(20);
  auto b_var = mSolver.new_onehot_var(20);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, eq30)
{
  auto a_var = mSolver.new_onehot_var(30);
  auto b_var = mSolver.new_onehot_var(30);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, eq100)
{
  auto a_var = mSolver.new_onehot_var(100);
  auto b_var = mSolver.new_onehot_var(100);
  check_add_eq(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne2)
{
  auto a_var = mSolver.new_onehot_var(2);
  auto b_var = mSolver.new_onehot_var(2);
  check_add_ne(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne3)
{
  auto a_var = mSolver.new_onehot_var(3);
  auto b_var = mSolver.new_onehot_var(3);
  check_add_ne(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne5)
{
  auto a_var = mSolver.new_onehot_var(5);
  auto b_var = mSolver.new_onehot_var(5);
  check_add_ne(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne10)
{
  auto a_var = mSolver.new_onehot_var(10);
  auto b_var = mSolver.new_onehot_var(10);
  check_add_ne(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne20)
{
  auto a_var = mSolver.new_onehot_var(20);
  auto b_var = mSolver.new_onehot_var(20);
  check_add_ne(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne30)
{
  auto a_var = mSolver.new_onehot_var(30);
  auto b_var = mSolver.new_onehot_var(30);
  check_add_ne(a_var, b_var);
}

TEST_P(SatOneHotVarTest, ne100)
{
  auto a_var = mSolver.new_onehot_var(100);
  auto b_var = mSolver.new_onehot_var(100);
  check_add_ne(a_var, b_var);
}

INSTANTIATE_TEST_SUITE_P(SatSolverTest,
			 SatOneHotVarTest,
			 ::testing::Values("lingeling", "glueminisat2", "minisat2", "minisat",
					   "ymsat1", "ymsat2", "ymsat1_old"));

END_NAMESPACE_YM
