
/// @file BcGraph_test.cc
/// @brief BcGraph_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BcGraph.h"


BEGIN_NAMESPACE_YM

TEST(BcGraphTest, empty)
{
  BcGraph g;

  EXPECT_EQ( 0, g.input_num() );
  EXPECT_EQ( 0, g.output_num() );
  EXPECT_EQ( 0, g.op_num() );
}

TEST(BcGraphTest, constr1)
{
  BcGraph g(2);

  EXPECT_EQ( 2, g.input_num() );
  EXPECT_EQ( 0, g.output_num() );
  EXPECT_EQ( 0, g.op_num() );
}

TEST(BcGraphTest, add_output1)
{
  BcGraph g(1);

  auto e = BcEdge::input(0);
  auto output_id = g.add_output(e);

  auto e2 = g.output(output_id);
  EXPECT_EQ( e, e2 );

  EXPECT_EQ( 0, g.edge_level(e) );

  std::vector<BcBvType> input_vals{0xAAAAAAAAAAAAAAAAUL};
  auto output_vals = g.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  EXPECT_EQ( 0xAAAAAAAAAAAAAAAAUL, output_vals[0] );
}

TEST(BcGraphTest, add_zero_output1)
{
  BcGraph g;
  auto output_id = g.add_zero_output();
  auto e = g.output(output_id);
  EXPECT_FALSE( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_TRUE ( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.inv() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  EXPECT_EQ( "0", e.to_string() );

  EXPECT_EQ( 0, g.edge_level(e) );
}

TEST(BcGraphTest, add_one_output1)
{
  BcGraph g;
  auto output_id = g.add_one_output();
  auto e = g.output(output_id);
  EXPECT_FALSE( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_TRUE ( e.is_one() );
  EXPECT_TRUE ( e.inv() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  EXPECT_EQ( "1", e.to_string() );

  EXPECT_EQ( 0, g.edge_level(e) );
}

TEST(BcGraphTest, add_op1)
{
  BcGraph g(2);
  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::AND, {e1, e2});
  auto e3 = BcEdge::op(op_id);
  auto output_id = g.add_output(e3);

  auto& op = g.op(op_id);
  EXPECT_EQ( op_id, op.id() );
  EXPECT_EQ( BcOpType::AND, op.type() );
  EXPECT_EQ( e1, op.operand(0) );
  EXPECT_EQ( e2, op.operand(1) );

  EXPECT_EQ( 1, g.output_level(output_id) );
  EXPECT_EQ( 1, g.max_level() );

  auto input_vals = std::vector<BcBvType>{
    0xAAAAAAAAAAAAAAAAUL,
    0xCCCCCCCCCCCCCCCCUL
  };
  auto output_vals = g.simulate(input_vals);
  ASSERT_EQ( 1, output_vals.size() );
  EXPECT_EQ( 0x8888888888888888UL, output_vals[0] );
}

TEST(BcGraphTest, add_op2)
{
  BcGraph g(3);
  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto e3 = BcEdge::input(2);

  auto op1_id = g.add_op(BcOpType::AND, {e1, e2});
  auto e4 = BcEdge::op(op1_id);
  auto op2_id = g.add_op(BcOpType::OR,  {e3, e4});
  auto& op2 = g.op(op2_id);
  EXPECT_EQ( op2_id, op2.id() );

  auto e5 = BcEdge::op(op2_id);
  auto output_id = g.add_output(e5);

  EXPECT_EQ( 2, g.output_level(output_id) );
  EXPECT_EQ( 2, g.max_level() );
}

TEST(BcGraphTest, simulate1)
{
  BcGraph g(1);
  auto e1 = BcEdge::input(0, true);
  auto output_id = g.add_output(e1);

  auto ovals0 = g.simulate(std::vector<bool>{false});
  EXPECT_EQ( true,  ovals0[0] );
  auto ovals1 = g.simulate(std::vector<bool>{true });
  EXPECT_EQ( false, ovals1[0] );
}

TEST(BcGraphTest, simulate2)
{
  BcGraph g(2);
  auto e1 = BcEdge::input(0, true);
  auto e2 = BcEdge::input(1);

  auto op1_id = g.add_op(BcOpType::AND, {e1, e2});
  auto e3 = BcEdge::op(op1_id);

  auto output_id = g.add_output(e3);

  auto ovals00 = g.simulate(std::vector<bool>{false, false});
  EXPECT_EQ( false, ovals00[0] );
  auto ovals01 = g.simulate(std::vector<bool>{false, true });
  EXPECT_EQ( true,  ovals01[0] );
  auto ovals10 = g.simulate(std::vector<bool>{true,  false});
  EXPECT_EQ( false, ovals10[0] );
  auto ovals11 = g.simulate(std::vector<bool>{true,  true });
  EXPECT_EQ( false, ovals11[0] );
}

TEST(BcGraphTest, tvop1)
{
  BcGraph g(3);
  auto i0 = BcEdge::input(0, false);
  auto i1 = BcEdge::input(1, false);
  auto i2 = BcEdge::input(2, false);
  BcBvType tv = 0xCA;
  auto op_id = g.add_tvop({tv}, {i0, i1, i2});
  auto e = BcEdge::op(op_id);
  auto o = g.add_output(e);

  auto ovals000 = g.simulate(std::vector<bool>{false, false, false});
  EXPECT_EQ( false, ovals000[0] );
  auto ovals001 = g.simulate(std::vector<bool>{false, false, true });
  EXPECT_EQ( true,  ovals001[0] );
  auto ovals010 = g.simulate(std::vector<bool>{false, true,  false});
  EXPECT_EQ( false, ovals010[0] );
  auto ovals011 = g.simulate(std::vector<bool>{false, true,  true });
  EXPECT_EQ( true,  ovals011[0] );
  auto ovals100 = g.simulate(std::vector<bool>{true,  false, false});
  EXPECT_EQ( false, ovals100[0] );
  auto ovals101 = g.simulate(std::vector<bool>{true,  false, true });
  EXPECT_EQ( false, ovals101[0] );
  auto ovals110 = g.simulate(std::vector<bool>{true,  true,  false});
  EXPECT_EQ( true,  ovals110[0] );
  auto ovals111 = g.simulate(std::vector<bool>{true,  true,  true });
  EXPECT_EQ( true,  ovals111[0] );

  auto ovals2 = g.simulate(std::vector<BcBvType>{0xF0, 0xCC, 0xAA});
  EXPECT_EQ( 0xCA, ovals2[0] );
}

END_NAMESPACE_YM
