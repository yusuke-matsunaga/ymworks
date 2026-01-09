
/// @file dotlib_parser_test.cc
/// @brief dotlib_parser_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "dotlib/Parser.h"
#include "parse_group.h"


BEGIN_NAMESPACE_YM_DOT

int
parser_test(
  int argc,
  char** argv
)
{
  istringstream buf("( test ) {\n"
		    "  piece_type: piece_length;\n"
		    "}\n");
  FileInfo info{"dotlib_parser_test.lib"};
  string kwd;
  FileRegion kwd_loc;
  Parser parser{buf, info, false, false};

  auto dst = group_library(parser, kwd, kwd_loc);

  return 0;
}

END_NAMESPACE_YM_DOTLIB


int
main(
  int argc,
  char** argv
)
{
  return nsYm::nsClib::nsDotlib::parser_test(argc, argv);
}
