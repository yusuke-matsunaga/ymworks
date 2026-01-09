
/// @file AigMgr_test.cc
/// @brief AigMgr_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

TEST(AigMgrTest, constr)
{
  AigMgr mgr;

  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
}

TEST(AigMgrTest, shallow_copy)
{
  AigMgr mgr;
  auto mgr2 = mgr;
  EXPECT_EQ( mgr, mgr2 );
}

TEST(AigMgrTest, make_zero)
{
  AigMgr mgr;

  auto h = AigLiteral::zero();
  EXPECT_TRUE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_TRUE( h.is_const() );

  EXPECT_THROW( h.node_id(), std::logic_error );

  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
}

TEST(AigMgrTest, make_one)
{
  AigMgr mgr;

  auto h = AigLiteral::one();
  EXPECT_FALSE( h.is_zero() );
  EXPECT_TRUE( h.is_one() );
  EXPECT_TRUE( h.is_const() );

  EXPECT_THROW( h.node_id(), std::logic_error );

  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
}

TEST(AigMgrTest, input)
{
  AigMgr mgr;

  auto h = mgr.new_input();
  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );

  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_input(node_id) );
  EXPECT_TRUE( mgr.is_primary_input(node_id) );
  EXPECT_EQ( 0, mgr.input_id(node_id) );
  EXPECT_FALSE( mgr.is_dff_output(node_id) );
  EXPECT_THROW( mgr.dff_id(node_id), std::logic_error );
  EXPECT_FALSE( mgr.is_and(node_id) );
  EXPECT_THROW( mgr.fanin(node_id, 0), std::logic_error );
  EXPECT_THROW( mgr.fanin0(node_id), std::logic_error );
  EXPECT_THROW( mgr.fanin1(node_id), std::logic_error );

  EXPECT_EQ( 1, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
}

TEST(AigMgrTest, dff_output)
{
  AigMgr mgr;

  auto dff_id = mgr.new_dff();
  auto h = mgr.dff_output(dff_id);
  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );

  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_input(node_id) );
  EXPECT_FALSE( mgr.is_primary_input(node_id) );
  EXPECT_THROW( mgr.input_id(node_id), std::logic_error );
  EXPECT_TRUE( mgr.is_dff_output(node_id) );
  EXPECT_EQ( dff_id, mgr.dff_id(node_id));
  EXPECT_FALSE( mgr.is_and(node_id) );
  EXPECT_THROW( mgr.fanin(node_id, 0), std::logic_error );
  EXPECT_THROW( mgr.fanin0(node_id), std::logic_error );
  EXPECT_THROW( mgr.fanin1(node_id), std::logic_error );

  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  EXPECT_EQ( 1, mgr.dff_num() );
}

TEST(AigMgrTest, new_and2)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();

  auto ho = mgr.new_and({h1, h2});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  EXPECT_EQ( ho, h );

  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );

  auto node_id = h.node_id();
  EXPECT_FALSE( mgr.is_input(node_id) );
  EXPECT_FALSE( mgr.is_primary_input(node_id) );
  EXPECT_THROW( mgr.input_id(node_id), std::logic_error );
  EXPECT_FALSE( mgr.is_dff_output(node_id) );
  EXPECT_THROW( mgr.dff_id(node_id), std::logic_error );
  EXPECT_TRUE( mgr.is_and(node_id) );
  EXPECT_EQ( h1, mgr.fanin(node_id, 0) );
  EXPECT_EQ( h2, mgr.fanin(node_id, 1) );
  EXPECT_EQ( h1, mgr.fanin0(node_id) );
  EXPECT_EQ( h2, mgr.fanin1(node_id) );

  EXPECT_EQ( 2, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 1, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  ASSERT_EQ( 2, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
}

TEST(AigMgrTest, new_and3)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();

  auto ho = mgr.new_and({h1, h2, h3});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);

  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );

  auto node_id = h.node_id();
  EXPECT_FALSE( mgr.is_input(node_id) );
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 3, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 2, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  ASSERT_EQ( 3, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
  EXPECT_EQ( h3, fanin_list[2] );

  auto input_vals = std::vector<AigBitVect>{
    0xAA,
    0xCC,
    0x0F
  };
  auto output_vals = mgr.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  auto oval = output_vals[0];
  EXPECT_EQ( 0x08, oval );
}

