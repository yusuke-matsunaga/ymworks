
/// @file BcOp_test.cc
/// @brief BcOp_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BcGraph.h"


BEGIN_NAMESPACE_YM

TEST(BcOpTest, and1)
{
  BcGraph g(2);
  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::AND, {e1, e2});

  auto& op = g.op(op_id);
  EXPECT_EQ( op_id, op.id() );
  EXPECT_EQ( BcOpType::AND, op.type() );

  auto e3 = op.operand(0);
  EXPECT_TRUE( e3.is_input() );
  EXPECT_EQ( 0, e3.input_id() );

  auto e4 = op.operand(1);
  EXPECT_TRUE( e4.is_input() );
  EXPECT_EQ( 1, e4.input_id() );

  EXPECT_EQ( 1, op.level() );
}

TEST(BcOpTest, or1)
{
  BcGraph g(2);
  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::OR, {e1, e2});

  auto& op = g.op(op_id);
  EXPECT_EQ( op_id, op.id() );
  EXPECT_EQ( BcOpType::OR, op.type() );

  auto e3 = op.operand(0);
  EXPECT_TRUE( e3.is_input() );
  EXPECT_EQ( 0, e3.input_id() );

  auto e4 = op.operand(1);
  EXPECT_TRUE( e4.is_input() );
  EXPECT_EQ( 1, e4.input_id() );

  EXPECT_EQ( 1, op.level() );
}

TEST(BcOpTest, xor1)
{
  BcGraph g(2);
  auto e1 = BcEdge::input(0);
  auto e2 = BcEdge::input(1);
  auto op_id = g.add_op(BcOpType::XOR, {e1, e2});

  auto& op = g.op(op_id);
  EXPECT_EQ( op_id, op.id() );
  EXPECT_EQ( BcOpType::XOR, op.type() );

  auto e3 = op.operand(0);
  EXPECT_TRUE( e3.is_input() );
  EXPECT_EQ( 0, e3.input_id() );

  auto e4 = op.operand(1);
  EXPECT_TRUE( e4.is_input() );
  EXPECT_EQ( 1, e4.input_id() );

  EXPECT_EQ( 1, op.level() );
}

END_NAMESPACE_YM
