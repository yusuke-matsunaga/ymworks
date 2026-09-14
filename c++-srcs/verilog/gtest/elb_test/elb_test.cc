
/// @file elb_test.cc
/// @brief Elaborator のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ElbTest.h"


BEGIN_NAMESPACE_YM

TEST_F(ElbTest, elb_udp1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "udp1.v";

  auto r = vlmgr.read_file(filename);
  ASSERT_TRUE( r );

  auto nerr = vlmgr.elaborate();
  ASSERT_TRUE( nerr == 0 );
}

TEST_F(ElbTest, elb_and1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "and1.v";

  auto r = vlmgr.read_file(filename);
  ASSERT_TRUE( r );

  auto nerr = vlmgr.elaborate();
  ASSERT_TRUE( nerr == 0 );
}

TEST_F(ElbTest, elb_and2)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "and2.v";

  auto r = vlmgr.read_file(filename);
  ASSERT_TRUE( r );

  auto nerr = vlmgr.elaborate();
  ASSERT_TRUE( nerr == 0 );
}

END_NAMESPACE_YM
