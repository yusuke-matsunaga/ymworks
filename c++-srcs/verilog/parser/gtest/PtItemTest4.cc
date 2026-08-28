
/// @file PtItemTest4.cc
/// @brief PtItemTest の実装ファイルその4
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtItemTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtItemTest, Generate)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto name = "net1";
  auto net_item = factory.new_DeclItem(fr1, name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = factory.new_NetH(fr2, VpiNetType::Wire, net_item);

  auto fr_stmt = make_file_region(4, 4, 4, 4);
  auto stmt = factory.new_NullStmt(fr_stmt);
  auto fr_init = make_file_region(3, 3, 3, 3);
  auto init = factory.new_Initial(fr_init, stmt);

  auto fr = make_file_region(1, 2, 3, 4);
  auto item = factory.new_Generate(fr, net_decl, init);

  check_Generate(item, fr, {net_decl}, {init});
}

TEST_F(PtItemTest, GenBlock1)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto name = "net1";
  auto net_item = factory.new_DeclItem(fr1, name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = factory.new_NetH(fr2, VpiNetType::Wire, net_item);

  auto fr_stmt = make_file_region(4, 4, 4, 4);
  auto stmt = factory.new_NullStmt(fr_stmt);
  auto fr_init = make_file_region(3, 3, 3, 3);
  auto init = factory.new_Initial(fr_init, stmt);

  auto fr0 = make_file_region(1, 2, 3, 4);
  auto item = factory.new_GenBlock(fr0, net_decl, init);

  check_GenBlock(item, fr0, {net_decl}, {init});
}

TEST_F(PtItemTest, GenBlock2)
{
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto name = "net1";
  auto net_item = factory.new_DeclItem(fr1, name);
  auto fr2 = make_file_region(1, 1, 2, 2);
  auto net_decl = factory.new_NetH(fr2, VpiNetType::Wire, net_item);

  auto fr_stmt = make_file_region(4, 4, 4, 4);
  auto stmt = factory.new_NullStmt(fr_stmt);
  auto fr_init = make_file_region(3, 3, 3, 3);
  auto init = factory.new_Initial(fr_init, stmt);

  auto fr0 = make_file_region(1, 2, 3, 4);
  auto block_name = "block1";
  auto item = factory.new_GenBlock(fr0, block_name, net_decl, init);

  check_GenBlock(item, fr0, {net_decl}, {init}, block_name);
}

TEST_F(PtItemTest, GenIf)
{
  auto fr_net1 = make_file_region(1, 1, 1, 1);
  auto name_net1 = "net1";
  auto net1 = factory.new_DeclItem(fr_net1, name_net1);
  auto fr_nethead1 = make_file_region(1, 1, 2, 2);
  auto nethead1 = factory.new_NetH(fr_nethead1, VpiNetType::Wire, net1);

  auto fr_null1 = make_file_region(4, 4, 4, 4);
  auto null1 = factory.new_NullStmt(fr_null1);
  auto fr_init1 = make_file_region(3, 3, 3, 3);
  auto init1 = factory.new_Initial(fr_init1, null1);

  auto fr_cond = make_file_region(1, 1, 1, 1);
  auto cond = factory.new_IntConst(fr_cond, 1U);
  auto fr0 = make_file_region(1, 2, 3, 4);
  auto item = factory.new_GenIf(fr0, cond, nethead1, init1);

  check_GenIf(item, fr0, cond, {nethead1}, {init1}, {}, {});
}


TEST_F(PtItemTest, GenIfElse)
{
  auto fr_net1 = make_file_region(1, 1, 1, 1);
  auto name_net1 = "net1";
  auto net1 = factory.new_DeclItem(fr_net1, name_net1);
  auto fr_nethead1 = make_file_region(1, 1, 2, 2);
  auto nethead1 = factory.new_NetH(fr_nethead1, VpiNetType::Wire, net1);

  auto fr_null1 = make_file_region(4, 4, 4, 4);
  auto null1 = factory.new_NullStmt(fr_null1);
  auto fr_init1 = make_file_region(3, 3, 3, 3);
  auto init1 = factory.new_Initial(fr_init1, null1);

  auto fr_net2 = make_file_region(10, 1, 10, 1);
  auto name_net2 = "net2";
  auto net2 = factory.new_DeclItem(fr_net2, name_net2);
  auto fr_nethead2 = make_file_region(10, 1, 12, 2);
  auto nethead2 = factory.new_NetH(fr_nethead2, VpiNetType::Wire, net2);

  auto fr_null2 = make_file_region(14, 4, 14, 4);
  auto null2 = factory.new_NullStmt(fr_null2);
  auto fr_init2 = make_file_region(13, 3, 13, 3);
  auto init2 = factory.new_Initial(fr_init2, null2);

  auto fr0 = make_file_region(1, 2, 3, 4);
  auto fr_cond = make_file_region(1, 1, 1, 1);
  auto cond = factory.new_IntConst(fr_cond, 1U);
  auto item = factory.new_GenIfElse(fr0, cond, nethead1, init1, nethead2, init2);

  check_GenIf(item, fr0, cond, {nethead1}, {init1}, {nethead2}, {init2});
}

