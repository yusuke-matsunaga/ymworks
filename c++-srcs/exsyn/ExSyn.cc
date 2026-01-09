
/// @file ExSyn.cc
/// @brief ExSyn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include <thread>
#include <mutex>
#include "ym/ExSyn.h"
#include "XformInfo.h"
#include "Engine_Naive.h"
#include "Engine_PD.h"
#include "Engine_BD.h"
#include "FenceGen.h"
#include "PdGen.h"
#include "BdGen.h"
#include "get_opt.h"
#include "ym/Timer.h"


BEGIN_NAMESPACE_YM_EXSYN

BEGIN_NONAMESPACE

// マルチスレッドで ExSynEngine を起動する．
BcGraph
do_multithread(
  const TruthTable& func,
  const std::vector<PD>& pd_pool,
  const JsonValue& option
)
{
  // スレッド数
  int thread_num = get_int(option, "thread_num", 0);
  if ( thread_num == 0 ) {
    // デフォルト
    thread_num = std::thread::hardware_concurrency();
  }
  // スレッドのリスト
  std::vector<std::thread> thr_list(thread_num);

  // 次に処理すべき PD 番号
  SizeType pd_id = 0;
  // pd_id 用のミューテックス
  std::mutex pd_mtx;
  // 結果の書き込み用ミューテックス
  std::mutex wr_mtx;
  // 結果のグラフ
  BcGraph g;
  // スレッドを生成する．
  for ( SizeType i = 0; i < thread_num; ++ i ) {
    thr_list[i] = std::thread{[&](){
      for ( ; ; ) {
	SizeType my_id = 0;
	{ // unique_lock 用のスコープ
	  std::unique_lock lck{pd_mtx};
	  if ( pd_id >= pd_pool.size() ) {
	    break;
	  }
	  my_id = pd_id;
	  ++ pd_id;
	}
	auto& pd = pd_pool[my_id];
	Engine_PD engine(func, pd, option);
	auto g1 = engine.solve();
	if ( g1.input_num() == func.input_num() ) {
	  {
	    std::unique_lock lck{wr_mtx};
	    g = g1;
	  }
	  {
	    // pd_id を末尾に移動する．
	    std::unique_lock lck{pd_mtx};
	    pd_id = pd_pool.size();
	  }
	}
      }
    }};
  }
  // 子スレッドが終了するまで待つ．
  for ( auto& thr: thr_list ) {
    thr.join();
  }
  return g;
}

#if 0
// マルチスレッドで ExSynEngine を起動する．
BcGraph
do_multithread(
  const TruthTable& func,
  const std::vector<BD>& bd_pool,
  const JsonValue& option
)
{
  // スレッド数
  int thread_num = get_int(option, "thread_num", 0);
  if ( thread_num == 0 ) {
    // デフォルト
    thread_num = std::thread::hardware_concurrency();
  }
  // スレッドのリスト
  std::vector<std::thread> thr_list(thread_num);

  // 次に処理すべき BD 番号
  SizeType bd_id = 0;
  // bd_id 用のミューテックス
  std::mutex bd_mtx;
  // 結果の書き込み用ミューテックス
  std::mutex wr_mtx;
  // 結果のグラフ
  BcGraph g;
  // スレッドを生成する．
  for ( SizeType i = 0; i < thread_num; ++ i ) {
    thr_list[i] = std::thread{[&](){
      for ( ; ; ) {
	SizeType my_id = 0;
	{ // unique_lock 用のスコープ
	  std::unique_lock lck{bd_mtx};
	  if ( bd_id >= bd_pool.size() ) {
	    break;
	  }
	  my_id = bd_id;
	  ++ bd_id;
	}
	auto& bd = bd_pool[my_id];
	Engine_BD engine(func, bd, option);
	auto g1 = engine.solve();
	if ( g1.input_num() == func.input_num() ) {
	  {
	    std::unique_lock lck{wr_mtx};
	    g = g1;
	  }
	  {
	    // pd_id を末尾に移動する．
	    std::unique_lock lck{bd_mtx};
	    bd_id = bd_pool.size();
	  }
	}
      }
    }};
  }
  // 子スレッドが終了するまで待つ．
  for ( auto& thr: thr_list ) {
    thr.join();
  }
  return g;
}
#endif

END_NONAMESPACE

