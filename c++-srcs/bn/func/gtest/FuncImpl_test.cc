
/// @file FuncImpl_test.cc
/// @brief FuncImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "FuncImpl.h"
#include "ym/SopCover.h"
#include "ym/Expr.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

TEST(FuncImpl_test, primitive_C0)
{
  auto input_num = 0;
  auto prim_type = PrimType::C0;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_C1)
{
  auto input_num = 0;
  auto prim_type = PrimType::C1;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Buff)
{
  auto input_num = 1;
  auto prim_type = PrimType::Buff;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Not)
{
  auto input_num = 1;
  auto prim_type = PrimType::Not;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_And)
{
  auto input_num = 2;
  auto prim_type = PrimType::And;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Nand)
{
  auto input_num = 2;
  auto prim_type = PrimType::Nand;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Or)
{
  auto input_num = 2;
  auto prim_type = PrimType::Or;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Nor)
{
  auto input_num = 2;
  auto prim_type = PrimType::Nor;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Xor)
{
  auto input_num = 2;
  auto prim_type = PrimType::Xor;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, primitive_Xnor)
{
  auto input_num = 2;
  auto prim_type = PrimType::Xnor;
  auto func = FuncImpl::new_primitive(input_num, prim_type);

  EXPECT_EQ( BnFunc::PRIMITIVE, func->type() );
  EXPECT_TRUE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( prim_type, func->primitive_type() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, null_cover)
{
  auto cover = SopCover{};
  auto func = FuncImpl::new_cover(cover, false);

  EXPECT_EQ( BnFunc::COVER, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_EQ( cover, func->input_cover() );
  EXPECT_FALSE( func->output_inv() );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, null_cube_test)
{
  auto cover = SopCover(0);
  auto func = FuncImpl::new_cover(cover, false);

  EXPECT_EQ( BnFunc::COVER, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_EQ( cover, func->input_cover() );
  EXPECT_FALSE( func->output_inv() );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, cover1)
{
  auto input_num = 2;
  auto lit0 = Literal(0, false);
  auto lit1 = Literal(1, false);
  auto cover = SopCover(input_num, {{lit0, lit1}});
  auto inv = true;
  auto func = FuncImpl::new_cover(cover, inv);

  EXPECT_EQ( BnFunc::COVER, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( cover, func->input_cover() );
  EXPECT_EQ( inv, func->output_inv() );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, cover2)
{
  auto input_num = 2;
  auto lit0 = Literal(0, false);
  auto lit1 = Literal(1, false);
  auto cover = SopCover(input_num, {{lit0}, {lit1}});
  auto inv = true;
  auto func = FuncImpl::new_cover(cover, inv);

  EXPECT_EQ( BnFunc::COVER, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_TRUE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_EQ( cover, func->input_cover() );
  EXPECT_EQ( inv, func->output_inv() );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( {func->tvfunc();}, std::invalid_argument );
  EXPECT_THROW( {func->bdd();}, std::invalid_argument );
}

TEST(FuncImpl_test, expr_zero)
{
  auto input_num = 0;
  auto expr = Expr::zero();
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFunc::EXPR, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_EQ( "C0", func->expr().rep_string() );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, expr_one)
{
  auto input_num = 0;
  auto expr = Expr::one();
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFunc::EXPR, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_EQ( "C1", func->expr().rep_string() );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, expr1)
{
  auto input_num = 2;
  auto v0 = Expr::literal(0);
  auto v1 = Expr::literal(1);
  auto expr = v0 | ~v1;
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFunc::EXPR, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_EQ( "O2P0N1", func->expr().rep_string() );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, expr2)
{
  // 1変数だが2入力関数とみなす．
  auto input_num = 2;
  auto v1 = Expr::literal(1);
  auto expr = v1;
  auto func = FuncImpl::new_expr(expr);

  EXPECT_EQ( BnFunc::EXPR, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_TRUE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_EQ( "P1", func->expr().rep_string() );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, tvfunc_zero)
{
  auto input_num = 4;
  auto tvfunc = TvFunc::zero(input_num);
  auto func = FuncImpl::new_tvfunc(tvfunc);

  EXPECT_EQ( BnFunc::TVFUNC, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_TRUE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_EQ( "0000000000000000", func->tvfunc().str() );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, tvfunc_one)
{
  auto input_num = 4;
  auto tvfunc = TvFunc::one(input_num);
  auto func = FuncImpl::new_tvfunc(tvfunc);

  EXPECT_EQ( BnFunc::TVFUNC, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_TRUE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_EQ( "1111111111111111", func->tvfunc().str() );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, tvfunc1)
{
  auto input_num = 3;
  auto v0 = TvFunc::positive_literal(input_num, 0);
  auto v1 = TvFunc::positive_literal(input_num, 1);
  auto v2 = TvFunc::positive_literal(input_num, 2);
  auto tvfunc = v0 | v1 | v2;
  auto func = FuncImpl::new_tvfunc(tvfunc);

  EXPECT_EQ( BnFunc::TVFUNC, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_TRUE( func->is_tvfunc() );
  EXPECT_FALSE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_EQ( "11111110", func->tvfunc().str() );
  EXPECT_THROW( func->bdd(), std::invalid_argument );
}

TEST(FuncImpl_test, bdd_zero)
{
  auto bdd = Bdd::zero();
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFunc::BDD, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

TEST(FuncImpl_test, bdd_one)
{
  auto bdd = Bdd::one();
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFunc::BDD, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( 0, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

TEST(FuncImpl_test, bdd1)
{
  BddMgr mgr;
  auto input_num = 2;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto bdd = var0 & ~var1;
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFunc::BDD, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

TEST(FuncImpl_test, bdd2)
{
  BddMgr mgr;
  auto input_num = 1;
  auto var0 = mgr.variable_bdd(0);
  auto bdd = ~var0;
  auto func = FuncImpl::new_bdd(bdd);

  EXPECT_EQ( BnFunc::BDD, func->type() );
  EXPECT_FALSE( func->is_primitive() );
  EXPECT_FALSE( func->is_cover() );
  EXPECT_FALSE( func->is_expr() );
  EXPECT_FALSE( func->is_tvfunc() );
  EXPECT_TRUE( func->is_bdd() );
  EXPECT_EQ( input_num, func->input_num() );
  EXPECT_THROW( func->input_cover(), std::invalid_argument );
  EXPECT_THROW( func->output_inv(), std::invalid_argument );
  EXPECT_THROW( func->expr(), std::invalid_argument );
  EXPECT_THROW( func->tvfunc(), std::invalid_argument );
  EXPECT_EQ( bdd, func->bdd() );
}

END_NAMESPACE_YM_BN
