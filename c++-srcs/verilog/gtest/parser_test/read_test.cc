
/// @file parser_test.cc
/// @brief parser_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "parser/AstMgr.h"
#include "parser/AstDumper.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST(ParserTest, read1)
{
  AstMgr mgr;
  Parser parser(mgr);

  auto data_dir = std::filesystem::path{DATA_DIR} / "verilog";
  auto filename = data_dir / "udp1.v";

  auto r = parser.read_file(filename);
  ASSERT_TRUE( r );

  AstDumper dumper(std::cout);

  dumper.put(mgr.udp_list(), mgr.module_list());

}

END_NAMESPACE_YM_VERILOG
