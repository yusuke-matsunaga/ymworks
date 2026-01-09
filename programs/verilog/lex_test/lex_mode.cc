
/// @file lex_mode.cc
/// @brief lex モードのテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2025 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Timer.h"
#include "VlTestLineWatcher.h"

#include "ym/MsgMgr.h"
#include "ym/StreamMsgHandler.h"

#include "scanner/Lex.h"

#include "common.h"
#include "print_token.h"


BEGIN_NAMESPACE_YM_VERILOG

void
lex_mode(
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

    int nt_id = 0;
    int nt_sid = 0;
    int nt_un = 0;
    int nt_unb = 0;
    int nt_uni = 0;
    int nt_rn = 0;
    int nt_s = 0;
    int ns = 0;
    for ( auto filename: filename_list ) {
      if ( !lex.open_file(filename) ) {
	std::cerr << filename << " : cannot open" << std::endl;
	return;
      }

      for ( ; ; ) {
	int id = lex.get_token();
	if ( id == EOF ) {
	  break;
	}
	if ( dump_token ) {
	  std::cout << lex.cur_token_loc() << ": ";
	  print_token(std::cout, id, lex.cur_string());
	  std::cout << std::endl;
	}

	switch ( id ) {
	case IDENTIFIER:
	  ++ nt_id;
	  break;
	case SYS_IDENTIFIER:
	  ++ nt_sid;
	  break;
	case UNUMBER:
	  ++ nt_un;
	  break;
	case UNUM_BIG:
	  ++ nt_unb;
	case UNUM_INT:
	  ++ nt_uni;
	  break;
	case RNUMBER:
	  ++ nt_rn;
	  break;
	case STRING:
	  ++ nt_s;
	  break;
	default:
	  ++ ns;
	  break;
	}
      }
    }

    timer.stop();
    auto time = timer.get_time();

    if ( verbose ) {
      std::cout << "Total " << std::setw(10) << nt_id << " IDENTIFIER tokens" << std::endl
		<< "      " << std::setw(10) << nt_sid << " SYS_IDENTIFIER tokens" << std::endl
		<< "      " << std::setw(10) << nt_un << " UNUMBER tokens" << std::endl
		<< "      " << std::setw(10) << nt_unb << " UNUM BIG" << std::endl
		<< "      " << std::setw(10) << nt_uni << " UNUM INT" << std::endl
		<< "      " << std::setw(10) << nt_rn << " RNUMBER tokens" << std::endl
		<< "      " << std::setw(10) << nt_s << " STRING tokens" << std::endl
		<< "      " << std::setw(10) << ns << " symbols" << std::endl
		<< "Time: " << time << std::endl;
    }

    FileInfo::clear();
  }

  sleep(10);
}

END_NAMESPACE_YM_VERILOG
