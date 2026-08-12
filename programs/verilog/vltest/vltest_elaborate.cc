
/// @file vltest_elaborate.cc
/// @brief elabrate モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "ym/Timer.h"
#include "VlTestLineWatcher.h"
#include "ym/vl/VlMgr.h"
#include "VlDumper.h"
#include "ym/clib.h"
#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"


BEGIN_NAMESPACE_YM_VERILOG

void
elaborate_mode(
  const std::vector<std::string>& filename_list,
  bool all_msg,
  const std::string& spath,
  const ClibCellLibrary& cell_library,
  int watch_line,
  bool verbose,
  bool profile,
  int loop,
  bool dump_vpi
)
{
  auto tmh = new StreamMsgHandler(std::cerr);
  if ( all_msg ) {
    tmh->set_mask(kMsgMaskAll);
  }
  else {
    tmh->set_mask(kMsgMaskAll);
    tmh->delete_mask(MsgType::Info);
    tmh->delete_mask(MsgType::Debug);
  }
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
#if !defined(YM_DEBUG)
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
	vlmgr.read_file(name, splist, watcher_list);
	if ( verbose ) {
	  std::cerr << " end" << std::endl;
	}
      }

      timer.stop();
      auto time = timer.get_time();
      if ( verbose ) {
	std::cerr << "Parsing time: " << time << std::endl;
      }

      if ( !MsgMgr::error_num() ) {
	Timer timer;
	timer.start();

	vlmgr.elaborate(cell_library);

	timer.stop();
	auto time = timer.get_time();
	if ( verbose ) {
	  std::cerr << "Elaborating time: " << time << std::endl;
	}

	if ( MsgMgr::error_num() == 0 && dump_vpi ) {
	  VlDumper dumper(std::cout);
	  dumper(vlmgr);
	}
      }
      switch ( MsgMgr::error_num() ) {
      case 0:
	std::cerr << "No errors" << std::endl;
	break;
      case 1:
	std::cerr << "Total 1 error" << std::endl;
	break;

      default:
	std::cerr << "Total " << MsgMgr::error_num() << " errors"
		  << std::endl;
	break;
      }

#if !defined(YM_DEBUG)
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
