
/// @file BnModel_test.cc
/// @brief BnModel_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnFunc.h"
#include "ym/SopCover.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddVar.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_YM_BN

TEST( BnModelTest, constructor1 )
{
  BnModel model;

  EXPECT_EQ( std::string{}, model.name() );
  EXPECT_EQ( std::vector<std::string>{}, model.comment_list() );
  EXPECT_EQ( 0, model.node_num() );
  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.input_list().size() );
  EXPECT_EQ( 0, model.output_num() );
  EXPECT_EQ( 0, model.output_list().size() );
  EXPECT_EQ( 0, model.logic_num() );
  EXPECT_EQ( 0, model.logic_list().size() );
  EXPECT_EQ( 0, model.dff_num() );
  EXPECT_EQ( 0, model.func_num() );
}

TEST( BnModelTest, input_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input(0);}, std::out_of_range );
}

TEST( BnModelTest, input_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.input_name(0);}, std::out_of_range );
}

TEST( BnModelTest, output_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output(0);}, std::out_of_range );
}

TEST( BnModelTest, output_name_bad )
{
  BnModel model;

  EXPECT_THROW( {model.output_name(0);}, std::out_of_range );
}

TEST( BnModelTest, logic_bad )
{
  BnModel model;

  EXPECT_THROW( {model.logic(0);}, std::out_of_range );
}

TEST( BnModelTest, dff_bad )
{
  BnModel model;

  EXPECT_THROW( {model.dff(0);}, std::out_of_range );
}

TEST( BnModelTest, func_bad )
{
  BnModel model;

  EXPECT_THROW( {model.func(0);}, std::out_of_range );
}

TEST( BnModelTest, new_input )
{
  BnModel model;

  auto name = "input1";
  auto iid = model.input_num();
  auto node = model.new_input(name);

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::INPUT, node.type() );
  EXPECT_TRUE ( node.is_input() );
  EXPECT_TRUE ( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_EQ( iid, node.input_id() );
  EXPECT_THROW( node.dff_id(),
		std::logic_error );

  EXPECT_EQ( name, model.input_name(iid) );

  EXPECT_THROW( node.func(),
		std::logic_error );

  EXPECT_EQ( 0, node.fanin_num() );
  EXPECT_THROW( node.fanin(0),
		std::out_of_range );
  EXPECT_EQ( 0, node.fanin_list().size() );
}

TEST( BnModelTest, new_dff)
{
  BnModel model;

  auto name = "dff1";
  char reset_val = '0';
  auto dff_id = model.dff_num();
  auto dff = model.new_dff(name, reset_val);

  ASSERT_TRUE( dff.is_valid() );
  EXPECT_EQ( dff_id, dff.id() );
  EXPECT_EQ( name, model.dff_name(dff_id) );
  EXPECT_EQ( reset_val, reset_val );

  auto node = dff.output();

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::INPUT, node.type() );
  EXPECT_TRUE ( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_TRUE ( node.is_dff_output() );
  EXPECT_FALSE( node.is_logic() );
  EXPECT_THROW( node.input_id(),
		std::logic_error );
  EXPECT_EQ( dff_id, node.dff_id() );

  EXPECT_THROW( node.func(),
		std::logic_error );

  EXPECT_EQ( 0, node.fanin_num() );
  EXPECT_THROW( node.fanin(0),
		std::out_of_range );
  EXPECT_EQ( 0, node.fanin_list().size() );
}

TEST( BnModelTest, new_primitive )
{
  BnModel model;

  auto type = PrimType::And;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto node = model.new_primitive(type, fanin_list);

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_TRUE ( node.is_logic() );
  EXPECT_THROW( node.input_id(),
		std::logic_error );
  EXPECT_THROW( node.dff_id(),
		std::logic_error );

  auto func = node.func();
  EXPECT_EQ( fanin_list.size(), func.input_num() );
  EXPECT_EQ( BnFunc::PRIMITIVE, func.type() );
  EXPECT_EQ( "PRIMITIVE", func.type_str() );
  EXPECT_TRUE ( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_EQ( type, func.primitive_type() );
  EXPECT_THROW( func.input_cover(),
		std::logic_error );
  EXPECT_THROW( func.output_inv(),
		std::logic_error );
  EXPECT_THROW( func.expr(),
		std::logic_error );
  EXPECT_THROW( func.tvfunc(),
		std::logic_error );
  EXPECT_THROW( func.bdd(),
		std::logic_error );

  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list().to_vector() );
}

