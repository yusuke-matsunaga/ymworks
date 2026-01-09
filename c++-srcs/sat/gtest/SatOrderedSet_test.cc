
/// @file SatOrderedSetTest.cc
/// @brief SatOrderedSetTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/SatSolver.h"
#include "ym/SatModel.h"
#include "ym/SatOrderedSet.h"


BEGIN_NAMESPACE_YM_SAT

class SatOrderedSetTest :
public ::testing::TestWithParam<std::string>
{
public:

  /// @brief コンストラクタ
  SatOrderedSetTest();

  /// @brief デストラクタ
  ~SatOrderedSetTest();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 単体テスト
  void
  check_basic(
    const SatOrderedSet& var
  );

  /// @brief add_lt() のテスト
  void
  check_add_lt(
    const SatOrderedSet& a_var,
    int b_val
  );

  /// @brief add_le() のテスト
  void
  check_add_le(
    const SatOrderedSet& a_var,
    int b_val
  );

  /// @brief add_gt() のテスト
  void
  check_add_gt(
    const SatOrderedSet& a_var,
    int b_val
  );

  /// @brief add_ge() のテスト
  void
  check_add_ge(
    const SatOrderedSet& a_var,
    int b_val
  );

  /// @brief テストの共通部分
  void
  check_common(
    const SatOrderedSet& a_var,
    int b_val,
    std::function<bool(int, int)> cmp_func
  );

  /// @brief add_eq() のテスト
  void
  check_add_eq(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var
  );

  /// @brief add_nq() のテスト
  void
  check_add_ne(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var
  );

  /// @brief add_lt() のテスト
  void
  check_add_lt(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var
  );

  /// @brief add_le() のテスト
  void
  check_add_le(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var
  );

  /// @brief add_gt() のテスト
  void
  check_add_gt(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var
  );

  /// @brief add_ge() のテスト
  void
  check_add_ge(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var
  );

  /// @brief テストの共通部分
  void
  check_common(
    const SatOrderedSet& a_var,
    const SatOrderedSet& b_var,
    std::function<bool(int, int)> cmp_func
  );

  /// @brief add_eq() のテスト
  void
  check_add_eq(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var
  );

  /// @brief add_nq() のテスト
  void
  check_add_ne(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var
  );

  /// @brief add_lt() のテスト
  void
  check_add_lt(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var
  );

  /// @brief add_le() のテスト
  void
  check_add_le(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var
  );

  /// @brief add_gt() のテスト
  void
  check_add_gt(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var
  );

  /// @brief add_ge() のテスト
  void
  check_add_ge(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var
  );

  /// @brief テストの共通部分
  void
  check_common(
    const std::vector<SatOrderedSet>& a_var,
    const std::vector<SatOrderedSet>& b_var,
    std::function<bool(int, int, int, int)> cmp_func
  );

  // SATソルバ
  SatSolver mSolver;

};

// @brief コンストラクタ
SatOrderedSetTest::SatOrderedSetTest() :
  mSolver(GetParam())
{
}

// @brief デストラクタ
SatOrderedSetTest::~SatOrderedSetTest()
{
}

// @brief 単体テスト
void
SatOrderedSetTest::check_basic(
  const SatOrderedSet& var
)
{
  auto& lits = var.vars();
  SizeType n = lits.size();

  SizeType nexp = (1 << n);
  for ( SizeType bits = 0; bits < nexp; ++ bits ) {
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(n);
    int c = 0;
    for ( int i = 0; i < n; ++ i ) {
      auto lit = lits[i];
      if ( bits & (1 << i) ) {
	assumptions.push_back(lit);
	++ c;
      }
      else {
	assumptions.push_back(~lit);
      }
    }
    auto exp_ans = (c == 1) ? SatBool3::True : SatBool3::False;
    auto ans = mSolver.solve(assumptions);
    EXPECT_EQ( exp_ans, ans );
  }
}

// @brief add_lt() のテスト
void
SatOrderedSetTest::check_add_lt(
  const SatOrderedSet& a_var,
  int b_val
)
{
  mSolver.add_lt(a_var, b_val);
  check_common(a_var, b_val,
	       [](int val1, int val2) -> bool
	       {
		 return val1 < val2;
	       });
}

