
/// @file NodeImpl_test.cc
/// @brief NodeImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "NodeImpl.h"
#include "NodeImpl_sub.h"


BEGIN_NAMESPACE_YM_BN

TEST( NodeImplTest, primary_input )
{
  SizeType iid = 10;
  NodeImpl_PrimaryInput input{iid};
  NodeImpl* node = &input;

  EXPECT_EQ( BnNode::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_FALSE( node->is_logic() );

  EXPECT_TRUE( node->is_primary_input() );
  EXPECT_FALSE( node->is_dff_output() );
  EXPECT_EQ( iid, node->input_id() );
  EXPECT_THROW( node->dff_id(),
		std::logic_error );

  EXPECT_THROW( node->func_id(),
		std::logic_error );
  EXPECT_EQ( 0, node->fanin_num() );
  EXPECT_THROW( node->fanin_id(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<SizeType>{}, node->fanin_id_list() );
}

TEST( NodeImplTest, dff_output )
{
  SizeType dff_id = 10;
  NodeImpl_DffOutput dff_output(dff_id);
  NodeImpl* node = &dff_output;

  EXPECT_EQ( BnNode::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_FALSE( node->is_logic() );

  EXPECT_FALSE( node->is_primary_input() );
  EXPECT_TRUE( node->is_dff_output() );
  EXPECT_THROW( node->input_id(),
		std::logic_error );
  EXPECT_EQ( dff_id, node->dff_id() );

  EXPECT_THROW( node->func_id(),
		std::logic_error );
  EXPECT_EQ( 0, node->fanin_num() );
  EXPECT_THROW( node->fanin_id(0),
		std::out_of_range );
  EXPECT_EQ( std::vector<SizeType>{}, node->fanin_id_list() );
}

TEST( NodeImplTest, logic )
{
  SizeType func_id = 7;
  std::vector<SizeType> fanin_list{0, 1, 4};
  NodeImpl_Logic logic(func_id, fanin_list);
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

  EXPECT_EQ( func_id, node->func_id() );
  EXPECT_EQ( fanin_list.size(), node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin_id(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_id_list() );
}

END_NAMESPACE_YM_BN
