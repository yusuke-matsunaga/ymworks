
/// @file BddVarSet_test.cc
/// @brief BddVarSet のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddVarSet.h"
#include "ym/BddMgr.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"
#include "BddTest.h"


BEGIN_NAMESPACE_YM

TEST(BddVarSetTest, invalid)
{
  // 不正値になっているはず．
  auto varset = BddVarSet::invalid();

  EXPECT_TRUE( varset.is_invalid() );
  EXPECT_THROW( varset.size(),
		std::invalid_argument );
  EXPECT_THROW( varset.var_list(),
		std::invalid_argument );

  auto mgr = varset.mgr();
  EXPECT_FALSE( mgr.is_valid() );
}

TEST(BddVarSetTest, empty_constr)
{
  // 空集合になっているはず
  auto varset = BddVarSet{};

  EXPECT_TRUE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 0, varset.size() );
  auto var_list = varset.var_list();
  EXPECT_TRUE( var_list.empty() );

  auto mgr = varset.mgr();
  EXPECT_FALSE( mgr.is_valid() );
}

TEST(BddVarSetTest, empty_set)
{
  auto varset = BddVarSet::empty_set();

  EXPECT_TRUE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 0, varset.size() );
  auto var_list = varset.var_list();
  EXPECT_TRUE( var_list.empty() );

  auto mgr = varset.mgr();
  EXPECT_FALSE( mgr.is_valid() );
}

TEST(BddVarSetTest, constr1)
{
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(2);
  auto varset = BddVarSet({var1, var2});

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 2, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
}

TEST(BddVarSetTest, constr_invalid1)
{
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  BddMgr mgr2;
  auto var2 = mgr2.variable(2);
  EXPECT_THROW( BddVarSet({var1, var2}),
		std::invalid_argument );
}

TEST(BddVarSetTest, copy_constructor)
{
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(2);
  auto varset = BddVarSet({var1, var2});

  auto varset1 = BddVarSet{varset};

  EXPECT_FALSE( varset1.is_empty_set() );
  EXPECT_FALSE( varset1.is_invalid() );

  EXPECT_EQ( 2, varset1.size() );
  auto var_list = varset1.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
}

TEST(BddVarSetTest, copy_assignment)
{
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(2);
  auto varset = BddVarSet({var1, var2});

  BddVarSet varset1;
  varset1 = varset;

  EXPECT_FALSE( varset1.is_empty_set() );
  EXPECT_FALSE( varset1.is_invalid() );

  EXPECT_EQ( 2, varset1.size() );
  auto var_list = varset1.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
}

TEST(BddVarSetTest, add1)
{ // var + var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset = var1 + var2;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 2, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
}

TEST(BddVarSetTest, add2)
{ // var + varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var2, var3, var4});
  auto varset = var1 + varset1;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 4, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 4, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
  EXPECT_EQ( var4, var_list[3] );
}

TEST(BddVarSetTest, add3)
{ // varset + var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3});
  auto varset = varset1 + var4;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 4, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 4, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
  EXPECT_EQ( var4, var_list[3] );
}

TEST(BddVarSetTest, add4)
{ // varset + varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var4});
  auto varset2 = BddVarSet({var2, var3});
  auto varset = varset1 + varset2;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 4, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 4, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
  EXPECT_EQ( var4, var_list[3] );
}

