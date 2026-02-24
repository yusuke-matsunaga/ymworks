
/// @file mincov.cc
/// @brief mincov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MinCov.h"
#include "ym/split.h"
#include "ym/Timer.h"
#include <libgen.h>


BEGIN_NAMESPACE_YM

void
usage(
  char* arg0
)
{
  std::cerr << "Usage: " << basename(arg0) << " <dimacs-file>" << std::endl;
}


MinCov
read_pi(
  const std::string& filename
)
{
  std::ifstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not open";
    throw std::invalid_argument{buf.str()};
  }

  MinCov mincov;
  std::string line;
  while ( getline(s, line) ) {
    if ( line.size() == 0 ) {
      // 空行はスキップ
      continue;
    }
    if ( line[0] == '#' ) {
      // 先頭の # はコメント行
      continue;
    }
    auto str_list = split(line);
    if ( str_list.size() != 2 ) {
      throw std::invalid_argument{"illegal format"};
    }
    auto row_str = str_list[0];
    auto col_str = str_list[1];
    int row = stoi(row_str);
    int col = stoi(col_str);
    if ( row == -1 && col == -1 ) {
      // End-Of-Data
      break;
    }
    mincov.insert_elem(static_cast<SizeType>(row),
		       static_cast<SizeType>(col));
  }
  return mincov;
}

int
mincov_test(
  int argc,
  char** argv
)
{
  std::string filename;
  std::string algorithm;

  switch ( argc ) {
  case 2:
    filename = argv[1];
    algorithm = "";
    break;

  case 3:
    filename = argv[2];
    algorithm = argv[1];
    break;

  default:
    usage(argv[0]);
    return 1;
  }

  try {
    Timer timer;
    timer.start();
    auto mincov = read_pi(filename);
    timer.stop();
    std::cout << "Reading " << filename << " end."
	      << std::endl
	      << " " << timer.get_time() / 1000.0 << "(s)"
	      << std::endl;

    timer.reset();
    auto option = JsonValue::object();
    option.add("algorithm", JsonValue{algorithm});
    timer.start();
    std::vector<SizeType> solution;
    auto nc = mincov.solve(solution, option);
    timer.stop();
    std::cout << "Mincov(" << algorithm << ") end."
	      << std::endl
	      << " " << nc << " columns. "
	      << timer.get_time() / 1000.0 << "(s)"
	      << std::endl;
  }
  catch ( std::invalid_argument& error ) {
    std::cerr << error.what() << std::endl;
    return 1;
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
  return nsYm::mincov_test(argc, argv);
}
