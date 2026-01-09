
/// @file read_aig_test.cc
/// @brief read_aig_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

TEST( AigMgrTest, read_aag1)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "aig";
  auto filepath = path / "test1.aag";

  auto mgr = AigMgr::read_aag(filepath);

  const SizeType ni = 3;
  const SizeType no = 1;
  const SizeType nd = 0;
  const SizeType ng = 2;

  EXPECT_EQ( ni, mgr.input_num() );
  EXPECT_EQ( no, mgr.output_num() );
  EXPECT_EQ( nd, mgr.dff_num() );
  EXPECT_EQ( ng, mgr.and_num() );

  std::ostringstream buf;
  mgr.print(buf);

  auto exp_str = std::string{
    "Node#0: Input#0 [1]-> Node#3\n"
    "Node#1: Input#1 [1]-> Node#3\n"
    "Node#2: Input#2 [1]-> Node#4\n"
    "Node#3: And(Node#0, Node#1) [1]-> Node#4\n"
    "Node#4: And(Node#2, Node#3) [1]-> Output#0\n"
    "Input#0: Node#0[]\n"
    "Input#1: Node#1[]\n"
    "Input#2: Node#2[]\n"
    "Output#0[] = Node#4\n"
    "# of inputs:  3\n"
    "# of outputs: 1\n"
    "# of ANDs:    2( 5 )\n"
  };
  EXPECT_EQ( exp_str, buf.str() );
}

TEST( AigMgrTest, read_aag3)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "aig";
  auto filepath = path / "test3.aag";

  auto mgr = AigMgr::read_aag(filepath);

  const SizeType ni = 2;
  const SizeType no = 2;
  const SizeType nd = 1;
  const SizeType ng = 4;

  EXPECT_EQ( ni, mgr.input_num() );
  EXPECT_EQ( no, mgr.output_num() );
  EXPECT_EQ( nd, mgr.dff_num() );
  EXPECT_EQ( ng, mgr.and_num() );

  std::stringstream buf;
  mgr.print(buf);

  auto exp_str = std::string{
  "Node#0: Input#0 [2]-> Node#5 Node#6\n"
  "Node#1: Input#1 [1]-> Node#3\n"
  "Node#2: DFF#0 [4]-> Output#0 Output#1 Node#5 Node#6\n"
  "Node#3: And(Node#1, Node#4) [1]-> DFF#0\n"
  "Node#4: And(~Node#5, ~Node#6) [1]-> Node#3\n"
  "Node#5: And(Node#0, Node#2) [1]-> Node#4\n"
  "Node#6: And(~Node#0, ~Node#2) [1]-> Node#4\n"
  "Input#0: Node#0[]\n"
  "Input#1: Node#1[]\n"
  "Dff#0: Node#2[] = Node#3\n"
  "Output#0[] = Node#2\n"
  "Output#1[] = ~Node#2\n"
  "# of inputs:  2\n"
  "# of outputs: 2\n"
  "# of ANDs:    4( 7 )\n"
  };
  EXPECT_EQ( exp_str, buf.str() );
}

TEST( AigMgrTest, read_aig1)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "aig";
  auto filepath = path / "test1.aig";

  auto mgr = AigMgr::read_aig(filepath);

  const SizeType ni = 3;
  const SizeType no = 1;
  const SizeType nd = 0;
  const SizeType ng = 2;

  EXPECT_EQ( ni, mgr.input_num() );
  EXPECT_EQ( no, mgr.output_num() );
  EXPECT_EQ( nd, mgr.dff_num() );
  EXPECT_EQ( ng, mgr.and_num() );

  std::stringstream buf;
  mgr.print(buf);

  auto exp_str = std::string{
  "Node#0: Input#0 [1]-> Node#3\n"
  "Node#1: Input#1 [1]-> Node#3\n"
  "Node#2: Input#2 [1]-> Node#4\n"
  "Node#3: And(Node#1, Node#0) [1]-> Node#4\n"
  "Node#4: And(Node#3, Node#2) [1]-> Output#0\n"
  "Input#0: Node#0[]\n"
  "Input#1: Node#1[]\n"
  "Input#2: Node#2[]\n"
  "Output#0[] = Node#4\n"
  "# of inputs:  3\n"
  "# of outputs: 1\n"
  "# of ANDs:    2( 5 )\n"
  };
  EXPECT_EQ( exp_str, buf.str() );
}

TEST( AigMgrTest, read_aig3)
{
  // 普通のファイルの読み込みテスト
  auto path = std::filesystem::path{DATAPATH} / "aig";
  auto filepath = path / "test3.aig";

  auto mgr = AigMgr::read_aig(filepath);

  const SizeType ni = 2;
  const SizeType no = 2;
  const SizeType nd = 1;
  const SizeType ng = 4;

  EXPECT_EQ( ni, mgr.input_num() );
  EXPECT_EQ( no, mgr.output_num() );
  EXPECT_EQ( nd, mgr.dff_num() );
  EXPECT_EQ( ng, mgr.and_num() );

  std::stringstream buf;
  mgr.print(buf);

  auto exp_str = std::string{
  "Node#0: Input#0 [2]-> Node#3 Node#4\n"
  "Node#1: Input#1 [1]-> Node#6\n"
  "Node#2: DFF#0 [4]-> Output#0 Output#1 Node#3 Node#4\n"
  "Node#3: And(Node#2, Node#0) [1]-> Node#5\n"
  "Node#4: And(~Node#2, ~Node#0) [1]-> Node#5\n"
  "Node#5: And(~Node#4, ~Node#3) [1]-> Node#6\n"
  "Node#6: And(Node#5, Node#1) [1]-> DFF#0\n"
  "Input#0: Node#0[]\n"
  "Input#1: Node#1[]\n"
  "Dff#0: Node#2[] = Node#6\n"
  "Output#0[] = Node#2\n"
  "Output#1[] = ~Node#2\n"
  "# of inputs:  2\n"
  "# of outputs: 2\n"
  "# of ANDs:    4( 7 )\n"
  };
  EXPECT_EQ( exp_str, buf.str() );
}

END_NAMESPACE_YM
