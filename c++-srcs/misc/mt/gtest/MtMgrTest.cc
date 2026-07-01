
/// @file MtMgrTest.cc
/// @brief MtMgrTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/MtMgr.h"
#include "ym/IdPool.h"


BEGIN_NAMESPACE_YM

int
fibo(
  int x
)
{
  if ( x == 0 ) {
    return 1;
  }
  if ( x == 1 ) {
    return 2;
  }
  return fibo(x - 2) + fibo(x - 1);
}

TEST(MtMgrTest, test1)
{
  SizeType N = 30;
  std::vector<SizeType> array(N);

  auto thread_num = MtMgr::actual_thread_num(0);
  MtMgr::run([&](SizeType th_id) {
    for ( SizeType i = th_id; i < N; i += thread_num ) {
      array[i] = fibo(i);
    }
  });
  EXPECT_EQ( 1346269, array[N - 1] );
}

TEST(MtMgrTest, test2)
{
  SizeType N = 30;
  std::vector<SizeType> array(N);

  auto thread_num = MtMgr::actual_thread_num(0);
  IdPool id_pool(N);
  MtMgr::run([&]() {
    SizeType id;
    while ( id_pool.get(id) ) {
      array[id] = fibo(id);
    }
  });
  EXPECT_EQ( 1346269, array[N - 1] );
}

END_NAMESPACE_YM
