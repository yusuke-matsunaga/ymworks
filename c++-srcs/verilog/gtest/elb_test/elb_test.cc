
/// @file elb_test.cc
/// @brief Elaborator のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ElaboratorTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ElaboratorTest, elb_udp1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto src_filename = data_dir / "udp1.v";
  auto ref_filename = data_dir / "udp1.vldump";

  check(src_filename, ref_filename);
}

TEST_F(ElaboratorTest, elb_and1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto src_filename = data_dir / "and1.v";
  auto ref_filename = data_dir / "and1.vldump";

  check(src_filename, ref_filename);
}

TEST_F(ElaboratorTest, elb_and2)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto src_filename = data_dir / "and2.v";
  auto ref_filename = data_dir / "and2.vldump";

  check(src_filename, ref_filename);
}

END_NAMESPACE_YM_VERILOG