TEST_F(PtItemTest, GenCaseItem)
{
  auto fr_net1 = make_file_region(1, 1, 1, 1);
  auto name_net1 = "net1";
  auto net1 = factory.new_DeclItem(fr_net1, name_net1);
  auto fr_nethead1 = make_file_region(1, 1, 2, 2);
  auto nethead1 = factory.new_NetH(fr_nethead1, VpiNetType::Wire, net1);

  auto fr_null1 = make_file_region(4, 4, 4, 4);
  auto null1 = factory.new_NullStmt(fr_null1);
  auto fr_init1 = make_file_region(3, 3, 3, 3);
  auto init1 = factory.new_Initial(fr_init1, null1);

  auto fr_val1 = make_file_region(1, 1, 1, 1);
  auto val1 = parser.factory().new_IntConst(fr_val1, 1U);
  auto fr_caseitem1 = make_file_region(1, 2, 3, 4);
  auto caseitem1 = factory.new_GenCaseItem(fr_caseitem1, val1, nethead1, init1);

  check_GenCaseItem(caseitem1, fr_caseitem1, {val1}, {nethead1}, {init1});
}

TEST_F(PtItemTest, GenCase)
{
  auto fr_net1 = make_file_region(1, 1, 1, 1);
  auto name_net1 = "net1";
  auto net1 = factory.new_DeclItem(fr_net1, name_net1);
  auto fr_nethead1 = make_file_region(1, 1, 2, 2);
  auto nethead1 = factory.new_NetH(fr_nethead1, VpiNetType::Wire, net1);
  auto fr_val1 = make_file_region(1, 1, 1, 1);
  auto val1 = parser.factory().new_IntConst(fr_val1, 1U);
  auto fr_caseitem1 = make_file_region(1, 2, 3, 4);
  auto caseitem1 = factory.new_GenCaseItem(fr_caseitem1, val1, nethead1, nullptr);

  auto fr_caseitem2 = make_file_region(4, 4, 4, 4);
  // default の場合
  auto caseitem2 = factory.new_GenCaseItem(fr_caseitem2, nullptr, nullptr, nullptr);

  PtGenCaseItemList gci_list;
  gci_list.init(caseitem1);
  gci_list.add(caseitem2);

  auto fr_cond = make_file_region(5, 5, 5, 5);
  auto cond = parser.factory().new_IntConst(fr_cond, 123U);
  auto fr_case = make_file_region(6, 6, 6, 6);
  auto item = factory.new_GenCase(fr_case, cond, gci_list.top);

  check_GenCase(item, fr_case, cond, {caseitem1, caseitem2});
}

TEST_F(PtItemTest, GenFor)
{
  auto fr_net1 = make_file_region(1, 1, 1, 1);
  auto name_net1 = "net1";
  auto net1 = factory.new_DeclItem(fr_net1, name_net1);
  auto fr_nethead1 = make_file_region(1, 1, 2, 2);
  auto nethead1 = factory.new_NetH(fr_nethead1, VpiNetType::Wire, net1);

  auto fr_null1 = make_file_region(4, 4, 4, 4);
  auto null1 = factory.new_NullStmt(fr_null1);
  auto fr_init1 = make_file_region(3, 3, 3, 3);
  auto init1 = factory.new_Initial(fr_init1, null1);

  auto fr_init_expr = make_file_region(1, 1, 1, 1);
  auto init_expr = parser.factory().new_IntConst(fr_init_expr, 1U);
  auto fr_cond_expr = make_file_region(4, 4, 4, 4);
  auto cond_expr = parser.factory().new_IntConst(fr_cond_expr, 2U);
  auto fr_next_expr = make_file_region(5, 5, 5, 5);
  auto next_expr = parser.factory().new_IntConst(fr_next_expr, 3U);
  auto varname = "i";
  auto blockname = "block1";
  auto fr0 = make_file_region(1, 2, 3, 4);
  auto item = factory.new_GenFor(fr0, varname,
				 init_expr, cond_expr, next_expr,
				 blockname, nethead1, init1);

  check_GenFor(item, fr0, blockname, varname,
	       init_expr, cond_expr, next_expr,
	       {nethead1}, {init1});
}

END_NAMESPACE_YM_VERILOG
