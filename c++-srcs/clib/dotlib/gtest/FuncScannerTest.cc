
/// @file FuncScannerTest.cc
/// @brief FuncScannerTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "FuncScanner.h"


BEGIN_NAMESPACE_YM_DOTLIB

TEST(FuncScannerTest, read_token1)
{
  FileRegion loc; // ダミー
  FuncScanner scanner{" * + & |  !", loc};

  TokenType exp_list[] = {
    TokenType::AND,
    TokenType::OR,
    TokenType::AND,
    TokenType::OR,
    TokenType::NOT,
    TokenType::END
  };

  for ( auto exp: exp_list ) {
    auto token = scanner.read_token();
    EXPECT_EQ( exp, token.type() );
  }
}

END_NAMESPACE_YM_DOTLIB
