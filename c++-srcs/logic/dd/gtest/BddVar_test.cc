
/// @file BddVar_test.cc
/// @brief BddVar_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BddVar.h"
#include "ym/BddLit.h"
#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"
#include "BddTest.h"


BEGIN_NAMESPACE_YM

TEST_F(BddTest, bddvar_invalid)
{
  auto var = BddVar::invalid();

  EXPECT_TRUE( var.is_invalid() );
  EXPECT_THROW( var.varid(),
		std::invalid_argument );
  EXPECT_THROW( var.positive_literal(),
		std::invalid_argument );
  EXPECT_THROW( var.negative_literal(),
		std::invalid_argument );
}

TEST_F(BddTest, bddvar_invalid2)
{
  auto var = BddVar{};

  EXPECT_TRUE( var.is_invalid() );
}

TEST_F(BddTest, bddvar_constr1)
{
  auto var = mMgr.variable(0);

  EXPECT_FALSE( var.is_invalid() );
  EXPECT_EQ( 0, var.varid() );

  std::ostringstream buf;
  buf << var;
  EXPECT_EQ( "v0", buf.str() );

}

TEST_F(BddTest, bddvar_variable1)
{
  SizeType id = 10;
  auto var = mMgr.variable(id);

  EXPECT_FALSE( var.is_invalid() );
  EXPECT_EQ( id, var.varid() );
}

TEST_F(BddTest, bddvar_positive_literal)
{
  auto var = mMgr.variable(0);
  EXPECT_FALSE( var.is_invalid() );

  auto lit = var.positive_literal();
  EXPECT_FALSE( lit.is_invalid() );
  EXPECT_TRUE( lit.is_positive() );
  EXPECT_FALSE( lit.is_negative() );

  auto var1 = lit.var();
  EXPECT_EQ( var, var1 );

  auto lit1 = lit.positive_literal();
  EXPECT_EQ( lit, lit1 );

  auto lit2 = lit.negative_literal();
  EXPECT_NE( lit, lit2 );

  auto bdd = lit.bdd();

  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
  EXPECT_FALSE( bdd.is_const() );
  EXPECT_FALSE( bdd.is_invalid() );

  check(bdd, "10");

  Bdd f0;
  Bdd f1;
  auto var2 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var2, var );
  EXPECT_TRUE( f0.is_zero() );
  EXPECT_TRUE( f1.is_one() );
}

TEST_F(BddTest, bddvar_negative_literal)
{
  auto var = mMgr.variable(0);
  EXPECT_FALSE( var.is_invalid() );

  auto lit = var.negative_literal();
  EXPECT_FALSE( lit.is_invalid() );
  EXPECT_FALSE( lit.is_positive() );
  EXPECT_TRUE( lit.is_negative() );

  auto var1 = lit.var();
  EXPECT_EQ( var, var1 );

  auto lit1 = lit.positive_literal();
  EXPECT_NE( lit, lit1 );

  auto lit2 = lit.negative_literal();
  EXPECT_EQ( lit, lit2 );

  auto bdd = lit.bdd();

  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
  EXPECT_FALSE( bdd.is_const() );
  EXPECT_FALSE( bdd.is_invalid() );

  check(bdd, "01");

  Bdd f0;
  Bdd f1;
  auto var2 = bdd.root_decomp(f0, f1);

  EXPECT_EQ( var2, var );
  EXPECT_TRUE( f0.is_one() );
  EXPECT_TRUE( f1.is_zero() );
}

TEST_F(BddTest, bddvar_copy)
{
  SizeType id = 5;
  auto var = mMgr.variable(id);

  auto var1 = BddVar{var};

  EXPECT_EQ( var, var1 );
}

TEST_F(BddTest, bddvar_copy_assignment)
{
  SizeType id = 5;
  auto var = mMgr.variable(id);

  BddVar var1;
  var1 = var;

  EXPECT_EQ( var, var1 );
}

TEST_F(BddTest, bddvar_copy_assignment2)
{
  SizeType ni = 5;
  auto var = mMgr.variable(ni - 1);

  std::vector<BddVar> var_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto var = mMgr.variable(i);
    var_list[i] = var;
  }
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto var = var_list[i];
    EXPECT_EQ( i, var.varid() );
  }
}

TEST_F(BddTest, bddvar_add)
{
  auto var1 = mMgr.variable(5);
  auto var2 = mMgr.variable(10);

  auto varset = var1 + var2;
  EXPECT_FALSE( varset.is_invalid() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
}

TEST_F(BddTest, bddvar_add_invalid1)
{ // var + var(invalid)
  auto var1 = BddVar::invalid();
  auto var2 = mMgr.variable(10);

  EXPECT_THROW( var1 + var2,
		std::invalid_argument );
}

TEST_F(BddTest, bddvar_add_invalid2)
{ // var(invalid) + var
  auto var1 = mMgr.variable(5);
  auto var2 = BddVar::invalid();

  EXPECT_THROW( var1 + var2,
		std::invalid_argument );
}

TEST_F(BddTest, bddvar_add_invalid3)
{ // var + var(another mgr)
  auto var1 = mMgr.variable(5);
  BddMgr mgr2;
  auto var2 = mgr2.variable(0);

  EXPECT_THROW( var1 + var2,
		std::invalid_argument );
}

END_NAMESPACE_YM
