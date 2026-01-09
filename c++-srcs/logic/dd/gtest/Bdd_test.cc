
/// @file Bdd_test.cc
/// @brief Bdd のユニットテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "ym/Bdd.h"
#include "ym/BddVarSet.h"
#include "ym/BddCube.h"
#include "ym/BddMgr.h"
#include "ym/BinEnc.h"
#include "ym/BinDec.h"
#include "BddTest.h"


BEGIN_NAMESPACE_YM

TEST_F(BddTest, invalid1)
{
  // 不正値になっているはず．
  auto bdd = Bdd{};

  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
  EXPECT_FALSE( bdd.is_const() );
  EXPECT_TRUE( bdd.is_invalid() );

  auto mgr = bdd.mgr();
  EXPECT_FALSE( mgr.is_valid() );

  // 不正値でも否定は計算できる．
  auto ibdd = ~bdd;
  EXPECT_FALSE( ibdd.is_zero() );
  EXPECT_FALSE( ibdd.is_one() );
  EXPECT_FALSE( ibdd.is_const() );
  EXPECT_TRUE( ibdd.is_invalid() );

  auto mgr2 = ibdd.mgr();
  EXPECT_FALSE( mgr2.is_valid() );

}

TEST_F(BddTest, invalid2)
{
  // 不正値になっているはず．
  auto bdd = Bdd::invalid();

  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
  EXPECT_FALSE( bdd.is_const() );
  EXPECT_TRUE( bdd.is_invalid() );

  auto mgr = bdd.mgr();
  EXPECT_FALSE( mgr.is_valid() );

  // 不正値でも否定は計算できる．
  auto ibdd = ~bdd;
  EXPECT_FALSE( ibdd.is_zero() );
  EXPECT_FALSE( ibdd.is_one() );
  EXPECT_FALSE( ibdd.is_const() );
  EXPECT_TRUE( ibdd.is_invalid() );

  auto mgr2 = ibdd.mgr();
  EXPECT_FALSE( mgr2.is_valid() );
}

TEST_F(BddTest, zero)
{
  auto bdd = Bdd::zero();

  EXPECT_TRUE( bdd.is_zero() );
  EXPECT_FALSE( bdd.is_one() );
  EXPECT_TRUE( bdd.is_const() );
  EXPECT_FALSE( bdd.is_invalid() );

  auto mgr = bdd.mgr();
  EXPECT_FALSE( mgr.is_valid() );

  auto ibdd = ~bdd;
  EXPECT_FALSE( ibdd.is_zero() );
  EXPECT_TRUE( ibdd.is_one() );
  EXPECT_TRUE( ibdd.is_const() );
  EXPECT_FALSE( ibdd.is_invalid() );

  auto mgr2 = ibdd.mgr();
  EXPECT_FALSE( mgr2.is_valid() );

  check(bdd, "0");
}

TEST_F(BddTest, one)
{
  auto bdd = Bdd::one();

  EXPECT_FALSE( bdd.is_zero() );
  EXPECT_TRUE( bdd.is_one() );
  EXPECT_TRUE( bdd.is_const() );
  EXPECT_FALSE( bdd.is_invalid() );

  auto mgr = bdd.mgr();
  EXPECT_FALSE( mgr.is_valid() );

  auto ibdd = ~bdd;
  EXPECT_TRUE( ibdd.is_zero() );
  EXPECT_FALSE( ibdd.is_one() );
  EXPECT_TRUE( ibdd.is_const() );
  EXPECT_FALSE( ibdd.is_invalid() );

  auto mgr2 = ibdd.mgr();
  EXPECT_FALSE( mgr2.is_valid() );

  check(bdd, "1");
}

TEST_F(BddTest, copy_constructor)
{
  const char* exp_str = "1101";
  auto bdd = from_truth(exp_str);

  auto bdd1 = Bdd{bdd};

  check(bdd1, exp_str);
}