TEST(BddVarSetTest, add_invalid1)
{ // varset + var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 + var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid2)
{ // varset(invalid) + var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 + var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid3)
{ // varset + var(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(5);

  EXPECT_THROW( varset1 + var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid4)
{ // var(invalid) + varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( var5 + varset1,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid5)
{ // var + varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( var1 + varset1,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid6)
{ // var + varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(0);

  EXPECT_THROW( var5 + varset1,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid7)
{ // varset(invalid) + varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 + varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid8)
{ // varset + varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 + varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_invalid9)
{ // varset + varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);
  auto var6 = mgr2.variable(3);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 + varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_int3)
{ // varset += var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset = BddVarSet({var1, var2, var3});
  auto varset1 = varset += var4;

  EXPECT_EQ( varset1, varset );
  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 4, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 4, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
  EXPECT_EQ( var4, var_list[3] );
}

TEST(BddVarSetTest, add_int4)
{ // varset += varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset = BddVarSet({var1, var4});
  auto varset2 = BddVarSet({var2, var3});
  auto varset1 = varset += varset2;

  EXPECT_EQ( varset, varset1 );
  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 4, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 4, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
  EXPECT_EQ( var4, var_list[3] );
}

TEST(BddVarSetTest, add_int_invalid1)
{ // varset += var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 += var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_int_invalid2)
{ // varset(invalid) += var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 += var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_int_invalid3)
{ // varset += var(anoher mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);

  EXPECT_THROW( varset1 += var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_int_invalid4)
{ // varset(invalid) + varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 += varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_int_invalid5)
{ // varset + varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 += varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, add_int_invalid6)
{ // varset + varset(another mbr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);
  auto var6 = mgr2.variable(3);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 += varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff3)
{ // varset - var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3});
  auto varset = varset1 - var4;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 3, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 3, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
}

TEST(BddVarSetTest, diff4)
{ // varset - varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var4});
  auto varset2 = BddVarSet({var2, var3});
  auto varset = varset1 - varset2;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 2, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var4, var_list[1] );
}

TEST(BddVarSetTest, diff_invalid1)
{ // varset - var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 - var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_invalid2)
{ // varset(invalid) - var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 - var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_invalid3)
{ // varset - var(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet::invalid();
  BddMgr mgr2;
  auto var5 = mgr2.variable(1);

  EXPECT_THROW( varset1 - var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_invalid4)
{ // varset(invalid) - varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 - varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_invalid5)
{ // varset - varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 - varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_invalid6)
{ // varset - varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(3);
  auto var6 = mgr2.variable(4);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 - varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_int3)
{ // varset -= var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3});
  auto varset = varset1 -= var4;

  EXPECT_EQ( varset, varset1 );
  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 3, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 3, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var2, var_list[1] );
  EXPECT_EQ( var3, var_list[2] );
}

TEST(BddVarSetTest, diff_int4)
{ // varset -= varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var4});
  auto varset2 = BddVarSet({var2, var3});
  auto varset = varset1 -= varset2;

  EXPECT_EQ( varset, varset1 );
  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 2, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 2, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
  EXPECT_EQ( var4, var_list[1] );
}

TEST(BddVarSetTest, diff_int_invalid1)
{ // varset -= var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 -= var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_int_invalid2)
{ // varset(invalid) -= var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 -= var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_int_invalid3)
{ // varset -= var(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(1);

  EXPECT_THROW( varset1 -= var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_int_invalid4)
{ // varset(invalid) -= varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 -= varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_int_invalid5)
{ // varset -= varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 -= varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, diff_int_invalid6)
{ // varset -= varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(0);
  auto var6 = mgr2.variable(1);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 -= varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap3)
{ // varset & var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3});
  auto varset = varset1 & var4;

  EXPECT_TRUE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 0, varset.size() );
  auto var_list = varset.var_list();
  EXPECT_TRUE( var_list.empty() );
}

TEST(BddVarSetTest, cap4)
{ // varset & varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var4});
  auto varset2 = BddVarSet({var1, var3});
  auto varset = varset1 & varset2;

  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 1, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 1, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
}

TEST(BddVarSetTest, cap_invalid1)
{ // varset & var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 & var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_invalid2)
{ // varset(invalid) & var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 & var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_invalid3)
{ // varset & var(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);

  EXPECT_THROW( varset1 & var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_invalid4)
{ // varset(invalid) & varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 & varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_invalid5)
{ // varset & varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 & varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_invalid6)
{ // varset & varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);
  auto var6 = mgr2.variable(3);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 & varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_int3)
{ // varset &= var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3});
  auto varset = varset1 &= var4;

  EXPECT_EQ( varset, varset1 );
  EXPECT_TRUE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 0, varset.size() );
  auto var_list = varset.var_list();
  EXPECT_TRUE( var_list.empty() );
}

TEST(BddVarSetTest, cap_int4)
{ // varset &= varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var4});
  auto varset2 = BddVarSet({var1, var3});
  auto varset = varset1 &= varset2;

  EXPECT_EQ( varset, varset1 );
  EXPECT_FALSE( varset.is_empty_set() );
  EXPECT_FALSE( varset.is_invalid() );

  EXPECT_EQ( 1, varset.size() );
  auto var_list = varset.var_list();
  ASSERT_EQ( 1, var_list.size() );
  EXPECT_EQ( var1, var_list[0] );
}

TEST(BddVarSetTest, cap_int_invalid1)
{ // varset &= var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 &= var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_int_invalid2)
{ // varset(invalid) &= var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 &= var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_int_invalid3)
{ // varset &= var(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(0);

  EXPECT_THROW( varset1 &= var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_int_invalid4)
{ // varset(invalid) &= varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 &= varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_int_invalid5)
{ // varset &= varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 &= varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, cap_int_invalid6)
{ // varset &= varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);
  auto var6 = mgr2.variable(3);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 &= varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, check3)
{ // varset && var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3});
  auto res1 = varset1 && var4;
  EXPECT_FALSE( res1 );

  auto res2 = varset1 && var1;
  EXPECT_TRUE( res2 );
}

TEST(BddVarSetTest, check4)
{ // varset && varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var4});
  auto varset2 = BddVarSet({var2, var3});

  auto res = varset1 && varset2;
  EXPECT_TRUE( res );
}

TEST(BddVarSetTest, check_invalid1)
{ // varset && var(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto var5 = BddVar::invalid();

  EXPECT_THROW( varset1 && var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, check_invalid2)
{ // varset(invalid) && var
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();

  EXPECT_THROW( varset1 && var1,
		std::invalid_argument );
}

TEST(BddVarSetTest, check_invalid3)
{ // varset && var(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(2);

  EXPECT_THROW( varset1 && var5,
		std::invalid_argument );
}

TEST(BddVarSetTest, check_invalid4)
{ // varset(invalid) && varset
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet::invalid();
  auto varset2 = BddVarSet({var1, var2, var3, var4});

  EXPECT_THROW( varset1 && varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, check_invalid5)
{ // varset && varset(invalid)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);

  auto varset1 = BddVarSet({var1, var2, var3, var4});
  auto varset2 = BddVarSet::invalid();

  EXPECT_THROW( varset1 && varset2,
		std::invalid_argument );
}

TEST(BddVarSetTest, check_invalid6)
{ // varset && varset(another mgr)
  BddMgr mgr;
  auto var1 = mgr.variable(0);
  auto var2 = mgr.variable(1);
  auto var3 = mgr.variable(2);
  auto var4 = mgr.variable(3);
  auto varset1 = BddVarSet({var1, var2, var3, var4});
  BddMgr mgr2;
  auto var5 = mgr2.variable(0);
  auto var6 = mgr2.variable(1);
  auto varset2 = BddVarSet({var5, var6});

  EXPECT_THROW( varset1 && varset2,
		std::invalid_argument );
}

END_NAMESPACE_YM
