
/// @file add_aig_test.cc
/// @brief add_aig_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SatSolver.h"
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

class SatSolverTest:
  public ::testing::Test
{
public:

  void
  check();

  AigMgr mMgr;

  SatSolver mSolver;

};

void
SatSolverTest::check()
{
  auto ni = mMgr.input_num();
  auto input_lits = std::vector<SatLiteral>(ni);
  auto lit_map = std::unordered_map<SizeType, SatLiteral>{};
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto aig = mMgr.input(i);
    auto lit = mSolver.new_variable(true);
    input_lits[i] = lit;
    lit_map.emplace(mMgr.input_id(aig.node_id()), lit);
  }
  ASSERT_EQ( 1, mMgr.output_num() );
  auto olits_list = mSolver.add_aig(mMgr, lit_map);
  ASSERT_EQ( 1, olits_list.size() );
  auto olits = olits_list.front();

  auto ni_exp = 1U << ni;
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto input_vals = std::vector<AigBitVect>(ni, 0);
    for ( SizeType i = 0; i < ni; ++ i ) {
      if ( b & (1 << i) ) {
	input_vals[i] = 0xFFFFFFFFFFFFFFFFL;
      }
    }
    auto output_vals = mMgr.simulate(input_vals);
    auto oval = output_vals[0];
    std::vector<SatLiteral> assumptions;
    assumptions.reserve(ni + olits.size());
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto ilit = input_lits[i];
      if ( (b & (1 << i)) == 0 ) {
	ilit = ~ilit;
      }
      assumptions.push_back(ilit);
    }
    for ( auto olit: olits ) {
      assumptions.push_back(olit);
    }
    auto res = mSolver.solve(assumptions);
    auto exp_res = (oval == 0) ? SatBool3::False : SatBool3::True;
    EXPECT_EQ( exp_res, res );
  }
}

TEST_F(SatSolverTest, add_aig_const0)
{
  auto aig = AigLiteral::zero();
  mMgr.new_output(aig);
  auto lit_map = std::unordered_map<SizeType, SatLiteral>{};

  EXPECT_THROW( mSolver.add_aig(mMgr, lit_map), std::logic_error );
}

TEST_F(SatSolverTest, add_aig_const1)
{
  auto aig = AigLiteral::one();
  mMgr.new_output(aig);
  auto lit_map = std::unordered_map<SizeType, SatLiteral>{};

  auto lits_list = mSolver.add_aig(mMgr, lit_map);
  ASSERT_EQ( 1, lits_list.size() );
  auto lits = lits_list.front();
  EXPECT_TRUE( lits.empty() );
}

TEST_F(SatSolverTest, add_aig_lit1)
{
  auto aig = mMgr.new_input();
  mMgr.new_output(aig);

  check();
}

TEST_F(SatSolverTest, add_aig_lit2)
{
  auto aig = mMgr.new_input();
  mMgr.new_output(~aig);

  check();
}

TEST_F(SatSolverTest, add_aig_and1)
{
  auto aig_lit0 = mMgr.new_input();
  auto aig_lit1 = mMgr.new_input();
  auto aig = mMgr.new_and({aig_lit0, aig_lit1});
  mMgr.new_output(aig);

  check();
}

TEST_F(SatSolverTest, add_aig_or1)
{
  auto aig_lit0 = mMgr.new_input();
  auto aig_lit1 = mMgr.new_input();
  auto aig = mMgr.new_or({aig_lit0, aig_lit1});
  mMgr.new_output(aig);

  check();
}

TEST_F(SatSolverTest, add_aig_xor1)
{
  auto aig_lit0 = mMgr.new_input();
  auto aig_lit1 = mMgr.new_input();
  auto aig = mMgr.new_xor({aig_lit0, aig_lit1});
  mMgr.new_output(aig);

  check();
}

TEST_F(SatSolverTest, add_aig_andor1)
{
  auto aig_lit0 = mMgr.new_input();
  auto aig_lit1 = mMgr.new_input();
  auto aig_lit2 = mMgr.new_input();
  auto aig_lit3 = mMgr.new_input();
  auto e1 = mMgr.new_and({aig_lit0, aig_lit1});
  auto e2 = mMgr.new_and({aig_lit2, aig_lit3});
  auto aig = mMgr.new_or({e1, e2});
  mMgr.new_output(aig);

  check();
}

END_NAMESPACE_YM
