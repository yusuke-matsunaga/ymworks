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
