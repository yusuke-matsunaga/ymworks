#ifndef PARSERTEST_H
#define PARSERTEST_H

/// @file ParserTest.h
/// @brief ParserTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "dotlib/dotlib_nsdef.h"
#include "ym/MsgMgr.h"
#include "ym/StrListMsgHandler.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

class ParserTest :
  public ::testing::Test
{
public:

  ParserTest()
    : mh2{std::cout}
  {
    MsgMgr::attach_handler(&mh);
    //MsgMgr::attach_handler(&mh2);
  }

  FileInfo info{"parser_test.lib"};
  std::string kwd;
  FileRegion kwd_loc;
  StrListMsgHandler mh;
  StreamMsgHandler mh2;

};

class ParserTestLibrary :
  public ParserTest
{
public:

  ParserTestLibrary()
  {
    kwd = "library";
  }

};

END_NAMESPACE_YM_DOTLIB

#endif // PARSERTEST_H
