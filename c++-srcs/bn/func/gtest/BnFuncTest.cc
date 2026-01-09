
/// @file BnFuncTest.cc
/// @brief BnFuncTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnFunc.h"


BEGIN_NAMESPACE_YM

TEST(BnFuncTest, empty)
{
  auto func = BnFunc();

  EXPECT_FALSE( func.is_valid() );
}

END_NAMESPACE_YM
