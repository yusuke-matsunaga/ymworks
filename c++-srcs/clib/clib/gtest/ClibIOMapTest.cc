
/// @file ClibIOMapTest.cc
/// @brief ClibIOMapTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "ym/ClibIOMap.h"


BEGIN_NAMESPACE_YM_CLIB

TEST(ClibPinMap_test, default_constructor)
{
  // 空のコンストラクタ
  ClibPinMap pinmap;

  EXPECT_FALSE( pinmap.is_valid() );
}

TEST(ClibPinMap_test, invalid)
{
  auto pinmap = ClibPinMap::invalid();

  EXPECT_FALSE( pinmap.is_valid() );
}

TEST(ClibPinMap_test, test1)
{
  SizeType id = 2;
  bool inv = true;

  ClibPinMap pinmap{id, inv};

  EXPECT_TRUE( pinmap.is_valid() );
  EXPECT_EQ( id, pinmap.id() );
  EXPECT_EQ( inv, pinmap.inv() );
}

TEST(ClibPinMap_test, test2)
{
  SizeType id = 3;
  bool inv = false;

  ClibPinMap pinmap1{id, inv};

  EXPECT_TRUE( pinmap1.is_valid() );
  EXPECT_EQ( id, pinmap1.id() );
  EXPECT_EQ( inv, pinmap1.inv() );

  ClibPinMap pinmap2{id, inv};
  EXPECT_TRUE( pinmap1 == pinmap2 );
}

TEST(ClibIOMap_test, default_constructor)
{
  ClibIOMap iomap;

  EXPECT_EQ( 0, iomap.input_num() );
  EXPECT_EQ( 0, iomap.output_num() );
  EXPECT_EQ( 0, iomap.inout_num() );
}

TEST(ClibIOMap_test, constructor1)
{
  ClibPinMap pinmap1{1, false};
  ClibPinMap pinmap2{0, true};
  std::vector<ClibPinMap> input_map{pinmap1, pinmap2};
  ClibIOMap iomap{input_map, true};

  EXPECT_EQ( 2, iomap.input_num() );
  EXPECT_EQ( pinmap1, iomap.input_map(0) );
  EXPECT_EQ( pinmap2, iomap.input_map(1) );
  EXPECT_EQ( 1, iomap.output_num() );
  ClibPinMap opinmap{0, true};
  EXPECT_EQ( opinmap, iomap.output_map(0) );
}

TEST(ClibIOMap_test, construct2)
{
  ClibPinMap pinmap1{1, false};
  ClibPinMap pinmap2{0, true};
  std::vector<ClibPinMap> input_map{pinmap1, pinmap2};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{1, true};
  std::vector<ClibPinMap> output_map{pinmap3, pinmap4};
  ClibIOMap iomap{input_map, output_map};

  EXPECT_EQ( 2, iomap.input_num() );
  EXPECT_EQ( pinmap1, iomap.input_map(0) );
  EXPECT_EQ( pinmap2, iomap.input_map(1) );
  EXPECT_EQ( 2, iomap.output_num() );
  EXPECT_EQ( pinmap3, iomap.output_map(0) );
  EXPECT_EQ( pinmap4, iomap.output_map(1) );
}

TEST(ClibIOMap_test, construct3)
{
  ClibPinMap pinmap1{1, false};
  ClibPinMap pinmap2{0, true};
  std::vector<ClibPinMap> input_map{pinmap1, pinmap2};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{1, true};
  std::vector<ClibPinMap> output_map{pinmap3, pinmap4};
  ClibPinMap pinmap5{0, false};
  std::vector<ClibPinMap> inout_map{pinmap5};
  ClibIOMap iomap{input_map, output_map, inout_map};

  EXPECT_EQ( 2, iomap.input_num() );
  EXPECT_EQ( pinmap1, iomap.input_map(0) );
  EXPECT_EQ( pinmap2, iomap.input_map(1) );
  EXPECT_EQ( 2, iomap.output_num() );
  EXPECT_EQ( pinmap3, iomap.output_map(0) );
  EXPECT_EQ( pinmap4, iomap.output_map(1) );
  EXPECT_EQ( 1, iomap.inout_num() );
  EXPECT_EQ( pinmap5, iomap.inout_map(0) );
}

TEST(ClibIOMap_test, compose1)
{
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> input_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map1, false};

  ClibPinMap pinmap5{3, false};
  ClibPinMap pinmap6{2, true};
  ClibPinMap pinmap7{1, true};
  ClibPinMap pinmap8{0, false};
  std::vector<ClibPinMap> input_map2{pinmap5, pinmap6, pinmap7, pinmap8};
  ClibIOMap iomap2{input_map2, false};

  auto iomap3 = iomap1 * iomap2;

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{1, true};
  ClibPinMap pinmap11{3, false};
  ClibPinMap pinmap12{0, false};
  std::vector<ClibPinMap> input_map3{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap4{input_map3, false};

  EXPECT_EQ( iomap4, iomap3 );
}

TEST(ClibIOMap_test, compose_int1)
{
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> input_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map1, false};

  ClibPinMap pinmap5{3, false};
  ClibPinMap pinmap6{2, true};
  ClibPinMap pinmap7{1, true};
  ClibPinMap pinmap8{0, false};
  std::vector<ClibPinMap> input_map2{pinmap5, pinmap6, pinmap7, pinmap8};
  ClibIOMap iomap2{input_map2, false};

  iomap1 *= iomap2;

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{1, true};
  ClibPinMap pinmap11{3, false};
  ClibPinMap pinmap12{0, false};
  std::vector<ClibPinMap> input_map3{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap4{input_map3, false};

  EXPECT_EQ( iomap4, iomap1 );
}

