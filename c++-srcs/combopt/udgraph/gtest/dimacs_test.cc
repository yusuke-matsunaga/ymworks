
/// @file dimacs_test.cc
/// @brief dimacs_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/UdGraph.h"


BEGIN_NAMESPACE_YM

TEST(UdGraphTest, read_dimacs_error0)
{
  auto filename = "bad_filename";
  bool caught = false;
  EXPECT_THROW( UdGraph::read_dimacs(filename),
		std::invalid_argument );
}

TEST(UdGraphTest, read_dimacs)
{
  auto data_dir = std::filesystem::path{TESTDATA_DIR} / "combopt" / "udgraph";
  auto filename = data_dir / "anna.col";
  std::ifstream s{filename};
  ASSERT_TRUE( s );

  auto graph = UdGraph::read_dimacs(s);

  EXPECT_EQ( 138, graph.node_num() );
  EXPECT_EQ( 986, graph.edge_num() );

  std::ostringstream obuf;
  graph.write_dimacs(obuf);

  std::istringstream s1(obuf.str());
  auto graph2 = UdGraph::read_dimacs(s1);

  std::ostringstream obuf2;
  graph2.write_dimacs(obuf2);

  EXPECT_EQ( obuf2.str(), obuf.str() );
}

TEST(UdGraphTest, read_dimacs2)
{
  auto data_dir = std::filesystem::path{TESTDATA_DIR} / "combopt" / "udgraph";
  auto filename = data_dir / "johnson8-2-4.clq";
  std::ifstream s{filename};
  ASSERT_TRUE( s );

  auto graph = UdGraph::read_dimacs(s);

  EXPECT_EQ( 28, graph.node_num() );
  EXPECT_EQ( 210, graph.edge_num() );

  std::ostringstream obuf;
  graph.write_dimacs(obuf);

  std::istringstream s1(obuf.str());
  auto graph2 = UdGraph::read_dimacs(s1);

  std::ostringstream obuf2;
  graph2.write_dimacs(obuf2);

  EXPECT_EQ( obuf2.str(), obuf.str() );
}

END_NAMESPACE_YM
