
/// @file vltest_parse.cc
/// @brief parse モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Timer.h"
#include "ym/VlMgr.h"
#include "VlTestLineWatcher.h"
#include "parser/PtDumper.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_YM_VERILOG

void
parse_mode(
  const std::vector<std::string>& filename_list,
  const std::string& spath,
  int watch_line,
  bool verbose,
  bool profile,
  int loop,
  bool dump_pt
)
{
  auto tmh = new StreamMsgHandler(std::cerr);
  MsgMgr::attach_handler(tmh);

  PathList splist;
  if ( spath != std::string{} ) {
    splist.set_from_string(spath);
  }

  VlTestLineWatcher watcher(watch_line);
  std::vector<VlLineWatcher*> watcher_list;
  if ( watch_line ) {
    watcher_list.push_back(&watcher);
  }

  int c = loop + 1;
  for ( int i = 0; i < c; ++ i ) {
#if 0
    try {
#endif
      Timer timer;
      timer.start();
      VlMgr vlmgr;
      for ( auto name: filename_list ) {
	if ( verbose ) {
	  std::cerr << "Reading " << name;
	  std::cerr.flush();
	}
	bool stat = vlmgr.read_file(name, splist, watcher_list);
	if ( verbose ) {
	  std::cerr << " end" << std::endl;
	}
      }
      timer.stop();
      auto time = timer.get_time();
      if ( verbose ) {
	std::cerr << "Parsing time: " << time << std::endl;
      }

      if ( dump_pt ) {
	auto& udp_list = vlmgr.pt_udp_list();
	auto& module_list = vlmgr.pt_module_list();
	PtDumper dp(std::cout);
	dp.enable_file_loc_mode();
	dp.put(udp_list, module_list);
      }

      switch ( MsgMgr::error_num() ) {
      case 0:
	std::cerr << "No errors" << std::endl;
	break;
      case 1:
	std::cerr << "Total 1 error" << std::endl;
	break;

      default:
	std::cerr << "Total " << MsgMgr::error_num() << " errors" << std::endl;
	break;
      }
#if 0
    }
    catch ( AssertError x ) {
      std::cerr << x << std::endl;
    }
    catch ( std::bad_alloc x ) {
      std::cerr << "no more memory" << std::endl;
      exit(1);
    }
    catch (...) {
      std::cerr << "unkown exception" << std::endl;
    }
#endif
  }
}

END_NAMESPACE_YM_VERILOG