// @brief add_le() のテスト
void
SatOrderedSetTest::check_add_le(
  const SatOrderedSet& a_var,
  int b_val
)
{
  mSolver.add_le(a_var, b_val);
  check_common(a_var, b_val,
	       [](int val1, int val2) -> bool
	       {
		 return val1 <= val2;
	       });
}

// @brief add_gt() のテスト
void
SatOrderedSetTest::check_add_gt(
  const SatOrderedSet& a_var,
  int b_val
)
{
  mSolver.add_gt(a_var, b_val);
  check_common(a_var, b_val,
	       [](int val1, int val2) -> bool
	       {
		 return val1 > val2;
	       });
}

// @brief add_ge() のテスト
void
SatOrderedSetTest::check_add_ge(
  const SatOrderedSet& a_var,
  int b_val
)
{
  mSolver.add_ge(a_var, b_val);
  check_common(a_var, b_val,
	       [](int val1, int val2) -> bool
	       {
		 return val1 >= val2;
	       });
}

// @brief テストの共通部分
void
SatOrderedSetTest::check_common(
  const SatOrderedSet& a_var,
  int b_val,
  std::function<bool(int, int)> cmp_func
)
{
  auto& lits = a_var.vars();
  SizeType n = lits.size();
  for ( SizeType i = 0; i < n; ++ i ) {
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(n);
    int val = a_var.min() + i;
    for ( int j = 0; j < n; ++ j ) {
      auto lit = lits[j];
      if ( j == i ) {
	assumptions.push_back(lit);
      }
      else {
	assumptions.push_back(~lit);
      }
    }
    auto exp_ans = cmp_func(val, b_val) ? SatBool3::True : SatBool3::False;
    auto ans = mSolver.solve(assumptions);
    EXPECT_EQ( exp_ans, ans );
  }
}

// @brief add_eq() のテスト
void
SatOrderedSetTest::check_add_eq(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var
)
{
  mSolver.add_eq(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 == val2;
	       });
}

// @brief add_nq() のテスト
void
SatOrderedSetTest::check_add_ne(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var
)
{
  mSolver.add_ne(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 != val2;
	       });
}

// @brief add_lt() のテスト
void
SatOrderedSetTest::check_add_lt(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var
)
{
  mSolver.add_lt(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 < val2;
	       });
}

// @brief add_le() のテスト
void
SatOrderedSetTest::check_add_le(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var
)
{
  mSolver.add_le(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 <= val2;
	       });
}

// @brief add_gt() のテスト
void
SatOrderedSetTest::check_add_gt(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var
)
{
  mSolver.add_gt(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 > val2;
	       });
}

// @brief add_ge() のテスト
void
SatOrderedSetTest::check_add_ge(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var
)
{
  mSolver.add_ge(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1, int val2) -> bool
	       {
		 return val1 >= val2;
	       });
}

// @brief テストの共通部分
void
SatOrderedSetTest::check_common(
  const SatOrderedSet& a_var,
  const SatOrderedSet& b_var,
  std::function<bool(int, int)> cmp_func
)
{
  auto& a_lits = a_var.vars();
  auto& b_lits = b_var.vars();

  auto n1 = a_lits.size();
  auto n2 = b_lits.size();
  for ( SizeType i1 = 0; i1 < n1; ++ i1 ) {
    for ( SizeType i2 = 0; i2 < n2; ++ i2 ) {
      std::vector<SatLiteral> assumptions;
      assumptions.reserve(n1 + n2);
      int val1 = a_var.min() + i1;
      int val2 = b_var.min() + i2;
      for ( int i = 0; i < n1; ++ i ) {
	auto lit = a_lits[i];
	if ( i == i1 ) {
	  assumptions.push_back(lit);
	}
	else {
	  assumptions.push_back(~lit);
	}
      }
      for ( int i = 0; i < n2; ++ i ) {
	auto lit = b_lits[i];
	if ( i == i2 ) {
	  assumptions.push_back(lit);
	}
	else {
	  assumptions.push_back(~lit);
	}
      }
      std::ostringstream buf;
      buf << "val1 = " << val1
	  << ", val2 = " << val2;
      auto exp_ans = cmp_func(val1, val2) ? SatBool3::True : SatBool3::False;
      auto ans = mSolver.solve(assumptions);
      EXPECT_EQ( exp_ans, ans ) << buf.str();
    }
  }
}