TEST_F(BddTest, copy_assignment)
{
  const char* exp_str = "1101";
  auto bdd = from_truth(exp_str);

  Bdd bdd1;
  bdd1 = bdd;
  check(bdd1, exp_str);
}

TEST_F(BddTest, variable1)
{
  auto var = variable(0);
  auto bdd = var.bdd();
  check(bdd, "10");
}

TEST_F(BddTest, variable2)
{
  auto var = variable(1);
  auto bdd = ~var.bdd();
  check(bdd, "0101");
}

TEST_F(BddTest, variable3)
{
  auto var = variable(2);
  auto bdd = var.bdd();
  check(bdd, "10101010");
}

TEST_F(BddTest, variable4)
{
  auto var = variable(3);
  auto bdd = ~var.bdd();
  check(bdd, "0101010101010101");
}

TEST_F(BddTest, invert1)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  auto bdd1 = bdd.invert();
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, invert_int1)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  bdd.invert_int();
  const char* exp_str = "0010";
  check(bdd, exp_str);
}

TEST_F(BddTest, invert2)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  auto bdd1 = ~bdd;
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, invert_invalid)
{
  auto bdd = invalid();

  EXPECT_TRUE( bdd.is_invalid() );

  auto bdd1 = ~bdd;
  EXPECT_TRUE( bdd1.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, invert_int_invalid)
{
  auto bdd = invalid();

  EXPECT_TRUE( bdd.is_invalid() );

  bdd.invert_int();
  EXPECT_TRUE( bdd.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv1)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  auto bdd1 = bdd * true;
  const char* exp_str = "0010";
  check(bdd1, exp_str);
}

TEST_F(BddTest, pol_inv_int1)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  bdd *= true;
  const char* exp_str = "0010";
  check(bdd, exp_str);
}

TEST_F(BddTest, pol_inv1_invalid)
{
  auto bdd = invalid();

  EXPECT_TRUE( bdd.is_invalid() );

  auto bdd1 = bdd * true;
  EXPECT_TRUE( bdd1.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv1_int_invalid)
{
  auto bdd = invalid();

  EXPECT_TRUE( bdd.is_invalid() );

  bdd *= true;
  EXPECT_TRUE( bdd.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv2)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  auto bdd1 = bdd * false;
  check(bdd1, src_str);
}

TEST_F(BddTest, pol_inv_int2)
{
  const char* src_str = "1101";
  auto bdd = from_truth(src_str);

  bdd *= false;
  check(bdd, src_str);
}

TEST_F(BddTest, pol_inv2_invalid)
{
  auto bdd = invalid();

  EXPECT_TRUE( bdd.is_invalid() );

  auto bdd1 = bdd * false;
  EXPECT_TRUE( bdd1.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, pol_inv2_int_invalid)
{
  auto bdd = invalid();

  EXPECT_TRUE( bdd.is_invalid() );

  bdd *= false;
  EXPECT_TRUE( bdd.is_invalid() );
  // 結果も不正値だが例外は送出されない．
}

TEST_F(BddTest, and_op1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1.and_op(bdd2);

  check(bdd, "1000");
}

TEST_F(BddTest, and_op2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 & bdd2;

  check(bdd, "1000");
}

TEST_F(BddTest, and_op3)
{
  auto bdd1 = Bdd::zero();
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 & bdd2;

  check(bdd, "0000");
}

TEST_F(BddTest, and_op4)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = Bdd::zero();
  auto bdd = bdd1 & bdd2;

  check(bdd, "0000");
}

TEST_F(BddTest, and_op5)
{
  auto bdd1 = Bdd::one();
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 & bdd2;

  check(bdd, "1010");
}

TEST_F(BddTest, and_op6)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = Bdd::one();
  auto bdd = bdd1 & bdd2;

  check(bdd, "1100");
}

