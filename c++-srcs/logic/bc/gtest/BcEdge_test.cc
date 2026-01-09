
/// @file BcEdge_test.cc
/// @brief BcEdge_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BcGraph.h"


BEGIN_NAMESPACE_YM

TEST(BcEdgeTest, input)
{
  SizeType input_id = 7;
  auto e = BcEdge::input(input_id);

  EXPECT_TRUE ( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.inv() );

  EXPECT_EQ( input_id, e.input_id() );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  std::ostringstream buf2;
  buf2 << "I#" << input_id;
  EXPECT_EQ( buf2.str(), e.to_string() );
}

TEST(BcEdgeTest, input2)
{
  SizeType input_id = 7;
  auto e = BcEdge::input(input_id, false);

  EXPECT_TRUE ( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.inv() );

  EXPECT_EQ( input_id, e.input_id() );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  std::ostringstream buf2;
  buf2 << "I#" << input_id;
  EXPECT_EQ( buf2.str(), e.to_string() );
}

TEST(BcEdgeTest, input_inv)
{
  SizeType input_id = 7;
  auto e = BcEdge::input(input_id, true);

  EXPECT_TRUE ( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_TRUE ( e.inv() );

  EXPECT_EQ( input_id, e.input_id() );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  std::ostringstream buf2;
  buf2 << "~I#" << input_id;
  EXPECT_EQ( buf2.str(), e.to_string() );
}

TEST(BcEdgeTest, op)
{
  BcGraph g(2);

  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::AND, {e1, e2});
  auto e = BcEdge::op(op_id);

  EXPECT_FALSE( e.is_input() );
  EXPECT_TRUE ( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.inv() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_EQ( op_id, e.op_id() );

  std::ostringstream buf2;
  buf2 << "OP#" << op_id;
  EXPECT_EQ( buf2.str(), e.to_string() );
}

TEST(BcEdgeTest, op2)
{
  BcGraph g(2);

  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::AND, {e1, e2});
  auto e = BcEdge::op(op_id, false);

  EXPECT_FALSE( e.is_input() );
  EXPECT_TRUE ( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_FALSE( e.inv() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_EQ( op_id, e.op_id() );

  std::ostringstream buf2;
  buf2 << "OP#" << op_id;
  EXPECT_EQ( buf2.str(), e.to_string() );
}

TEST(BcEdgeTest, op_inv)
{
  BcGraph g(2);

  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::AND, {e1, e2});
  auto e = BcEdge::op(op_id, true);

  EXPECT_FALSE( e.is_input() );
  EXPECT_TRUE ( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_FALSE( e.is_one() );
  EXPECT_TRUE ( e.inv() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_EQ( op_id, e.op_id() );

  std::ostringstream buf2;
  buf2 << "~OP#" << op_id;
  EXPECT_EQ( buf2.str(), e.to_string() );
}

TEST(BcEdgeTest, zero)
{
  auto e = BcEdge::zero();

  EXPECT_FALSE( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_TRUE ( e.is_zero() );
  EXPECT_FALSE( e.is_one() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  EXPECT_EQ( "0", e.to_string() );
}

TEST(BcEdgeTest, one)
{
  auto e = BcEdge::one();

  EXPECT_FALSE( e.is_input() );
  EXPECT_FALSE( e.is_op() );
  EXPECT_FALSE( e.is_zero() );
  EXPECT_TRUE ( e.is_one() );

  EXPECT_THROW( e.input_id(),
		std::invalid_argument );
  EXPECT_THROW( e.op_id(),
		std::invalid_argument );

  EXPECT_EQ( "1", e.to_string() );
}

TEST(BcEdgeTest, invert1)
{
  BcGraph g;

  auto input1 = BcEdge::input(0);
  auto input1_inv = BcEdge::input(0, true);
  EXPECT_EQ( input1_inv, ~input1 );
  EXPECT_EQ( input1, ~input1_inv );

  auto input2 = BcEdge::input(1);
  auto op1_id = g.add_op(BcOpType::AND, {input1, input2});
  auto op1_edge = BcEdge::op(op1_id);
  auto op1_inv_edge = BcEdge::op(op1_id, true);
  EXPECT_EQ( op1_inv_edge, ~op1_edge );
  EXPECT_EQ( op1_edge, ~op1_inv_edge );

  auto zero = BcEdge::zero();
  auto one = BcEdge::one();
  EXPECT_EQ( one, ~zero );
  EXPECT_EQ( zero, ~one );
}

TEST(BcEdgeTest, invert2)
{
  BcGraph g;

  auto input1 = BcEdge::input(0);
  auto input1_inv = BcEdge::input(0, true);
  auto inv1_edge = input1.invert();
  EXPECT_EQ( inv1_edge, input1 );
  EXPECT_EQ( input1_inv, input1 );

  auto input2 = BcEdge::input(1);
  auto input2_inv = BcEdge::input(1, true);
  auto inv2_edge = input2_inv.invert();
  EXPECT_EQ( inv2_edge, input2_inv );
  EXPECT_EQ( input2, input2_inv );

  auto op1_id = g.add_op(BcOpType::AND, {input1, input2});
  auto op1_edge = BcEdge::op(op1_id);
  auto op1_inv_edge = BcEdge::op(op1_id, true);
  auto tmp1 = op1_edge;
  auto tmp2 = tmp1.invert();
  EXPECT_EQ( tmp2, tmp1 );
  EXPECT_EQ( op1_inv_edge, tmp1 );

  auto tmp3 = op1_inv_edge;
  auto tmp4 = tmp3.invert();
  EXPECT_EQ( tmp4, tmp3 );
  EXPECT_EQ( op1_edge, tmp3 );

  auto zero = BcEdge::zero();
  auto one = BcEdge::one();
  auto tmp5 = zero;
  auto tmp6 = tmp5.invert();
  EXPECT_EQ( tmp6, tmp5 );
  EXPECT_EQ( one, tmp5 );
  auto tmp7 = one;
  auto tmp8 = tmp7.invert();
  EXPECT_EQ( tmp7, tmp8 );
  EXPECT_EQ( zero, tmp8 );

}

TEST(BcEdgeTest, equal)
{
  BcGraph g;

  auto input1 = BcEdge::input(0);
  auto input2 = BcEdge::input(1);
  auto input_inv1 = BcEdge::input(0, true);
  auto input_inv2 = BcEdge::input(1, true);
  auto op1_id = g.add_op(BcOpType::AND, {input1, input2});
  auto op2_id = g.add_op(BcOpType::OR,  {input1, input2});
  auto op3_id = g.add_op(BcOpType::AND, {input1, input_inv2});
  auto op1_edge = BcEdge::op(op1_id);
  auto op1_inv_edge = BcEdge::op(op1_id, true);
  auto op2_edge = BcEdge::op(op2_id);

  auto edge_list = std::vector<BcEdge>{
    input1,
    input2,
    input_inv1,
    input_inv2,
    op1_edge,
    op1_inv_edge,
    op2_edge
  };
  auto n = edge_list.size();
  for ( SizeType i1 = 0; i1 < n; ++ i1 ) {
    auto edge1 = edge_list[i1];
    for ( SizeType i2 = 0; i2 < n; ++ i2 ) {
      auto edge2 = edge_list[i2];
      auto exp_ans = i1 == i2;
      EXPECT_EQ( exp_ans, edge1 == edge2 );
    }
  }
}

END_NAMESPACE_YM