// @brief add_eq() のテスト
void
SatOrderedSetTest::check_add_eq(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var
)
{
  mSolver.add_eq(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1_0, int val1_1,
		  int val2_0, int val2_1) -> bool
	       {
		 return val1_0 == val2_0 && val1_1 == val2_1;
	       });
}

// @brief add_nq() のテスト
void
SatOrderedSetTest::check_add_ne(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var
)
{
  mSolver.add_ne(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1_0, int val1_1,
		  int val2_0, int val2_1) -> bool
	       {
		 return val1_0 != val2_0 || val1_1 != val2_1;
	       });
}

// @brief add_lt() のテスト
void
SatOrderedSetTest::check_add_lt(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var
)
{
  mSolver.add_lt(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1_0, int val1_1,
		  int val2_0, int val2_1) -> bool
	       {
		 if ( val1_0 < val2_0 ) {
		   return true;
		 }
		 if ( val1_0 > val2_0 ) {
		   return false;
		 }
		 return val1_1 < val2_1;
	       });
}

// @brief add_le() のテスト
void
SatOrderedSetTest::check_add_le(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var
)
{
  mSolver.add_le(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1_0, int val1_1,
		  int val2_0, int val2_1) -> bool
	       {
		 if ( val1_0 < val2_0 ) {
		   return true;
		 }
		 if ( val1_0 > val2_0 ) {
		   return false;
		 }
		 return val1_1 <= val2_1;
	       });
}

// @brief add_gt() のテスト
void
SatOrderedSetTest::check_add_gt(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var
)
{
  mSolver.add_gt(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1_0, int val1_1,
		  int val2_0, int val2_1) -> bool
	       {
		 if ( val1_0 > val2_0 ) {
		   return true;
		 }
		 if ( val1_0 < val2_0 ) {
		   return false;
		 }
		 return val1_1 > val2_1;
	       });
}

// @brief add_ge() のテスト
void
SatOrderedSetTest::check_add_ge(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var
)
{
  mSolver.add_ge(a_var, b_var);
  check_common(a_var, b_var,
	       [](int val1_0, int val1_1,
		  int val2_0, int val2_1) -> bool
	       {
		 if ( val1_0 > val2_0 ) {
		   return true;
		 }
		 if ( val1_0 < val2_0 ) {
		   return false;
		 }
		 return val1_1 >= val2_1;
	       });
}

// @brief テストの共通部分
void
SatOrderedSetTest::check_common(
  const std::vector<SatOrderedSet>& a_var,
  const std::vector<SatOrderedSet>& b_var,
  std::function<bool(int, int, int, int)> cmp_func
)
{
  auto& a0_lits = a_var[0].vars();
  auto& a1_lits = a_var[1].vars();
  auto& b0_lits = b_var[0].vars();
  auto& b1_lits = b_var[1].vars();

  auto n0 = a0_lits.size();
  auto n1 = a1_lits.size();
  for ( SizeType i1_0 = 0; i1_0 < n0; ++ i1_0 ) {
    for ( SizeType i1_1 = 0; i1_1 < n1; ++ i1_1 ) {
      for ( SizeType i2_0 = 0; i2_0 < n0; ++ i2_0 ) {
	for ( SizeType i2_1 = 0; i2_1 < n1; ++ i2_1 ) {
	  std::vector<SatLiteral> assumptions;
	  assumptions.reserve((n0 + n1) * 2);
	  int val1_0 = a_var[0].min() + i1_0;
	  int val1_1 = a_var[1].min() + i1_1;
	  int val2_0 = b_var[0].min() + i2_0;
	  int val2_1 = b_var[1].min() + i2_1;
	  for ( int i = 0; i < n0; ++ i ) {
	    auto lit = a0_lits[i];
	    if ( i == i1_0 ) {
	      assumptions.push_back(lit);
	    }
	    else {
	      assumptions.push_back(~lit);
	    }
	  }
	  for ( int i = 0; i < n1; ++ i ) {
	    auto lit = a1_lits[i];
	    if ( i == i1_1 ) {
	      assumptions.push_back(lit);
	    }
	    else {
	      assumptions.push_back(~lit);
	    }
	  }
	  for ( int i = 0; i < n0; ++ i ) {
	    auto lit = b0_lits[i];
	    if ( i == i2_0 ) {
	      assumptions.push_back(lit);
	    }
	    else {
	      assumptions.push_back(~lit);
	    }
	  }
	  for ( int i = 0; i < n1; ++ i ) {
	    auto lit = b1_lits[i];
	    if ( i == i2_1 ) {
	      assumptions.push_back(lit);
	    }
	    else {
	      assumptions.push_back(~lit);
	    }
	  }
	  std::ostringstream buf;
	  buf << "val1 = " << val1_0 << "." << val1_1
	      << ", val2 = " << val2_0 << "." << val2_1;
	  auto exp_ans = cmp_func(val1_0, val1_1,
				  val2_0, val2_1) ? SatBool3::True : SatBool3::False;
	  auto ans = mSolver.solve(assumptions);
	  EXPECT_EQ( exp_ans, ans ) << buf.str();
	}
      }
    }
  }
}

