#ifndef PARSERTEST_EXPR_H
#define PARSERTEST_EXPR_H

/// @file ParserTest_Expr.h
/// @brief ParserTest_Expr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "ParserTest_Expr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ParserTest_Expr ParserTest_Expr.h "ParserTest_Expr.h"
/// @brief ParserTest の AstExpr用テスト環境
//////////////////////////////////////////////////////////////////////
class ParserTest_Expr :
  public ParserTest
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // AstExpr の 単項演算子関係のテストを行う．
  void
  check_expr_opr1(
    const AstExpr* expr,    // 対称の AstExpr
    VpiOpType op_type,      // 演算の型
    const AstExpr* operand0 // オペランド
  )
  {
    EXPECT_EQ( AstExpr::Opr, expr->type() );
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
    EXPECT_EQ( operand0, expr->operand0() );
    EXPECT_THROW( expr->operand1(),
		  std::logic_error );
    EXPECT_THROW( expr->operand2(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_num(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
  }

  // AstExpr の 二項演算子関係のテストを行う．
  void
  check_expr_opr2(
    const AstExpr* expr,     // 対称の AstExpr
    VpiOpType op_type,       // 演算の型
    const AstExpr* operand0, // オペランド
    const AstExpr* operand1  // オペランド
  )
  {
    EXPECT_EQ( AstExpr::Opr, expr->type() );
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
    EXPECT_EQ( operand0, expr->operand0() );
    EXPECT_EQ( operand1, expr->operand1() );
    EXPECT_THROW( expr->operand2(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_num(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
  }

  // AstExpr の 三項演算子関係のテストを行う．
  void
  check_expr_opr3(
    const AstExpr* expr,     // 対称の AstExpr
    VpiOpType op_type,       // 演算の型
    const AstExpr* operand0, // オペランド
    const AstExpr* operand1, // オペランド
    const AstExpr* operand2  // オペランド
  )
  {
    EXPECT_EQ( AstExpr::Opr, expr->type() );
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
    EXPECT_EQ( operand0, expr->operand0() );
    EXPECT_EQ( operand1, expr->operand1() );
    EXPECT_EQ( operand2, expr->operand2() );
    EXPECT_THROW( expr->operand_num(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
  }

  // AstExpr の Concat Operation 関係のテストを行う．
  void
  check_expr_concat(
    const AstExpr* expr,                            // 対称の AstExpr
    VpiOpType op_type,                              // 演算の型
    const std::vector<const AstExpr*>& operand_list // オペランドのリスト
  )
  {
    EXPECT_EQ( AstExpr::Opr, expr->type() );
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
    EXPECT_THROW( expr->operand0(),
		  std::logic_error);
    EXPECT_THROW( expr->operand1(),
		  std::logic_error);
    EXPECT_THROW( expr->operand2(),
		  std::logic_error);
    check_expr_operand(expr, operand_list);
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
  }

  // AstExpr の MultiConcat Operation 関係のテストを行う．
  void
  check_expr_multiconcat(
    const AstExpr* expr,                            // 対称の AstExpr
    VpiOpType op_type,                              // 演算の型
    const AstExpr* rep,                             // 繰り返し数
    const std::vector<const AstExpr*>& operand_list // オペランドのリスト
  )
  {
    EXPECT_EQ( AstExpr::Opr, expr->type() );
    EXPECT_EQ( VpiOpType::MultiConcat, expr->op_type() );
    EXPECT_EQ( rep, expr->rep() );
    EXPECT_THROW( expr->operand0(),
		  std::logic_error);
    EXPECT_THROW( expr->operand1(),
		  std::logic_error);
    EXPECT_THROW( expr->operand2(),
		  std::logic_error);
    check_expr_operand(expr, operand_list);
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
  }

  // AstExpr の Primary 関係のテストを行う．
  void
  check_expr_primary(
    const AstExpr* expr,                          // 対象の AstExpr
    bool is_const_index,                         // 添字に使える時 true
    const std::vector<const AstExpr*>& index_list // インデックスのリスト
    = {},
    const AstPart* part = nullptr                 // 範囲指定
  )
  {
    EXPECT_EQ( AstExpr::Primary, expr->type() );
    EXPECT_EQ( is_const_index, expr->is_const_index() );
    auto num = index_list.size();
    EXPECT_EQ( num, expr->index_num() );
    auto index_vec = expr->index_list().to_vector();
    for ( SizeType i = 0; i < num; ++ i ) {
      EXPECT_EQ( index_list[i], index_vec[i] );
    }
    EXPECT_EQ( part, expr->part() );
    auto is_simple = (num == 0) && (part == nullptr);
    EXPECT_EQ( is_simple, expr->is_simple() );
    check_expr_no_opr(expr);
    check_expr_no_const(expr);
  }

  // AstExpr の関数呼び出し関係のテストを行う．
  void
  check_expr_funccall(
    const AstExpr* expr,
    AstExpr::Type type,
    const std::vector<const AstExpr*>& arg_list
  )
  {
    EXPECT_EQ( type, expr->type() );
    check_expr_operand(expr, arg_list);
    // operand() を使っているので check_expr_no_opr() は使えない．
    EXPECT_THROW( expr->op_type(),
		  std::logic_error );
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
  }

  // AstExpr のオペランド関係のテストを行う．
  void
  check_expr_operand(
    const AstExpr* expr,
    const std::vector<const AstExpr*>& operand_list
  )
  {
    auto num = operand_list.size();
    EXPECT_EQ( num, expr->operand_num() );
    SizeType index = 0;
    for ( auto expr1: expr->operand_list() ) {
      EXPECT_EQ( operand_list[index], expr1 );
      ++ index;
    }
  }

  // AstExpr の整数定数関係のテストを行う．
  void
  check_expr_int_const(
    const AstExpr* expr,
    SizeType size,
    VpiConstType type,
    const BitVector& bv_val,
    const char* str_val
  )
  {
    EXPECT_EQ( AstExpr::Type::Const, expr->type() );
    EXPECT_EQ( type, expr->const_type() );
    EXPECT_EQ( size, expr->const_size() );
    EXPECT_EQ( bv_val, expr->const_bitvect() );
    EXPECT_EQ( bv_val.verilog_string(), expr->const_bitvect().verilog_string() );
    EXPECT_STREQ( str_val, expr->const_str() );
    EXPECT_THROW( expr->const_real(),
		  std::logic_error );
    check_expr_no_opr(expr);
    check_expr_no_primary(expr);
  }

  // AstExpr の Operation でない時のテストを行う．
  void
  check_expr_no_opr(
    const AstExpr* expr // 対象の AstExpr
  )
  {
    EXPECT_THROW( expr->op_type(),
		  std::logic_error );
    EXPECT_THROW( expr->operand0(),
		  std::logic_error );
    EXPECT_THROW( expr->operand1(),
		  std::logic_error );
    EXPECT_THROW( expr->operand2(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_num(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
  }

  // AstExpr の Primary でない時のテストを行う．
  void
  check_expr_no_primary(
    const AstExpr* expr // 対象の AstExpr
  )
  {
    EXPECT_FALSE( expr->is_const_index() );
    EXPECT_EQ( 0, expr->index_num() );
    auto index_vec = expr->index_list().to_vector();
    EXPECT_EQ( std::vector<const AstExpr*>{}, index_vec );
    EXPECT_EQ( nullptr, expr->part() );
    EXPECT_THROW( expr->is_simple(),
		  std::logic_error );
  }

  // AstExpr の定数でない時のテストを行う．
  void
  check_expr_no_const(
    const AstExpr* expr // 対象の AstExpr
  )
  {
    EXPECT_THROW( expr->const_type(),
		  std::logic_error );
    EXPECT_THROW( expr->const_size(),
		  std::logic_error );
    EXPECT_THROW( expr->const_bitvect(),
		  std::logic_error );
    EXPECT_THROW( expr->const_str(),
		  std::logic_error );
    EXPECT_THROW( expr->const_real(),
		  std::logic_error );
  }

};

END_NAMESPACE_YM_VERILOG

#endif // PARSERTEST_EXPR_H
