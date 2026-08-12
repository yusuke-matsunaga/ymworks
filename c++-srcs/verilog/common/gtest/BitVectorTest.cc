
/// @file BitVectorTest.cc
/// @brief BitVectorTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "BitVectorTest.h"


BEGIN_NAMESPACE_YM

TEST_F(BitVectorTest, constr_empty)
{
  // 空のコンストラクタ
  auto bv = BitVector();

  check(bv, false, false,
	std::vector<VlScalarVal>{VlScalarVal::x()},
	10);
  EXPECT_EQ( 10, bv.base() );
  EXPECT_FALSE( bv.is_negative() );
  EXPECT_FALSE( bv.is_uint32() );
  EXPECT_FALSE( bv.is_int() );
  EXPECT_EQ( VlScalarVal::x(), bv.to_scalar() );
  EXPECT_EQ( VlScalarVal::x(), bv.to_logic() );
  EXPECT_FALSE( bv.to_bool() );
  EXPECT_FALSE( bv.is_time() );
  EXPECT_EQ( "X", bv.verilog_string() );
}

TEST_F(BitVectorTest, constr_size)
{
  // SizeType の値を指定したコンストラクタ
  SizeType val = 123;
  auto bv = BitVector(val);

  std::vector<VlScalarVal> val_list(32);
  for ( SizeType i = 0; i < 32; ++ i ) {
    if ( val & (1 << i) ) {
      val_list[i] = VlScalarVal::one();
    }
    else {
      val_list[i] = VlScalarVal::zero();
    }
  }
  check(bv, false, false, val_list, 10);
  EXPECT_TRUE( bv.is_uint32() );
  EXPECT_EQ( val, bv.to_uint32() );
  EXPECT_TRUE( bv.is_int() );
  EXPECT_EQ( val, bv.to_int() );
  EXPECT_EQ( static_cast<double>(val), bv.to_real() );
  EXPECT_EQ( "123", bv.verilog_string() );
}

TEST_F(BitVectorTest, constr_int)
{
  // int の値を指定したコンストラクタ
  int val = -123;
  auto bv = BitVector(val);
  std::vector<VlScalarVal> val_list(32);

  SizeType uval = static_cast<SizeType>(val);
  for ( SizeType i = 0; i < 32; ++ i ) {
    if ( uval & (1 << i) ) {
      val_list[i] = VlScalarVal::one();
    }
    else {
      val_list[i] = VlScalarVal::zero();
    }
  }
  check(bv, true, false, val_list, 10);
  EXPECT_TRUE( bv.is_uint32() );
  EXPECT_EQ( static_cast<std::uint32_t>(val), bv.to_uint32() );
  EXPECT_TRUE( bv.is_int() );
  EXPECT_EQ( val, bv.to_int() );
  EXPECT_EQ( static_cast<double>(static_cast<std::uint32_t>(val)), bv.to_real() );
  EXPECT_EQ( "-123", bv.verilog_string() );
}

TEST_F(BitVectorTest, constr_bool)
{
  // bool の値を指定したコンストラクタ
  for ( auto val: { true, false } ) {
    auto bv = BitVector(val);

    std::vector<VlScalarVal> val_list = {VlScalarVal(val)};
    check(bv, false, true, val_list, 2);
    EXPECT_TRUE( bv.is_uint32() );
    EXPECT_EQ( val, bv.to_uint32() );
    EXPECT_TRUE( bv.is_int() );
    EXPECT_EQ( val, bv.to_int() );
    EXPECT_EQ( static_cast<double>(val), bv.to_real() );
  }
}

TEST_F(BitVectorTest, constr_time)
{
  // VlTImet の値を指定したコンストラクタ
  std::uint32_t val_h = 0x12345678UL;
  std::uint32_t val_l = 0x9abcedefUL;
  auto val = VlTime(val_l, val_h);
  auto bv = BitVector(val);

  std::vector<VlScalarVal> val_list(64);
  for ( SizeType i = 0; i < 32; ++ i ) {
    if ( val_l & (1 << i) ) {
      val_list[i] = VlScalarVal::one();
    }
    else {
      val_list[i] = VlScalarVal::zero();
    }
  }
  for ( SizeType i = 0; i < 32; ++ i ) {
    if ( val_h & (1 << i) ) {
      val_list[i + 32] = VlScalarVal::one();
    }
    else {
      val_list[i + 32] = VlScalarVal::zero();
    }
  }
  check(bv, false, true, val_list, 10);
  // この判断は微妙
  EXPECT_TRUE( bv.is_uint32() );
  EXPECT_EQ( val_l, bv.to_uint32() );
  EXPECT_TRUE( bv.is_int() );
  EXPECT_EQ( static_cast<int>(val_l), bv.to_int() );
}

TEST_F(BitVectorTest, constr_scalar1)
{
  // VlSclarVal の値を指定したコンストラクタ
  auto val_0 = VlScalarVal::zero();
  auto val_1 = VlScalarVal::one();
  auto val_x = VlScalarVal::x();
  auto val_z = VlScalarVal::z();
  for ( auto val: { val_0, val_1, val_x, val_z } ) {
    auto bv = BitVector(val);

    check(bv, false, true, {val}, 2);
    if ( val == val_0 ) {
      EXPECT_TRUE( bv.is_uint32() );
      EXPECT_EQ( 0, bv.to_uint32() );
      EXPECT_TRUE( bv.is_int() );
      EXPECT_EQ( 0, bv.to_int() );
    }
    else if ( val == val_1 ) {
      EXPECT_TRUE( bv.is_uint32() );
      EXPECT_EQ( 1, bv.to_uint32() );
      EXPECT_TRUE( bv.is_int() );
      EXPECT_EQ( 1, bv.to_int() );
    }
    else {
      EXPECT_FALSE( bv.is_uint32() );
      EXPECT_FALSE( bv.is_int() );
    }
  }
}

END_NAMESPACE_YM
