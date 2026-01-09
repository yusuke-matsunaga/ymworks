
/// @file npn_gen.cc
/// @brief npn_gen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/TvFunc.h"
#include "ym/NpnMap.h"
#include <unistd.h>
#include <libgen.h>


BEGIN_NAMESPACE_YM

void
print(
  std::ostream& s,
  const std::vector<TvFunc>& func_list,
  int mode
)
{
  for ( auto& f: func_list ) {
    s << f.str(mode) << std::endl;
  }
}

int
npn_gen(
  int argc,
  char** argv
)
{
  SizeType ni = 0;
  int ch;
  bool bin = false;
  bool hex = false;
  auto argv0 = std::string{basename(argv[0])};
  while ( (ch = getopt(argc, argv, "bx012345")) != -1 ) {
    switch ( ch ) {
    case 'b': bin = true; break;
    case 'x': hex = true; break;
    case '0': ni = 0; break;
    case '1': ni = 1; break;
    case '2': ni = 2; break;
    case '3': ni = 3; break;
    case '4': ni = 4; break;
    case '5': ni = 5; break;
    case '?':
    default:
      goto usage;
    }
  }

  argc -= optind;
  argv += optind;

  if ( argc != 0 && argc != 1 ) {
  usage:
    std::cerr << "USAGE: " << basename(argv[0]) << " [-b|-x] [-0|-1|-2|-3|-4|-5]"
	      << " [<filename>]" << std::endl;
    return -1;
  }

  if ( bin && hex ) {
    std::cerr << "-b and -x are mutually exclusive" << std::endl;
    return -2;
  }
  int mode = 16;
  if ( bin ) {
    mode = 2;
  }
  if ( hex ) {
    mode = 16;
  }

  std::vector<TvFunc> func_list;
  SizeType ni_exp = 1UL << ni;
  SizeType ni_exp_exp = 1UL << ni_exp;
  for ( SizeType p = 0; p < ni_exp_exp; ++ p ) {
    std::vector<int> values;
    values.reserve(ni_exp);
    for ( SizeType b = 0; b < ni_exp; ++ b ) {
      if ( p & (1UL << b) ) {
	values.push_back(1);
      }
      else {
	values.push_back(0);
      }
    }
    auto tv = TvFunc(ni, values);
    auto map = tv.npn_canonical_map();
    auto tv0 = tv.xform(map);
    if ( tv0 == tv ) {
      func_list.push_back(tv);
    }
    std::cerr << (static_cast<double>(p + 1) / ni_exp_exp) * 100.0
	      << "\r";
    std::cerr.flush();
  }
  std::cerr << std::endl;
  std::cerr << "Total " << func_list.size() << " rep. funcs" << std::endl;

  if ( argc == 1 ) {
    // 出力ファイル名だと思う．
    std::ofstream s{argv[0]};
    if ( !s ) {
      std::cerr << argv[0] << ": Could not open" << std::endl;
      return -3;
    }
    print(s, func_list, mode);
  }
  else {
    print(std::cout, func_list, mode);
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
  return nsYm::npn_gen(argc, argv);
}
