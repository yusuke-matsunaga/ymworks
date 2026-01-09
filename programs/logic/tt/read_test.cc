
/// @file read_test.cc
/// @brief read_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TruthTable.h"
#include <libgen.h>


BEGIN_NAMESPACE_YM

int
read_test(
  int argc,
  char** argv
)
{
  if ( argc != 2 ) {
    std::cerr << "Usage: " << basename(argv[0])
	      << " <filename>" << std::endl;
    return -1;
  }

  auto filename = std::string{argv[1]};
  auto tt = TruthTable::read_file(filename);

  tt.print(std::cout);

  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  using namespace YM_NAMESPACE;
  return read_test(argc, argv);
}
