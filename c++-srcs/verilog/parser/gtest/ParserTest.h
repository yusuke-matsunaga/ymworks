#ifndef PARSERTEST_H
#define PARSERTEST_H

/// @file ParserTest.h
/// @brief ParserTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2026 Yusuke Matsunaga
/// All rights reserved.

#include <gtest/gtest.h>
#include "parser/Parser.h"
#include "ym/vl/BitVector.h"
#include "parser/PtDecl.h"
#include "parser/PtExpr.h"
#include "parser/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ParserTest ParserTest.h "ParserTest.h"
/// @brief Parser のテスト環境
//////////////////////////////////////////////////////////////////////
class ParserTest :
  public ::testing::Test
{
public:

  // コンストラクタ
  ParserTest(
  ) : parser(ptmgr),
      file_info("filename1")
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // FileRegion を作る．
  FileRegion
  make_file_region(
    SizeType start_line,
    SizeType start_column,
    SizeType end_line,
    SizeType end_column
  )
  {
    return FileRegion(file_info,
		      start_line, start_column,
		      end_line, end_column);
  }

  // Range を作る．
  const PtRange*
  new_range(
    const FileRegion& left_fr,
    SizeType left_val,
    const FileRegion& right_fr,
    SizeType right_val
  )
  {
    auto left = parser.new_IntConst(left_fr, left_val);
    auto right = parser.new_IntConst(right_fr, right_val);
    return parser.new_Range(FileRegion(left_fr, right_fr), left, right);
  }

  // Part を作る．
  const PtPart*
  new_part(
    const FileRegion& left_fr,
    SizeType left_val,
    const FileRegion& right_fr,
    SizeType right_val,
    VpiRangeMode mode
  )
  {
    auto left = parser.new_IntConst(left_fr, left_val);
    auto right = parser.new_IntConst(right_fr, right_val);
    return parser.new_Part(FileRegion(left_fr, right_fr),
			   mode, left, right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // AstExpr 用のテスト
  //////////////////////////////////////////////////////////////////////

  // AstExpr の名前関係のテストを行う．
  void
  check_expr_name(
    const AstExpr* expr,                          // 対象の AstExpr
    const char* name = nullptr,                  // 期待される名前
    const std::vector<const char*>& nb_list = {} // 階層名のリスト
  )
  {
    EXPECT_STREQ( name, expr->name() );
    auto n = nb_list.size();
    EXPECT_EQ( n, expr->namebranch_num() );
    EXPECT_THROW( expr->namebranch(n),
		  std::out_of_range );
    std::vector<const AstNameBranch*> exp_list;
    exp_list.reserve(n);
    for ( SizeType i = 0; i < n; ++ i ) {
      auto nb = expr->namebranch(i);
      EXPECT_STREQ( nb_list[i], nb->name() );
      exp_list.push_back(nb);
    }
    EXPECT_EQ( exp_list, expr->namebranch_list() );
    std::string exp_fullname;
    if ( expr->name() != nullptr ) {
      for ( SizeType i = 0; i < n; ++ i ) {
	auto nb = expr->namebranch(i);
	exp_fullname += nb_list[i];
	exp_fullname += ".";
      }
      exp_fullname += expr->name();
    }
    EXPECT_EQ( exp_fullname, expr->fullname() );
  }

  // AstExpr の Operation 関係のテストを行う．
  void
  check_expr_opr(
    const AstExpr* expr,                            // 対称の AstExpr
    VpiOpType op_type,                              // 演算の型
    const std::vector<const AstExpr*>& operand_list // オペランドのリスト
  )
  {
    EXPECT_EQ( AstExpr::Opr, expr->type() );
    EXPECT_EQ( op_type, expr->op_type() );
    EXPECT_THROW( expr->rep(),
		  std::logic_error );
    auto n = operand_list.size();
    EXPECT_EQ( n, expr->operand_num() );
    EXPECT_THROW( expr->operand(n),
		  std::out_of_range );
    if ( n >= 1 ) {
      EXPECT_EQ( operand_list[0], expr->operand0() );
      if ( n >= 2 ) {
	EXPECT_EQ( operand_list[1], expr->operand1() );
	if ( n >= 3 ) {
	  EXPECT_EQ( operand_list[2], expr->operand2() );
	}
      }
    }
    for ( SizeType i = 0; i < n; ++ i ) {
      EXPECT_EQ( operand_list[i], expr->operand(i) );
    }
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
    auto n = operand_list.size();
    EXPECT_EQ( n, expr->operand_num() );
    EXPECT_THROW( expr->operand(n),
		  std::out_of_range );
    if ( n >= 1 ) {
      EXPECT_EQ( operand_list[0], expr->operand0() );
      if ( n >= 2 ) {
	EXPECT_EQ( operand_list[1], expr->operand1() );
	if ( n >= 3 ) {
	  EXPECT_EQ( operand_list[2], expr->operand2() );
	}
      }
    }
    for ( SizeType i = 0; i < n; ++ i ) {
      EXPECT_EQ( operand_list[i], expr->operand(i) );
    }
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
    auto n = index_list.size();
    EXPECT_EQ( n, expr->index_num() );
    EXPECT_THROW( expr->index(n),
		  std::out_of_range );
    for ( SizeType i = 0; i < n; ++ i ) {
      EXPECT_EQ( index_list[i], expr->index(i) );
    }
    EXPECT_EQ( part, expr->part() );
    auto is_simple = (n == 0) && (part == nullptr);
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
    auto n = arg_list.size();
    EXPECT_EQ( n, expr->operand_num() );
    EXPECT_THROW( expr->operand(n),
		  std::out_of_range );
    for ( SizeType i = 0; i < n; ++ i ) {
      EXPECT_EQ( arg_list[i], expr->operand(i) );
    }
    // operand() を使っているので check_expr_no_opr() は使えない．
    EXPECT_THROW( expr->op_type(),
		  std::logic_error );
    check_expr_no_primary(expr);
    check_expr_no_const(expr);
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
    EXPECT_EQ( 0, expr->operand_num() );
    EXPECT_THROW( expr->operand(0),
		  std::out_of_range );
    EXPECT_THROW( expr->operand0(),
		  std::logic_error );
    EXPECT_THROW( expr->operand1(),
		  std::logic_error );
    EXPECT_THROW( expr->operand2(),
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
    EXPECT_THROW( expr->index(0),
		  std::out_of_range );
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

public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  AstMgr ptmgr;
  Parser parser;
  FileInfo file_info;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSERTEST_H
