
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
  bool verify = false;
  bool debug = false;
  bool verbose = false;
  int max_input = 2;
  int limit = 0;
  auto argv0 = std::string{basename(argv[0])};

  int ch;
  while ( (ch = getopt(argc, argv, "s:cvdti:y:l:")) != -1 ) {
    switch (ch) {
    case 's': selecter = std::string{optarg}; break;
    case 'c': cegar = true; break;
    case 'v': verbose = true; break;
    case 'd': debug = true; break;
    case 't': verify = true; break;
    case 'i': max_input = atoi(optarg); break;
    case 'y': mode = std::string{optarg}; break;
    case 'l': limit = atoi(optarg); break;
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
	      << " [-s <selecter>] [-c] [-dvt] <filename>" << std::endl;
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
  option_dict.emplace("verbose", JsonValue(verbose));
  option_dict.emplace("debug", JsonValue(debug));
  option_dict.emplace("mode", JsonValue(mode));
  option_dict.emplace("max_input", JsonValue(max_input));
  option_dict.emplace("limit", JsonValue(limit));

  std::vector<std::string> sel_list = {
    "SSV", "MSV", "OSV", "SMART"
  };
  auto sel_num = sel_list.size();
  std::vector<JsonValue> option_list;
  option_list.reserve(sel_num);
  for ( auto sel: sel_list ) {
    auto tmp_dict = option_dict;
    tmp_dict.emplace("selecter", JsonValue(sel));
    auto option = JsonValue(tmp_dict);
    option_list.push_back(option);
  }
  std::vector<double> acc_time_list(sel_num, 0.0);

  std::string line;
  double acc_time = 0.0;
  SizeType count = 0;
  while ( std::getline(s, line) ) {
    auto tt = TruthTable::from_hex(line);
    std::cout << tt.hex_str() << std::endl;
    for ( SizeType i = 0; i < sel_num; ++ i ) {
      auto& option = option_list[i];
      Timer timer;
      timer.start();
      auto g = ExSyn::gen_boolchain(tt, option);
      timer.stop();
      auto time = timer.get_time();
      acc_time_list[i] += time;
      std::cout << "  " << std::setw(6) << sel_list[i]
		<< ": " << time << " ms"
		<< std::endl;
    }
    ++ count;
  }
  std::cout << "==================================" << std::endl;
  for ( SizeType i = 0; i < sel_num; ++ i ) {
    std::cout << std::setw(6) << sel_list[i]
	      << ": ave. "
	      << (acc_time_list[i] / count) << " ms" << std::endl;
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
  return nsYm::exsyn(argc, argv);
}
