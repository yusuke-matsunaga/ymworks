
/// @file exsyn.cc
/// @brief exsyn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ExSyn.h"
#include "ym/Timer.h"
#include "Engine_PD.h"
#include "XformInfo.h"
#include "FenceGen.h"
#include "PdGen.h"
#include <unistd.h>
#include <libgen.h>
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

struct Stats {
  std::vector<double> time_list;
  SizeType count{0};

  Stats(
    SizeType n = 0
  ) : time_list(n, 0.0)
  {
  }

};

struct PatTimeDict
{
  std::vector<std::string> pat_list;
  std::vector<std::string> mode_list;
  std::unordered_map<std::string, Stats> dict;

  PatTimeDict(
    const std::vector<std::string>& arg_mode_list
  ) : mode_list{arg_mode_list}
  {
  }

  void
  add_pat(
    const PD& pd,
    const std::vector<double>& time_list
  )
  {
    auto pat = pd_pat(pd);
    auto n = mode_list.size();
    if ( dict.count(pat) == 0 ) {
      pat_list.push_back(pat);
      dict.emplace(pat, Stats(n));
    }
    auto& stats = dict[pat];
    for ( SizeType i = 0; i < n; ++ i ) {
      stats.time_list[i] += time_list[i];
    }
    ++ stats.count;
    { // デバッグ
      print(std::cerr);
    }
  }

  void
  print(
    std::ostream& s
  ) const
  {
    auto n = mode_list.size();
    for ( auto& pat: pat_list ) {
      auto& stats = dict.at(pat);
      s << pat << std::endl;
      for ( SizeType i = 0; i < n; ++ i ) {
	auto tmp_str = mode_list[i] + ":";
	s << "  " << std::setw(8) << tmp_str
	  << stats.time_list[i] / stats.count << std::endl;
      }
    }
  }

  static
  std::string
  pd_pat(
    const PD& pd
  )
  {
    std::ostringstream buf;
    const char* sep = "";
    for ( auto _: pd.group2_2() ) {
      buf << sep << "2";
      sep = ":";
    }
    if ( !pd.group2_2().empty() ) {
      sep = "|";
    }
    for ( auto _: pd.group3_3() ) {
      buf << sep << "3";
      sep = ":";
    }
    for ( auto& group2_1: pd.group2_1_list() ) {
      sep = "|";
      for ( auto _: group2_1 ) {
	buf << sep << "1";
	sep = ":";
      }
    }
    for ( auto& group3_1: pd.group3_1_list() ) {
      sep = "|";
      for ( auto _: group3_1 ) {
	buf << sep << "2";
	sep = ":";
      }
    }
    for ( auto& group3_2: pd.group3_2_list() ) {
      sep = "|";
      for ( auto _: group3_2 ) {
	buf << sep << "1";
	sep = ":";
      }
    }
    {
      std::cout << "PD: " << pd.to_string() << std::endl
		<< "pat: " << buf.str() << std::endl;
    }
    return buf.str();
  }
};

// @brief 2項演算で演算数が最小となる実装を求める．
BcGraph
gen_boolchain(
  const TruthTable& tt,
  PatTimeDict& time_dict,
  const JsonValue& option
)
{
  auto mode = get_string(option, "mode", "");
  auto max_input = get_int(option, "max_input", 2);
  SizeType limit = get_int(option, "limit", 0);
  if ( limit == 0 ) {
    limit = std::numeric_limits<SizeType>::max();
  }

  // 正規化と前処理を行う．
  auto xf = XformInfo(tt);
  auto& normal_func = xf.normal_func();
  SizeType no = normal_func.output_num();

  if ( no == 0 ) {
    return xf.make_graph();
  }

  auto mode_num = time_dict.mode_list.size();
  std::vector<JsonValue> option_list;
  option_list.reserve(mode_num);
  for ( auto& mode: time_dict.mode_list ) {
    std::unordered_map<std::string, JsonValue> tmp_dict;
    tmp_dict.emplace("selecter", mode);
    option_list.push_back(JsonValue{tmp_dict});
  }

  // Partial DAG
  FenceGen fg;
  bool use_all_steps = (no == 1);
  PdGen pg(use_all_steps);
  for ( SizeType k = no; k < limit; ++ k ) {
    for ( SizeType l = 1; l <= k; ++ l ) {
      auto fence_list = fg(k, l, max_input, no);
      for ( auto& fence: fence_list ) {
	auto pd_list = pg(fence, max_input);
	for ( auto& pd: pd_list ) {
	  BcGraph g;
	  std::vector<double> time_list;
	  time_list.reserve(mode_num);
	  for ( SizeType i = 0; i < mode_num; ++ i ) {
	    Timer timer;
	    timer.start();
	    Engine_PD engine(normal_func, pd, option_list[i]);
	    g = engine.solve();
	    timer.stop();
	    auto time = timer.get_time();
	    time_list.push_back(time);
	  }
	  time_dict.add_pat(pd, time_list);
	  if ( g.input_num() == normal_func.input_num() ) {
	    return xf.denormalize(g);
	  }
	}
      }
    }
  }
  return BcGraph();
}

int
exsyn(
  int argc,
  char** argv
)
{
  std::string mode = "";
  int max_input = 2;
  int limit = 0;
  auto argv0 = std::string{basename(argv[0])};

  int ch;
  while ( (ch = getopt(argc, argv, "i:l:")) != -1 ) {
    switch (ch) {
    case 'i': max_input = atoi(optarg); break;
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
	      << " [-cv] <filename>" << std::endl;
    return -1;
  }

  auto filename = std::string{argv[0]};
  std::ifstream s{filename};
  if ( !s ) {
    std::ostringstream buf;
    buf << filename << ": Could not open";
    throw std::invalid_argument{buf.str()};
  }

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("max_input", JsonValue(max_input));
  option_dict.emplace("limit", JsonValue(limit));
  auto option = JsonValue(option_dict);

  std::vector<std::string> mode_list = {
    "SSV", "MSV", "OSV", "SMART"
  };
  PatTimeDict time_dict(mode_list);
  std::string line;
  SizeType count = 0;
  while ( std::getline(s, line) ) {
    auto tt = TruthTable::from_hex(line);
    std::cerr << tt.hex_str() << std::endl;
    auto g = gen_boolchain(tt, time_dict, option);
  }
  time_dict.print(std::cout);
  return 0;
}

END_NAMESPACE_YM_EXSYN


int
main(
  int argc,
  char** argv
)
{
  return nsYm::nsExSyn::exsyn(argc, argv);
}
