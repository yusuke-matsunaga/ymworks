
/// @file exsyn.cc
/// @brief exsyn の実装ファイル
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
exsyn(
  int argc,
  char** argv
)
{
  std::string selecter = "";
  std::string mode = "";
  bool cegar = false;
  bool use_all_inputs = true;
  bool use_all_steps = true;
  bool no_reapplication = true;
  bool op_order = true;
  bool verify = false;
  bool debug = false;
  bool verbose = false;
  int max_input = 2;
  int limit = 0;
  auto argv0 = std::string{basename(argv[0])};

  int ch;
  while ( (ch = getopt(argc, argv, "s:cvdti:y:l:Aaro")) != -1 ) {
    switch (ch) {
    case 's': selecter = std::string{optarg}; break;
    case 'c': cegar = true; break;
    case 'v': verbose = true; break;
    case 'd': debug = true; break;
    case 't': verify = true; break;
    case 'i': max_input = atoi(optarg); break;
    case 'y': mode = std::string{optarg}; break;
    case 'l': limit = atoi(optarg); break;
    case 'A': use_all_inputs = false; break;
    case 'a': use_all_steps = false; break;
    case 'r': no_reapplication = false; break;
    case 'o': op_order = false; break;
    case '?':
    default:
      goto usage;
    }
  }

  argc -= optind;
  argv += optind;
  if ( argc != 1 ) {
  usage:
    std::cerr << "USAGE: " << argv0
	      << " [-s <selecter>] [-y mode] [-c] [-aro] [-dvt] <filename>"
	      << std::endl;
    return -1;
  }

  if ( selecter == "" ) {
    selecter = "SMART";
  }

  auto filename = std::string{argv[0]};
  std::ifstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not open";
    throw std::invalid_argument{buf.str()};
  }

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("cegar", JsonValue(cegar));
  option_dict.emplace("selecter", JsonValue(selecter));
  option_dict.emplace("verbose", JsonValue(verbose));
  option_dict.emplace("debug", JsonValue(debug));
  option_dict.emplace("mode", JsonValue(mode));
  option_dict.emplace("max_input", JsonValue(max_input));
  option_dict.emplace("limit", JsonValue(limit));
  option_dict.emplace("use_all_inputs", JsonValue(use_all_inputs));
  option_dict.emplace("use_all_steps", JsonValue(use_all_steps));
  option_dict.emplace("no_reapplication", JsonValue(no_reapplication));
  option_dict.emplace("op_order", JsonValue(op_order));
  auto option = JsonValue(option_dict);

  std::string line;
  double acc_time = 0.0;
  SizeType count = 0;
  while ( std::getline(s, line) ) {
    auto tt = TruthTable::from_hex(line);
    if ( verbose ) {
      std::cout << tt.hex_str() << std::endl;
    }
    Timer timer;
    timer.start();
    auto g = ExSyn::gen_boolchain(tt, option);
    timer.stop();
    auto time = timer.get_time();
    acc_time += time;
    ++ count;
    if ( verbose ) {
      std::cout << "End:  " << g.op_num()
		<< " " << time << " ms"
		<< std::endl;
    }
    if ( verify ) {
      if ( !ExSyn::check_boolchain(tt, g) ) {
	std::cerr << "Error!" << std::endl;
	return 1;
      }
    }
  }
  std::cout << "Total " << acc_time << " ms" << std::endl
	    << "Ave. Time " << acc_time / count << " ms" << std::endl;
  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return nsYm::exsyn(argc, argv);
}