TEST_F(BddTest, and_op_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1.and_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, and_op_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1.and_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, and_op_invalid3)
{

  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1.and_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, and_op2_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1 & bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, and_op2_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1 & bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, and_op2_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1 & bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, and_int1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1.and_int(bdd2);

  check(bdd1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_int_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1.and_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, and_int_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1.and_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, and_int_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1.and_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, and_int2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 &= bdd2;

  check(bdd1, "1000");
  check(bdd, "1000");
}

TEST_F(BddTest, and_int2_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1 &= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, and_int2_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1 &= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, and_int2_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1 &= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, or_op1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1.or_op(bdd2);

  check(bdd, "1110");
}

TEST_F(BddTest, or_op_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1.or_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, or_op_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1.or_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, or_op_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1.or_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, or_int1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1.or_int(bdd2);

  check(bdd1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_int_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1.or_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, or_int_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1.or_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, or_int_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1.or_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, or_op2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 | bdd2;

  check(bdd, "1110");
}

TEST_F(BddTest, or_op2_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1 | bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, or_op2_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1 | bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, or_op2_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1 | bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, or_op3)
{
  auto bdd1 = Bdd::zero();
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 | bdd2;

  check(bdd, "1010");
}

TEST_F(BddTest, or_op4)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = Bdd::zero();
  auto bdd = bdd1 | bdd2;

  check(bdd, "1100");
}

TEST_F(BddTest, or_op5)
{
  auto bdd1 = Bdd::one();
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 | bdd2;

  check(bdd, "1111");
}

TEST_F(BddTest, or_op6)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = Bdd::one();
  auto bdd = bdd1 | bdd2;

  check(bdd, "1111");
}

TEST_F(BddTest, or_int2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 |= bdd2;

  check(bdd1, "1110");
  check(bdd, "1110");
}

TEST_F(BddTest, or_int2_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1 |= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, or_int2_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1 |= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, or_int2_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1 |= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor_op1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1.xor_op(bdd2);

  check(bdd, "0110");
}

