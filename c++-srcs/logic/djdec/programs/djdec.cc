
/// @file djdec.cc
/// @brief djdec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/DjDec.h"
#include "ym/BcGraph.h"
#include "ym/BddMgr.h"
#include "ym/TvFunc.h"
#include "ym/Tv2Bdd.h"
#include "ym/Timer.h"
#include <unistd.h>
#include <libgen.h>


BEGIN_NAMESPACE_YM

void
djdec(
  std::istream& s
)
{
  std::string line;
  double acc_tv_time = 0.0;
  double acc_bdd_time = 0.0;
  SizeType count = 0;
  while ( std::getline(s, line) ) {
    auto tv_func = TvFunc(line, 16);
    std::cout << tv_func.hex_str() << std::endl;
    BddMgr mgr;
    auto bdd_func = Tv2Bdd::gen_bdd(tv_func, mgr);
    {
      Timer timer;
      timer.start();
      auto g = DjDec::decompose(bdd_func);
      timer.stop();
      auto time = timer.get_time();
      g.print(std::cout);
      std::cout << time << " ms"
		<< std::endl;
      acc_bdd_time += time;
    }
    {
      Timer timer;
      timer.start();
      auto g = DjDec::decompose(tv_func);
      timer.stop();
      g.print(std::cout);
      auto time = timer.get_time();
      std::cout << time << " ms"
		<< std::endl;
      acc_tv_time += time;
    }
    ++ count;
  }
  std::cout << "==================================" << std::endl;
  std::cout << "ave(BDD).: "
	    << (acc_bdd_time / count) << " ms" << std::endl;
  std::cout << "ave(TvFunc).: "
	    << (acc_tv_time / count) << " ms" << std::endl;
}

int
djdec_main(
  int argc,
  char** argv
)
{
  if ( argc == 1 ) {
    djdec(std::cin);
  }
  else if ( argc == 2 ) {
    auto filename = std::string{argv[1]};
    std::ifstream s{filename};
    if ( !s ) {
      std::ostringstream buf;
      buf << filename << ": Could not open";
      throw std::invalid_argument{buf.str()};
    }
    djdec(s);
  }
  else {
    std::cerr << "USAGE: " << basename(argv[0])
	      << " <filename>" << std::endl;
    return -1;
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
  return nsYm::djdec_main(argc, argv);
}
