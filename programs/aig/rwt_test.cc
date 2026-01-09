
/// @file rwt_test.cc
/// @brief rwt_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/AigMgr.h"


BEGIN_NAMESPACE_YM

int
rwt_test(
  int argc,
  char** argv
)
{
  if ( argc != 2 ) {
    std::cerr << "USAGE: rwt_test <aig-file>"
	      << std::endl;
    return -1;
  }
  auto filename = std::string{argv[1]};
  try {
    auto mgr = AigMgr::read(filename);
    mgr.print(std::cout);
    std::cout << "Before rewrite: " << mgr.and_num()
	      << std::endl;
    mgr.rewrite();
    std::cout << "After rewrite: " << mgr.and_num()
	      << std::endl;
  }
  catch ( std::invalid_argument err ) {
    std::cerr << err.what()
	      << std::endl;
    return -1;
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
  return YM_NAMESPACE::rwt_test(argc, argv);
}
