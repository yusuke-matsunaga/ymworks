
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

TEST( ModelImplTest, alloc_node )
{
  ModelImpl model;

  auto id = model.alloc_node();
  EXPECT_EQ( 0, id );
}

TEST( ModelImplTest, new_input )
{
  ModelImpl model;

  auto id = model.new_input();
  auto& node = model.node_impl(id);
  EXPECT_EQ( BnNode::INPUT, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_TRUE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
}

TEST( ModelImplTest, new_dff_output )
{
  ModelImpl model;

  auto dff_id = model.new_dff();
  auto id = model.new_dff_output(dff_id);
  auto& node = model.node_impl(id);
  EXPECT_EQ( BnNode::INPUT, node.type() );
  EXPECT_TRUE( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_TRUE( node.is_dff_output() );
}

TEST( ModelImplTest, new_output )
{
  ModelImpl model;

  auto id = model.new_input();

  auto oid = model.new_output(id);

  EXPECT_EQ( 1, model.output_num() );
  EXPECT_EQ( id, model.output_id(oid) );
}

TEST( ModelImplTest, new_logic )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  std::vector<SizeType> fanin_list{id1, id2};
  auto input_num = fanin_list.size();
  auto type = PrimType::Xor;
  auto func_id = model.reg_primitive(input_num, type);
  auto id3 = model.new_logic(func_id, fanin_list);

  auto& node = model.node_impl(id3);
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  auto func_id1 = node.func_id();
  EXPECT_EQ( func_id, func_id1 );
  EXPECT_EQ( input_num, node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin_id(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_id_list() );
}

TEST( ModelImplTest, set_input )
{
  ModelImpl model;

  auto id = model.alloc_node();
  model.set_input(id);
  auto& node = model.node_impl(id);
  EXPECT_EQ( BnNode::INPUT, node.type() );
}

TEST( ModelImplTest, set_logic )
{
  ModelImpl model;

  auto id1 = model.new_input();
  auto id2 = model.new_input();
  auto id3 = model.alloc_node();
  auto fanin_list = std::vector<SizeType>{id1, id2};
  auto input_num = fanin_list.size();
  auto prim_type = PrimType::Xor;
  auto func_id = model.reg_primitive(input_num, prim_type);
  model.set_logic(id3, func_id, fanin_list);

  auto& node = model.node_impl(id3);
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    EXPECT_EQ( fanin_list[i], node.fanin_id(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_id_list() );
  EXPECT_EQ( func_id, node.func_id() );
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

  SizeType src_id = 10;
  auto dff_id = model.new_dff();
  model.set_dff_src(dff_id, src_id);
  auto& dff = model.dff_impl(dff_id);
  EXPECT_EQ( src_id, dff.src_id );
}

END_NAMESPACE_YM_BN
