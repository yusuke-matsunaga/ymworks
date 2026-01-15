
/// @file NodeImpl_test.cc
/// @brief NodeImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "NodeImpl.h"
#include "NodeImpl_sub.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST( NodeImplTest, primary_input )
{
  ModelImpl model;
  SizeType id = 123;
  SizeType iid = 10;
  NodeImpl_PrimaryInput input(&model, id, iid);
  NodeImpl* node = &input;

  EXPECT_EQ( BnNode::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_FALSE( node->is_logic() );

  EXPECT_TRUE( node->is_primary_input() );
  EXPECT_FALSE( node->is_dff_output() );
  EXPECT_EQ( iid, node->input_id() );
  EXPECT_THROW( node->dff_id(),
		std::logic_error );

  EXPECT_THROW( node->func(),
		std::logic_error );
  EXPECT_EQ( 0, node->fanin_num() );
  EXPECT_THROW( node->fanin(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const NodeImpl*>{}, node->fanin_list() );
}

TEST( NodeImplTest, dff_output )
{
  ModelImpl model;
  SizeType id = 123;
  SizeType dff_id = 10;
  NodeImpl_DffOutput dff_output(&model, id, dff_id);
  NodeImpl* node = &dff_output;

  EXPECT_EQ( BnNode::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_FALSE( node->is_logic() );

  EXPECT_FALSE( node->is_primary_input() );
  EXPECT_TRUE( node->is_dff_output() );
  EXPECT_THROW( node->input_id(),
		std::logic_error );
  EXPECT_EQ( dff_id, node->dff_id() );

  EXPECT_THROW( node->func(),
		std::logic_error );
  EXPECT_EQ( 0, node->fanin_num() );
  EXPECT_THROW( node->fanin(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<const NodeImpl*>{}, node->fanin_list() );
}

TEST( NodeImplTest, logic )
{
  ModelImpl model;
  SizeType id = 123;
  auto func = model.reg_primitive(2, PrimType::And);
  auto in1 = model.new_input();
  auto in2 = model.new_input();
  std::vector<const NodeImpl*> fanin_list{in1, in2};
  NodeImpl_Logic logic(&model, id, func, fanin_list);
  NodeImpl* node = &logic;

  EXPECT_EQ( BnNode::LOGIC, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_TRUE( node->is_logic() );

  EXPECT_FALSE( node->is_primary_input() );
  EXPECT_FALSE( node->is_dff_output() );
  EXPECT_THROW( node->input_id(),
		std::logic_error );
  EXPECT_THROW( node->dff_id(),
		std::logic_error );

  EXPECT_EQ( func, node->func() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
}

END_NAMESPACE_YM_BN