TEST_F(BddTest, xor_op_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1.xor_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, xor_op_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1.xor_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, xor_op_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1.xor_op(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, xor_int1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1.xor_int(bdd2);

  check(bdd1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_int_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1.xor_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, xor_int_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1.xor_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, xor_int_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1.xor_int(bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, xor_op2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 ^ bdd2;

  check(bdd, "0110");
}

TEST_F(BddTest, xor_op3)
{
  auto bdd1 = Bdd::zero();
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 ^ bdd2;

  check(bdd, "1010");
}

TEST_F(BddTest, xor_op4)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = Bdd::zero();
  auto bdd = bdd1 ^ bdd2;

  check(bdd, "1100");
}

TEST_F(BddTest, xor_op5)
{
  auto bdd1 = Bdd::one();
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 ^ bdd2;

  check(bdd, "0101");
}

TEST_F(BddTest, xor_op6)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = Bdd::one();
  auto bdd = bdd1 ^ bdd2;

  check(bdd, "0011");
}

TEST_F(BddTest, xor_op2_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1 ^ bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor_op2_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1 ^ bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor_op2_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1 ^ bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor_int2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd = bdd1 ^= bdd2;

  check(bdd1, "0110");
  check(bdd, "0110");
}

TEST_F(BddTest, xor_int2_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = literal_bdd(1);
  EXPECT_THROW( bdd1 ^= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor_int2_invalid2)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = invalid();
  EXPECT_THROW( bdd1 ^= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor_int2_invalid3)
{
  auto bdd1 = literal_bdd(0);
  BddMgr mgr2;
  auto bdd2 = mgr2.variable_bdd(1);
  EXPECT_THROW( bdd1 ^= bdd2,
		std::invalid_argument );
}

TEST_F(BddTest, xor3)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd3 = literal_bdd(2);
  auto bdd = bdd1 ^ bdd2 ^ bdd3;

  check(bdd, "10010110");
}

TEST_F(BddTest, complex_expr1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd3 = literal_bdd(2);
  auto bdd = (bdd1 & ~bdd2) | bdd3;

  check(bdd, "10111010");
}

TEST_F(BddTest, complex_expr_invalid1)
{
  auto bdd1 = literal_bdd(0);
  auto bdd2 = literal_bdd(1);
  auto bdd3 = literal_bdd(2);
  BddMgr mgr2;
  auto bdd4 = mgr2.variable_bdd(1);
  EXPECT_THROW( (bdd1 & ~bdd2) | (bdd3 & ~bdd4),
		std::invalid_argument );
}

TEST_F(BddTest, ite1)
{
  auto bdd1 = from_truth("10101010");
  auto bdd2 = from_truth("11001100");
  auto bdd3 = from_truth("11110000");

  auto bdd = ite(bdd1, bdd2, bdd3);

  auto exp_bdd = bdd1 & bdd2 | ~bdd1 & bdd3;
  EXPECT_EQ( exp_bdd, bdd );
}

TEST_F(BddTest, ite2)
{
  auto bdd1 = Bdd::zero();
  auto bdd2 = from_truth("11001100");
  auto bdd3 = from_truth("11110000");

  auto bdd = ite(bdd1, bdd2, bdd3);

  auto exp_bdd = bdd3;
  EXPECT_EQ( exp_bdd, bdd );
}

TEST_F(BddTest, ite3)
{
  auto bdd1 = Bdd::one();
  auto bdd2 = from_truth("11001100");
  auto bdd3 = from_truth("11110000");

  auto bdd = ite(bdd1, bdd2, bdd3);

  auto exp_bdd = bdd2;
  EXPECT_EQ( exp_bdd, bdd );
}

TEST_F(BddTest, ite4)
{
  auto bdd1 = Bdd::one();
  auto bdd2 = Bdd::zero();
  auto bdd3 = Bdd::one();

  auto bdd = ite(bdd1, bdd2, bdd3);

  auto exp_bdd = Bdd::zero();
  EXPECT_EQ( exp_bdd, bdd );
}

TEST_F(BddTest, ite_invalid1)
{
  auto bdd1 = invalid();
  auto bdd2 = from_truth("11001100");
  auto bdd3 = from_truth("11110000");

  EXPECT_THROW( ite(bdd1, bdd2, bdd3),
		std::invalid_argument );
}

TEST_F(BddTest, ite_invalid2)
{
  auto bdd1 = from_truth("10101010");
  auto bdd2 = invalid();
  auto bdd3 = from_truth("11110000");

  EXPECT_THROW( ite(bdd1, bdd2, bdd3),
		std::invalid_argument );
}

TEST_F(BddTest, ite_invalid3)
{
  auto bdd1 = from_truth("10101010");
  auto bdd2 = from_truth("11001100");
  auto bdd3 = invalid(); // 不正値

  EXPECT_THROW( ite(bdd1, bdd2, bdd3),
		std::invalid_argument );
}

TEST_F(BddTest, simplify1)
{
  const char* src_str = "10010111";
  auto bdd1 = from_truth(src_str);
  auto bdd2 = Bdd::zero();
  auto bdd = simplify(bdd1, bdd2);
  EXPECT_EQ( bdd1, bdd );
}

TEST_F(BddTest, simplify_invalid1)
{
  const char* src_str = "10010111";
  auto bdd1 = Bdd::invalid();
  auto bdd2 = Bdd::zero();

  EXPECT_THROW( simplify(bdd1, bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, simplify_invliad2)
{
  const char* src_str = "10010111";
  auto bdd1 = from_truth(src_str);
  auto bdd2 = Bdd::invalid();

  EXPECT_THROW( simplify(bdd1, bdd2),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor2)
{ // (BddVar, bool) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto var = variable(0);
  auto bdd1 = bdd.cofactor(var, true);

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor2_invalid1)
{ // bdd が invalid
  auto bdd = invalid();
  auto var = variable(0);

  EXPECT_THROW( bdd.cofactor(var, true),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor2_invalid2)
{ // var が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto var = BddVar::invalid();

  EXPECT_THROW( bdd.cofactor(var, true),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor_int2)
{ // (BddVar, bool) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto var = variable(0);
  auto bdd1 = bdd.cofactor_int(var, true);

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int2_invalid1)
{ // bdd が invalid
  auto bdd = invalid();
  auto var = variable(0);

  EXPECT_THROW( bdd.cofactor_int(var, true),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor_int2_invalid2)
{ // var が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto var = BddVar::invalid();

  EXPECT_THROW( bdd.cofactor_int(var, true),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor3)
{ // (BddLit) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit = literal(0);
  auto bdd1 = bdd.cofactor(~lit);

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor3_invalid1)
{ // bdd が invalid
  auto bdd = invalid();
  auto lit = literal(0);
  EXPECT_THROW( bdd.cofactor(lit),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor3_invalid2)
{ // lit が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit = BddLit::invalid();

  EXPECT_THROW( bdd.cofactor(lit),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor_int3)
{ // (BddLit) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto var = variable(0);
  auto lit = var.negative_literal();
  auto bdd1 = bdd.cofactor_int(lit);

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int3_invalid1)
{ // bdd が invalid
  auto bdd = invalid();
  auto var = variable(0);
  auto lit = var.negative_literal();
  EXPECT_THROW( bdd.cofactor_int(lit),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor_int3_invalid2)
{ // lit が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit = BddLit::invalid();

  EXPECT_THROW( bdd.cofactor_int(lit),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor4)
{ // (BddCube) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;
  auto bdd1 = bdd.cofactor(cube);

  const char* exp_str = "01010101";
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor4_invalid1)
{ // bdd が invalid
  auto bdd = Bdd::invalid();
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;

  EXPECT_THROW( bdd.cofactor(cube),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor4_invalid2)
{ // cube が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto cube = BddCube::invalid();

  EXPECT_THROW( bdd.cofactor(cube),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor_int4)
{ // (BddCube) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;

  auto bdd1 = bdd.cofactor_int(cube);

  const char* exp_str = "01010101";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, cofactor_int4_invalid1)
{ // bdd が invalid
  auto bdd = Bdd::invalid();
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;

  EXPECT_THROW( bdd.cofactor_int(cube),
		std::invalid_argument );
}

TEST_F(BddTest, cofactor_int4_invalid2)
{ // cube が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto cube = BddCube::invalid();

  EXPECT_THROW( bdd.cofactor_int(cube),
		std::invalid_argument );
}

TEST_F(BddTest, div1)
{ // (BddLit) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto var = variable(0);
  auto lit = var.negative_literal();

  auto bdd1 = bdd / lit;

  const char* exp_str = "01110111";
  check(bdd1, exp_str);
}

TEST_F(BddTest, div1_invalid1)
{ // bdd が invalid
  auto bdd = invalid();
  auto var = variable(0);
  auto lit = var.negative_literal();

  EXPECT_THROW( bdd / lit,
		std::invalid_argument );
}

TEST_F(BddTest, div_int1)
{ // (BddLit) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);

  auto var = variable(0);
  auto lit = var.negative_literal();
  auto bdd1 = bdd /= lit;

  const char* exp_str = "01110111";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, div_int1_invalid1)
{ // bdd が invalid
  auto bdd = invalid();
  auto var = variable(0);
  auto lit = var.negative_literal();

  EXPECT_THROW( bdd /= lit,
		std::invalid_argument );
}

TEST_F(BddTest, div2)
{ // (BddCube) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;
  auto bdd1 = bdd / cube;

  const char* exp_str = "01010101";
  check(bdd1, exp_str);
}

TEST_F(BddTest, div2_invalid1)
{ // bdd が invalid
  auto bdd = Bdd::invalid();
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;

  EXPECT_THROW( bdd / cube,
		std::invalid_argument );
}

TEST_F(BddTest, div2_invalid2)
{ // cube が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto cube = BddCube::invalid();

  EXPECT_THROW( bdd / cube,
		std::invalid_argument );
}

TEST_F(BddTest, div_int2)
{ // (BddCube) のバージョン
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;
  auto bdd1 = bdd /= cube;

  const char* exp_str = "01010101";
  check(bdd, exp_str);
  check(bdd1, exp_str);
}

TEST_F(BddTest, div_int2_invalid1)
{ // bdd が invalid
  auto bdd = Bdd::invalid();
  auto lit0 = literal(0);
  auto lit1 = literal(1);
  auto cube = ~lit0 & lit1;

  EXPECT_THROW( bdd /= cube,
		std::invalid_argument );
}

TEST_F(BddTest, div_int2_invalid2)
{ // cube が invalid
  const char* src_str = "10010111";
  auto bdd = from_truth(src_str);
  auto cube = BddCube::invalid();

  EXPECT_THROW( bdd /= cube,
		std::invalid_argument );
}

TEST_F(BddTest, check_sup1)
{
  auto bdd = from_truth("1100");

  test_check_sup(bdd, 0);
  test_check_sup(bdd, 1);
}

TEST_F(BddTest, check_sym1)
{
  auto bdd = from_truth("11101010");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym2)
{
  auto bdd = from_truth("01001011");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym3)
{
  auto bdd = from_truth("0110");

  test_check_sym(bdd, 0, 1);
}

TEST_F(BddTest, check_sym4)
{
  auto bdd = from_truth("01100101");

  test_check_sym(bdd, 0, 1);
  test_check_sym(bdd, 0, 2);
  test_check_sym(bdd, 1, 2);
}

TEST_F(BddTest, check_sym5)
{
  auto bdd = from_truth("0100");

  test_check_sym(bdd, 0, 1);
}

TEST_F(BddTest, get_support1)
{
  auto lit1 = literal_bdd(1);
  auto lit3 = literal_bdd(3);
  auto lit5 = literal_bdd(5);
  auto lit7 = literal_bdd(7);

  auto bdd = (lit1 & lit3) | (~lit1 & lit5) | (lit3 & ~lit7);

  auto sup = bdd.get_support();

  test_support(bdd, sup, 8);
}

TEST_F(BddTest, get_support_invalid1)
{
  Bdd bdd1; // 不正値

  EXPECT_THROW({
      auto sup1 = bdd1.get_support();
    }, std::invalid_argument );
}

TEST_F(BddTest, get_support2)
{
  auto lit1 = literal_bdd(1);
  auto lit2 = literal_bdd(2);
  auto lit3 = literal_bdd(3);

  auto bdd = (lit1 & lit2) | (~lit1 & lit2) | lit3;

  auto sup = bdd.get_support();

  test_support(bdd, sup, 4);
}

TEST_F(BddTest, get_onepath1)
{
  auto lit1 = literal_bdd(0);
  auto lit2 = literal_bdd(1);
  auto lit3 = literal_bdd(2);

  auto bdd = lit1 & ~lit2 & lit3;

  auto bdd1 = bdd.get_onepath();

  EXPECT_EQ( bdd, bdd1 );
}

TEST_F(BddTest, get_onepath_invalid1)
{
  auto bdd = invalid();
  EXPECT_THROW( bdd.get_onepath(),
		std::invalid_argument );
}

TEST_F(BddTest, get_onepath2)
{
  auto bdd = Bdd::zero();

  auto bdd1 = bdd.get_onepath();

  EXPECT_TRUE( bdd1.is_invalid() );
}

TEST_F(BddTest, get_onepath3)
{
  auto lit1 = literal_bdd(0);
  auto lit2 = literal_bdd(1);
  auto lit3 = literal_bdd(2);

  auto bdd = lit1 & ~lit2 | lit3;

  auto bdd1 = bdd.get_onepath();

  test_onepath(bdd, bdd1);
}

TEST_F(BddTest, get_zeropath1)
{
  auto lit1 = literal_bdd(0);
  auto lit2 = literal_bdd(1);
  auto lit3 = literal_bdd(2);

  auto bdd = lit1 | ~lit2 | lit3;

  auto bdd1 = bdd.get_zeropath();

  auto exp_bdd = ~lit1 & lit2 & ~lit3;

  EXPECT_EQ( exp_bdd, bdd1 );
}

TEST_F(BddTest, get_zeropath_invalid1)
{
  Bdd bdd; // 不正値
  EXPECT_THROW(bdd.get_zeropath(),
	       std::invalid_argument );
}

TEST_F(BddTest, get_zeropath2)
{
  auto bdd = Bdd::one();

  auto bdd1 = bdd.get_zeropath();

  auto exp_bdd = Bdd::invalid();

  EXPECT_EQ( exp_bdd, bdd1 );
}

TEST_F(BddTest, get_zeropath3)
{
  auto lit1 = literal_bdd(0);
  auto lit2 = literal_bdd(1);
  auto lit3 = literal_bdd(2);

  auto bdd = lit1 & ~lit2 | lit3;

  auto bdd1 = bdd.get_zeropath();

  test_zeropath(bdd, bdd1);
}

TEST_F(BddTest, root_decomp1)
{
  auto bdd = from_truth("1000");

  Bdd bdd0;
  Bdd bdd1;
  auto top = bdd.root_decomp(bdd0, bdd1);

  EXPECT_EQ( 0, top.varid() );
  EXPECT_TRUE( bdd0.is_zero() );

  Bdd bdd00;
  Bdd bdd01;
  EXPECT_THROW( bdd0.root_decomp(bdd00, bdd01),
		std::invalid_argument );

  Bdd bdd10;
  Bdd bdd11;
  auto second = bdd1.root_decomp(bdd10, bdd11);

  EXPECT_EQ( 1, second.varid() );
  EXPECT_TRUE( bdd10.is_zero() );
  EXPECT_TRUE( bdd11.is_one() );
}

TEST_F(BddTest, root_decomp_invalid1)
{
  auto bdd = Bdd::invalid();

  Bdd bdd0;
  Bdd bdd1;
  EXPECT_THROW( bdd.root_decomp(bdd0, bdd1),
		std::invalid_argument );
}

TEST_F(BddTest, root_decomp2)
{
  auto bdd = from_truth("1000");

  auto top = bdd.root_var();
  auto bdd0 = bdd.root_cofactor0();
  auto bdd1 = bdd.root_cofactor1();

  EXPECT_EQ( 0, top.varid() );
  EXPECT_TRUE( bdd0.is_zero() );

  EXPECT_THROW( bdd0.root_var(),
		std::invalid_argument );
  EXPECT_THROW( bdd0.root_cofactor0(),
		std::invalid_argument );
  EXPECT_THROW( bdd0.root_cofactor1(),
		std::invalid_argument );

  auto second = bdd1.root_var();
  auto bdd10 = bdd1.root_cofactor0();
  auto bdd11 = bdd1.root_cofactor1();

  EXPECT_EQ( 1, second.varid() );
  EXPECT_TRUE( bdd10.is_zero() );
  EXPECT_TRUE( bdd11.is_one() );
}

TEST_F(BddTest, root_inv1)
{
  auto bdd = Bdd::zero();

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv2)
{
  auto bdd = Bdd::one();

  EXPECT_TRUE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv3)
{
  auto bdd = from_truth("1110");

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv4)
{
  auto bdd = from_truth("0001");

  EXPECT_TRUE( bdd.root_inv() );
}

TEST_F(BddTest, root_inv_invalid1)
{
  auto bdd = invalid();

  EXPECT_FALSE( bdd.root_inv() );

  auto ibdd = ~bdd;

  EXPECT_FALSE( bdd.root_inv() );
}

TEST_F(BddTest, size1)
{
  auto bdd = Bdd::zero();

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, size2)
{
  auto bdd = Bdd::one();

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, size3)
{
  auto bdd = from_truth("1000");

  EXPECT_EQ( 2, bdd.size() );
}

TEST_F(BddTest, size4)
{
  auto bdd1 = from_truth("1100");
  auto bdd2 = from_truth("1010");
  auto bdd3 = from_truth("1000");

  EXPECT_EQ( 1, bdd1.size() );
  EXPECT_EQ( 1, bdd2.size() );
  EXPECT_EQ( 2, bdd3.size() );
  EXPECT_EQ( 3, bdd_size({bdd1, bdd2, bdd3}) );
}

TEST_F(BddTest, size_invalid1)
{
  Bdd bdd; // 不正値

  EXPECT_EQ( 0, bdd.size() );
}

TEST_F(BddTest, is_identical1)
{
  Bdd bdd1 = from_truth("1011");
  Bdd bdd2 = from_truth("1011");

  EXPECT_TRUE( is_identical(bdd1, bdd2) );

  BddMgr mgr2;

  Bdd bdd3 = mgr2.from_truth("1011");
  EXPECT_TRUE( is_identical(bdd1, bdd3) );
}

TEST_F(BddTest, is_identical2)
{
  Bdd bdd1 = from_truth("1011");
  Bdd bdd2 = from_truth("1010");

  EXPECT_FALSE( is_identical(bdd1, bdd2) );

  BddMgr mgr2;
  Bdd bdd3 = mgr2.from_truth("1010");
  EXPECT_FALSE( is_identical(bdd1, bdd3) );
}

TEST_F(BddTest, is_identical_invalid1)
{
  Bdd bdd1; // 不正値
  Bdd bdd2 = from_truth("1011");

  EXPECT_FALSE( is_identical(bdd1, bdd2) );
}

TEST_F(BddTest, is_identical_invalid2)
{
  Bdd bdd1 = from_truth("1011");
  Bdd bdd2; // 不正値

  EXPECT_FALSE( is_identical(bdd1, bdd2) );
}

TEST_F(BddTest, dump_restore)
{
  auto bdd1 = from_truth("1100");
  auto bdd2 = from_truth("1010");
  auto bdd3 = from_truth("1000");

  std::ostringstream obuf;
  BinEnc enc{obuf};
  dump(enc, {bdd1, bdd2, bdd3});
  auto tmp = obuf.str();

  std::istringstream ibuf{tmp};
  BinDec dec{ibuf};
  auto bdd_list = mMgr.restore(dec);

  EXPECT_EQ( 3, bdd_list.size() );
  EXPECT_EQ( bdd1, bdd_list[0] );
  EXPECT_EQ( bdd2, bdd_list[1] );
  EXPECT_EQ( bdd3, bdd_list[2] );
}

TEST_F(BddTest, display1)
{
  auto bdd1 = from_truth("1000");

  std::ostringstream os;
  display(os, bdd1);

  static const char* exp_str =
    "     2 \n"
    "     2:    0   ZERO:      1 \n"
    "     1:    1   ZERO:     ONE\n";

  EXPECT_EQ( exp_str, os.str() );
}

TEST_F(BddTest, check_intersect1)
{
  auto bdd1 = from_truth("1000");
  auto bdd2 = from_truth("1110");

  EXPECT_TRUE( check_intersect(bdd1, bdd2) );
}

TEST_F(BddTest, check_intersect2)
{
  auto bdd1 = from_truth("00001111");
  auto bdd2 = from_truth("11100000");

  EXPECT_FALSE( check_intersect(bdd1, bdd2) );
}

TEST_F(BddTest, check_containment1)
{
  auto bdd1 = from_truth("11110000");
  auto bdd2 = from_truth("11100000");

  EXPECT_TRUE( check_containment(bdd1, bdd2) );
}

END_NAMESPACE_YM
