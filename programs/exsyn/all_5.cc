
/// @file all_5.cc
/// @brief all_5 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/Timer.h"
#include <unistd.h>
#include <libgen.h>


BEGIN_NAMESPACE_YM

static
const char* f_str_list[] = {
#include "npn5.txt"
  ""
};

int
all_5(
  int argc,
  char** argv
)
{
  auto argv0 = std::string{basename(argv[0])};
  bool cegar = false;
  bool verify = false;
  bool all_4 = false;
  bool opt1 = false;
  int loop_count = 1;
  int ch;
  while ( (ch = getopt(argc, argv, "cvol:4")) != -1 ) {
    switch (ch) {
    case 'c': cegar = true; break;
    case 'v': verify = true; break;
    case 'o': opt1 = true; break;
    case 'l': loop_count = atoi(optarg); break;
    case '4': all_4 = true; break;
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

  Timer timer;
  timer.start();
  SizeType count = 0;
  for ( SizeType _ = 0; _ < loop_count; ++ _ ) {
    if ( all_4 ) {
      const SizeType NUM = 0x10000;
      for ( SizeType p = 0; p < NUM; ++ p ) {
	TruthTable tt(4, 1);
	for ( SizeType b = 0; b < 16; ++ b ) {
	  if ( p & (1 << b) ) {
	    tt.set_value(b, 0, Val3::ONE);
	  }
	}
	auto g = ExSyn::gen_boolchain(tt, option);
	if ( verify ) {
	  if ( !ExSyn::check_boolchain(tt, g) ) {
	    throw std::invalid_argument{"incorrect graph"};
	  }
	}
      }
      count += NUM;
    }
    else {
      SizeType pos = 0;
      for ( ; ; ++ count, ++ pos ) {
	auto f_str = f_str_list[pos];
	if ( strcmp(f_str, "") == 0 ) {
	  break;
	}
	TruthTable tt(5, 1);
	for ( SizeType b = 0; b < 32; ++ b ) {
	  if ( f_str[b] == '1' ) {
	    tt.set_value(b, 0, Val3::ONE);
	  }
	}
	auto g = ExSyn::gen_boolchain(tt, option);
	if ( verify ) {
	  if ( !ExSyn::check_boolchain(tt, g) ) {
	    throw std::invalid_argument{"incorrect graph"};
	  }
	}
      }
    }
  }
  timer.stop();
  auto time = timer.get_time();
  std::cout << "Total " << time << " ms" << std::endl
	    << "Ave. " << time / count << " ms" << std::endl;
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return nsYm::all_5(argc, argv);
}