TEST( BnModelTest, new_cover )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto lit0 = Literal{0, false};
  auto lit1 = Literal{1, false};
  auto input_cover = SopCover(input_num, {{lit0, lit1}});
  auto output_inv = true;

  auto node = model.new_cover(input_cover, output_inv, fanin_list);

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_TRUE ( node.is_logic() );
  EXPECT_THROW( node.input_id(),
		std::logic_error );
  EXPECT_THROW( node.dff_id(),
		std::logic_error );

  auto func = node.func();
  EXPECT_EQ( input_num, func.input_num() );
  EXPECT_EQ( BnFunc::COVER, func.type() );
  EXPECT_EQ( "COVER", func.type_str() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_TRUE ( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_THROW( func.primitive_type(),
		std::logic_error );
  EXPECT_EQ( input_cover, func.input_cover() );
  EXPECT_EQ( output_inv, func.output_inv() );
  EXPECT_THROW( func.expr(),
		std::logic_error );
  EXPECT_THROW( func.tvfunc(),
		std::logic_error );
  EXPECT_THROW( func.bdd(),
		std::logic_error );

  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list().to_vector() );
}

TEST( BnModelTest, new_cover_bad )
{
  // input_cover の入力数と fanin_list のサイズが異なるケース
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};

  auto lit0 = Literal{0, false};
  auto lit1 = Literal{1, false};
  auto lit2 = Literal{2, false};
  auto input_cover = SopCover(3, {{lit0, lit1}, {lit2}});
  auto output_inv = true;

  EXPECT_THROW( model.new_cover(input_cover, output_inv, fanin_list),
		std::invalid_argument );
}

TEST( BnModelTest, new_expr )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto v0 = Expr::literal(0);
  auto v1 = Expr::literal(1);
  auto expr = v0 | ~v1;

  auto node = model.new_expr(expr, fanin_list);

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_TRUE ( node.is_logic() );
  EXPECT_THROW( node.input_id(),
		std::logic_error );
  EXPECT_THROW( node.dff_id(),
		std::logic_error );

  auto func = node.func();
  EXPECT_EQ( input_num, func.input_num() );
  EXPECT_EQ( BnFunc::EXPR, func.type() );
  EXPECT_EQ( "EXPR", func.type_str() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_TRUE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_THROW( func.primitive_type(),
		std::logic_error );
  EXPECT_THROW( func.input_cover(),
		std::logic_error );
  EXPECT_THROW( func.output_inv(),
		std::logic_error );
  EXPECT_EQ( expr, func.expr() );
  EXPECT_THROW( func.tvfunc(),
		std::logic_error );
  EXPECT_THROW( func.bdd(),
		std::logic_error );

  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list().to_vector() );

}

TEST( BnModelTest, new_expr_bad )
{
  // expr の入力数と fanin_list のサイズが異なるケース
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};

  auto v0 = Expr::literal(0);
  auto v1 = Expr::literal(1);
  auto v2 = Expr::literal(2);
  auto expr = v0 | ~v1 & v2;

  EXPECT_THROW( model.new_expr(expr, fanin_list),
		std::invalid_argument );
}

TEST( BnModelTest, new_tvfunc )
{
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto input3 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2, input3};
  auto input_num = fanin_list.size();

  auto v0 = TvFunc::positive_literal(input_num, 0);
  auto v1 = TvFunc::positive_literal(input_num, 1);
  auto v2 = TvFunc::positive_literal(input_num, 2);
  auto tvfunc = v0 | v1 | v2;

  auto node = model.new_tvfunc(tvfunc, fanin_list);

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_TRUE ( node.is_logic() );
  EXPECT_THROW( node.input_id(),
		std::logic_error );
  EXPECT_THROW( node.dff_id(),
		std::logic_error );

  auto func = node.func();
  EXPECT_EQ( input_num, func.input_num() );
  EXPECT_EQ( BnFunc::TVFUNC, func.type() );
  EXPECT_EQ( "TVFUNC", func.type_str() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_TRUE ( func.is_tvfunc() );
  EXPECT_FALSE( func.is_bdd() );
  EXPECT_THROW( func.primitive_type(),
		std::logic_error );
  EXPECT_THROW( func.input_cover(),
		std::logic_error );
  EXPECT_THROW( func.output_inv(),
		std::logic_error );
  EXPECT_THROW( func.expr(),
		std::logic_error );
  EXPECT_EQ( tvfunc, func.tvfunc() );
  EXPECT_THROW( func.bdd(),
		std::logic_error );

  EXPECT_EQ( fanin_list.size(), node.fanin_num() );
  for ( SizeType i = 0; i < fanin_list.size(); ++ i ) {
    auto node1 = fanin_list[i];
    EXPECT_EQ( node1, node.fanin(i) );
  }
  EXPECT_EQ( fanin_list, node.fanin_list().to_vector() );

}

