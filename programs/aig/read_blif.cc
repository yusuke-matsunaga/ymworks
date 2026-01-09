
/// @file read_blif.cc
/// @brief AigMgr::read_blif() のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"
#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"
#include <libgen.h>


void
usage(
  char* argv0
)
{
  using namespace std;

  cerr << "USAGE : " << basename(argv0) << " blif-file" << endl;
}

int
main(
  int argc,
  char** argv
)
{
  using namespace std;
  using namespace nsYm;
  using namespace nsYm::nsAig;

  int base = 1;

  if ( base + 1 != argc ) {
    usage(argv[0]);
    return 2;
  }

  string filename = argv[base];

  StreamMsgHandler msg_handler(cerr);
  MsgMgr::attach_handler(&msg_handler);

  try {
    auto mgr = AigMgr::read_blif(filename);
    mgr.write_aag(cout);
  }
  catch ( std::invalid_argument err ) {
    cout << err.what() << endl;
  }

  return 0;
}
