
/// @file all_4.cc
/// @brief all_4 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/Timer.h"
#include <unistd.h>
#include <libgen.h>


BEGIN_NAMESPACE_YM

int
all_4(
  int argc,
  char** argv
)
{
  auto argv0 = std::string{basename(argv[0])};
  bool cegar = false;
  bool verify = false;
  int loop_count = 1;
  int ch;
  while ( (ch = getopt(argc, argv, "cvl:")) != -1 ) {
    switch (ch) {
    case 'c': cegar = true; break;
    case 'v': verify = true; break;
    case 'l': loop_count = atoi(optarg); break;
    case '?':
    default:
      goto usage;
    }
  }

  argc -= optind;
  argv += optind;
  if ( argc != 0 ) {
  usage:
    std::cerr << "USAGE: " << argv0
	      << " [-c] [-v]" << std::endl;
    return -1;
  }

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("cegar", JsonValue(cegar));
  auto option = JsonValue(option_dict);

  double acc_time = 0.0;
  SizeType acc_count = 0;
  const SizeType NUM = 0x10000;
  for ( SizeType count = 0; count < loop_count; ++ count ) {
    for ( SizeType p = 0; p < NUM; ++ p ) {
      TruthTable tt(4, 1);
      for ( SizeType b = 0; b < 16; ++ b ) {
	if ( p & (1 << b) ) {
	  tt.set_value(b, 0, Val3::ONE);
	}
	else {
	  tt.set_value(b, 0, Val3::ZERO);
	}
      }
      std::cout << tt.hex_str();
      std::cout.flush();
      Timer timer;
      timer.start();
      auto g = ExSyn::gen_boolchain(tt, option);
      timer.stop();
      auto time = timer.get_time();
      std::cout << " " << time << " ms" << std::endl;
      acc_time += time;
      ++ acc_count;
      if ( verify ) {
	if ( !ExSyn::check_boolchain(tt, g) ) {
	  throw std::invalid_argument{"incorrect graph"};
	}
      }
    }
  }
  std::cout << "Total " << acc_time << " ms" << std::endl
	    << "Ave. " << acc_time / acc_count << " ms" << std::endl;
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return nsYm::all_4(argc, argv);
}