TEST_P(SatOrderedSetTest, basic1)
{
  int min = 0;
  int max = 5;
  auto var = mSolver.new_ordered_set(min, max);

  check_basic(var);
}

TEST_P(SatOrderedSetTest, basic2)
{
  int min = 3;
  int max = 3;
  auto var = mSolver.new_ordered_set(min, max);

  check_basic(var);
}

TEST_P(SatOrderedSetTest, lt1)
{
  int min = 3;
  int max = 10;
  auto var = mSolver.new_ordered_set(min, max);

  int uval = 5;
  check_add_lt(var, uval);
}

TEST_P(SatOrderedSetTest, le1)
{
  int min = 3;
  int max = 10;
  auto var = mSolver.new_ordered_set(min, max);

  int uval = 5;
  check_add_le(var, uval);
}

TEST_P(SatOrderedSetTest, gt1)
{
  int min = 3;
  int max = 10;
  auto var = mSolver.new_ordered_set(min, max);

  int lval = 7;
  check_add_gt(var, lval);
}

TEST_P(SatOrderedSetTest, ge1)
{
  int min = 3;
  int max = 10;
  auto var = mSolver.new_ordered_set(min, max);

  int lval = 7;
  check_add_ge(var, lval);
}

TEST_P(SatOrderedSetTest, eq2_ll)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_le)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_lg)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_el)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_ee)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_eg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_gl)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_ge)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, eq2_gg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_ll)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_le)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_lg)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_el)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_ee)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_eg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_gl)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_ge)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_ne(var1, var2);
}

TEST_P(SatOrderedSetTest, ne2_gg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_eq(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_ll)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_le)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_lg)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_el)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_ee)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_eg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_gl)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_ge)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_gg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_ok)
{
  int min1 = 2;
  int max1 = 4;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 5;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, lt2_ng)
{
  int min1 = 5;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 4;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_ll)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_le)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_lg)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_el)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_ee)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_eg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_gl)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_ge)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_gg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_le(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_ok)
{
  int min1 = 2;
  int max1 = 4;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 5;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, le2_ng)
{
  int min1 = 5;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 4;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_ll)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_le)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_lg)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_el)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_ee)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_eg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_gl)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_ge)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_gg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_ok)
{
  int min1 = 5;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 4;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, gt2_ng)
{
  int min1 = 2;
  int max1 = 4;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 5;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_ll)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_le)
{
  int min1 = 2;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_lg)
{
  int min1 = 2;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_el)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_ee)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_eg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 3;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_gl)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_ge)
{
  int min1 = 3;
  int max1 = 7;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_gg)
{
  int min1 = 3;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 7;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_gt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_ok)
{
  int min1 = 5;
  int max1 = 8;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 2;
  int max2 = 4;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, ge2_ng)
{
  int min1 = 2;
  int max1 = 4;
  auto var1 = mSolver.new_ordered_set(min1, max1);

  int min2 = 5;
  int max2 = 8;
  auto var2 = mSolver.new_ordered_set(min2, max2);

  check_add_lt(var1, var2);
}

