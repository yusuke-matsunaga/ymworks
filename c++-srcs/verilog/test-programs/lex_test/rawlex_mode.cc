
/// @file rawlex_mode.cc
/// @brief rawlex モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.

#include "VlTestLineWatcher.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"
#include "ym/Timer.h"

#include "scanner/Lex.h"

#include "common.h"
#include "print_token.h"


BEGIN_NAMESPACE_YM_VERILOG

void
rawlex_mode(
  const std::vector<std::string>& filename_list,
  const std::string& spath,
  int watch_line,
  int loop,
  bool verbose,
  bool dump_token
)
{
  auto handler = new StreamMsgHandler(std::cerr);
  MsgMgr::attach_handler(handler);

  int c = loop + 1;
  for ( int i = 0; i < c; ++ i ) {
    Lex lex;

    if ( spath != std::string{} ) {
      PathList splist(spath);
      lex.set_searchpath(splist);
    }
    if ( watch_line ) {
      auto mw = new VlTestLineWatcher(watch_line);
      lex.reg_watcher(mw);
    }

    Timer timer;
    timer.start();

    for ( auto filename: filename_list ) {
      if ( !lex.open_file(filename) ) {
	std::cerr << filename << " : cannot open" << std::endl;
	return;
      }

      for ( ; ; ) {
	int id = lex.get_raw_token();
	if ( id == EOF ) {
	  break;
	}
	if ( dump_token ) {
	  std::cout << lex.cur_token_loc() << ": ";
	  print_token(std::cout, id, lex.cur_string());
	  std::cout << std::endl;
	}
      }
    }

    timer.stop();
    if ( verbose ) {
      auto time = timer.get_time();
      std::cout << "Time: " << time << std::endl;
    }

    FileInfo::clear();
  }

  sleep(10);

}

END_NAMESPACE_YM_VERILOG
