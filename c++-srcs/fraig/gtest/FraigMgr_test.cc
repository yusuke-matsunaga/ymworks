
/// @file FraigMgr_test.cc
/// @brief FraigMgr_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/FraigMgr.h"


BEGIN_NAMESPACE_YM

TEST(FraigMgrTest, constr)
{
  FraigMgr mgr(64);
}

TEST(FraigMgrTest, new_and)
{
  FraigMgr mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();
  auto i3 = mgr.new_input();

  auto e1 = mgr.new_and({i1, i2, i3});

  auto e2 = mgr.new_and(i1, i2);
  auto e3 = mgr.new_and(e2, i3);
  auto res1 = mgr.check_equiv(e1, e3);
  EXPECT_EQ( SatBool3::True, res1 );
}

TEST(FraigMgrTest, new_or1)
{
  FraigMgr mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();

  auto e1 = mgr.new_or(i1, i2);

  auto e2 = ~mgr.new_and(~i1, ~i2);
  auto res1 = mgr.check_equiv(e1, e2);
  EXPECT_EQ( SatBool3::True, res1 );
}

TEST(FraigMgrTest, new_or2)
{
  FraigMgr mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();
  auto i3 = mgr.new_input();

  auto e1 = mgr.new_or({i1, i2, i3});

  auto e2 = mgr.new_or(i1, i2);
  auto e3 = mgr.new_or(e2, i3);
  auto res1 = mgr.check_equiv(e1, e3);
  EXPECT_EQ( SatBool3::True, res1 );
}

TEST(FraigMgrTest, new_xor1)
{
  FraigMgr mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();

  auto e1 = mgr.new_xor(i1, i2);

  auto e2 = mgr.new_and(i1, ~i2);
  auto e3 = mgr.new_and(~i1, i2);
  auto e4 = mgr.new_or(e2, e3);
  auto res1 = mgr.check_equiv(e1, e4);
  EXPECT_EQ( SatBool3::True, res1 );
}

TEST(FraigMgrTest, new_xor2)
{
  FraigMgr mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();
  auto i3 = mgr.new_input();

  auto e1 = mgr.new_xor({i1, i2, i3});

  auto e2 = mgr.new_xor(i1, i2);
  auto e3 = mgr.new_xor(e2, i3);
  auto res1 = mgr.check_equiv(e1, e3);
  EXPECT_EQ( SatBool3::True, res1 );
}

TEST(FraigMgrTest, ite)
{
  FraigMgr mgr(64);

  auto i1 = mgr.new_input();
  auto i2 = mgr.new_input();
  auto i3 = mgr.new_input();

  auto e1 = mgr.ite(i1, i2, i3);

  auto e2 = mgr.new_and(i1, i2);
  auto e3 = mgr.new_and(~i1, i3);
  auto e4 = mgr.new_or(e2, e3);
  auto res1 = mgr.check_equiv(e1, e4);
  EXPECT_EQ( SatBool3::True, res1 );
}

END_NAMESPACE_YM