TEST(AigMgrTest, new_and4)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();
  auto h4 = mgr.new_input();

  auto ho = mgr.new_and({h1, h2, h3, h4});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 4, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 3, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  EXPECT_EQ( 4, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
  EXPECT_EQ( h3, fanin_list[2] );
  EXPECT_EQ( h4, fanin_list[3] );

  auto input_vals = std::vector<AigBitVect>{
    0xAAAA,
    0xCCCC,
    0x0F0F,
    0x00FF,
  };
  auto output_vals = mgr.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  auto oval = output_vals[0];
  EXPECT_EQ( 0x0008, oval );
}

TEST(AigMgrTest, new_and8)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();
  auto h4 = mgr.new_input();
  auto h5 = mgr.new_input();
  auto h6 = mgr.new_input();
  auto h7 = mgr.new_input();
  auto h8 = mgr.new_input();

  auto ho = mgr.new_and({h1, h2, h3, h4, h5, h6, h7, h8});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 8, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 7, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  ASSERT_EQ( 8, fanin_list.size() );
  EXPECT_EQ( h1, fanin_list[0] );
  EXPECT_EQ( h2, fanin_list[1] );
  EXPECT_EQ( h3, fanin_list[2] );
  EXPECT_EQ( h4, fanin_list[3] );
  EXPECT_EQ( h5, fanin_list[4] );
  EXPECT_EQ( h6, fanin_list[5] );
  EXPECT_EQ( h7, fanin_list[6] );
  EXPECT_EQ( h8, fanin_list[7] );
}

TEST(AigMgrTest, new_or3)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();

  auto ho = mgr.new_or({h1, h2, h3});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 3, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 2, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  ASSERT_EQ( 3, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );
  EXPECT_EQ( ~h3, fanin_list[2] );

  auto input_vals = std::vector<AigBitVect>{
    0xAA,
    0xCC,
    0x0F
  };
  auto output_vals = mgr.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  auto oval = output_vals[0];
  EXPECT_EQ( 0xEF, oval );
}

TEST(AigMgrTest, new_or4)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();
  auto h4 = mgr.new_input();

  auto ho = mgr.new_or({h1, h2, h3, h4});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 4, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 3, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  ASSERT_EQ( 4, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );
  EXPECT_EQ( ~h3, fanin_list[2] );
  EXPECT_EQ( ~h4, fanin_list[3] );

  auto input_vals = std::vector<AigBitVect>{
    0xAAAA,
    0xCCCC,
    0x0F0F,
    0x00FF,
  };
  auto output_vals = mgr.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  auto oval = output_vals[0];
  EXPECT_EQ( 0xEFFF, oval );
}

TEST(AigMgrTest, new_or8)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();
  auto h4 = mgr.new_input();
  auto h5 = mgr.new_input();
  auto h6 = mgr.new_input();
  auto h7 = mgr.new_input();
  auto h8 = mgr.new_input();

  auto ho = mgr.new_or({h1, h2, h3, h4, h5, h6, h7, h8});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 8, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 7, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  ASSERT_EQ( 8, fanin_list.size() );
  EXPECT_EQ( ~h1, fanin_list[0] );
  EXPECT_EQ( ~h2, fanin_list[1] );
  EXPECT_EQ( ~h3, fanin_list[2] );
  EXPECT_EQ( ~h4, fanin_list[3] );
  EXPECT_EQ( ~h5, fanin_list[4] );
  EXPECT_EQ( ~h6, fanin_list[5] );
  EXPECT_EQ( ~h7, fanin_list[6] );
  EXPECT_EQ( ~h8, fanin_list[7] );
}

TEST(AigMgrTest, new_xor3)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();

  auto ho = mgr.new_xor({h1, h2, h3});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 3, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 6, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  EXPECT_EQ( 2, fanin_list.size() );

  auto input_vals = std::vector<AigBitVect>{
    0xAA,
    0xCC,
    0x0F
  };
  auto output_vals = mgr.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  auto oval = output_vals[0];
  EXPECT_EQ( 0x69, oval );
}