TEST( BnModelTest, new_tvfunc_bad )
{
  // tvfunc の入力数と fanin_list のサイズが異なるケース
  BnModel model;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};

  auto v0 = TvFunc::positive_literal(3, 0);
  auto v1 = TvFunc::positive_literal(3, 1);
  auto v2 = TvFunc::positive_literal(3, 2);
  auto tvfunc = v0 | v1 | v2;

  EXPECT_THROW( model.new_tvfunc(tvfunc, fanin_list),
		std::invalid_argument );
}

TEST( BnModelTest, new_bdd )
{
  BnModel model;

  BddMgr mgr;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto bdd = var0 & ~var1;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  auto node = model.new_bdd(bdd, fanin_list);

  ASSERT_TRUE( node.is_valid() );
  EXPECT_EQ( BnNode::LOGIC, node.type() );
  EXPECT_FALSE( node.is_input() );
  EXPECT_FALSE( node.is_primary_input() );
  EXPECT_FALSE( node.is_dff_output() );
  EXPECT_TRUE ( node.is_logic() );
  EXPECT_THROW( node.input_id(),
		std::logic_error );
  EXPECT_THROW( node.dff_id(),
		std::logic_error );

  auto func = node.func();
  EXPECT_EQ( 2, func.input_num() );
  EXPECT_EQ( BnFunc::BDD, func.type() );
  EXPECT_EQ( "BDD", func.type_str() );
  EXPECT_FALSE( func.is_primitive() );
  EXPECT_FALSE( func.is_cover() );
  EXPECT_FALSE( func.is_expr() );
  EXPECT_FALSE( func.is_tvfunc() );
  EXPECT_TRUE ( func.is_bdd() );
  EXPECT_THROW( func.primitive_type(),
		std::logic_error );
  EXPECT_THROW( func.input_cover(),
		std::logic_error );
  EXPECT_THROW( func.output_inv(),
		std::logic_error );
  EXPECT_THROW( func.expr(),
		std::logic_error );
  EXPECT_THROW( func.tvfunc(),
		std::logic_error );
  EXPECT_EQ( bdd, mgr.copy(func.bdd()) );
}

TEST( BnModelTest, new_bdd_bad )
{
  // bdd の入力数と fanin_list のサイズが異なるケース
  BnModel model;

  BddMgr mgr;
  auto var0 = mgr.variable_bdd(0);
  auto var1 = mgr.variable_bdd(1);
  auto var2 = mgr.variable_bdd(2);
  auto bdd = var0 & ~var1 | var2;

  auto input1 = model.new_input();
  auto input2 = model.new_input();
  auto fanin_list = std::vector<BnNode>{input1, input2};
  auto input_num = fanin_list.size();

  EXPECT_THROW( model.new_bdd(bdd, fanin_list),
		std::invalid_argument );
}

TEST( BnModelTest, fsm1 )
{
  auto model = BnModel();

  auto input1 = model.new_input();
  auto dff1 = model.new_dff();
  auto dff1_output = dff1.output();
  auto fanin_list = std::vector<BnNode>{input1, dff1_output};
  auto and_node = model.new_primitive(PrimType::And, fanin_list);
  model.set_dff_src(dff1, and_node);
  model.new_output(dff1_output);

  EXPECT_EQ( std::vector<BnNode>{input1}, model.input_list().to_vector() );
  EXPECT_EQ( std::vector<BnNode>{dff1_output}, model.output_list().to_vector() );
  EXPECT_EQ( std::vector<BnNode>{and_node}, model.logic_list().to_vector() );
  EXPECT_EQ( std::vector<BnDff>{dff1}, model.dff_list().to_vector() );

  std::ostringstream buf;
  model.write(buf);

  const char* exp_str =
    "I#0: N#0\n"
    "O#0: N#1\n"
    "Q#0: output = N#1, src = N#2\n"
    "N#2 = F#0(N#0, N#1)\n"
    "F#0: Primitive: And(2)\n";
  EXPECT_EQ( exp_str, buf.str() );
}

END_NAMESPACE_YM_BN
