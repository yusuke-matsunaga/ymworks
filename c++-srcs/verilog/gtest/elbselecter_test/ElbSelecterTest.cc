
/// @file ElbSelecterTest.cc
/// @brief ElbSelecterTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "elaborator/ElbSelecter.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST(ElbSelecterTest, NONE)
{
  SizeType width = 16;
  auto sel = ElbSelecter::new_NONE(width);

  EXPECT_EQ( VlSelectType::NONE, sel.type() );
  EXPECT_EQ( width, sel.select_width() );

  auto sel1 = sel.slice(width, 0);
  EXPECT_EQ( VlSelectType::CONST, sel1.type() );
  EXPECT_EQ( width, sel1.select_width() );
  EXPECT_EQ( 0, sel1.bit_offset() );

  SizeType width2 = 8;
  SizeType offset2 = 4;
  auto sel2 = sel.slice(width2, offset2);
  EXPECT_EQ( VlSelectType::CONST, sel2.type() );
  EXPECT_EQ( width2, sel2.select_width() );
  EXPECT_EQ( offset2, sel2.bit_offset() );
}

TEST(ElbSelecterTest, CONST)
{
  SizeType width = 16;
  SizeType offset = 4;
  auto sel = ElbSelecter::new_CONST(width, offset);

  EXPECT_EQ( VlSelectType::CONST, sel.type() );
  EXPECT_EQ( width, sel.select_width() );
  EXPECT_EQ( offset, sel.bit_offset() );

  auto sel1 = sel.slice(width, 0);
  EXPECT_EQ( VlSelectType::CONST, sel1.type() );
  EXPECT_EQ( width, sel1.select_width() );
  EXPECT_EQ( offset, sel1.bit_offset() );

  SizeType width2 = 8;
  SizeType offset2 = 4;
  auto sel2 = sel.slice(width2, offset2);
  EXPECT_EQ( VlSelectType::CONST, sel2.type() );
  EXPECT_EQ( width2, sel2.select_width() );
  auto exp_offset = offset + offset2;
  EXPECT_EQ( exp_offset, sel2.bit_offset() );
}

END_NAMESPACE_YM_VERILOG