TEST_P(SatOrderedSetTest, droppoff1)
{
  int min = 3;
  int max = 10;
  auto var = mSolver.new_ordered_set(min, max);

  int uval = 5;
  int lval = 7;
  mSolver.add_dropoff(var, uval, lval);

  int n = max - min + 1;
  std::vector<SatLiteral> lits(n);
  for ( int i = 0; i < n; ++ i ) {
    lits[i] = var.var(i + min);
  }

  int nexp = (1 << n);
  for ( int bits = 0; bits < nexp; ++ bits ) {
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(n);
    int c = 0;
    int val = -1;
    for ( int i = 0; i < n; ++ i ) {
      auto lit{lits[i]};
      if ( bits & (1 << i) ) {
	assumptions.push_back(lit);
	++ c;
	val = min + i;
      }
      else {
	assumptions.push_back(~lit);
      }
    }
    auto exp_ans = (c == 1) ? SatBool3::True : SatBool3::False;
    if ( uval < val && val < lval ) {
      exp_ans = SatBool3::False;
    }
    auto ans = mSolver.solve(assumptions);
    EXPECT_EQ( exp_ans, ans );
  }
}

TEST_P(SatOrderedSetTest, eq3)
{
  int min1 = 3;
  int max1 = 5;
  auto var1 = mSolver.new_ordered_set(min1, max1);
  auto var2 = mSolver.new_ordered_set(min1, max1);
  auto var3 = mSolver.new_ordered_set(min1, max1);
  auto var4 = mSolver.new_ordered_set(min1, max1);

  check_add_eq({var1, var2}, {var3, var4});
}

TEST_P(SatOrderedSetTest, ne3)
{
  int min1 = 3;
  int max1 = 5;
  auto var1 = mSolver.new_ordered_set(min1, max1);
  auto var2 = mSolver.new_ordered_set(min1, max1);
  auto var3 = mSolver.new_ordered_set(min1, max1);
  auto var4 = mSolver.new_ordered_set(min1, max1);

  check_add_ne({var1, var2}, {var3, var4});
}

TEST_P(SatOrderedSetTest, lt3)
{
  int min1 = 3;
  int max1 = 5;
  auto var1 = mSolver.new_ordered_set(min1, max1);
  auto var2 = mSolver.new_ordered_set(min1, max1);
  auto var3 = mSolver.new_ordered_set(min1, max1);
  auto var4 = mSolver.new_ordered_set(min1, max1);

  check_add_lt({var1, var2}, {var3, var4});
}

TEST_P(SatOrderedSetTest, le3)
{
  int min1 = 3;
  int max1 = 5;
  auto var1 = mSolver.new_ordered_set(min1, max1);
  auto var2 = mSolver.new_ordered_set(min1, max1);
  auto var3 = mSolver.new_ordered_set(min1, max1);
  auto var4 = mSolver.new_ordered_set(min1, max1);

  check_add_le({var1, var2}, {var3, var4});
}

TEST_P(SatOrderedSetTest, gt3)
{
  int min1 = 3;
  int max1 = 5;
  auto var1 = mSolver.new_ordered_set(min1, max1);
  auto var2 = mSolver.new_ordered_set(min1, max1);
  auto var3 = mSolver.new_ordered_set(min1, max1);
  auto var4 = mSolver.new_ordered_set(min1, max1);

  check_add_gt({var1, var2}, {var3, var4});
}

TEST_P(SatOrderedSetTest, ge3)
{
  int min1 = 3;
  int max1 = 5;
  auto var1 = mSolver.new_ordered_set(min1, max1);
  auto var2 = mSolver.new_ordered_set(min1, max1);
  auto var3 = mSolver.new_ordered_set(min1, max1);
  auto var4 = mSolver.new_ordered_set(min1, max1);

  check_add_ge({var1, var2}, {var3, var4});
}

INSTANTIATE_TEST_SUITE_P(SatSolverTest,
			 SatOrderedSetTest,
			 ::testing::Values("lingeling", "glueminisat2", "minisat2", "minisat",
					   "ymsat1", "ymsat2", "ymsat1_old"));

END_NAMESPACE_YM_SAT
