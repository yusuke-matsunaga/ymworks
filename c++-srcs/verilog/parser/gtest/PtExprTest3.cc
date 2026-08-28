
/// @file PtExprTest3.cc
/// @brief PtExprTest の実装ファイルその3
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtExprTest.h"


BEGIN_NAMESPACE_YM_VERILOG

TEST_F(PtExprTest, FuncCall1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "func1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = factory.new_IntConst(fr2, 2U);
  auto arg_list = PtExprList();
  arg_list.init();
  arg_list.add(arg1);
  arg_list.add(arg2);
  auto expr = factory.new_FuncCall(fr, name, arg_list.top);

  check_FuncCall(expr, fr, name, {arg1, arg2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "func1(1, 2)", expr->decompile() );
}

TEST_F(PtExprTest, FuncCall2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto head = "head1";
  auto name = "func1";
  auto hname = parser.new_HierName(head, name);
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = factory.new_IntConst(fr2, 2U);
  auto arg_list = PtExprList();
  arg_list.init();
  arg_list.add(arg1);
  arg_list.add(arg2);
  auto expr = factory.new_FuncCall(fr, hname, arg_list.top);

  check_FuncCall(expr, fr, name, {NameBranchSpec{head, 0}}, {arg1, arg2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "head1.func1(1, 2)", expr->decompile() );
}

TEST_F(PtExprTest, SysFuncCall)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto name = "$func1";
  auto fr1 = make_file_region(1, 1, 1, 1);
  auto arg1 = factory.new_IntConst(fr1, 1U);
  auto fr2 = make_file_region(2, 2, 2, 2);
  auto arg2 = factory.new_IntConst(fr2, 2U);
  auto arg_list = PtExprList();
  arg_list.add(arg1);
  arg_list.add(arg2);
  auto expr = factory.new_SysFuncCall(fr, name, arg_list.top);

  check_SysFuncCall(expr, fr, name, {arg1, arg2});
  EXPECT_FALSE( expr->is_index_expr() );
  EXPECT_THROW( expr->index_value(),
		std::logic_error );
  EXPECT_EQ( "$func1(1, 2)", expr->decompile() );
}

#if 0
TEST_F(PtExprTest, IntConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  SizeType uint_val = 1234;
  auto bv_val = BitVector(uint_val);
  auto expr = factory.new_IntConst(fr, uint_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Int, bv_val, nullptr);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( uint_val, expr->index_value() );
  EXPECT_EQ( "1234", expr->decompile() );
}

TEST_F(PtExprTest, IntConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* str_val = "1234";
  auto bv_val = BitVector(0, false, 10, str_val);
  auto int_val = bv_val.to_int();
  auto expr = factory.new_IntConst(fr, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Int, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( str_val, expr->decompile() );
}

TEST_F(PtExprTest, DecConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* str_val = "1234";
  auto bv_val = BitVector(0, false, 10, str_val);
  auto int_val = bv_val.to_int();
  auto expr = factory.new_IntConst(fr, VpiConstType::Dec, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Dec, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "d1234", expr->decompile() );
}

TEST_F(PtExprTest, DecConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  SizeType size = 16;
  const char* str_val = "1234";
  auto bv_val = BitVector(16, false, 10, str_val);
  int int_val = bv_val.to_int();
  auto expr = factory.new_IntConst(fr, size, VpiConstType::Dec, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, size, VpiConstType::Dec, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "16'd1234", expr->decompile() );
}

TEST_F(PtExprTest, BinConst1)
{
  auto fr = make_file_region(1, 2, 3, 4);
  const char* str_val = "1001";
  auto bv_val = BitVector(0, false, 2, str_val);
  int int_val = bv_val.to_int();
  auto expr = factory.new_IntConst(fr, VpiConstType::Binary, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, 0, VpiConstType::Binary, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "b1001", expr->decompile() );
}

TEST_F(PtExprTest, BinConst2)
{
  auto fr = make_file_region(1, 2, 3, 4);
  SizeType size = 4;
  const char* str_val = "1001";
  auto bv_val = BitVector(size, false, 2, str_val);
  int int_val = bv_val.to_int();
  auto expr = factory.new_IntConst(fr, size, VpiConstType::Binary, str_val);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  check_expr_int_const(expr, size, VpiConstType::Binary, bv_val, str_val);
  EXPECT_TRUE( expr->is_index_expr() );
  EXPECT_EQ( int_val, expr->index_value() );
  EXPECT_EQ( "4'b1001", expr->decompile() );
}

TEST_F(PtExprTest, RealConst)
{
  auto fr = make_file_region(1, 2, 3, 4);
  double value = 123.456;
  auto expr = parser.new_RealConst(fr, value);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  EXPECT_EQ( fr, expr->file_region() );
  EXPECT_EQ( AstExpr::Const, expr->type() );
  EXPECT_FALSE( expr->is_index_expr() );
  std::ostringstream buf;
  buf << value;
  EXPECT_EQ( buf.str(), expr->decompile() );
  check_expr_no_opr(expr);
  check_expr_no_primary(expr);
  EXPECT_EQ( VpiConstType::Real, expr->const_type() );
  EXPECT_THROW( expr->const_size(),
		std::logic_error );
  EXPECT_THROW( expr->const_bitvect(),
		std::logic_error );
  EXPECT_THROW( expr->const_str(),
		std::logic_error );
  EXPECT_EQ( value, expr->const_real() );
}

TEST_F(PtExprTest, StringConst)
{
  auto fr = make_file_region(1, 2, 3, 4);
  auto value = "Hello, Verilog-HDL!";
  auto expr = parser.new_StringConst(fr, value);

  ASSERT_TRUE( expr != nullptr );
  check_expr_name(expr);
  EXPECT_EQ( fr, expr->file_region() );
  EXPECT_EQ( AstExpr::Const, expr->type() );
  EXPECT_FALSE( expr->is_index_expr() );
  std::ostringstream buf;
  buf << value;
  EXPECT_EQ( buf.str(), expr->decompile() );
  check_expr_no_opr(expr);
  check_expr_no_primary(expr);
  EXPECT_EQ( VpiConstType::String, expr->const_type() );
  EXPECT_THROW( expr->const_size(),
		std::logic_error );
  EXPECT_THROW( expr->const_bitvect(),
		std::logic_error );
  EXPECT_EQ( buf.str(), expr->const_str() );
  EXPECT_THROW( expr->const_real(),
		std::logic_error );
}
#endif

END_NAMESPACE_YM_VERILOG
