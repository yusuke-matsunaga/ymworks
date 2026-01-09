
/// @file AigMgrImpl_test.cc
/// @brief AigMgrImpl_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "AigMgrImpl.h"


BEGIN_NAMESPACE_YM_AIG

TEST(AigMgrImplTest, constr)
{
  AigMgrImpl mgr;

  EXPECT_EQ( 0, mgr.node_num() );
  EXPECT_EQ( 0, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
  EXPECT_EQ( 0, mgr.and_num() );
}

TEST(AigMgrImplTest, new_input)
{
  AigMgrImpl mgr;

  auto h = mgr.new_input();
  EXPECT_FALSE( h.is_zero() );
  EXPECT_FALSE( h.is_one() );
  EXPECT_FALSE( h.is_const() );
  EXPECT_TRUE ( h.is_input() );
  EXPECT_TRUE ( h.is_primary_input() );
  EXPECT_FALSE( h.is_dff_output() );
  EXPECT_EQ( 0, h.input_id() );

  auto node1 = mgr.input_node(0);
  EXPECT_EQ( h.node(), node1 );

  EXPECT_EQ( 1, mgr.node_num() );
  EXPECT_EQ( 1, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
  EXPECT_EQ( 0, mgr.and_num() );
}

TEST(AigMgrImplTest, new_and)
{
  AigMgrImpl mgr;

  auto e1 = mgr.new_input();
  auto e2 = mgr.new_input();
  auto e3 = mgr.new_and(e1, e2);
  EXPECT_FALSE( e3.is_zero() );
  EXPECT_FALSE( e3.is_one() );
  EXPECT_FALSE( e3.is_const() );
  EXPECT_FALSE( e3.is_input() );
  EXPECT_TRUE( e3.is_and() );

  EXPECT_EQ( 2, e3.node()->id() );

  auto e4 = mgr.new_and(e1, e2);
  EXPECT_EQ( e3, e4 );

  auto e5 = mgr.new_and(e2, e1);
  EXPECT_EQ( e3, e5);

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 2, mgr.input_num() );
  EXPECT_EQ( 0, mgr.output_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
  EXPECT_EQ( 0, mgr.and_num() );
}

TEST(AigMgrImplTest, new_output)
{
  AigMgrImpl mgr;

  auto e1 = mgr.new_input();
  auto e2 = mgr.new_input();

  auto e = mgr.new_and(e1, e2);

  auto name = "Output1";
  auto output = mgr.new_output(e, name);

  EXPECT_EQ( e, output->edge() );
  EXPECT_TRUE( output->is_output() );
  EXPECT_FALSE( output->is_dff_input() );
  EXPECT_EQ( name, mgr.output_name(output->output_id()) );

  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 2, mgr.input_num() );
  EXPECT_EQ( 1, mgr.output_num() );
  EXPECT_EQ( 0, mgr.dff_num() );
  EXPECT_EQ( 1, mgr.and_num() );
}

TEST(AigMgrImplTest, new_dff)
{
  AigMgrImpl mgr;

  auto name = "DFF1";
  auto dff_id = mgr.new_dff(name);
  EXPECT_EQ( name, mgr.dff_name(dff_id) );

  auto dff_output = mgr.dff_output(dff_id);
  EXPECT_TRUE( dff_output->is_dff_output() );
  EXPECT_EQ( dff_id, dff_output->dff_id() );

  auto e = mgr.new_input();
  mgr.set_dff_input(dff_id, e);

  auto dff_input = mgr.dff_input(dff_id);
  EXPECT_EQ( e, dff_input->edge() );
  EXPECT_FALSE( dff_input->is_output() );
  EXPECT_TRUE( dff_input->is_dff_input() );
}

TEST(AigMgrImplTest, cleanup)
{
  AigMgrImpl mgr;

  auto e1 = mgr.new_input();
  auto e2 = mgr.new_input();
  auto e3 = mgr.new_input();

  {
    auto e4 = mgr.new_and(e1, e2);
    auto e5 = mgr.new_and(e3, e4);
    EXPECT_TRUE( e5.is_and() );

    EXPECT_EQ( 4, e5.node()->id() );
    EXPECT_EQ( 5, mgr.node_num() );
    EXPECT_EQ( 0, mgr.and_num() );
  }
  mgr.cleanup();
  EXPECT_EQ( 3, mgr.node_num() );
  EXPECT_EQ( 0, mgr.and_num() );
  {
    auto e6 = mgr.new_and(e1, e2);
    auto e7 = mgr.new_and(e3, e6);
    EXPECT_TRUE( e7.is_and() );

    EXPECT_EQ( 4, e7.node()->id() );
    EXPECT_EQ( 5, mgr.node_num() );
    EXPECT_EQ( 0, mgr.and_num() );
  }
}

END_NAMESPACE_YM_AIG
