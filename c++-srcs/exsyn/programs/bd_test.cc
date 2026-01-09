
/// @file bd_test.cc
/// @brief bd_test の実装ファイル
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
#include "BdGen.h"
#include <unistd.h>
#include <libgen.h>
#include "get_opt.h"


BEGIN_NAMESPACE_YM_EXSYN

// @brief 2項演算で演算数が最小となる実装を求める．
BcGraph
gen_boolchain(
  const TruthTable& tt
)
{
  // 正規化と前処理を行う．
  auto xf = XformInfo(tt);
  auto& normal_func = xf.normal_func();
  SizeType no = normal_func.output_num();

  if ( no == 0 ) {
    return xf.make_graph();
  }

  std::unordered_map<std::string, JsonValue> option_dict;
  option_dict.emplace("selecter", "MSV");
  JsonValue option(option_dict);
  SizeType max_input = 2;

  // Partial DAG
  FenceGen fg;
  bool use_all_steps = (no == 1);
  PdGen pg(use_all_steps);
  for ( SizeType k = no; ; ++ k ) {
    for ( SizeType l = 1; l <= k; ++ l ) {
      auto fence_list = fg(k, l, max_input, no);
      for ( auto& fence: fence_list ) {
	auto pd_list = pg(fence, max_input);
	for ( auto& pd: pd_list ) {
	  Engine_PD engine(normal_func, pd, option);
	  auto g = engine.solve();
	  if ( g.input_num() == normal_func.input_num() ) {
	    {
	      std::cout << "found in PD: " << pd << std::endl;
	      g.print(std::cout);
	    }
#if 0
	    BdGen bg(pd, normal_func.input_num());
	    auto bd_list = bg();
	    bool found = false;
	    for ( auto& bd: bd_list ) {
	      //std::cout << bd << std::endl;
	      Engine_BD engine(normal_func, bd, option);
	      auto g = engine.solve();
	      if ( g.input_num() == normal_func.input_num() ) {
		found = true;
		{
		  std::cout << "found in BD: " << bd << std::endl;
		  g.print(std::cout);
		}
		break;
	      }
	    }
	    if ( !found ) {
	      std::cout << pd << ": not found in BD" << std::endl;
	      g.print(std::cout);
	      abort();
	    }
#endif
	    return xf.denormalize(g);
	  }
	}
      }
    }
  }
  return BcGraph();
}

int
bd_test(
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
  std::string line;
  SizeType count = 0;
  while ( std::getline(s, line) ) {
    auto tt = TruthTable::from_hex(line);
    std::cerr << tt.hex_str() << std::endl;
    auto g = gen_boolchain(tt);
  }
  return 0;
}

END_NAMESPACE_YM_EXSYN


int
main(
  int argc,
  char** argv
)
{
  return nsYm::nsExSyn::bd_test(argc, argv);
}
