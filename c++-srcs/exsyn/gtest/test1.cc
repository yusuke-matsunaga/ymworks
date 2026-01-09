
/// @file ExSyn_test.cc
/// @brief ExSyn_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/ExSyn.h"
#include "ym/JsonValue.h"


BEGIN_NAMESPACE_YM_EXSYN

void
check(
  const TruthTable& tt,
  const BcGraph& g,
  const char* exp_str
)
{
  EXPECT_TRUE( ExSyn::check_boolchain(tt, g) );

  std::ostringstream buf;
  g.print(buf);
  EXPECT_EQ( exp_str, buf.str() );
}

TEST(ExSynTest, test0001)
{
  TruthTable tt(2, 1);

  tt.set_value(0, 0, Val3::ZERO);
  tt.set_value(1, 0, Val3::ZERO);
  tt.set_value(2, 0, Val3::ZERO);
  tt.set_value(3, 0, Val3::ONE);

  auto g = ExSyn::gen_boolchain(tt, JsonValue());
  const char* exp_str = "2 1 1\n"
    "OP#0: AND(I#0, I#1)\n"
    "O#0: OP#0\n";
  check(tt, g, exp_str);
}

END_NAMESPACE_YM_EXSYN
