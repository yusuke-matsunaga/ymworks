
/// @file BnNode_test.cc
/// @brief BnNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/BnNode.h"
#include "ym/BnNodeList.h"
#include "ym/BnModel.h"
#include "ym/BnFunc.h"
#include "ModelImpl.h"


BEGIN_NAMESPACE_YM_BN

TEST(BnNodeTest, constructor1)
{
  BnNode node;

  EXPECT_FALSE( node.is_valid() );

  EXPECT_THROW( node.type(), std::logic_error );
  EXPECT_THROW( node.is_input(), std::logic_error );
  EXPECT_THROW( node.is_logic(), std::logic_error );

  EXPECT_THROW( node.is_primary_input(), std::logic_error );
  EXPECT_THROW( node.input_id(), std::logic_error );
  EXPECT_THROW( node.is_dff_output(), std::logic_error );
  EXPECT_THROW( node.dff_id(), std::logic_error );

  EXPECT_THROW( node.func(), std::logic_error );
  EXPECT_THROW( node.fanin_num(), std::logic_error );
  EXPECT_THROW( node.fanin(0), std::logic_error );
  EXPECT_THROW( node.fanin_list(), std::logic_error );
}

END_NAMESPACE_YM_BN
