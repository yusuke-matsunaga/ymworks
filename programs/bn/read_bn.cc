
/// @file read_bn.cc
/// @brief BnModel::read_bn() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnModel.h"
#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"
#include <libgen.h> // for basename
#include <unistd.h> // for getopt


BEGIN_NAMESPACE_YM

static char* argv0 = nullptr;

void
usage()
{
  using namespace std;

  cerr << "USAGE : " << basename(argv0) << " blif-file" << endl;
}

int
read_bn(
  int argc,
  char** argv
)
{
  argv0 = argv[0];

  std::string input_fmt = "";
  int ch;
  while ( (ch = getopt(argc, argv, "i:")) != -1 ) {
    switch ( ch ) {
    case 'i':
      input_fmt = optarg;
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

  auto filename = argv[0];

  StreamMsgHandler msg_handler(std::cerr);
  MsgMgr::attach_handler(&msg_handler);

  try {
    auto model = BnModel::read(filename, input_fmt);
    model.write(std::cout);
  }
  catch ( std::invalid_argument err ) {
    std::cout << err.what() << std::endl;
  }

  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::read_bn(argc, argv);
}
