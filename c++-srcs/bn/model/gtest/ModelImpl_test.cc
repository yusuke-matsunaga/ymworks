
/// @file ModelImpl_test.cc
/// @brief ModelImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ModelImpl.h"
#include "ym/SopCover.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_YM_BN

TEST( ModelImplTest, constructor1 )
{
  ModelImpl model;

  EXPECT_EQ( std::string{}, model.name() );
  EXPECT_EQ( std::vector<std::string>{}, model.comment_list() );
  EXPECT_EQ( 0, model.node_num() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.dff_num() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( 0, model.func_num() );
}

TEST( ModelImplTest, set_name )
{
  ModelImpl model;

  std::string name{"abcd"};

  model.set_name(name);

  EXPECT_EQ( name, model.name() );
}

TEST( ModelImplTest, add_comment )
{
  ModelImpl model;

  std::string comment{"abcd"};

  model.add_comment(comment);

  auto& comment_list = model.comment_list();
  ASSERT_EQ( 1, comment_list.size() );
  EXPECT_EQ( comment, comment_list[0] );
}

TEST( ModelImplTest, set_output_name )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto oid = model.new_output(id1);
  std::string name{"abc"};

  model.set_output_name(oid, name);

  EXPECT_EQ( name, model.output_name(oid) );
}

TEST( ModelImplTest, new_input )
{
  ModelImpl model;

  auto node = model.new_input();
  EXPECT_EQ( BnNode::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_TRUE( node->is_primary_input() );
  EXPECT_FALSE( node->is_dff_output() );
}

TEST( ModelImplTest, new_dff_output )
{
  ModelImpl model;

  auto dff_id = model.new_dff();
  auto node = model.new_dff_output(dff_id);
  EXPECT_EQ( BnNode::INPUT, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_FALSE( node->is_primary_input() );
  EXPECT_TRUE( node->is_dff_output() );
}

TEST( ModelImplTest, new_output )
{
  ModelImpl model;

  auto node = model.new_input();

  auto oid = model.new_output(node);

  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( node, model.output(oid) );
}

TEST( ModelImplTest, new_logic )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  std::vector<const NodeImpl*> fanin_list{id1, id2};
  auto input_num = fanin_list.size();
  auto type = PrimType::Xor;
  auto func = model.reg_primitive(input_num, type);
  auto node = model.new_logic(func, fanin_list);

  EXPECT_EQ( BnNode::LOGIC, node->type() );
  auto func1 = node->func();
  EXPECT_EQ( func, func1 );
  EXPECT_EQ( input_num, node->fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node->fanin(i) );
  }
  EXPECT_EQ( fanin_list, node->fanin_list() );
}

TEST( ModelImplTest, set_dff_name )
{
  ModelImpl model;

  auto dff_id = model.new_dff();
  auto name = std::string{"abcd"};

  model.set_dff_name(dff_id, name);

  EXPECT_EQ( name, model.dff_name(dff_id) );
}

TEST( ModelImplTest, set_dff_src )
{
  ModelImpl model;

  auto src = model.new_input();
  auto dff_id = model.new_dff();
  model.set_dff_src(dff_id, src);
  auto dff = model.dff_impl(dff_id);
  EXPECT_EQ( src, dff->src() );
}

END_NAMESPACE_YM_BN
