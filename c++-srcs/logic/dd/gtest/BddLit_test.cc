
/// @file BddLit_test.cc
/// @brief BddLit_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BddLit.h"
#include "ym/BddVar.h"
#include "ym/BddCube.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_DD

TEST(BddLitTest, default)
{
  auto lit = BddLit{}; // 不正値

  EXPECT_TRUE( lit.is_invalid() );
  // is_positive(), is_negative() は意味を持たない．
  // 場合によってはどちらもありうる．
  EXPECT_THROW( lit.var(),
		std::invalid_argument );
  EXPECT_THROW( lit.positive_literal(),
		std::invalid_argument );
  EXPECT_THROW( lit.negative_literal(),
		std::invalid_argument );
  EXPECT_THROW( lit.complement(),
		std::invalid_argument );
  EXPECT_THROW( ~lit,
		std::invalid_argument );
  EXPECT_THROW( lit * true,
		std::invalid_argument );
}

TEST(BddLitTest, invalid)
{
  auto lit = BddLit::invalid();

  EXPECT_TRUE( lit.is_invalid() );
  // is_positive(), is_negative() は意味を持たない．
  // 場合によってはどちらもありうる．
  EXPECT_THROW( lit.var(),
		std::invalid_argument );
  EXPECT_THROW( lit.positive_literal(),
		std::invalid_argument );
  EXPECT_THROW( lit.negative_literal(),
		std::invalid_argument );
  EXPECT_THROW( lit.complement(),
		std::invalid_argument );
  EXPECT_THROW( ~lit,
		std::invalid_argument );
  EXPECT_THROW( lit * true,
		std::invalid_argument );
}

TEST(BddLitTest, constr1)
{
  BddMgr mgr;
  SizeType varid = 10;
  bool inv = true;
  auto lit = BddLit(mgr, varid, inv);

  EXPECT_FALSE( lit.is_invalid() );
  auto var = lit.var();
  EXPECT_EQ( varid, var.varid() );
  EXPECT_EQ( !inv, lit.is_positive() );
  EXPECT_EQ( inv, lit.is_negative() );

  auto plit = lit.positive_literal();
  EXPECT_EQ( var, plit.var() );
  EXPECT_TRUE( plit.is_positive() );
  EXPECT_FALSE( plit.is_negative() );

  auto nlit = lit.negative_literal();
  EXPECT_EQ( var, nlit.var() );
  EXPECT_FALSE( nlit.is_positive() );
  EXPECT_TRUE( nlit.is_negative() );

  auto ilit1 = lit.complement();
  EXPECT_EQ( var, ilit1.var() );
  EXPECT_TRUE( ilit1.is_positive() ^ lit.is_positive() );

  auto ilit2 = ~lit;
  EXPECT_EQ( var, ilit2.var() );
  EXPECT_TRUE( ilit2.is_positive() ^ lit.is_positive() );

  auto lit3 = lit * false;
  EXPECT_EQ( lit, lit3 );

  auto lit4 = lit * true;
  EXPECT_EQ( ilit1, lit4 );

  auto lit5 = lit;
  auto lit6 = lit5 *= false;
  EXPECT_EQ( lit, lit5 );
  EXPECT_EQ( lit, lit6 );

  auto lit7 = lit;
  auto lit8 = lit7 *= true;
  EXPECT_EQ( ilit1, lit7 );
  EXPECT_EQ( ilit1, lit8 );

  std::ostringstream buf;
  buf << lit;
  EXPECT_EQ( "n10", buf.str() );
}

TEST(BddLitTest, constr2)
{
  BddMgr mgr;
  SizeType varid = 10;
  auto var = mgr.variable(varid);
  bool inv = false;
  auto lit = BddLit(var, inv);

  EXPECT_FALSE( lit.is_invalid() );
  EXPECT_EQ( var, lit.var() );
  EXPECT_EQ( !inv, lit.is_positive() );
  EXPECT_EQ( inv, lit.is_negative() );

  auto plit = lit.positive_literal();
  EXPECT_EQ( var, plit.var() );
  EXPECT_TRUE( plit.is_positive() );
  EXPECT_FALSE( plit.is_negative() );

  auto nlit = lit.negative_literal();
  EXPECT_EQ( var, nlit.var() );
  EXPECT_FALSE( nlit.is_positive() );
  EXPECT_TRUE( nlit.is_negative() );

  auto ilit1 = lit.complement();
  EXPECT_EQ( var, ilit1.var() );
  EXPECT_TRUE( ilit1.is_positive() ^ lit.is_positive() );

  auto ilit2 = ~lit;
  EXPECT_EQ( var, ilit2.var() );
  EXPECT_TRUE( ilit2.is_positive() ^ lit.is_positive() );

  auto lit3 = lit * false;
  EXPECT_EQ( lit, lit3 );

  auto lit4 = lit * true;
  EXPECT_EQ( ilit1, lit4 );
}

TEST(BddLitTest, prod1)
{
  BddMgr mgr;

  auto lit1 = BddLit(mgr, 1, false);
  auto lit2 = BddLit(mgr, 2, false);

  auto cube = lit1 & lit2;
  EXPECT_FALSE( cube.is_invalid() );
  ASSERT_EQ( 2, cube.size() );
  auto lit_list = cube.lit_list();
  ASSERT_EQ( 2, lit_list.size() );
  EXPECT_EQ( lit1, lit_list[0] );
  EXPECT_EQ( lit2, lit_list[1] );
}

TEST(BddLitTest, prod_invalid1)
{ // lit(invalid) & lit
  BddMgr mgr;

  auto lit1 = BddLit::invalid();
  auto lit2 = BddLit(mgr, 2, false);

  EXPECT_THROW( lit1 & lit2,
		std::invalid_argument );
}

TEST(BddLitTest, prod_invalid2)
{ // lit & lit(invalid)
  BddMgr mgr;

  auto lit1 = BddLit(mgr, 1, false);
  auto lit2 = BddLit::invalid();

  EXPECT_THROW( lit1 & lit2,
		std::invalid_argument );
}

TEST(BddLitTest, prod_invalid3)
{ // lit & lit(another mgr)
  BddMgr mgr;
  auto lit1 = BddLit(mgr, 1, false);
  BddMgr mgr2;
  auto lit2 = BddLit(mgr2, 2, true);

  EXPECT_THROW( lit1 & lit2,
		std::invalid_argument );
}

END_NAMESPACE_YM_DD
