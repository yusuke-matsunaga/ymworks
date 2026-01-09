
/// @file read_write_test.cc
/// @brief read_write_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include <libgen.h> // for basename
#include <unistd.h> // for getopt


BEGIN_NAMESPACE_YM

static char* argv0 = nullptr;

void
usage()
{
  std::cerr << "USAGE: " << basename(argv0)
	    << "[-i aag|aig|blif|iscas89] [-o aag|aig] <filename>"
	    << std::endl;
}

int
read_write_test(
  int argc,
  char** argv
)
{
  argv0 = argv[0];

  auto input_fmt = std::string{};
  auto output_fmt = std::string{"aag"};
  int ch;
  while ( (ch = getopt(argc, argv, "i:o:")) != -1 ) {
    switch ( ch ) {
    case 'i':
      input_fmt = optarg;
      break;
    case 'o':
      output_fmt = optarg;
      break;
    case '?':
    default:
      usage();
      return 255;
    }
  }
  argc -= optind;
  argv += optind;
  if ( argc != 1 ) {
    usage();
    return 255;
  }
  auto filename = std::string{argv[0]};

  auto mgr = AigMgr::read(filename, input_fmt);
  if ( output_fmt != "aag" &&
       output_fmt != "aig" ) {
    usage();
    return 255;
  }

  mgr.write(std::cout, output_fmt);

  return 0;
}

END_NAMESPACE_YM

int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::read_write_test(argc, argv);
}
