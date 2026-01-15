
/// @file BnModel_copymove_test.cc
/// @brief BnModel_copymove_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnModel.h"
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnSeq.h"
#include "ym/BnFunc.h"
#include "ym/SopCover.h"
#include "ym/TvFunc.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/ClibCellLibrary.h"
#include "ym/ClibCell.h"


BEGIN_NAMESPACE_YM_BN

class BnModelCopyMoveTest :
  public ::testing::Test
{
public:

  void
  SetUp()
  {
    auto input1 = model.new_input();
    auto input2 = model.new_input();
    auto input3 = model.new_input();

    {
      vector<BnNode> fanin_list{input1, input2};
      PrimType type = PrimType::And;
      auto node = model.new_primitive(type, fanin_list);
    }
    {
      bool inv0 = false;
      bool inv1 = true;
      auto node = model.new_aig(input1, input2, inv0, inv1);
    }
    {
      Literal lit1{0, false};
      Literal lit2{1, false};
      vector<vector<Literal>> cube_list{{lit1}, {lit2}};
      auto func = model.reg_cover(2, cube_list, '0');
      vector<BnNode> fanin_list{input1, input2};
      auto node = model.new_func(func, fanin_list);
    }
    {
      auto lit1 = Expr::make_posi_literal(0);
      auto lit2 = Expr::make_posi_literal(1);
      auto expr = lit1 | lit2;
      auto func = model.reg_expr(expr);
      vector<BnNode> fanin_list{input1, input2};
      auto node = model.new_func(func, fanin_list);
    }
    {
      auto v0 = TvFunc::make_posi_literal(3, 0);
      auto v1 = TvFunc::make_posi_literal(3, 1);
      auto v2 = TvFunc::make_posi_literal(3, 2);
      auto tvfunc = v0 | v1 | v2;
      auto func = model.reg_tvfunc(tvfunc);
      vector<BnNode> fanin_list{input1, input2, input3};
      auto node = model.new_func(func, fanin_list);
    }
    {
      BddMgr mgr;
      auto v0 = mgr.posi_literal(0);
      auto v1 = mgr.posi_literal(1);
      auto bdd = v0 | v1;
      auto func = model.reg_bdd(bdd);
      vector<BnNode> fanin_list{input1, input2};
      auto node = model.new_func(func, fanin_list);
    }
  }


public:

  BnModel model;

};


TEST_F( BnModelCopyMoveTest, copy_constructor )
{
  EXPECT_EQ( 3, model.input_num() );
  EXPECT_EQ( 6, model.logic_num() );

  ostringstream s1;
  model.print(s1);

  BnModel copied_model{model};

  model.clear();

  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.logic_num() );

  EXPECT_EQ( 3, copied_model.input_num() );
  EXPECT_EQ( 6, copied_model.logic_num() );

  ostringstream s2;
  copied_model.print(s2);

  EXPECT_EQ( s1.str(), s2.str() );
}

TEST_F( BnModelCopyMoveTest, copy_assignment )
{
  EXPECT_EQ( 3, model.input_num() );
  EXPECT_EQ( 6, model.logic_num() );

  ostringstream s1;
  model.print(s1);

  auto copied_model = model;

  model.clear();

  EXPECT_EQ( 0, model.input_num() );
  EXPECT_EQ( 0, model.logic_num() );

  EXPECT_EQ( 3, copied_model.input_num() );
  EXPECT_EQ( 6, copied_model.logic_num() );

  ostringstream s2;
  copied_model.print(s2);

  EXPECT_EQ( s1.str(), s2.str() );
}

TEST_F( BnModelCopyMoveTest, move_constructor )
{
  EXPECT_EQ( 3, model.input_num() );
  EXPECT_EQ( 6, model.logic_num() );

  ostringstream s1;
  model.print(s1);

  BnModel moved_model{std::move(model)};

  EXPECT_EQ( 2, moved_model.input_num() );
  EXPECT_EQ( 1, moved_model.logic_num() );

  ostringstream s2;
  moved_model.print(s2);

  EXPECT_EQ( s1.str(), s2.str() );
}

TEST_F( BnModelCopyMoveTest, move_assignment )
{
  EXPECT_EQ( 3, model.input_num() );
  EXPECT_EQ( 6, model.logic_num() );

  ostringstream s1;
  model.print(s1);

  auto moved_model = std::move(model);

  EXPECT_EQ( 2, moved_model.input_num() );
  EXPECT_EQ( 1, moved_model.logic_num() );

  ostringstream s2;
  moved_model.print(s2);

  EXPECT_EQ( s1.str(), s2.str() );
}

END_NAMESPACE_YM_BN
