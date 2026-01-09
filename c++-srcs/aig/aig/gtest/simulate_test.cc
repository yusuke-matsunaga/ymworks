
/// @file simulate_test.cc
/// @brief simulate_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

TEST(AigMgrTest, simulate1)
{
  AigMgr mgr;

  auto i0 = mgr.new_input();
  auto i1 = mgr.new_input();

  auto e00 = mgr.new_and({ i0,  i1});
  auto e01 = mgr.new_and({ i0, ~i1});
  auto e10 = mgr.new_and({~i0,  i1});
  auto e11 = mgr.new_and({~i0, ~i1});

  mgr.new_output(e00);
  mgr.new_output(e01);
  mgr.new_output(e10);
  mgr.new_output(e11);

  auto input_vals = std::vector<AigBitVect>{
    0xA, 0xC
  };

  auto output_vals = mgr.simulate(input_vals);

  ASSERT_EQ( 4, output_vals.size() );
  EXPECT_EQ( 0x8, output_vals[0] & 0xF );
  EXPECT_EQ( 0x2, output_vals[1] & 0xF );
  EXPECT_EQ( 0x4, output_vals[2] & 0xF );
  EXPECT_EQ( 0x1, output_vals[3] & 0xF );
}

END_NAMESPACE_YM
