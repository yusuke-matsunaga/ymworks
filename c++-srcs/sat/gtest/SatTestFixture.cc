
/// @file SatTestFixture.cc
/// @brief SatTestFixture の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "SatTestFixture.h"
#include "ym/SatModel.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_YM

SatTestFixture::SatTestFixture() :
  mSolver{GetParam()},
  mVarNum{200},
  mVarList(mVarNum),
  mCondVarList(2)
{
  for ( int i: Range(mVarNum) ) {
    mVarList[i] = mSolver.new_variable(true);
  }
  mCondVarList[0] = mSolver.new_variable(true);
  mCondVarList[1] = mSolver.new_variable(true);
}

// @brief 設定されたCNFが vals[] で示された真理値表と等しいか調べる．
void
SatTestFixture::check(
  int ni,
  const std::vector<int>& vals
)
{
  try {
    int np = 1U << ni;
    for ( int p: Range(np) ) {
      std::vector<SatLiteral> assumptions;
      for ( int i: Range(ni) ) {
	auto lit = mVarList[i];
	if ( (p & (1 << i)) == 0 ) {
	  lit = ~lit;
	}
	assumptions.push_back(lit);
      }
      // 条件変数はオフにしておく．
      for ( auto lit: mCondVarList ) {
	assumptions.push_back(~lit);
      }
      SatBool3 exp_ans = vals[p] ? SatBool3::True : SatBool3::False;
      SatBool3 stat = mSolver.solve(assumptions);
      std::ostringstream buf;
      for ( int i: Range(ni) ) {
	int val = (p >> i) & 1;
	buf << "lit#" << i << " = " << val << std::endl;
      }
      EXPECT_EQ( exp_ans, stat ) << buf.str();
    }
  }
  catch ( AssertError x ) {
    std::cout << x << std::endl;
  }
}

// @brief check の条件リテラル付き版
//
// 最初の変数が1のときのみ意味を持つ．
void
SatTestFixture::check_with_cond1(
  int ni,
  const std::vector<int>& vals
)
{
  try {
    int np = 1U << ni;
    for ( int p: Range(np) ) {
      std::vector<SatLiteral> assumptions;
      assumptions.push_back(~SatLiteral(mCondVarList[0]));
      for ( int i: Range(ni) ) {
	auto lit = mVarList[i];
	if ( (p & (1 << i)) == 0 ) {
	  lit = ~lit;
	}
	assumptions.push_back(lit);
      }
      SatBool3 exp_ans = SatBool3::True;
      SatBool3 stat = mSolver.solve(assumptions);
      EXPECT_EQ( exp_ans, stat );
    }
    for ( int p: Range(np) ) {
      std::vector<SatLiteral> assumptions;
      assumptions.push_back(SatLiteral(mCondVarList[0]));
      for ( int i: Range(ni) ) {
	auto lit = mVarList[i];
	if ( (p & (1 << i)) == 0 ) {
	  lit = ~lit;
	}
	assumptions.push_back(lit);
      }
      SatBool3 exp_ans = vals[p] ? SatBool3::True : SatBool3::False;
      SatBool3 stat = mSolver.solve(assumptions);
      EXPECT_EQ( exp_ans, stat );
    }
  }
  catch ( AssertError x ) {
    std::cout << x << std::endl;
  }
}

// @brief 論理ゲートの真理値表からチェック用のベクタを作る．
std::vector<int>
SatTestFixture::make_vals(
  int ni,
  const std::vector<int>& tv,
  bool inv
)
{
  int np = 1 << ni;
  int ni1 = ni + 1;
  int np1 = 1 << ni1;
  std::vector<int> vals(np * 2);
  int v0 = inv ? 1 : 0;
  int v1 = inv ? 0 : 1;
  for ( int p: Range(np) ) {
    vals[p +  0] = (tv[p] == v0);
    vals[p + np] = (tv[p] == v1);
  }
  return vals;
}

END_NAMESPACE_YM