TEST(AigMgrTest, new_xor4)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();
  auto h4 = mgr.new_input();

  auto ho = mgr.new_xor({h1, h2, h3, h4});

  auto oid = mgr.new_output(ho);

  auto h = mgr.output(oid);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  EXPECT_EQ( 4, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 9, mgr.and_num() );
  EXPECT_EQ( 0, mgr.dff_num() );

  auto fanin_list = mgr.ex_fanin_list(node_id);
  EXPECT_EQ( 2, fanin_list.size() );

  auto input_vals = std::vector<AigBitVect>{
    0xAAAA,
    0xCCCC,
    0x0F0F,
    0x00FF
  };
  auto output_vals = mgr.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  auto oval = output_vals[0];
  EXPECT_EQ( 0x6996, oval );
}

TEST(AigMgrTest, ex_fanin_list)
{
  AigMgr mgr;

  auto h1 = mgr.new_input();
  auto h2 = mgr.new_input();
  auto h3 = mgr.new_input();

  auto t1 = mgr.new_and({h1, h2});
  auto t2 = mgr.new_and({h2, h3});
  auto t3 = mgr.new_and({h3, h1});

  auto s1 = mgr.new_and({t1, t2});
  auto s2 = mgr.new_and({t2, t3});

  auto o = mgr.new_and({s1, s2});
  auto node_id = o.node_id();
  auto fanin_list = mgr.ex_fanin_list(node_id);
  EXPECT_EQ( 3, fanin_list.size() );
}

TEST(AigMgrTest, from_expr_zero)
{
  auto expr = Expr::zero();

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.is_zero() );
}

TEST(AigMgrTest, from_expr_one)
{
  auto expr = Expr::one();

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.is_one() );
}

TEST(AigMgrTest, from_expr_literal)
{
  SizeType varid = 10;
  bool inv = true;
  auto expr = Expr::literal(varid, inv);

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_input(node_id) );
  EXPECT_EQ( varid, mgr.input_id(node_id) );
  EXPECT_EQ( inv, h.inv() );
}

TEST(AigMgrTest, from_expr_and)
{
  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto expr = lit0 & lit1;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_FALSE( h.inv() );

  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  auto h0 = mgr.fanin0(node_id);
  EXPECT_FALSE( h0.inv() );
  auto node0_id = h0.node_id();
  EXPECT_TRUE( mgr.is_input(node0_id) );
  EXPECT_EQ( 0, mgr.input_id(node0_id) );

  auto h1 = mgr.fanin1(node_id);
  EXPECT_FALSE( h1.inv() );
  auto node1_id = h1.node_id();
  EXPECT_TRUE( mgr.is_input(node1_id) );
  EXPECT_EQ( 1, mgr.input_id(node1_id) );
}

TEST(AigMgrTest, from_expr_or)
{
  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto expr = lit0 | lit1;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  EXPECT_TRUE( h.inv() );

  auto node_id = h.node_id();
  EXPECT_TRUE( mgr.is_and(node_id) );

  auto h0 = mgr.fanin0(node_id);
  EXPECT_TRUE( h0.inv() );
  auto node0_id = h0.node_id();
  EXPECT_TRUE( mgr.is_input(node0_id) );
  EXPECT_EQ( 0, mgr.input_id(node0_id) );

  auto h1 = mgr.fanin1(node_id);
  EXPECT_TRUE( h1.inv() );
  auto node1_id = h1.node_id();
  EXPECT_TRUE( mgr.is_input(node1_id) );
  EXPECT_EQ( 1, mgr.input_id(node1_id) );
}

TEST(AigMgrTest, from_expr_xor)
{
  auto lit0 = Expr::literal(0);
  auto lit1 = Expr::literal(1);
  auto expr = lit0 ^ lit1;

  AigMgr mgr;

  auto h = mgr.from_expr(expr);
  mgr.new_output(h);

  mgr.print(std::cout);
}

TEST(AigMgrTest, ref_count)
{
  AigMgr mgr;

  {
    auto h1 = mgr.new_input();
    auto h2 = mgr.new_input();
    auto h3 = mgr.new_input();

    auto h0 = mgr.new_and({h1, h2, h3});
  }
  mgr.print(std::cout);
}

END_NAMESPACE_YM
