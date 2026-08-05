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
#include "ym/pt/PtExpr.h"
#include "ym/pt/PtMisc.h"


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

  // PtExpr の名前関係のテストを行う．
  void
  check_expr_name(
    const PtExpr* expr,                          // 対象の PtExpr
    const char* name = nullptr,                  // 期待される名前
    const std::vector<const char*>& nb_list = {} // 階層名のリスト
  )
  {
    EXPECT_STREQ( name, expr->name() );
    auto n = nb_list.size();
    EXPECT_EQ( n, expr->namebranch_num() );
    EXPECT_THROW( expr->namebranch(n),
		  std::out_of_range );
    std::vector<const PtNameBranch*> exp_list;
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


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  PtMgr ptmgr;
  Parser parser;
  FileInfo file_info;

};

END_NAMESPACE_YM_VERILOG

#endif // PARSERTEST_H
