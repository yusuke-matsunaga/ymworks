#ifndef PTEXPRTEST_H
#define PTEXPRTEST_H

/// @file PtExprTest.h
/// @brief PtExprTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include "PtTest.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class PtExprTest PtExprTest.h "PtExprTest.h"
/// @brief PtTest の AstExpr用テスト環境
//////////////////////////////////////////////////////////////////////
class PtExprTest :
  public PtTest
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 単項演算子のテスト
  void
  check_Opr1(
    const AstExpr* expr,    // 対称の AstExpr
    const FileRegion& file_region,
    VpiOpType op_type,      // 演算の型
    const AstExpr* operand0 // オペランド
  )
  {
    check_common(expr, file_region, AstExpr::Opr);
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_EQ( operand0, expr->operand0() );
    EXPECT_THROW( expr->operand1(),
		  std::logic_error );
    EXPECT_THROW( expr->operand2(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
  }

  /// @brief 二項演算子関係のテスト
  void
  check_Opr2(
    const AstExpr* expr,     // 対称の AstExpr
    VpiOpType op_type,       // 演算の型
    const AstExpr* operand0, // オペランド
    const AstExpr* operand1  // オペランド
  )
  {
    auto file_region = FileRegion(operand0->file_region(), operand1->file_region());
    check_common(expr, file_region, AstExpr::Opr);
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_EQ( operand0, expr->operand0() );
    EXPECT_EQ( operand1, expr->operand1() );
    EXPECT_THROW( expr->operand2(),
		  std::logic_error );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
  }

  /// @brief 三項演算子関係のテスト
  void
  check_Opr3(
    const AstExpr* expr,     // 対称の AstExpr
    VpiOpType op_type,       // 演算の型
    const AstExpr* operand0, // オペランド
    const AstExpr* operand1, // オペランド
    const AstExpr* operand2  // オペランド
  )
  {
    auto file_region = FileRegion(operand0->file_region(), operand2->file_region());
    check_common(expr, file_region, AstExpr::Opr);
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_EQ( operand0, expr->operand0() );
    EXPECT_EQ( operand1, expr->operand1() );
    EXPECT_EQ( operand2, expr->operand2() );
    EXPECT_THROW( expr->operand_list(),
		  std::logic_error );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
  }

  /// @brief Concat Operation 関係のテスト
  void
  check_Concat(
    const AstExpr* expr,                            // 対称の AstExpr
    const FileRegion& file_region,
    const std::vector<const AstExpr*>& operand_list // オペランドのリスト
  )
  {
    check_common(expr, file_region, AstExpr::Opr);
    EXPECT_EQ( VpiOpType::Concat, expr->op_type() );
    EXPECT_THROW( expr->operand0(),
		  std::logic_error);
    EXPECT_THROW( expr->operand1(),
		  std::logic_error);
    EXPECT_THROW( expr->operand2(),
		  std::logic_error);
    EXPECT_EQ( operand_list, expr->operand_list().to_vector() );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
  }

  /// @brief MultiConcat Operation 関係のテスト
  void
  check_MultiConcat(
    const AstExpr* expr,                            // 対称の AstExpr
    const FileRegion& file_region,
    const AstExpr* rep,                             // 繰り返し数
    const std::vector<const AstExpr*>& operand_list // オペランドのリスト
  )
  {
    check_common(expr, file_region, AstExpr::Opr);
    EXPECT_EQ( VpiOpType::MultiConcat, expr->op_type() );
    EXPECT_THROW( expr->operand0(),
		  std::logic_error);
    EXPECT_THROW( expr->operand1(),
		  std::logic_error);
    EXPECT_THROW( expr->operand2(),
		  std::logic_error);
    EXPECT_EQ( operand_list, expr->operand_list().to_vector() );
    EXPECT_EQ( rep, expr->rep() );
  }

  /// @brief Primary 関係のテスト
  void
  check_Primary(
    const AstExpr* expr,                          // 対象の AstExpr
    const FileRegion& file_region,
    const char* name,
    bool is_const_index                         // 添字に使える時 true
  )
  {
    check_Primary(expr, file_region, name, {}, is_const_index);
  }

  /// @brief Primary 関係のテスト
  void
  check_Primary(
    const AstExpr* expr,                          // 対象の AstExpr
    const FileRegion& file_region,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list,
    bool is_const_index                         // 添字に使える時 true
  )
  {
    check_Primary(expr, file_region, name, nbspec_list, is_const_index, nullptr);
  }

  /// @brief Primary 関係のテスト
  void
  check_Primary(
    const AstExpr* expr,                          // 対象の AstExpr
    const FileRegion& file_region,
    const char* name,
    bool is_const_index,                // 添字に使える時 true
    const AstPart* part                 // 範囲指定
  )
  {
    check_Primary(expr, file_region, name, {}, is_const_index, {}, part);
  }

  /// @brief Primary 関係のテスト
  void
  check_Primary(
    const AstExpr* expr,                          // 対象の AstExpr
    const FileRegion& file_region,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list,
    bool is_const_index,                // 添字に使える時 true
    const AstPart* part                 // 範囲指定
  )
  {
    check_Primary(expr, file_region, name, nbspec_list, is_const_index, {}, part);
  }

  /// @brief Primary 関係のテスト
  void
  check_Primary(
    const AstExpr* expr,                          // 対象の AstExpr
    const FileRegion& file_region,
    const char* name,
    bool is_const_index,                           // 添字に使える時 true
    const std::vector<const AstExpr*>& index_list, // インデックスのリスト
    const AstPart* part = nullptr                  // 範囲指定
  )
  {
    check_Primary(expr, file_region, name, {}, is_const_index, index_list, part);
  }

  /// @brief Primary 関係のテスト
  void
  check_Primary(
    const AstExpr* expr,                          // 対象の AstExpr
    const FileRegion& file_region,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list,
    bool is_const_index,                           // 添字に使える時 true
    const std::vector<const AstExpr*>& index_list, // インデックスのリスト
    const AstPart* part = nullptr                  // 範囲指定
  )
  {
    check_common(expr, file_region, AstExpr::Primary, name, nbspec_list);
    EXPECT_EQ( is_const_index, expr->is_const_index() );
    EXPECT_EQ( index_list, expr->index_list().to_vector() );
    EXPECT_EQ( part, expr->part() );
    auto is_simple = (index_list.size() == 0) && (part == nullptr);
    EXPECT_EQ( is_simple, expr->is_simple() );
  }

  /// @brief 関数呼び出し関係のテスト
  void
  check_FuncCall(
    const AstExpr* expr,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const AstExpr*>& arg_list
  )
  {
    check_FuncCall(expr, file_region, name, {}, arg_list);
  }

  /// @brief 関数呼び出し関係のテスト
  void
  check_FuncCall(
    const AstExpr* expr,
    const FileRegion& file_region,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list,
    const std::vector<const AstExpr*>& arg_list
  )
  {
    check_common(expr, file_region, AstExpr::FuncCall, name, nbspec_list);
    EXPECT_EQ( arg_list, expr->arg_list().to_vector() );
  }

  /// @brief システム関数呼び出し関係のテスト
  void
  check_SysFuncCall(
    const AstExpr* expr,
    const FileRegion& file_region,
    const char* name,
    const std::vector<const AstExpr*>& arg_list
  )
  {
    check_common(expr, file_region, AstExpr::SysFuncCall, name, {});
    EXPECT_EQ( arg_list, expr->arg_list().to_vector() );
  }

  // AstExpr の整数定数関係のテストを行う．
  void
  check_expr_int_const(
    const AstExpr* expr,
    const FileRegion& file_region,
    SizeType size,
    VpiConstType type,
    const BitVector& bv_val,
    const char* str_val
  )
  {
    check_common(expr, file_region, AstExpr::Const);
    EXPECT_EQ( AstExpr::Type::Const, expr->type() );
    EXPECT_EQ( type, expr->const_type() );
    EXPECT_EQ( size, expr->const_size() );
    EXPECT_EQ( bv_val, expr->const_bitvect() );
    EXPECT_EQ( bv_val.verilog_string(), expr->const_bitvect().verilog_string() );
    EXPECT_STREQ( str_val, expr->const_str() );
    EXPECT_THROW( expr->const_real(),
		  std::logic_error );
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通のテスト
  void
  check_common(
    const AstExpr* expr,
    const FileRegion& file_region,
    AstExpr::Type type
  )
  {
    ASSERT_TRUE( expr != nullptr );
    check_HierNamedBase(expr, file_region);
    check_common_sub(expr, file_region, type);
  }

  /// @brief 共通のテスト
  void
  check_common(
    const AstExpr* expr,
    const FileRegion& file_region,
    AstExpr::Type type,
    const char* name
  )
  {
    ASSERT_TRUE( expr != nullptr );
    check_HierNamedBase(expr, file_region, name);
    check_common_sub(expr, file_region, type);
  }

  /// @brief 共通のテスト
  void
  check_common(
    const AstExpr* expr,
    const FileRegion& file_region,
    AstExpr::Type type,
    const char* name,
    const std::vector<NameBranchSpec>& nbspec_list
  )
  {
    ASSERT_TRUE( expr != nullptr );
    check_HierNamedBase(expr, file_region, name, nbspec_list);
    check_common_sub(expr, file_region, type);
  }

  /// @brief check_common() の下請け関数
  void
  check_common_sub(
    const AstExpr* expr,
    const FileRegion& file_region,
    AstExpr::Type type
  )
  {
    EXPECT_EQ( type, expr->type() );
    if ( type != AstExpr::Opr ) {
      EXPECT_THROW( expr->op_type(),
		    std::logic_error );
      EXPECT_THROW( expr->operand0(),
		    std::logic_error );
      EXPECT_THROW( expr->operand1(),
		    std::logic_error );
      EXPECT_THROW( expr->operand2(),
		    std::logic_error );
      EXPECT_THROW( expr->operand_list(),
		    std::logic_error );
      EXPECT_THROW( expr->rep(),
		    std::logic_error );
    }
    if ( type != AstExpr::FuncCall &&
	 type != AstExpr::SysFuncCall ) {
      EXPECT_THROW( expr->arg_list(),
		    std::logic_error );
    }
    if ( type != AstExpr::Primary ) {
      EXPECT_THROW( expr->is_const_index(),
		    std::logic_error );
      EXPECT_THROW( expr->index_list(),
		    std::logic_error );
      EXPECT_THROW( expr->part(),
		    std::logic_error );
      EXPECT_THROW( expr->is_simple(),
		    std::logic_error );
    }
    if ( type != AstExpr::Const ) {
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
  }
};

END_NAMESPACE_YM_VERILOG

#endif // PTEXPRTEST_H
