
/// @file copy_test.cc
/// @brief copy_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/SopCover.h"
#include "ym/SopCube.h"
#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

class CopyTest:
public ::testing::Test
{
public:

  bool
  check(
    AigEdge edge1,
    AigEdge edge2
  );


private:

  std::unordered_map<AigEdge, AigEdge> mPairMap;

};

bool
CopyTest::check(
  AigEdge edge1,
  AigEdge edge2
)
{
  if ( edge1.is_zero() ) {
    return edge2.is_zero();
  }
  if ( edge1.is_one() ) {
    return edge2.is_one();
  }
  if ( edge1.is_input() ) {
    if ( edge2.is_input() && edge1.input_id() == edge2.input_id() ) {
      return true;
    }
    return false;
  }
  if ( edge2.is_input() ) {
    return false;
  }
  if ( edge1.inv() != edge2.inv() ) {
    return false;
  }
  if ( mPairMap.count(edge1) > 0 ) {
    return mPairMap.at(edge1) == edge2;
  }
  // edge1 と edge2 は共に AND
  if ( check(edge1.fanin0(), edge2.fanin0()) &&
       check(edge1.fanin1(), edge2.fanin1()) ) {
    mPairMap.emplace(edge1, edge2);
    return true;
  }
  return false;
}

TEST_F(CopyTest, empty)
{
  AigMgr mgr;

  auto src_list = std::vector<AigEdge>{};
  auto edge_list = mgr.copy(src_list);
  EXPECT_TRUE( edge_list.empty() );
}

TEST_F(CopyTest, zero)
{
  AigMgr mgr;

  auto src_edge = AigEdge::zero();
  auto edge = mgr.copy(src_edge);
  EXPECT_TRUE( check(src_edge, edge) );
}

TEST_F(CopyTest, one)
{
  AigMgr mgr;

  auto src_edge = AigEdge::one();
  auto edge = mgr.copy(src_edge);
  EXPECT_TRUE( check(src_edge, edge) );
}

TEST_F(CopyTest, input)
{
  AigMgr mgr;

  auto src_edge = mgr.input(0);
  auto edge = mgr.copy(src_edge);
  EXPECT_TRUE( check(src_edge, edge) );
}

TEST_F(CopyTest, and2)
{
  AigMgr mgr;

  auto i0 = mgr.input(0);
  auto i1 = mgr.input(1);
  auto src_edge = i0 & i1;
  auto edge = mgr.copy(src_edge);
  EXPECT_TRUE( check(src_edge, edge) );
}

END_NAMESPACE_YM
