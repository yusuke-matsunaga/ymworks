
/// @file BddDecMgr_test.cc
/// @brief BddDecMgr_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include <random>
#include "BddDecMgr.h"
#include "BddDecEdge.h"
#include "ym/BddMgr.h"
#include "ym/DjDec.h"
#include "ym/BcGraph.h"
#include "ym/TvFunc.h"
#include "ym/Tv2Bdd.h"


BEGIN_NAMESPACE_YM_DJDEC

TEST(BddDecMgrTest, test1)
{
  SizeType ni = 3;
  SizeType ni_exp = 1 << ni;
  SizeType ni_exp_exp = 1 << ni_exp;
  for ( SizeType pat = 0; pat < ni_exp_exp; ++ pat ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType index = 0; index < ni_exp; ++ index ) {
      if ( pat & (1 << index) ) {
	val_list[index] = 1;
      }
      else {
	val_list[index] = 0;
      }
    }
    auto tv = TvFunc(ni, val_list);
    BddMgr bdd_mgr;
    auto bdd = Tv2Bdd::gen_bdd(tv, bdd_mgr);
    BddDecMgr mgr(bdd);
    auto r = mgr.decompose();
    EXPECT_EQ( mgr.bdd(), r.global_func() );
  }
}

TEST(BddDecMgrTest, test2)
{
  SizeType ni = 4;
  SizeType ni_exp = 1 << ni;
  SizeType ni_exp_exp = 1 << ni_exp;
  for ( SizeType pat = 0; pat < ni_exp_exp; ++ pat ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType index = 0; index < ni_exp; ++ index ) {
      if ( pat & (1 << index) ) {
	val_list[index] = 1;
      }
      else {
	val_list[index] = 0;
      }
    }
    auto tv = TvFunc(ni, val_list);
    BddMgr bdd_mgr;
    auto bdd = Tv2Bdd::gen_bdd(tv, bdd_mgr);
    BddDecMgr mgr(bdd);
    auto r = mgr.decompose();
    EXPECT_EQ( mgr.bdd(), r.global_func() );
  }
}

TEST(BddDecMgrTest, test3)
{
  SizeType ni = 5;
  SizeType ni_exp = 1 << ni;
  SizeType nsample = 10000;
  std::mt19937 rand_gen;
  std::uniform_int_distribution<int> rand_dist(0, 1);
  for ( SizeType _ = 0; _ < nsample; ++ _ ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( rand_dist(rand_gen) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    auto tv = TvFunc(ni, val_list);
    BddMgr bdd_mgr;
    auto bdd = Tv2Bdd::gen_bdd(tv, bdd_mgr);
    BddDecMgr mgr(bdd);
    auto r = mgr.decompose();
    EXPECT_EQ( mgr.bdd(), r.global_func() );
  }
}

TEST(BddDecMgrTest, test4)
{
  SizeType ni = 6;
  SizeType ni_exp = 1 << ni;
  SizeType nsample = 10000;
  std::mt19937 rand_gen;
  std::uniform_int_distribution<int> rand_dist(0, 1);
  for ( SizeType _ = 0; _ < nsample; ++ _ ) {
    std::vector<int> val_list(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( rand_dist(rand_gen) ) {
	val_list[b] = 1;
      }
      else {
	val_list[b] = 0;
      }
    }
    auto tv = TvFunc(ni, val_list);
    BddMgr bdd_mgr;
    auto bdd = Tv2Bdd::gen_bdd(tv, bdd_mgr);
    BddDecMgr mgr(bdd);
    auto r = mgr.decompose();
    EXPECT_EQ( mgr.bdd(), r.global_func() );
  }
}

END_NAMESPACE_YM_DJDEC
