
/// @file read_mislib_test.cc
/// @brief mislib 形式のファイル読み込みのテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ClibCellLibrary.h"

BEGIN_NAMESPACE_YM

int
read_mislib(
  int argc,
  char** argv
)
{
  auto data_dir = std::filesystem::path{DATA_DIR};
  auto filename = data_dir / "clib" / "lib2.genlib";

  auto library = ClibCellLibrary::read_mislib(filename);

  library.display(std::cout);

  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::read_mislib(argc, argv);
}
