
/// @file parser_test.cc
/// @brief parser_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest.h"
#include "parser/Parser.h"
#include "parser/AstMgr.h"
#include "parser/AstDumper.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(ParserTest, read_udp1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "udp1.v";

  auto r = parser.read_file(filename);
  ASSERT_TRUE( r );

  //AstDumper dumper(std::cout);

  //dumper.put(mgr.udp_list(), mgr.module_list());

  auto json_obj = astmgr.json_obj();

  json_obj.write(std::cout, true);
}

TEST_F(ParserTest, read_and1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "and1.v";

  auto r = parser.read_file(filename);
  ASSERT_TRUE( r );

  //AstDumper dumper(std::cout);

  //dumper.put(mgr.udp_list(), mgr.module_list());

  auto json_obj = astmgr.json_obj();

  json_obj.write(std::cout, true);
}

TEST_F(ParserTest, read_and2)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "and2.v";

  auto r = parser.read_file(filename);
  ASSERT_TRUE( r );

  //AstDumper dumper(std::cout);

  //dumper.put(mgr.udp_list(), mgr.module_list());

  auto json_obj = astmgr.json_obj();

  json_obj.write(std::cout, true);
}

TEST_F(ParserTest, read_bad1)
{
  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "bad1.v";

  auto r = parser.read_file(filename);
  ASSERT_FALSE( r );
}

END_NAMESPACE_YM_VERILOG
