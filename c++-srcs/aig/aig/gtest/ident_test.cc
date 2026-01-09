
/// @file ident_test.cc
/// @brief ident_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

TEST(AigMgrTest, ident1)
{
  std::string filename1{"test1.aag"};
  std::string path1{DATAPATH + filename1};

  auto mgr1 = AigMgr::read_aag(path1);

  std::string filename2{"test2.aag"};
  std::string path2{DATAPATH + filename2};

  auto mgr2 = AigMgr::read_aag(path2);

  auto res = mgr1.is_identical(mgr2);

  EXPECT_TRUE( res );
}

END_NAMESPACE_YM
