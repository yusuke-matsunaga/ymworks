
/// @file IsopTest.cc
/// @brief Tv2Sop のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

Bdd
cov2bdd(
  const SopCover& cover,
  BddMgr& mgr
)
{
  auto bdd = Bdd::zero();
  for ( auto& cube: cover.cube_list() ) {
    auto bdd1 = Bdd::one();
    for ( auto lit: cube.literal_list() ) {
      auto varid = lit.varid();
      auto lit_bdd = mgr.variable_bdd(varid);
      if ( lit.is_negative() ) {
	lit_bdd = ~lit_bdd;
      }
      bdd1 &= lit_bdd;
    }
    bdd |= bdd1;
  }
  return bdd;
}

END_NONAMESPACE

TEST(IsopTest, isop0)
{
  BddMgr mgr;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto var2 = mgr.variable_bdd(2);
  auto f = var0 & var1 | var0 & var2;
  auto d = ~(var1 | var2);

  auto cov = isop(f, d);

  auto cov_func = cov2bdd(cov, mgr);
  EXPECT_EQ( var0, cov_func );
}

TEST(IsopTest, isop0_1)
{
  BddMgr mgr;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto var2 = mgr.variable_bdd(2);
  auto f = var1 & var2 | var0 & var2;
  auto d = ~(var0 | var1);

  auto cov = isop(f, d);

  auto cov_func = cov2bdd(cov, mgr);
  EXPECT_EQ( var2, cov_func );
}

TEST(IsopTest, isop1)
{
  BddMgr mgr;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto var2 = mgr.variable_bdd(2);
  auto var3 = mgr.variable_bdd(3);
  auto func = var0 & var2 | var0 & var3 | var1 & var2 | var1 & var3;
  auto dc = ~(var2 | var3);

  auto cov = isop(func, dc);

  auto cov_func = cov2bdd(cov, mgr);

  EXPECT_TRUE( check_containment(cov_func, func & ~dc) );
  EXPECT_TRUE( check_containment(func | dc, cov_func) );
}

TEST(IsopTest, isop2)
{
  BddMgr mgr;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto var2 = mgr.variable_bdd(2);
  auto var3 = mgr.variable_bdd(3);
  auto func = var0 & var2 | var0 & var3 | var1 & var2 | var1 & var3;
  auto dc = ~(var0 | var1);

  auto cov = isop(func, dc);

  auto cov_func = cov2bdd(cov, mgr);
  EXPECT_TRUE( check_containment(cov_func, func & ~dc) );
  EXPECT_TRUE( check_containment(func | dc, cov_func) );
}

#if 0
TEST_P(TvFuncTestWithParam, isop)
{
  SizeType ni = GetParam();
  auto ni_exp = 1 << ni;

  // テストのサンプル数
  SizeType n;
  if ( ni < 1 ) {
    return;
  }
  else if ( ni <= 4 ) {
    n = (1 << ni_exp);
  }
  else if ( ni < 7 ) {
    n = 1000;
  }
  else if ( ni < 10 ) {
    n =100;
  }
  else {
    return;
  }

  auto values = vector<int>(ni_exp, 0);
  for ( SizeType c: Range(n) ) {
    for ( auto p: Range(ni_exp) ) {
      if ( ni <= 4 ) {
	if ( c & (1 << p) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
      else {
	if ( mRandDist(mRandGen) ) {
	  values[p] = 1;
	}
	else {
	  values[p] = 0;
	}
      }
    }
    auto func = TvFunc{ni, values};
    auto cov = Tv2Sop::isop(func);
    // TvFunc に再変換して func と等しいか調べる．
    // ヒューリスティックの簡単化なので正解はない．
    auto exp_func = TvFunc::cover(ni, cov);
    EXPECT_EQ( exp_func, func );
  }
}

INSTANTIATE_TEST_SUITE_P(Test0to20,
			 TvFuncTestWithParam,
			 ::testing::Range(0, 11));
#endif

END_NAMESPACE_YM