// @brief 2項演算で演算数が最小となる実装を求める．
BcGraph
ExSyn::gen_boolchain(
  const TruthTable& tt,
  const JsonValue& option
)
{
  auto verbose = get_bool(option, "verbose", false);
  auto debug = get_bool(option, "debug", false);
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

  if ( mode == "" ) {
    // デフォルト
    for ( SizeType k = no; k < limit; ++ k ) {
      if ( debug ) {
	std::cout << "k = " << k
		  << std::endl;
      }
      Engine_Naive engine( normal_func, k, max_input, option);
      auto g = engine.solve();
      if ( g.input_num() == normal_func.input_num() ) {
	return xf.denormalize(g);
      }
    }
  }
  else if ( mode == "PD" ) {
    // Partial DAG
    FenceGen fg;
    // 1出力の時は PD を作る時に use_all_steps 制約を考える．
    auto use_all_steps = no == 1;
    PdGen pg(use_all_steps);
    for ( SizeType k = no; k < limit; ++ k ) {
      for ( SizeType l = 1; l <= k; ++ l ) {
	auto fence_list = fg(k, l, max_input, no);
	for ( auto& fence: fence_list ) {
	  if ( debug ) {
	    std::cout << "fence: " << fence << std::endl;
	  }
	  auto pd_list = pg(fence, max_input);
	  for ( auto& pd: pd_list ) {
	    if ( debug ) {
	      std::cout << "PD: " << pd << std::endl;
	    }
	    Timer timer;
	    timer.start();
	    Engine_PD engine(normal_func, pd, option);
	    auto g = engine.solve();
	    timer.stop();
	    if ( debug ) {
	      std::cout << "  " << timer.get_time() << " ms" << std::endl;
	    }
	    if ( g.input_num() == normal_func.input_num() ) {
	      {
		if ( !check_boolchain(normal_func, g) ) {
		  g.print(std::cout);
		  normal_func.print(std::cout);
		}
	      }
	      return xf.denormalize(g);
	    }
	  }
	}
      }
    }
  }
  else if ( mode == "BD" ) {
    // Bound DAG
    FenceGen fg;
    bool use_all_steps = (no == 1);
    PdGen pg(use_all_steps);
    for ( SizeType k = no; k < limit; ++ k ) {
      for ( SizeType l = 1; l <= k; ++ l ) {
	auto fence_list = fg(k, l, max_input, no);
	for ( auto& fence: fence_list ) {
	  if ( debug ) {
	    std::cout << "fence: " << fence << std::endl;
	  }
	  auto pd_list = pg(fence, max_input);
	  for ( auto& pd: pd_list ) {
	    if ( debug ) {
	      std::cout << "PD: " << pd << std::endl;
	    }
	    BdGen bg(pd, normal_func.input_num());
	    auto bd_list = bg();
	    if ( bd_list.empty() ) {
	      continue;
	    }
	    if ( debug ) {
	      std::cout << "BD" << std::endl;
	      for ( auto& bd: bd_list ) {
		std::cout << bd << std::endl;
	      }
	    }
	    Timer timer;
	    timer.start();
	    Engine_BD engine(normal_func, bd_list, option);
	    auto g = engine.solve();
	    timer.stop();
	    if ( debug ) {
	      std::cout << "  " << timer.get_time() << " ms" << std::endl;
	    }
	    if ( g.input_num() == normal_func.input_num() ) {
	      return xf.denormalize(g);
	    }
	  }
	}
      }
    }
  }
  else if ( mode == "MTPD" ) {
    // Multi-Threaded Partial DAG
    FenceGen fg;
    bool use_all_steps = (no == 1);
    PdGen pg(use_all_steps);
    for ( SizeType k = no; k < limit; ++ k ) {
      for ( SizeType l = 1; l <= k; ++ l ) {
	std::vector<PD> pd_pool;
	auto fence_list = fg(k, l, max_input, no);
	for ( auto& fence: fence_list ) {
	  auto pd_list = pg(fence, max_input);
	  pd_pool.insert(pd_pool.end(), pd_list.begin(), pd_list.end());
	}
	if ( pd_pool.empty() ) {
	  continue;
	}
	if ( debug ) {
	  std::cout << "k: " << k << ", level: " << l
		    << ", Total # of PDs: " << pd_pool.size()
		    << std::endl;
	}
	auto g = do_multithread(normal_func, pd_pool, option);
	if ( g.input_num() == normal_func.input_num() ) {
	  return xf.denormalize(g);
	}
      }
    }
  }
#if 0
  else if ( mode == "MTBD" ) {
    // Multi-Threaded Bound DAG
    FenceGen fg;
    bool use_all_steps = (no == 1);
    PdGen pg(use_all_steps);
    for ( SizeType k = no; k < limit; ++ k ) {
      for ( SizeType l = 1; l <= k; ++ l ) {
	std::vector<BD> bd_pool;
	auto fence_list = fg(k, l, max_input, no);
	for ( auto& fence: fence_list ) {
	  if ( debug ) {
	    std::cout << "fence: " << fence << std::endl;
	  }
	  auto pd_list = pg(fence, max_input);
	  for ( auto& pd: pd_list ) {
	    if ( debug ) {
	      std::cout << "PD: " << pd << std::endl;
	    }
	    BdGen bg(pd, normal_func.input_num());
	    auto bd_list = bg();
	    bd_pool.insert(bd_pool.end(), bd_list.begin(), bd_list.end());
	  }
	  if ( bd_pool.empty() ) {
	    continue;
	  }
	  if ( debug ) {
	    std::cout << "k: " << k << ", level: " << l
		      << ", Total # of BDs: " << bd_pool.size()
		      << std::endl;
	  }
	  auto g = do_multithread(normal_func, bd_pool, option);
	  if ( g.input_num() == normal_func.input_num() ) {
	    return xf.denormalize(g);
	  }
	}
      }
    }
  }
#endif
  else {
    std::ostringstream buf;
    buf << mode << ": unknown mode" << std::endl;
    throw std::invalid_argument{buf.str()};
  }
  std::cout << "loop limit exceeded" << std::endl;
  return BcGraph();
}

// @brief BcGraph が正しいかチェックする．
bool
ExSyn::check_boolchain(
  const TruthTable& tt,
  const BcGraph& graph
)
{
  SizeType ni = tt.input_num();
  SizeType ni_exp = 1 << ni;
  SizeType no = tt.output_num();
  for ( SizeType b = 0; b < ni_exp; ++ b ) {
    auto input_vals = tt.decode_index(b);
    auto output_vals = graph.simulate(input_vals);
    for ( SizeType o = 0; o < no; ++ o ) {
      auto val = tt.get_value(b, o);
      auto exp_val = false;
      switch ( val ) {
      case Val3::ZERO:
	if ( output_vals[o] ) {
	  return false;
	}
	break;
      case Val3::ONE:
	if ( !output_vals[o] ) {
	  return false;
	}
	break;
      case Val3::X:
	break;
      }
    }
  }
  return true;
}

END_NAMESPACE_YM_EXSYN