TEST(ClibIOMap_test, compose2)
{
  std::vector<ClibPinMap> input_map{ClibPinMap{0, false}};
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> output_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map, output_map1};

  ClibPinMap pinmap5{3, false};
  ClibPinMap pinmap6{2, true};
  ClibPinMap pinmap7{1, true};
  ClibPinMap pinmap8{0, false};
  std::vector<ClibPinMap> output_map2{pinmap5, pinmap6, pinmap7, pinmap8};
  ClibIOMap iomap2{input_map, output_map2};

  auto iomap3 = iomap1 * iomap2;

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{1, true};
  ClibPinMap pinmap11{3, false};
  ClibPinMap pinmap12{0, false};
  std::vector<ClibPinMap> output_map3{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap4{input_map, output_map3};

  EXPECT_EQ( iomap4, iomap3 );
}

TEST(ClibIOMap_test, compose_int2)
{
  std::vector<ClibPinMap> input_map{ClibPinMap{0, false}};
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> output_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map, output_map1};

  ClibPinMap pinmap5{3, false};
  ClibPinMap pinmap6{2, true};
  ClibPinMap pinmap7{1, true};
  ClibPinMap pinmap8{0, false};
  std::vector<ClibPinMap> output_map2{pinmap5, pinmap6, pinmap7, pinmap8};
  ClibIOMap iomap2{input_map, output_map2};

  iomap1 *= iomap2;

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{1, true};
  ClibPinMap pinmap11{3, false};
  ClibPinMap pinmap12{0, false};
  std::vector<ClibPinMap> output_map3{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap4{input_map, output_map3};

  EXPECT_EQ( iomap4, iomap1 );
}

TEST(ClibIOMap_test, compose3)
{
  std::vector<ClibPinMap> input_map{ClibPinMap{0, false}};
  std::vector<ClibPinMap> output_map{ClibPinMap{0, false}};
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> inout_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map, output_map, inout_map1};

  ClibPinMap pinmap5{3, false};
  ClibPinMap pinmap6{2, true};
  ClibPinMap pinmap7{1, true};
  ClibPinMap pinmap8{0, false};
  std::vector<ClibPinMap> inout_map2{pinmap5, pinmap6, pinmap7, pinmap8};
  ClibIOMap iomap2{input_map, output_map, inout_map2};

  auto iomap3 = iomap1 * iomap2;

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{1, true};
  ClibPinMap pinmap11{3, false};
  ClibPinMap pinmap12{0, false};
  std::vector<ClibPinMap> inout_map3{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap4{input_map, output_map, inout_map3};

  EXPECT_EQ( iomap4, iomap3 );
}

TEST(ClibIOMap_test, compose_int3)
{
  std::vector<ClibPinMap> input_map{ClibPinMap{0, false}};
  std::vector<ClibPinMap> output_map{ClibPinMap{0, false}};
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> inout_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map, output_map, inout_map1};

  ClibPinMap pinmap5{3, false};
  ClibPinMap pinmap6{2, true};
  ClibPinMap pinmap7{1, true};
  ClibPinMap pinmap8{0, false};
  std::vector<ClibPinMap> inout_map2{pinmap5, pinmap6, pinmap7, pinmap8};
  ClibIOMap iomap2{input_map, output_map, inout_map2};

  iomap1 *= iomap2;

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{1, true};
  ClibPinMap pinmap11{3, false};
  ClibPinMap pinmap12{0, false};
  std::vector<ClibPinMap> inout_map3{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap4{input_map, output_map, inout_map3};

  EXPECT_EQ( iomap4, iomap1 );
}

TEST(ClibIOMap_test, inverse1)
{
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> input_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map1, false};

  auto iomap2 = iomap1.inverse();

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{0, true};
  ClibPinMap pinmap11{1, false};
  ClibPinMap pinmap12{3, false};
  std::vector<ClibPinMap> input_map2{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap3{input_map2, false};

  EXPECT_EQ( iomap3, iomap2 );
}

TEST(ClibIOMap_test, inverse2)
{
  std::vector<ClibPinMap> input_map{ClibPinMap{0, false}};
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> output_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map, output_map1};

  auto iomap2 = iomap1.inverse();

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{0, true};
  ClibPinMap pinmap11{1, false};
  ClibPinMap pinmap12{3, false};
  std::vector<ClibPinMap> output_map2{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap3{input_map, output_map2};

  EXPECT_EQ( iomap3, iomap2 );
}

TEST(ClibIOMap_test, inverse3)
{
  std::vector<ClibPinMap> input_map{ClibPinMap{0, false}};
  std::vector<ClibPinMap> output_map{ClibPinMap{0, false}};
  ClibPinMap pinmap1{1, true};
  ClibPinMap pinmap2{2, false};
  ClibPinMap pinmap3{0, false};
  ClibPinMap pinmap4{3, false};
  std::vector<ClibPinMap> inout_map1{pinmap1, pinmap2, pinmap3, pinmap4};
  ClibIOMap iomap1{input_map, output_map, inout_map1};

  auto iomap2 = iomap1.inverse();

  ClibPinMap pinmap9{2, false};
  ClibPinMap pinmap10{0, true};
  ClibPinMap pinmap11{1, false};
  ClibPinMap pinmap12{3, false};
  std::vector<ClibPinMap> inout_map2{pinmap9, pinmap10, pinmap11, pinmap12};
  ClibIOMap iomap3{input_map, output_map, inout_map2};

  EXPECT_EQ( iomap3, iomap2 );
}

END_NAMESPACE_YM_CLIB
